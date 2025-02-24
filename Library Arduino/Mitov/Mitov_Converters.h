////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_CONVERTERS_h
#define _MITOV_CONVERTERS_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_OutputPin,
		typename T_Scale,
		typename T_TYPE
	> class TypedToAnalog : 
		public T_OutputPin,
		public T_Scale
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Scale )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AFloatValue = ( *(T_TYPE*)_Data ) * Scale();
			T_OutputPin::SetPinValue( AFloatValue );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Base,
		typename T_MaxSize,
		typename T_OutputPin
	> class BinaryToText :
		public T_Base,
		public T_MaxSize,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( MaxSize )
		_V_PROP_( Base )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
			T_OutputPin::SetPinValue( ADataBlock.ToString( MaxSize(), Base() ).c_str(), false );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OutputPin,
		typename T_Round,
		typename T_Scale
	> class AnalogToInteger :
		public T_Round,
		public T_Scale,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Round )
		_V_PROP_( Scale )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AFloatValue = *(float*)_Data * Scale();
			if( Round() )
				AFloatValue += 0.5f;

			int32_t AIntValue = AFloatValue;
			T_OutputPin::SetPinValue( AIntValue );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Constrain,
		typename T_OutputPin,
		typename T_Round,
		typename T_Scale
	> class AnalogToUnsigned :
		public T_Constrain,
		public T_Round,
		public T_Scale,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Round )
		_V_PROP_( Constrain )
		_V_PROP_( Scale )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AFloatValue = *(float*)_Data * Scale();
			if( Round() )
				AFloatValue += 0.5f;

			long AIntValue = AFloatValue;
			if( Constrain() )
				AIntValue &= 0x7FFFFFFF;

			T_OutputPin::SetPinValue( AIntValue );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_MinWidth,
		typename T_OutputPin,
		typename T_Precision	
	> class AnalogToText :
		public T_MinWidth,
		public T_OutputPin,
		public T_Precision	
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( MinWidth )
		_V_PROP_( Precision )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AFloatValue = *(float*)_Data;
			char AText[ 16 ];
			dtostrf( AFloatValue, MinWidth(), Precision(), AText );
			T_OutputPin::SetPinValue( AText );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Base,
		typename T_OutputPin,
		typename T_TYPE
	> class TypedToText :
		public T_Base,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Base )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_TYPE AValue = *(T_TYPE *)_Data;
			char AText[ 16 ];
			ltoa( AValue, AText, Base() );
			T_OutputPin::SetPinValue( AText );
		}

	};
//---------------------------------------------------------------------------
/*
	class DigitalToText
	{
	public:
		OpenWire::SourcePin	OutputPin ;

	public:
		Mitov::String	TrueValue = "true";
		Mitov::String	FalseValue = "false";

	public:
		void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
//			Serial.println( AValue );
			const char *AText;
			if( AValue )
				AText = TrueValue.c_str();

			else
				AText = FalseValue.c_str();

//			Serial.println( AText );
			OutputPin.Notify( (void*)AText );
		}

	};
*/
//---------------------------------------------------------------------------
/*
	template <typename T, T T_FALSE, T C_TRUE> class DigitalToType
	{
	public:
		OpenWire::SourcePin	OutputPin ;

	public:
		T	TrueValue = C_TRUE;
		T	FalseValue = T_FALSE;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( *(bool *)_Data )
				OutputPin.Notify( &TrueValue );

			else
				OutputPin.Notify( &FalseValue );

		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_FalseValue,
		typename T_OutputPin,
//		typename T_TYPE,
		typename T_TrueValue
	> class DigitalToType :
		public T_FalseValue,
		public T_OutputPin,
//		public T_TYPE,
		public T_TrueValue
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( TrueValue )
		_V_PROP_( FalseValue )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( *(bool *)_Data )
				T_OutputPin::SetPinValue( TrueValue().GetValue() );

			else
				T_OutputPin::SetPinValue( FalseValue().GetValue() );

		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_OutputPin
	> class TextToAnalog : 
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			char * AText = (char*)_Data;
			float AValue = strtod( AText, nullptr );
			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Base,
		typename T_OutputPin
	> class TextToInteger :
		public T_Base,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Base )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			char * AText = (char*)_Data;
//			long AValue = atoi( AText );
			char *AEnd;
			long AValue = strtol( AText, &AEnd, Base() );
			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Base,
		typename T_OutputPin
	> class TextToUnsigned :
		public T_Base,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Base )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			char * AText = (char*)_Data;
//			unsigned long AValue = atoi( AText );
			char *AEnd;
			unsigned long AValue = strtol( AText, &AEnd, Base() );
			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Constrain,
		typename T_OutputPin
	> class UnsignedToInteger :
		public T_Constrain,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Constrain )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			unsigned long AValue = *(unsigned long*)_Data;
			if( Constrain() )
				AValue &= 0x7FFFFFFF;

			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Constrain,
		typename T_OutputPin
	> class IntegerToUnsigned :
		public T_Constrain,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Constrain )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			long AValue = *(long*)_Data;
			if( Constrain() )
				AValue &= 0x7FFFFFFF;

			T_OutputPin::SetPinValue( AValue );
		}

	};
//---------------------------------------------------------------------------
	template<
		int C_NUM_INPUTS,
		typename T_InitialValue,
		typename T_OutputPins_SetPinValue
	> class UnsignedToDigital :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void SetValue( uint32_t AValue, bool AUpdate )
		{
			for( long i = 0; i < C_NUM_INPUTS; ++i )
			{
				uint32_t ABit = uint32_t(1) << i;
				bool AOldBitValue = ( InitialValue() & ABit );
				bool ANewBitValue = ( AValue & ABit );
				if( AUpdate || AOldBitValue != ANewBitValue )
					T_OutputPins_SetPinValue::ChainCall( i, ANewBitValue );

			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			unsigned long AValue = *(unsigned long *)_Data;
			if( InitialValue() == AValue )
				return;

			SetValue( AValue, false );
			InitialValue() = AValue;
		}

	public:
		inline void SystemInit()
		{
			SetValue( InitialValue(), true );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Reverse
	> class CelsiusToFahrenheit :
		public T_Enabled,
		public T_OutputPin,
		public T_Reverse
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reverse )

	protected:
        inline float FilterValue( float AValue )
        {
			if( Reverse() )
				return ( AValue - 32.0 ) / ( 9.0/5.0 );

			else
				return AValue * ( 9.0/5.0 ) + 32.0;
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

            float AOutValue = FilterValue( *(float*)_Data );

			T_OutputPin::SetPinValue( AOutValue );
        }
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Reverse
	> class CelsiusToKelvin :
		public T_Enabled,
		public T_OutputPin,
		public T_Reverse
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reverse )

	protected:
        inline float FilterValue( float AValue )
        {
			if( Reverse() )
				return AValue - 273.15;

			else
				return AValue + 273.15;
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AInValue = *(float*)_Data;

            float AOutValue = FilterValue( AInValue );

			T_OutputPin::SetPinValue( AOutValue );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_OutputPin,
		typename T_Reverse
	> class MetresToFeet :
		public T_Enabled,
		public T_OutputPin,
		public T_Reverse
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reverse )

	protected:
        inline float FilterValue( float AValue )
        {
			if( Reverse() )
				return AValue * 0.3048;

			else
				return AValue / 0.3048;
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AInValue = *(float*)_Data;

            float AOutValue = FilterValue( AInValue );

			T_OutputPin::SetPinValue( AOutValue );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_FIndex,
		int T_SIZE,
		typename T_OutputPin,
		typename T_Truncate,
		typename T_UpdateOnEachChar
	> class CharToText :
		public T_Enabled,
		public T_FIndex,
		public T_OutputPin,
		public T_Truncate,
		public T_UpdateOnEachChar
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Truncate )
		_V_PROP_( UpdateOnEachChar )

	protected:
		char		FBuffer[ T_SIZE + 1 ];
		_V_PROP_( FIndex )

	protected:
		void SendBufferNoReset()
		{
			FBuffer[ FIndex() ] = '\0';
			T_OutputPin::SetPinValue( FBuffer, false );
		}

		void SendBuffer()
		{
			SendBufferNoReset();
			FIndex() = 0;
		}

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			char AValue = *(char *)_Data;
			if( AValue == '\n' )
				return;

			if( AValue == '\r' )
			{
				SendBuffer();
				return;
			}
				
			if( FIndex() >= T_SIZE )
			{
				if( Truncate() )
					return;

				SendBuffer();
			}

			FBuffer[ FIndex() ] = AValue;
			FIndex() = FIndex() + 1; // Bitfields reference is not possible
			if( UpdateOnEachChar() )
				SendBufferNoReset();

        }

		inline void ClearInputPin_o_Receive( void *_Data )
		{
			FIndex() = 0;
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SendBuffer();
		}

	public:
		CharToText()
		{
			FIndex() = 0;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_AddNewLine,
		typename T_AddReturn,
		typename T_Enabled,
		typename T_OutputPin
	> class TextToChar :
		public T_AddNewLine,
		public T_AddReturn,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
        _V_PROP_( Enabled )
		_V_PROP_( AddReturn )
		_V_PROP_( AddNewLine )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			char *AValue = (char *)_Data;
			while( *AValue )
				T_OutputPin::SetPinValue( *AValue ++, false );

			if( AddReturn() )
				T_OutputPin::SetPinValue( '\r', false );

			if( AddNewLine() )
				T_OutputPin::SetPinValue( '\n', false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_BaseLinePressure,
		typename T_Enabled,
		typename T_OutputPin
	> class PressureToAltitude :
		public T_BaseLinePressure,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( BaseLinePressure )

	protected:
        inline float FilterValue( float AValue )
        {
			return ( 44330.0 * ( 1 - pow( AValue / BaseLinePressure(), 1 / 5.255 )));
        }

	public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AInValue = *(float*)_Data;

            float AOutValue = FilterValue( AInValue );

			T_OutputPin::SetPinValue( AOutValue );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Altitude,
		typename T_Enabled,
		typename T_OutputPin
	> class AltitudePressureToSeaLevelPressure :
		public T_Altitude,
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Altitude )

	protected:
        float FilterValue( float AValue )
        {
			return ( AValue / pow( 1 - ( Altitude() / 44330.0 ), 5.255 ));
        }

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

			float AInValue = *(float*)_Data;

            float AOutValue = FilterValue( AInValue );

			T_OutputPin::SetPinValue( AOutValue );
        }
	};
//---------------------------------------------------------------------------
	template<
		int C_NUM_INPUTS, 
		typename T_FModified,
		typename T_OutputPin
	> class AnalogToAnalogArray : 
		public T_FModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin ) 

	protected:
		_V_PROP_( FModified )
		float	FBufferData[ C_NUM_INPUTS ];

	protected:
		void CalculateSendOutput( bool AFromStart )
		{
			TArray<float> ABuffer( C_NUM_INPUTS, FBufferData );
			T_OutputPin::SetPinValue( ABuffer );
		}

	public:
		void ClockInputPin_o_Receive( void * )
		{
			if( FModified() )
				CalculateSendOutput( false );
		}

		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			FBufferData[ AIndex ] = *(float *)_Data;
			FModified() = true;
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
				CalculateSendOutput( false );

		}

	public:
		AnalogToAnalogArray()
		{
			FModified() = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		int C_NUM_INPUTS, 
		typename T_FModified,
		typename T_OutputPin
	> class AnalogArrayToAnalogArray : // TODO!!!!
		public T_FModified,
		public T_OutputPin
	{
	};
//---------------------------------------------------------------------------
	class ReceivePin
	{
	public:
		virtual void Receive( void *_Data ) = 0;
	};
//---------------------------------------------------------------------------
	template<typename T> class ValueSinkPin : public ReceivePin
	{
	public:
		T Value = T( 0 );

	public:
		virtual void Receive( void *_Data ) override
		{
			Value = *(T*)_Data;
		}
	};
//---------------------------------------------------------------------------
	template<typename T> class SingleElementArrayPin : public ValueSinkPin<Mitov::TValueArray<T> >
	{
		typedef ValueSinkPin<Mitov::TValueArray<T> > inherited;

	public:
		virtual void Receive( void *_Data ) override
		{
/*
			Serial.print( "Pin: " ); Serial.println( (int)this );
			Serial.println( inherited::Value.Size );
			Serial.println( (int)inherited::Value.Data );
			delay( 200 );
*/
			T AValue = *(T*)_Data;
			*inherited::Value.Data = AValue;

//			OpenWire::SinkPin::Receive( _Data );
		}

	};
//---------------------------------------------------------------------------
	class FloatArraySinkPinsArray
	{
	protected:
		ValueSinkPin<Mitov::TValueArray<float> >	**FInputPins;

	public:
		ValueSinkPin<Mitov::TValueArray<float> >& operator[] ( int AIndex ) 
		{
          return *( FInputPins[ AIndex ] );
		}

	public:
		inline void SetPin( int AIndex, ValueSinkPin<Mitov::TValueArray<float> > *APin )
		{
			FInputPins[ AIndex ] = APin;
		}

	public:
		FloatArraySinkPinsArray( int ASize )
		{
			FInputPins = new ValueSinkPin<Mitov::TValueArray<float> >*[ ASize ];
		}
	};
//---------------------------------------------------------------------------
	template<
		int C_NUM_INPUTS, 
		typename T_FModified,
		typename T_OutputPin
	> class MixedAnalogToAnalogArray : 
		public T_FModified,
		public T_OutputPin
	{
	public:
		FloatArraySinkPinsArray		InputPins = FloatArraySinkPinsArray( C_NUM_INPUTS );

	public:
		_V_PIN_( OutputPin ) 

	protected:
		_V_PROP_( FModified )

	protected:
		void CalculateSendOutput( bool AFromStart )
		{
//			TArray<float> ABuffer( C_NUM_INPUTS, FBufferData );
//			float *AData = 
//			TArray<float> ABuffer( C_NUM_INPUTS, FBufferData );
			int ASize = 0;

			for( int i = 0; i < C_NUM_INPUTS; ++i )
				ASize += InputPins[ i ].Value.Size;

//			Serial.println( "Size" );
//			Serial.println( ASize );
//			delay( 1000 );

			float *AData = new float[ ASize ];
			float *APtr = AData;
			for( int i = 0; i < C_NUM_INPUTS; ++i )
			{
				int ABlockSize = InputPins[ i ].Value.Size;
				memcpy( APtr, InputPins[ i ].Value.Data, ABlockSize * sizeof( APtr[ 0 ] ));
				APtr += ABlockSize;
			}

			TArray<float> ABuffer( ASize, AData );
			T_OutputPin::SetPinValue( ABuffer );

			delete [] AData;
		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			InputPins[ AIndex ].Receive( _Data );
			FModified() = true;
		}

		void ClockInputPin_o_Receive( void * )
		{
			if( FModified() )
				CalculateSendOutput( false );
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
				CalculateSendOutput( false );

		}

	public:
		MixedAnalogToAnalogArray()
		{
			FModified() = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		int C_NUM_INPUTS,
		typename T_FStarted
	> class AnalogArrayToAnalog :
		public T_FStarted
	{
	public:
		OpenWire::TypedSourcePin<float>	OutputPins[ C_NUM_INPUTS ];

	protected:
		_V_PROP_( FStarted )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			int ASize = (( TArray<float> *)_Data )->Size;
			float *AData = (( TArray<float> *)_Data )->Data;

			for( int i = 0; i < C_NUM_INPUTS; ++i )
				OutputPins[ i ].SetValue( *AData++, FStarted() );

			FStarted() = true;
		}

	public:
		AnalogArrayToAnalog()
		{
			FStarted() = false;
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
