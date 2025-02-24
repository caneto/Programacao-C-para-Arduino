////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_PACKET_h
#define _MITOV_PACKET_h

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

#include <Mitov.h>
#include <Mitov_Structure.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	class BasicPacket
	{
	protected:
		uint32_t	FBufferSize;
		uint32_t	FCurrentSize;

		uint8_t *FBuffers[ 2 ];

	public:
		void CheckSize(	uint32_t ASize )
		{
			if( ASize <= FCurrentSize )
				return;

			uint8_t *AOldBuffer = FBuffers[ 0 ];
			FBuffers[ 0 ] = new uint8_t[ ASize ];
			memcpy( FBuffers[ 0 ], AOldBuffer, FCurrentSize );
			delete [] AOldBuffer;

			AOldBuffer = FBuffers[ 1 ];
			FBuffers[ 1 ] = new uint8_t[ ASize ];
			memcpy( FBuffers[ 1 ], AOldBuffer, FCurrentSize );
			delete [] AOldBuffer;

			FCurrentSize = ASize;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated,
		typename T_PIN,
		typename T
	> class BasicTypedPacketSinkElement :
		public T_FPopulated
	{
		_V_PROP_( FPopulated )

	protected:
		T	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			T AValue = *(T_PIN*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

//			Serial.println( "..." );
			FPopulated() = true;
			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = true;
//			Serial.println( sizeof( double ) );
			AResult = sizeof( T ) * 8; 
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{ 
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

			C_OWNER.CheckSize( ADataIndex + sizeof( T ) );

#ifdef VISUINO_MEMORY_ALLIGN
			memcpy( ADataStart + ADataIndex, &FValue, sizeof( T ));
#else
			*(T*)( ADataStart + ADataIndex ) = FValue;
#endif
			ADataIndex += sizeof( T );
		}

		inline void Expand( uint8_t * const & AInBuffer, uint8_t * const & AOutBuffer, int & ASize, bool & AResult ) 
		{ 
			AResult = false;
		}

	public:
		BasicTypedPacketSinkElement( bool APopulated )
		{
			FPopulated() = APopulated;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OutputPin,
		typename T_PIN,
		typename T
	> class BasicTypedUnpacketSourceElement : 
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
//		virtual int  Start( bool &AAllign ) override
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = true; 
			AResult = sizeof( T ) * 8; 
		}

//		virtual bool Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset ) override
		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{
			if( AOffset )
			{
				if( ! AAccumulator->SkipBytes( 1 ))
				{
					AResult = false;
					return;
				}

				AOffset = 0;
			}

			T_PIN AValue;
			if( ! AAccumulator->ExtractBytes( sizeof( T ), &AValue ))
			{
				AResult = false;
				return;
			}

			T_OutputPin::SetPinValue( AValue );
//			OutputPin.Notify( &AValue );
			AResult = true;
		}

//		virtual bool ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket ) override
		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{ 
			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;
			APosition += sizeof( T ) * 8;
			AResult = false;
		}
	};
//---------------------------------------------------------------------------
	template<typename T_OutputPin> class UnpacketDigitalBinaryElement : public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = false;
			AResult = 1; 
		}

		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{
			uint8_t AValue;
			if( ! AAccumulator->PeekBytes( 1, &AValue ))
			{
				AResult = false;
				return;
			}

			bool ABoolValue = (( AValue & ( 1 << AOffset )) != 0 );
			++AOffset;

			if( AOffset >= 8 )
			{
				AOffset = 0;
				if( ! AAccumulator->SkipBytes( 1 ))
				{
					AResult = false;
					return;
				}
			}

			T_OutputPin::SetPinValue( ABoolValue );
//			OutputPin.Notify( &ABoolValue );

			AResult = true;
		}

		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{ 
			++ APosition;
			AResult = false;
		}
	};
//---------------------------------------------------------------------------
	template<typename T_OutputPin> class UnpacketTextBinaryElement : public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = false;
			AResult = 1; 
		}

		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{
			uint32_t ASize = 0;
			uint8_t AShift = 0;
			uint8_t AByteSize;
			do
			{
				if( ! AAccumulator->PeekBytes( 1, &AByteSize ))
				{
					AResult = false;
					return;
				}

				AAccumulator->SkipBytes( 1 );

				ASize += ( AByteSize & 0x7F ) << ( AShift * 7 );

				++ AShift;
			}
			while( AByteSize & 0x80 );

			uint8_t *AData = new uint8_t[ ASize + 1 ];

			if( ! AAccumulator->PeekBytes( ASize, AData ))
			{
				delete [] AData;
				AResult = false;
				return;
			}

			AAccumulator->SkipBytes( ASize );

			AData[ ASize ] = '\0';

			T_OutputPin::SetPinValue( (char *)AData );

			delete [] AData;

			AResult = true;
		}

		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{ 
			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

			AResult = false;

			uint32_t	ASize1 = 0;
			uint8_t		AShift = 0;
			uint8_t		AByteSize;
			do
			{
				if( ! AInAccumulator->PeekBytesFrom( ( APosition / 8 ) + AShift, 1, &AByteSize ))
					return;

				ASize1 += ( AByteSize & 0x7F ) << ( AShift * 7 );
				++ AShift;
			}
			while( AByteSize & 0x80 );


//			uint8_t AValue;
//			if( ! AOperation.InAccumulator->PeekBytesFrom( AOperation.Position / 8, 1, &AValue ))
//				return;

			if( AInAccumulator->size() < ASize1 + AShift )
			{
				AIsValid = false;
				return;
			}

			APosition += ( ASize1 + AShift ) * 8;
		}

	};
//---------------------------------------------------------------------------
	template<typename T, typename T_OutputPin> class UnpacketArrayBinaryElement : public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = false;
			AResult = 1; 
		}

		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{
			uint32_t ASize = 0;
			uint8_t AShift = 0;
			uint8_t AByteSize;
			do
			{
				if( ! AAccumulator->PeekBytes( 1, &AByteSize ))
				{
					AResult = false;
					return;
				}

				AAccumulator->SkipBytes( 1 );

				ASize += ( AByteSize & 0x7F ) << ( AShift * 7 );

				++ AShift;
			}
			while( AByteSize & 0x80 );

			T *AData = new T[ ASize ];

			if( ! AAccumulator->PeekBytes( ASize * sizeof( T ), AData ))
			{
				delete [] AData;
				AResult = false;
				return;
			}

			AAccumulator->SkipBytes( ASize * sizeof( T ) );

			Mitov::TArray<T> ADataBlock( ASize, AData );
			T_OutputPin::SetPinValue( ADataBlock );

			delete [] AData;

			AResult = true;
		}

		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{ 
			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

			AResult = false;

			uint32_t	ASize1 = 0;
			uint8_t		AShift = 0;
			uint8_t		AByteSize;
			do
			{
				if( ! AInAccumulator->PeekBytesFrom( ( APosition / 8 ) + AShift, 1, &AByteSize ))
					return;

				ASize1 += ( AByteSize & 0x7F ) << ( AShift * 7 );
				++ AShift;
			}
			while( AByteSize & 0x80 );


//			uint8_t AValue;
//			if( ! AOperation.InAccumulator->PeekBytesFrom( AOperation.Position / 8, 1, &AValue ))
//				return;

			ASize1 *= sizeof( T );
			if( AInAccumulator->size() < ASize1 + AShift )
			{
				AIsValid = false;
				return;
			}

			APosition += ( ASize1 + AShift ) * 8;
		}

	};
//---------------------------------------------------------------------------
	class UnpacketMarkerBinaryElement
	{
	public:
		Mitov::Bytes	Bytes;

	public:
//		virtual int  Start( bool &AAllign ) override
		void Start( bool &AAllign, int &AResult )
		{ 
			AAllign = true; 
			AResult = Bytes.GetSize() * 2 * 8; 
		}

//		virtual bool ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket ) override
		void ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket, bool &AResult )
		{ 
			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

			int ABytesSize = Bytes.GetSize();
			int ASearchPos;
			if( ! AInAccumulator->FindBytes( ABytesSize, Bytes._Bytes, APosition / 8, Bytes.GetSize(), ASearchPos ))
				AIsCorruptedPacket = true;

			else if( ASearchPos != APosition / 8 + ABytesSize )
				AIsCorruptedPacket = true;

			APosition += ABytesSize * 8;
			AResult = false; 
		}

//		virtual bool Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset ) override
		void Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset, bool &AResult )
		{ 
			if( AOffset != 0 )
			{
				if( ! AAccumulator->SkipBytes( 1 ))
				{
					AResult = false;
					return;
				}

				AOffset = 0;
			}

			AResult = AAccumulator->SkipBytes( Bytes.GetSize() );
//			if( ! AAccumulator->SkipBytes( Bytes.GetSize() ))
//				return false;

//			return true; 
		}
	};
//---------------------------------------------------------------------------
/*
	class UnpacketChecksumElement
	{
	public:
		bool Enabled = true;

	public:
		void ProcessData( uint32_t ASelfIndex, uint32_t AIndex, PacketTypes::TBasicOperation *AOperation )
		{
			if( ASelfIndex != AIndex )
				return;

			switch( AOperation->Operation )
			{
				case PacketTypes::Process:
				{
					Process( *( PacketTypes::TProcessOperation *)AOperation );
					break;
				}

				case PacketTypes::ExpandBuffers:
				{
					ExpandBuffers( *( PacketTypes::TExpandBuffersOperation *)AOperation );
					break;
				}
			}

		}

	public:
//		virtual int  Start( bool &AAllign ) override
		void Start( bool &AAllign, int &AResult )
        {
			if( ! Enabled )
			{
				AResult = 0;
				return;
			}

            AAllign = true;
            AResult = 8;
        }

//		virtual bool ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket ) override
		void ExpandBuffers( PacketTypes::TExpandBuffersOperation &AOperation )
        {
			if( ! Enabled )
			{
				AOperation.Result = false;
                return;
			}

			AOperation.Position = ( AOperation.Position + 7 ) / 8;
			AOperation.Position *= 8;

            uint8_t *ABytes = new uint8_t[ AOperation.Position / 8 + 1 ];
//            SetLength( ABytes, AOperation.Position + 1 );

            if( ! AOperation.InAccumulator->PeekBytes( AOperation.Position / 8 + 1, ABytes ))
            {
                AOperation.IsValid = false;
                delete [] ABytes;
				AOperation.Result = false;
                return;
            }

            uint8_t AChecksum = 0;

//            for AByte in ABytes do
//                AChecksum ^= AByte;
            for( int i = 0; i < AOperation.Position / 8 + 1; ++i )
                AChecksum ^= ABytes[ i ];

            delete [] ABytes;

            if( AChecksum )
            {
                AOperation.IsCorruptedPacket = true;
				AOperation.Result = false;
                return;
            }

            AOperation.Position += 8;
			AOperation.Result = false;
        }

//		virtual bool Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset ) override
		void Process( PacketTypes::TProcessOperation &AOperation )
        {
			if( ! Enabled )
			{
				AOperation.Result = true;
                return;
			}

            if( AOperation.Offset )
            {
                if( ! AOperation.Accumulator->SkipBytes( 1 ))
				{
					AOperation.Result = false;
					return;
				}

                AOperation.Offset = 0;
            }

            AOperation.Accumulator->SkipBytes( 1 );
			AOperation.Result = true;
        }
	};
*/
//---------------------------------------------------------------------------
	template<typename T_OWNER, T_OWNER &C_OWNER> class PacketMarkerBinaryElement
	{
	public:
		Mitov::Bytes	Bytes;

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= true;
		}

//		virtual	int	 GetSize( bool &Alligned ) override
		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = true;
			AResult = Bytes.GetSize() * 8;
		}

//		virtual	void GetData( uint8_t *ADataStart, uint8_t *&AData, uint8_t &AOffset ) override
		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( Bytes.GetSize() == 0 )
				return;

			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );
			C_OWNER.CheckSize( ADataIndex + Bytes.GetSize() );

			memcpy( ADataStart + ADataIndex, Bytes._Bytes, Bytes.GetSize() );
			ADataIndex += Bytes.GetSize();
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult ) 
		{ 
			AResult = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled
	> class PacketChecksumElement :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= true;
		}

		inline int GetSize( bool &AAlligned )
		{ 
			if( Enabled() )
			{
				AAlligned = true;
				return 8;
			}

			return 0;
		}

//		virtual	void GetData( uint8_t *ADataStart, uint8_t *&AData, uint8_t &AOffset ) override
		inline void GetData( BasicPacket *AOwner, uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( ! Enabled() )
				return;

			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

//			BasicPacket &AInstance = *(( BasicPacket *) AOperation.Instance );
			AOwner->CheckSize( ADataIndex + 1 );

			uint8_t	AChecksum = 0;
			uint8_t *AData = ADataStart;
			for( int i = 0; i < ADataIndex; ++ i )
				AChecksum ^= *AData ++;

//			for( ; AOperation.DataStart < AOperation.Data; ++ AOperation.DataStart )
//				AChecksum ^= *AOperation.DataStart;

			*AData = AChecksum;
//			AOperation.DataStart[ AOperation.DataIndex ] = AChecksum;
			++ ADataIndex;
		}

		inline bool Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize ) 
		{ 
			return false;
		}

	public: // Unpack
		int Start( bool &AAllign )
        {
			if( ! Enabled() )
				return 0;

            AAllign = true;
            return 8;
        }

//		virtual bool ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket ) override
		bool ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket )
        {
			if( ! Enabled() )
				return false;

			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

            uint8_t *ABytes = new uint8_t[ APosition / 8 + 1 ];
//            SetLength( ABytes, AOperation.Position + 1 );

            if( ! AInAccumulator->PeekBytes( APosition / 8 + 1, ABytes ))
            {
                AIsValid = false;
                delete [] ABytes;
				return false;
            }

            uint8_t AChecksum = 0;

//            for AByte in ABytes do
//                AChecksum ^= AByte;
            for( int i = 0; i < APosition / 8 + 1; ++i )
                AChecksum ^= ABytes[ i ];

            delete [] ABytes;

            if( AChecksum )
            {
                AIsCorruptedPacket = true;
				return false;
            }

            APosition += 8;
			return false;
        }

//		virtual bool Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset ) override
		bool Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset )
        {
			if( ! Enabled() )
				return true;

            if( AOffset )
            {
                if( ! AAccumulator->SkipBytes( 1 ))
					return false;

                AOffset = 0;
            }

            AAccumulator->SkipBytes( 1 );
			return true;
        }
	};
//---------------------------------------------------------------------------
	class UnpacketHeadMarkerBinaryElement
	{
	public:
		Mitov::Bytes	Bytes;

//	protected:
//		Mitov::SimpleList<uint8_t>	FDoubledBytes;

	public:
//		virtual int  Start( bool &AAllign ) override
		int Start( bool &AAllign )
		{ 
//			for( unsigned int i = 0; i < Bytes.GetSize(); ++i )
//				FDoubledBytes.push_back( Bytes._Bytes[ i ] );

//			for( unsigned int i = 0; i < Bytes.GetSize(); ++i )
//				FDoubledBytes.push_back( Bytes._Bytes[ i ] );

			AAllign = true; 
			return Bytes.GetSize() * 2 * 8; 
		}

//		virtual bool Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset ) override
		bool Process( TPacketAccumulator *AAccumulator, uint8_t &AOffset )
		{
			if( AOffset )
			{
				if( ! AAccumulator->SkipBytes( 1 ))
					return false;

				AOffset = 0;
			}

			//  AOldDebugBuf := AAccumulator.GetAsBuffer();
			AAccumulator->SkipBytes( Bytes.GetSize() * 2 );
			return true;
		}

//		virtual bool ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket ) override
		bool ExpandBuffers( TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket )
		{ 
			APosition = ( APosition + 7 ) / 8;
			APosition *= 8;

			int ABytesSize = Bytes.GetSize();
			if( !ABytesSize )
				return false;

			uint8_t *ABytes = Bytes._Bytes;
			//  if( not AOperation.InAccumulator.FindSkipBytes( ABytesSize * 2, @FDoubledBytes[ 0 ], ASkipped )) then
			//    Exit( False );

			int AFindPosition;

			uint8_t *ADoubledBytes = new uint8_t[ ABytesSize * 2 ];
			memcpy( ADoubledBytes, ABytes, ABytesSize );
			memcpy( ADoubledBytes + ABytesSize, ABytes, ABytesSize );
//			for( unsigned int i = 0; i < Bytes.GetSize(); ++i )
//				ADoubledBytes[ i ] Bytes._Bytes[ i ] );

			bool AFindResult = AInAccumulator->FindBytes( ABytesSize * 2, ADoubledBytes, AFindPosition );
			delete [] ADoubledBytes;

			if( ! AFindResult )
			{
				AIsValid = false;
				return false;
			}

			AFindPosition -= ABytesSize * 2;
			AInAccumulator->SkipBytes( AFindPosition );

			APosition += ABytesSize * 2 * 8;

			//  Dec( AOperation.Size, AFindPosition );

			//  if( AOperation.Size <= ABytesSize * 2 ) then
			//    Exit( False );

			//  ANewDebugBuf := AOperation.InAccumulator.GetAsBuffer();

			if( ! AInAccumulator->FindBytes( ABytesSize, ABytes, ABytesSize * 2, ASize - ABytesSize * 2, AFindPosition ))
				return false;

			//  ANewDebugBuf := AOperation.InAccumulator.GetAsBuffer();
//			AMarkerBuffer := TSLBlockBuffer.CreateData( ABytes, ABytesSize );

			//  if( not AOperation.InAccumulator.FindBytes( ABytesSize, ABytes, AOperation.Size - ABytesSize * 2, AFindPosition )) then
			//    Exit( False );

			AOutAccumulator->append( ABytes, ABytesSize );
			AOutAccumulator->append( ABytes, ABytesSize );

			for(;;)
			{
				Mitov::SimpleList<uint8_t>	ABuffer;
				ABuffer.AddCount( AFindPosition );
//				ABuffer := TSLBlockBuffer.CreateSize( AFindPosition );
				AInAccumulator->ExtractBytes( AFindPosition, ABuffer );
				AOutAccumulator->append( ABuffer, AFindPosition );

				uint8_t ACount;
				AInAccumulator->ExtractBytes( 1, &ACount );

				for( int i = 0; i <= ACount; ++i )
					AOutAccumulator->append( ABytes, ABytesSize );

				ASize = ABytesSize * ACount + 1;

				if( ! AInAccumulator->FindBytes( ABytesSize, ABytes, AFindPosition, ASize - AFindPosition, AFindPosition ))
					return true;
			}

			return true;
		}

	public:
		inline void SkipHeader( TPacketAccumulator *AAccumulator ) 
		{
			AAccumulator->SkipBytes( Bytes.GetSize() * 2 );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_OWNER> class PacketHeadMarkerBinaryElement
	{
	public:
		Mitov::Bytes	Bytes;

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= true;
		}

		inline int GetSize( bool &AAlligned )
		{ 
			AAlligned = true;
			return Bytes.GetSize() * 2 * 8; 
		}

//		virtual	void GetData( uint8_t *ADataStart, uint8_t *&AData, uint8_t &AOffset ) override
		inline void GetData( T_OWNER *AOwner, uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{
			if( Bytes.GetSize() == 0 )
				return;

			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );

			AOwner->CheckSize( ADataIndex + Bytes.GetSize() * 2 );

			memcpy( ADataStart + ADataIndex, Bytes._Bytes, Bytes.GetSize() );
			ADataIndex += Bytes.GetSize();

			memcpy( ADataStart + ADataIndex, Bytes._Bytes, Bytes.GetSize() );
			ADataIndex += Bytes.GetSize();
		}

//		virtual	bool Expand( uint8_t *AInBuffer, uint8_t *AOutBuffer, int &ASize ) override
		inline bool Expand( T_OWNER *AOwner, uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize ) 
		{
			if( Bytes.GetSize() == 0 )
				return false;

//			memcpy( AOperation.OutBuffer, AOperation.InBuffer, AOperation.Size );
//			return true;

			uint8_t *ABytes = Bytes._Bytes;
			int ABytesSize = Bytes.GetSize();

			int AOutSize = ABytesSize * 2;

//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );
//			Serial.println( "Expand 1" );
//			Serial.println( AOperation.Size );
			AOwner->CheckSize( AOutSize );
			memcpy( AOutBuffer, AInBuffer, AOutSize );

			uint32_t AInIndex = AOutSize;
			uint32_t AOutIndex = AOutSize;

//			AOperation.OutBuffer += AOutSize;
//			AOperation.InBuffer += AOutSize;

			int i = AOutSize;
			for( ; i < ASize - ABytesSize * 2 + 1; ++i )
			{
//				Serial.println( "Step 1" );
//				Serial.println( AOperation.Size );
				if( memcmp( AInBuffer + AInIndex, ABytes, ABytesSize ) == 0 )
					if( memcmp( AInBuffer + AInIndex + ABytesSize, ABytes, ABytesSize ) == 0 )
					{
//						Serial.println( "Step 2" );
//						Serial.println( AOperation.Size );
						AOwner->CheckSize( ABytesSize + MitovMax( AInIndex, AOutIndex ) );
						memcpy( AOutBuffer + AOutIndex, AInBuffer + AInIndex, ABytesSize );
						AInIndex += ABytesSize * 2;
//						AOperation.InBuffer += ABytesSize * 2;

//						Serial.println( "START" );
						AOutIndex += ABytesSize;
//						AOperation.OutBuffer += ABytesSize;
						uint8_t ACount = 0;
						i += ABytesSize;
						for( ; i < ASize - ABytesSize + 1; i += ABytesSize )
						{
							if( memcmp( AInBuffer + AInIndex, ABytes, ABytesSize ) != 0 )
								break;

//							Serial.println( "INC" );
							++ACount;
							if( ACount == 255 )
								break;

//							AOperation.InBuffer += ABytesSize;
							AInIndex += ABytesSize;
						}

						if( ACount == *ABytes )
							if( ACount > 0 )
							{
//								Serial.println( "DEC" );
								--ACount;
								AInIndex -= ABytesSize;
//								AOperation.InBuffer -= ABytesSize;
								i -= ABytesSize;
							}

						
//						Serial.println( "Step 3" );
//						Serial.println( AOperation.Size );
						AOwner->CheckSize( AOutIndex + 1 );
						AOutBuffer[ AOutIndex++ ] = ACount;
						AOutSize += ABytesSize + 1;
						continue;
					}

//				Serial.println( "Step 4" );
//				Serial.println( AOperation.Size );
//				Serial.println( AInIndex );
//				Serial.println( AOutIndex );
//				delay( 1000 );
//				Serial.println( uint32_t( AOperation.OutBuffer ));
				AOwner->CheckSize( MitovMax( AInIndex, AOutIndex ) + 1 );
//				Serial.println( uint32_t( AOperation.OutBuffer ));
//				Serial.println( "Step 4 +" );
//				Serial.println( AOperation.Size );
//				delay( 1000 );
				AOutBuffer[ AOutIndex ++ ] = AInBuffer[ AInIndex ++ ];
//Serial.print( "++AOutSize" );
				++AOutSize;
			}

			int ACorrection = ASize - i;
//			Serial.println( "Step 5" );
//			Serial.println( AOperation.Size );
//			Serial.println( AInIndex );
//			Serial.println( AOutIndex );
//			Serial.println( ACorrection );
//			delay( 1000 );
			AOwner->CheckSize( ACorrection + MitovMax( AInIndex, AOutIndex ) );
			memcpy( AOutBuffer + AOutIndex, AInBuffer + AInIndex, ACorrection );

//			Serial.println( "Step 6" );
//			Serial.println( AOperation.Size );
//			Serial.println( AOutSize );
//			Serial.println( ACorrection );
//			delay( 1000 );
//			*AOperation.OutBuffer = 5;

//			Serial.print( "ADebugCount : " );
//			Serial.println( ADebugCount );

//			Serial.print( "AOutSize : " );
//			Serial.println( AOutSize );

//			Serial.print( "TEST : " );
//			Serial.println( ACorrection );
//			Serial.println( i );

			ASize = AOutSize + ACorrection;
			return true;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FBoolValue,
		typename T_FPopulated
	> class PacketDigitalBinaryElement :
		public T_FBoolValue,
		public T_FPopulated
	{
	public:
		_V_PROP_( FBoolValue )

	protected:
		_V_PROP_( FPopulated )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FBoolValue() )
					return;

			FPopulated() = true;
			FBoolValue() = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = false;
			AResult = 1; 
		}

//		virtual	void GetData( uint8_t *ADataStart, uint8_t *&AData, uint8_t &AOffset ) override
		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{
//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );
			C_OWNER.CheckSize( ADataIndex );

			uint8_t *AData = ADataStart + ADataIndex;
			*AData &= 0xFF >> ( 8 - AOffset ); // Zero the upper bits
			if( FBoolValue() )
				*AData |= 1 << AOffset;	// Set the bit

			++AOffset;
			if( AOffset >= 8 )
			{
				AOffset = 0;
				++ ADataIndex;
			}
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult ) 
		{ 
			AResult = false;
		}

	public:
		PacketDigitalBinaryElement( bool APopulated )
		{
			FBoolValue() = false;
			FPopulated() = APopulated;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated
	> class PacketTextBinaryElement :
		public T_FPopulated
	{
	protected:
		_V_PROP_( FPopulated )

	protected:
		Mitov::String	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			Mitov::String AValue = (char*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

//			Serial.println( "..." );
			FPopulated() = true;
			if( FValue.length() != AValue.length() )
				C_OWNER.FNeedsNewSize() = true;

			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = true;

			uint32_t ALength = FValue.length();
			uint32_t ALength1 = ALength;

			uint8_t ACount = 0;
			do
			{
				++ ACount;
				ALength1 >>= 7;
			}
			while( ALength1 );

			AResult = ( ALength + ACount ) * 8;
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{ 
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

			uint32_t ALength = FValue.length();
			uint32_t ALength1 = ALength;
//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );

			uint8_t ACount = 0;
			do
			{
				++ ACount;
				C_OWNER.CheckSize( ADataIndex + ALength + ACount );
				ADataStart[ ADataIndex ] = ALength1 & 0x7F;

				ALength1 >>= 7;
				if( ALength1 )
					ADataStart[ ADataIndex ] |= 0x80;

				ADataIndex ++;
			}
			while( ALength1 );

//			AOperation.DataStart[ AOperation.DataIndex ] = ALength;
			memcpy( ADataStart + ADataIndex, FValue.c_str(), ALength );
			ADataIndex += ALength;
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult ) 
		{ 
			AResult = false;
		}

	public:
		PacketTextBinaryElement( bool APopulated )
		{
			FPopulated() = APopulated;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_FPopulated,
		typename T
	> class PacketArrayBinaryElement :
		public T_FPopulated
	{
		_V_PROP_( FPopulated )

	protected:
		TValueArray<T>	FValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TArray<T> &AValue = *(TArray<T>*)_Data;
			C_OWNER.FRefreshed() = true;
			if( FPopulated() )
				if( AValue == FValue )
					return;

//			Serial.println( "..." );
			FPopulated() = true;
			if( FValue.Size != AValue.Size )
				C_OWNER.FNeedsNewSize() = true;

			FValue = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void CheckPopulated( bool &AIsPopulated )
		{
			AIsPopulated &= FPopulated();
		}

		inline void GetSize( bool &AAlligned, int &AResult )
		{ 
			AAlligned = true;

			uint32_t ALength = FValue.Size;
			uint32_t ALength1 = ALength;

			uint8_t ACount = 0;
			do
			{
				++ ACount;
				ALength1 >>= 7;
			}
			while( ALength1 );

			AResult = ( ALength * sizeof( T ) + ACount ) * 8;
		}

		inline void GetData( uint8_t * &ADataStart, uint32_t &ADataIndex, uint8_t &AOffset )
		{ 
			if( AOffset )
			{
				++ ADataIndex;
				AOffset = 0;
			}

			uint32_t ALength = FValue.Size;
			uint32_t ALength1 = ALength;
//			T_OWNER &AInstance = *(( T_OWNER *) AOperation.Instance );

			uint8_t ACount = 0;
			do
			{
				++ ACount;
				C_OWNER.CheckSize( ADataIndex + ALength + ACount );
				ADataStart[ ADataIndex ] = ALength1 & 0x7F;

				ALength1 >>= 7;
				if( ALength1 )
					ADataStart[ ADataIndex ] |= 0x80;

				ADataIndex ++;
			}
			while( ALength1 );

			ALength *= sizeof( T );
			memcpy( ADataStart + ADataIndex, FValue.Data, ALength );
			ADataIndex += ALength;
		}

		inline void Expand( uint8_t * const &AInBuffer, uint8_t * const &AOutBuffer, int &ASize, bool &AResult ) 
		{ 
			AResult = false;
		}

	public:
		PacketArrayBinaryElement( bool APopulated )
		{
			FPopulated() = APopulated;
		}

	};
//---------------------------------------------------------------------------
	template<
		uint32_t C_COUNT, 
		typename T_Checksum,
		typename T_Elements_CheckPopulated,
		typename T_Elements_Expand,
		typename T_Elements_GetData,
		typename T_Elements_Elements_GetSize,
		typename T_FModified,
		typename T_FNeedsNewSize,
		typename T_FRefreshed,
		typename T_OnlyModified,
		typename T_OutputPin
	> class Packet : public BasicPacket,
		public T_Checksum,
		public T_FModified,
		public T_FNeedsNewSize,
		public T_FRefreshed,
		public T_OnlyModified,
		public T_OutputPin
	{
		typedef Packet<		
			C_COUNT, 
			T_Checksum,
			T_Elements_CheckPopulated,
			T_Elements_Expand,
			T_Elements_GetData,
			T_Elements_Elements_GetSize,
			T_FModified,
			T_FNeedsNewSize,
			T_FRefreshed,
			T_OnlyModified,
			T_OutputPin
		> SELF;

	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( OnlyModified )

	public:
		_V_PROP_( FNeedsNewSize )
		_V_PROP_( FModified )
		_V_PROP_( FRefreshed )
//		float	SizeReserve	= 2.0;

		PacketHeadMarkerBinaryElement<SELF>	HeadMarker;
		_V_PROP_( Checksum )

	protected:
		inline bool IsPopulated()
		{
			bool AResult = true;
			T_Elements_CheckPopulated::ChainCall( AResult );
			return AResult;
		}

		inline void GetData( uint8_t *&ADataStart, uint32_t &ADataIndex, uint8_t &AOffset ) 
		{
			HeadMarker.GetData( this, ADataStart, ADataIndex, AOffset );
			T_Elements_GetData::ChainCall( ADataStart, ADataIndex, AOffset );
			Checksum().GetData( this, ADataStart, ADataIndex, AOffset );

//			PacketTypes::TGetDataOperation AOperation( this, ADataStart, ADataIndex, AOffset );
//			ProcessCallChain( AIndex, &AOperation );
		}

		inline bool Expand( uint32_t AIndex, uint8_t *&AInBuffer, uint8_t *&AOutBuffer, int &ASize ) 
		{  
			if( ! AIndex )
				return HeadMarker.Expand( this, AInBuffer, AOutBuffer, ASize );

			if( AIndex == ( C_COUNT + 1 ))
				return Checksum().Expand( AInBuffer, AOutBuffer, ASize );

			bool AResult = false;
			T_Elements_Expand::ChainCall( AIndex - 1, AInBuffer, AOutBuffer, ASize, AResult );
			return AResult;
		}

		inline int GetSize( uint32_t AIndex, bool &AAlligned )
		{
			if( ! AIndex )
				return HeadMarker.GetSize( AAlligned );

			if( AIndex == ( C_COUNT + 1 ))
				return Checksum().GetSize( AAlligned );

			int AResult;
			T_Elements_Elements_GetSize::ChainCall( AIndex - 1, AAlligned, AResult );
			return AResult;
		}

	protected:
		void RecalculateSize()
		{
			FBufferSize = 0;
//			FAllElements.push_back( &HeadMarker );
//			for( int i = 0; i < Elements.size(); ++i )
//				FAllElements.push_back( Elements[ i ] );

//			FAllElements.push_back( &Checksum );

			for( int i = 0; i < C_COUNT + 2; ++i )
			{
				bool AAlligned = false;
				FBufferSize += GetSize( i, AAlligned );
//				Serial.println( FBufferSize );
				if( AAlligned )
				{
					FBufferSize = ( FBufferSize + 7 ) / 8;
					FBufferSize *= 8;
				}
			}

//			Serial.println( FBufferSize );

			FBufferSize = ( FBufferSize + 7 ) / 8;
			FCurrentSize = uint32_t( FBufferSize * 1.3 );
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "++++++" );
//			int AElementCount = FAllElements.size();
			if( OnlyModified() )
			{
				if( ! FModified() )
					return;
			}

			else if( FRefreshed() )
			{
				if( ! IsPopulated())
					return;

			}

			if( FNeedsNewSize() )
				RecalculateSize();

			FBuffers[ 0 ] = new uint8_t[ FCurrentSize ];
			FBuffers[ 1 ] = new uint8_t[ FCurrentSize ];

//			uint8_t *ADataPtr = FBuffers[ 0 ];
			uint32_t	ADataIndex = 0;
			uint8_t AOffset = 0;
			GetData( FBuffers[ 0 ], ADataIndex, AOffset );
//			for( int i = 0; i < C_COUNT + 2; ++i )
//			{
//				GetData( i, FBuffers[ 0 ], ADataIndex, AOffset );
//				while( AElementSize-- )
//					OutputPin.Notify( AData++ );


//			}

			int ASize = FBufferSize;
//			Serial.println( ASize );
			uint8_t ABufferIndex = 0;
//			Serial.println( "--------" );
			for( int i = 0; i < C_COUNT + 2; ++i )
			{
//				Serial.println( ABufferIndex );
//				Serial.println( 1 ^ ( ABufferIndex & 1 ) );
				uint8_t *&AInBuffer = FBuffers[ ABufferIndex ];
				uint8_t *&AOutBuffer = FBuffers[ 1 ^ ABufferIndex ];
				if( Expand( i, AInBuffer, AOutBuffer, ASize ))
					ABufferIndex ^= 1;

			}

//			Serial.println( "SendPinValue" );
//			Serial.println( ASize );
//			delay( 1000 );

			delete [] FBuffers[ 1 ^ ABufferIndex ];

			FModified() = false;
			FRefreshed() = false;

			uint8_t *ADataPtr = FBuffers[ ABufferIndex ];
			T_OutputPin::SendPinValue( Mitov::TDataBlock( ASize, ADataPtr ));

			delete [] FBuffers[ ABufferIndex ];

//			Serial.println( ASize );
//			while( ASize-- )
//				OutputPin.Notify( ADataPtr++ );

		}

	public:
		inline void SystemLoopBegin()
		{
			if( FRefreshed() )
				ClockInputPin_o_Receive( nullptr );

		}

		inline void SystemStart()
		{
			RecalculateSize();
		}

		Packet()
		{
			FNeedsNewSize() = false;
			FModified() = false;
			FRefreshed() = true;
		}

/*
		virtual ~Packet()
		{
			delete []FBuffers[ 1 ];
			delete []FBuffers[ 0 ];
		}
*/
	};
//---------------------------------------------------------------------------
	template<
		uint32_t C_COUNT,
		typename T_Checksum,
		typename T_Elements_ExpandBuffers,
		typename T_Elements_Process,
		typename T_Elements_Start
	> class Unpacket :
		public T_Checksum
	{
	public:
		UnpacketHeadMarkerBinaryElement	HeadMarker;
		_V_PROP_( Checksum )

	protected:
		TPacketAccumulator	FAccumulator;
		int					FMinSize = 0;
//		Mitov::SimpleList<BasicUnpacketElement*>	FAllElements;

//	public:
//		void ( *Elements_CallChain )( uint32_t AIndex, PacketTypes::TBasicOperation *AOperation ) = nullptr;

	protected:
		bool ExpandBuffers( uint32_t AIndex, TPacketAccumulator *AInAccumulator, TPacketAccumulator *AOutAccumulator, int &ASize, int &APosition, bool &AIsValid, bool &AIsCorruptedPacket )
		{
			if( ! AIndex )
				return HeadMarker.ExpandBuffers( AInAccumulator, AOutAccumulator, ASize, APosition, AIsValid, AIsCorruptedPacket );

			if( AIndex == ( C_COUNT + 1 ))
				return Checksum().ExpandBuffers( AInAccumulator, AOutAccumulator, ASize, APosition, AIsValid, AIsCorruptedPacket );

			bool AResult = false;

			T_Elements_ExpandBuffers::ChainCall( AIndex - 1, AInAccumulator, AOutAccumulator, ASize, APosition, AIsValid, AIsCorruptedPacket, AResult );

			return AResult;
		}

		inline int Start( uint32_t AIndex, bool &AAllign )
		{
			if( ! AIndex )
				return HeadMarker.Start( AAllign );

			if( AIndex == ( C_COUNT + 1 ))
				return Checksum().Start( AAllign );

			int AResult;
			T_Elements_Start::ChainCall( AIndex - 1, AAllign, AResult );

//			Serial.println( AOperation.Result );

			return AResult;
		}

		bool Process( uint32_t AIndex, TPacketAccumulator *AAccumulator, uint8_t &AOffset )
		{ 
			if( ! AIndex )
				return HeadMarker.Process( AAccumulator, AOffset );

			if( AIndex == ( C_COUNT + 1 ))
				return Checksum().Process( AAccumulator, AOffset );

			bool AResult= false;
			T_Elements_Process::ChainCall( AIndex - 1, AAccumulator, AOffset, AResult );
			return AResult;
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FMinSize == 0 )
				return;

//			Serial.println( "FMinSize" );
//			Serial.println( FMinSize );
//			Serial.println( "T0:" );
//			Serial.println( FAccumulator.size() );

			Mitov::TDataBlock ABlock = *(Mitov::TDataBlock *)_Data;

//			Serial.println( ABlock.Size );

			while( ABlock.Size-- )
			{
//				Serial.print( "+" );
				FAccumulator.push_back( *ABlock.Data++ );
//				Serial.print( "-" );
			}

//			Serial.println( "" );
//			Serial.println( FAccumulator.size() );

//			Serial.println( "T1" );

			TPacketAccumulator	AOldAccumulator;
			TPacketAccumulator	*AInAccumulator;
			TPacketAccumulator	ARealOutAccumulator;
			TPacketAccumulator	*AOutAccumulator = &ARealOutAccumulator;
			TPacketAccumulator	*AAccumulator;

			while( FAccumulator.size() >= FMinSize )
			{
//			Serial.println( "FMinSize" );
//			Serial.println( FMinSize );
//				Serial.println( "T2" );
//AOldAccumulator.clear();
//AOldAccumulator.append( FAccumulator, FAccumulator.size() );
//return;
				AOldAccumulator.assign( FAccumulator );
//return;
				AInAccumulator = &FAccumulator;

				AOutAccumulator->clear();
				AAccumulator = &FAccumulator;
				int ASize = FMinSize;
				bool AIsValid = true;

				bool AIsCorruptedPacket = false;
				int APosition = 0;
//return;
				for( int i = 0; i < C_COUNT + 2; ++i )
				{
//					Serial.println( "enter" );
//					Serial.println( FAccumulator.size() );

					if( ExpandBuffers( i, AInAccumulator, AOutAccumulator, ASize, APosition, AIsValid, AIsCorruptedPacket ))
					{
//						Serial.println( "ExpandBuffers" );
//return;
						if( AIsCorruptedPacket )
						{
							FAccumulator.assign( AOldAccumulator );
							HeadMarker.SkipHeader( &FAccumulator );
							break;
						}

						AAccumulator = AOutAccumulator;
						if( AAccumulator == &FAccumulator )
						{
//							Serial.println( "AAccumulator == &FAccumulator" );
							AInAccumulator = AOutAccumulator;
							AOutAccumulator->clear();
						}

						else
							swap( AInAccumulator, AOutAccumulator );

					}

//					Serial.println( "Step1" );
					if( AIsCorruptedPacket )
					{
//						Serial.println( "AIsCorruptedPacket" );
						FAccumulator.assign( AOldAccumulator );
						HeadMarker.SkipHeader( &FAccumulator );
						break;
					}

					if( ! AIsValid )
					{
						FAccumulator.assign( AOldAccumulator );
//						Serial.println( "Non Valid" );
						return;
					}

				}

//				Serial.println( "ExpandBuffers OUT" );
				if( AIsCorruptedPacket )
				{
//					Serial.println( "AIsCorruptedPacket SKIPPING" );
//return;
//					if( ! HeadMarker.Bytes.GetSize() )
						FAccumulator.SkipBytes( 1 );

					continue;
				}

				if( AAccumulator->size() < FMinSize )
				{
					FAccumulator.assign( AOldAccumulator );
					return;
				}

				if( ASize < FMinSize )
		//      for AItem in FAllElements do
		//        if( not AItem.CanProcess( AAccumulator )) then
				{
					FAccumulator.assign( AOldAccumulator );
					return;
				}

//				Serial.println( "PROCESS" );
				uint8_t AOffset = 0;
				for( int i = 0; i < C_COUNT + 2; ++i )
					if( ! Process( i, AAccumulator, AOffset ))
						break;


//				Serial.println( "exit" );
//				Serial.println( FAccumulator.size() );
//				return;

			}

//			Serial.println( "DoReceive exit" );
		}

	public:
		inline void SystemStart()
		{
//			Serial.println( "FMinSize" );
//			FAllElements.push_back( &HeadMarker );

//			for( int i = 0; i < Elements.size(); ++i )
//				FAllElements.push_back( Elements[ i ] );

//			FAllElements.push_back( &Checksum );

			FMinSize = 0;
			for( int i = 0; i < C_COUNT + 2; ++i )
			{
				  bool AAlligned = false;
				  FMinSize += Start( i, AAlligned );
				  if( AAlligned )
				  {
					  FMinSize = ( FMinSize + 7 ) / 8;
//					  FMinSize = FMinSize * 8;
					  FMinSize *= 8;
				  }
			}

//			Serial.println( "FMinSize" );
//			Serial.println( FMinSize );

			FMinSize = ( FMinSize + 7 ) / 8;

//			Serial.println( "FMinSize" );
//			Serial.println( FMinSize );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif