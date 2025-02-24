////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_VALUE_ARRAY_h
#define _MITOV_VALUE_ARRAY_h

#include <Mitov.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_CONST, 
		typename T_ClockInputPin_o_IsConnected,
		typename T_InitialIndex,
		typename T_OutputPin,
		typename T
	> class ValueArray :
		public T_ClockInputPin_o_IsConnected,
		public T_InitialIndex,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialIndex )

	public:
		T *_Values;
		uint32_t _MaxIndex;

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		inline operator T *() { return _Values; }

	public:
		inline void SystemStart()
		{
			if( InitialIndex() > _MaxIndex )
				InitialIndex() = _MaxIndex;

			UpdateArrayValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( _Values[ InitialIndex() ] );
		}
		
		inline void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > _MaxIndex )
				AIndex = _MaxIndex;

			if( InitialIndex() == AIndex )
				return;

			InitialIndex() = AIndex;

			UpdateArrayValue();
		}

	public:
		void UpdateArrayValue()
		{
			if( ! ClockInputPin_o_IsConnected() )
				T_OutputPin::SetPinValue( _Values[ InitialIndex() ] );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_InitialIndex,
		typename T_OutputPin
	> class ValueArrayDigital :
		public T_ClockInputPin_o_IsConnected,
		public T_InitialIndex,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialIndex )

	public:
		uint8_t *_Values;
		uint32_t _MaxIndex;

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		class _ByteAccess
		{
		protected:
			ValueArrayDigital	*FOwner;
			int					FIndex;

		public:
			void operator = ( bool AValue ) 
			{ 
				if( AValue )
					FOwner->_Values[ FIndex / 8 ] |= 1 << ( FIndex & 0b111 );

				else
					FOwner->_Values[ FIndex / 8 ] &= ~( 1 << ( FIndex & 0b111 ));

			}

			bool operator == ( bool AValue ) 
			{
				return ( FOwner->_Values[ FIndex / 8 ] & ( 1 << ( FIndex & 0b111 )) == AValue );
			}

			bool operator != ( bool AValue ) 
			{
				return ( FOwner->_Values[ FIndex / 8 ] & ( 1 << ( FIndex & 0b111 )) != AValue );
			}

		public:
			_ByteAccess( ValueArrayDigital *AOwner, int AIndex ) :
				FOwner( AOwner ),
				FIndex( AIndex )
			{
			}
		};

		inline _ByteAccess operator [](int AIndex) { return _ByteAccess( this, AIndex ); }

	public:
		inline void SystemStart()
		{
			if( InitialIndex() > _MaxIndex )
				InitialIndex() = _MaxIndex;

			UpdateArrayValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( _Values[ InitialIndex() / 8 ] & ( 1 << ( InitialIndex() & 0b111 )) );
		}

		void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > _MaxIndex )
				AIndex = _MaxIndex;

			if( InitialIndex() == AIndex )
				return;

			InitialIndex() = AIndex;
			UpdateArrayValue();
		}

	public:
		void UpdateArrayValue()
		{
			if( ! ClockInputPin_o_IsConnected() )
				T_OutputPin::SetPinValue( _Values[ InitialIndex() / 8 ] & ( 1 << ( InitialIndex() & 0b111 )) );
		}

	};
//---------------------------------------------------------------------------
/*
	class ValueConstFloatArray : public ValueArray_Const<float, float>
	{
	public:
		inline float GetValue()
		{
			return pgm_read_float( _Values + InitialIndex );
		}

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_CONST, 
		typename T
	> class ValueArray_Const_Basic
	{
	public:
		const /*PROGMEM*/ T_CONST *_Values;

	protected:
		inline T GetValue( uint32_t AIndex ) {} // Generic Placeholder

	};
//---------------------------------------------------------------------------
	template<> float ValueArray_Const_Basic<float, float>::GetValue( uint32_t AIndex )
	{
		return (float)pgm_read_float( _Values + AIndex );
	}
//---------------------------------------------------------------------------
	template<> Mitov::String ValueArray_Const_Basic<const char *, Mitov::String>::GetValue( uint32_t AIndex )
	{
		char *APtr = (char *)pgm_read_ptr( _Values + AIndex );
		return APtr;
	}
//---------------------------------------------------------------------------
/*
	template<> bool ValueArray_Const_Basic<uint8_t, bool>::GetValue( uint32_t AIndex )
	{
		return pgm_read_byte( _Values + ( AIndex / 8 )) & ( 1 << ( AIndex & 0b111 ));
	}
*/
//---------------------------------------------------------------------------
	template<> inline int32_t ValueArray_Const_Basic<int32_t, int32_t>::GetValue( uint32_t AIndex )
	{
		uint32_t AValue = pgm_read_dword( _Values + AIndex );
		return *(int32_t *)&AValue;
	}
//---------------------------------------------------------------------------
	template<> inline uint32_t ValueArray_Const_Basic<uint32_t, uint32_t>::GetValue( uint32_t AIndex )
	{
		return pgm_read_dword( _Values + AIndex );
	}
//---------------------------------------------------------------------------
	template<> inline Mitov::TColor ValueArray_Const_Basic<uint32_t, Mitov::TColor>::GetValue( uint32_t AIndex )
	{
		uint32_t AValue = pgm_read_dword( _Values + AIndex );
		return *(Mitov::TColor *)&AValue;
	}
//---------------------------------------------------------------------------
	template<> inline Mitov::TRGBWColor ValueArray_Const_Basic<uint32_t, Mitov::TRGBWColor>::GetValue( uint32_t AIndex )
	{
		uint32_t AValue = pgm_read_dword( _Values + AIndex );
		return *(Mitov::TRGBWColor *)&AValue;
	}
//---------------------------------------------------------------------------
	template<
		typename T_CONST, 
		typename T_ClockInputPin_o_IsConnected,
		typename T_InitialIndex,
		typename T_OutputPin,
		typename T
	> class ValueArray_Const :
		public ValueArray_Const_Basic<T_CONST, T>,
		public T_ClockInputPin_o_IsConnected,
		public T_InitialIndex,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialIndex )

	public:
		uint32_t _MaxIndex;

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		inline void SetValue( bool AChangeOnly )
		{
			T_OutputPin::SetPinValue( ValueArray_Const_Basic<T_CONST, T>::GetValue( InitialIndex() ), AChangeOnly );
		}

	public:
		inline void SystemStart()
		{
			if( InitialIndex() > _MaxIndex )
				InitialIndex() = _MaxIndex;
			
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				SetValue( true );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SetValue( false );
		}

		void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > _MaxIndex )
				AIndex = _MaxIndex;

			if( InitialIndex() == AIndex )
				return;

			InitialIndex() = AIndex;

			if( ! ClockInputPin_o_IsConnected() )
				SetValue( false );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_ClockInputPin_o_IsConnected,
		typename T_InitialIndex,
		typename T_OutputPin
	> class ValueArrayDigital_Const :
		public T_ClockInputPin_o_IsConnected,
		public T_InitialIndex,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialIndex )

	public:
		const /*PROGMEM*/ uint8_t *_Values;
		uint32_t _MaxIndex;

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	protected:
		inline void SetValue( bool AChangeOnly )
		{
			T_OutputPin::SetPinValue( GetValue(), AChangeOnly );
		}

		inline bool GetValue()
		{
			return pgm_read_byte( _Values + ( InitialIndex() / 8 )) & ( 1 << ( InitialIndex() & 0b111 ));
		}

	public:
		inline void SystemStart()
		{
			if( InitialIndex() > _MaxIndex )
				InitialIndex() = _MaxIndex;
			
			if( ! ClockInputPin_o_IsConnected().GetValue() )
				SetValue( true );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SetValue( false );
		}

		void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex > _MaxIndex )
				AIndex = _MaxIndex;

			if( InitialIndex() == AIndex )
				return;

			InitialIndex() = AIndex;

			if( ! ClockInputPin_o_IsConnected() )
				SetValue( false );
		}

	};
//---------------------------------------------------------------------------
/*
	template<> float ValueArray_Const<float, float>::GetValue()
	{
		return (float)pgm_read_float( _Values + InitialIndex );
	}
//---------------------------------------------------------------------------
	template<> Mitov::String ValueArray_Const<const char *, Mitov::String>::GetValue()
	{
		char *APtr = (char *)pgm_read_ptr( _Values + InitialIndex );
		return APtr;
	}
//---------------------------------------------------------------------------
	template<> bool ValueArray_Const<uint8_t, bool>::GetValue()
	{
		return pgm_read_byte( _Values + ( InitialIndex / 8 )) & ( 1 << ( InitialIndex & 0b111 ));
	}
//---------------------------------------------------------------------------
	template<> int32_t ValueArray_Const<int32_t, int32_t>::GetValue()
	{
		uint32_t AValue = pgm_read_dword( _Values + InitialIndex );
		return *(int32_t *)&AValue;
	}
//---------------------------------------------------------------------------
	template<> uint32_t ValueArray_Const<uint32_t, uint32_t>::GetValue()
	{
		return pgm_read_dword( _Values + InitialIndex );
	}
//---------------------------------------------------------------------------
	template<> Mitov::TColor ValueArray_Const<uint32_t, Mitov::TColor>::GetValue()
	{
		uint32_t AValue = pgm_read_dword( _Values + InitialIndex );
		return *(Mitov::TColor *)&AValue;
	}
//---------------------------------------------------------------------------
	template<> Mitov::TRGBWColor ValueArray_Const<uint32_t, Mitov::TRGBWColor>::GetValue()
	{
		uint32_t AValue = pgm_read_dword( _Values + InitialIndex );
		return *(Mitov::TRGBWColor *)&AValue;
	}
*/
//---------------------------------------------------------------------------
/*
	class ValueConstDateTimeArray : public ValueArray_Const<Mitov::TDateTime, Mitov::TDateTime>
	{
	protected:
		virtual void SetValue( bool AChangeOnly )
		{
			uint32_t ADate = pgm_read_dword( _Values + InitialIndex );
			uint32_t ATime = pgm_read_dword( ((byte *)_Values ) + InitialIndex * sizeof(TDateTime) + sizeof(uint32_t) );
			T_OutputPin::SetPinValue( TDateTime( ADate, ATime ), AChangeOnly );
		}

	};
*/
//---------------------------------------------------------------------------
}

#endif
