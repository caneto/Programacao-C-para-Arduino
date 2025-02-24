////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_MEMORY_h
#define _MITOV_MEMORY_h

#include <Mitov.h>

namespace Mitov
{
//---------------------------------------------------------------------------
//	class BasicMemoryElement;
//---------------------------------------------------------------------------
/*
	class MemoryIntf
	{
	public:
		Mitov::SimpleList<BasicMemoryElement *>	FElements;

	public:
		virtual bool ReadData( uint32_t AAddress, uint32_t ASize, uint8_t *AData ) = 0;
		virtual bool WriteData( uint32_t AAddress, uint32_t ASize, uint8_t *AData ) = 0;

	};
//---------------------------------------------------------------------------
	class BasicMemoryElement
	{
	protected:
		MemoryIntf &FOwner;

	public:
		virtual void PopulateAddress( uint32_t &AAddress, byte &AOffset ) = 0;
//		virtual void ProcessRead( MemoryIntf *AOwner ) = 0;
		virtual void ProcessWrite( MemoryIntf *AOwner ) = 0;
//		virtual void GetAddressAndSize( uint32_t &AAddress, &ASize ) = 0;

	public:
		BasicMemoryElement( MemoryIntf &AOwner ) :
			FOwner( AOwner )
		{
			AOwner.FElements.push_back( this );
		}

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FModified,
		typename T_FRememberClocked,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected,
		typename TYPE_DATA,
		typename TYPE_PIN
	> class TypedMemoryElement : // public BasicMemoryElement
		public T_FModified,
		public T_FRememberClocked,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FRememberClocked )
		_V_PROP_( FModified )

//		uint32_t	FAddress;
		TYPE_DATA	FValue;

	public:
/*
		inline void PopulateAddress( uint32_t &AAddress, byte &AOffset )
		{
			if( AOffset )
			{
				++ AAddress;
				AOffset = 0;
			}

			FAddress = AAddress;
			AAddress += sizeof( TYPE_DATA );
		}
*/
		inline void ProcessWrite()
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			else if( ! FModified() )
				return;

			FModified() = false;

//			Serial.println( "WRITE" );
//			Serial.println( InputPin.Value );
//			Serial.println( sizeof( T ) );

			C_OWNER.WriteData( C_Address, sizeof( TYPE_DATA ), (uint8_t *)&FValue );

			if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
				RecallInputPin_o_Receive( nullptr );
		}

	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
			TYPE_DATA AValue = 0;
			if( C_OWNER.ReadData( C_Address, sizeof( TYPE_DATA ), (uint8_t *)&AValue ))
				T_OutputPin::SetPinValue( TYPE_PIN( AValue ));
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			FValue = *(TYPE_PIN*)_Data;
			FModified() = true;
		}

	public:
		TypedMemoryElement()
		{
			FRememberClocked() = false;
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FModified,
		typename T_FRememberClocked,
		uint32_t C_MaxSize,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected
	> class TextMemoryElement :
		public T_FModified,
		public T_FRememberClocked,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FRememberClocked )
		_V_PROP_( FModified )

	protected:
//		Mitov::String		FValue;
		uint8_t	FValue[ C_MaxSize + 1 ];

	public:
/*
		virtual void PopulateAddress( uint32_t &AAddress, byte &AOffset ) override
		{
			if( AOffset )
			{
				++ AAddress;
				AOffset = 0;
			}

			FAddress = AAddress;
			AAddress += MaxSize + 1;
		}
*/
		inline void ProcessWrite()
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			else if( ! FModified() )
				return;

			FModified() = false;

//			Serial.println( "WRITE" );
//			Serial.println( InputPin.Value );
//			Serial.println( sizeof( T ) );
//			Mitov::String AText = FValue;
//			uint8_t ASize = MitovMin<unsigned int>( C_MaxSize, AText.length() );

//			Serial.println( ASize );
//			Serial.println( AText );

			if( FValue[ 0 ] == 0 )
				return;

			C_OWNER.WriteData( C_Address, FValue[ 0 ] + 1, (uint8_t *)FValue );
//			C_OWNER.WriteData( C_Address + 1, ASize, (uint8_t *)AText.c_str() );

			if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
				RecallInputPin_o_Receive( nullptr );
		}

	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
//			uint8_t *ABuffer = new uint8_t[ C_MaxSize ];
			if( C_OWNER.ReadData( C_Address, C_MaxSize, FValue ))
			{
				Mitov::String AValue;
				uint8_t ASize = MitovMin<unsigned int>( C_MaxSize, FValue[ 0 ] );
				AValue.reserve( ASize );

				for( int i = 0; i < ASize; ++ i )
					AValue += ((char *)FValue )[ i + 1 ];

				T_OutputPin::SetPinValue( AValue );
			}

//			delete [] ABuffer;
		}

		void InputPin_o_Receive( void *_Data )
		{
			Mitov::String AText = (char*)_Data;

			uint8_t ASize = MitovMin<unsigned int>( C_MaxSize, AText.length() );
			FValue[ 0 ] = ASize;
			memcpy( FValue + 1, AText.c_str(), ASize );

//			Serial.println( ASize );
//			Serial.println( AText );

//			C_OWNER.WriteData( C_Address, 1, (uint8_t *)&ASize );
//			C_OWNER.WriteData( C_Address + 1, ASize, (uint8_t *)AText.c_str() );
			FModified() = true;
		}

	public:
		TextMemoryElement()
		{
			FValue[ 0 ] = 0;
			FRememberClocked() = false;
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_Address,
		uint8_t  C_Mask,
		typename T_FModified,
		typename T_FRememberClocked,
		typename T_FValue,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RecallOnRemember,
		typename T_RememberInputPin_o_IsConnected
	> class DigitalMemoryElement :
		public T_FModified,
		public T_FRememberClocked,
		public T_FValue,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RecallOnRemember,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( RememberInputPin_o_IsConnected )
		_V_PROP_( RecallInputPin_o_IsConnected )

	public:
		_V_PROP_( RecallOnRemember )

	protected:
		_V_PROP_( FRememberClocked )
		_V_PROP_( FModified )
		_V_PROP_( FValue )
//		uint32_t	FAddress;

	public:
/*
		virtual void GetAddressAndSize( uint32_t &AAddress, &ASize ) override
		{
			AAddress = FAddress;
			ASize = sizeof( T );
		}
*/
//	public:
//		uint8_t	FMask;

	public:
/*
		virtual void PopulateAddress( uint32_t &AAddress, byte &AOffset ) override
		{
			FMask = 1 << AOffset;
			FAddress = AAddress;
			if( ++AOffset >= 8 )
			{
				++ AAddress;
				AOffset = 0;
			}
		}
*/
		inline void ProcessWrite()
		{
			if( RememberInputPin_o_IsConnected() )
			{
				if( ! FRememberClocked() )
					return;

				FRememberClocked() = false;
			}

			else if( ! FModified() )
				return;

			FModified() = false;

//			Serial.println( "WRITE" );

			uint8_t AByteValue;
			if( C_OWNER.ReadData( C_Address, 1, &AByteValue ))
			{
				if( FValue() )
					AByteValue |= C_Mask;

				else
					AByteValue &= ~ C_Mask;

				C_OWNER.WriteData( C_Address, 1, &AByteValue );

				if( RecallOnRemember() || ( ! RecallInputPin_o_IsConnected() ))
					RecallInputPin_o_Receive( nullptr );
			}
		}

	public:
		inline void RememberInputPin_o_Receive( void *_Data )
		{
			FRememberClocked() = true;
		}

		void RecallInputPin_o_Receive( void *_Data )
		{
//			Serial.print( "READ: " ); Serial.println( FAddress, HEX );

			uint8_t AByteValue;
			if( C_OWNER.ReadData( C_Address, 1, &AByteValue ))
			{
				bool AValue = (( AByteValue & C_Mask ) != 0 );
				T_OutputPin::SetPinValue( AValue );
			}
		}

		inline void InputPin_o_Receive( void *_Data )
		{
			FValue() = *(bool*)_Data;
			FModified() = true;
		}

	public:
		inline DigitalMemoryElement()
		{
			FRememberClocked() = false;
			FModified() = false;
			FValue() = false;
		}

	};
//---------------------------------------------------------------------------
}

#endif
