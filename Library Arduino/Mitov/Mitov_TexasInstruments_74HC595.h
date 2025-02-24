////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_TEXASINSTRUMENTS_74HC595_h
#define _MITOV_TEXASINSTRUMENTS_74HC595_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_ClockOutputPin,
		typename T_DataOutputPin,
		typename T_LatchOutputPin
	> class TexasInstruments74HC595 : 
		public T_ClockOutputPin,
		public T_DataOutputPin,
		public T_LatchOutputPin
	{
	public:
		_V_PIN_( DataOutputPin )
		_V_PIN_( ClockOutputPin )
		_V_PIN_( LatchOutputPin )

	protected:
		uint8_t	FBits = 0;

	protected:
		void UpdateAll()
		{
//			C_I2C.beginTransmission( C_ADDRESS );
//			C_I2C.write( FDirectionBits );
//			C_I2C.endTransmission();

//			digitalWrite(MOTORLATCH, LOW);

			T_LatchOutputPin::SetPinValueLow();

//			digitalWrite(MOTORDATA, LOW);
			T_DataOutputPin::SetPinValueLow();

			for ( uint8_t i=0; i<8; ++i ) 
			{
//				digitalWrite(MOTORCLK, LOW);
				T_ClockOutputPin::SetPinValueLow();

/*
				if ( FBits & ( 1 << ( 7-i ))) 
				{
					digitalWrite(MOTORDATA, HIGH);
				} 
				else 
				{
					digitalWrite(MOTORDATA, LOW);
				}
*/
				T_DataOutputPin::SetPinValue( FBits & ( 1 << ( 7-i )));

//				digitalWrite(MOTORCLK, HIGH);
				T_ClockOutputPin::SetPinValueHigh();
			}

//			digitalWrite(MOTORLATCH, HIGH);
			T_LatchOutputPin::SetPinValueHigh();
		}

	public:
		void SetChannelValue( int AIndex, bool AValue )
		{
			if( AValue )
				FBits |= ( (uint8_t)1 ) << AIndex;

			else
				FBits &= ~( ((uint8_t)1 ) << AIndex );
		}

	public:
		inline void SystemStart()
		{
			UpdateAll();
		}

		inline void SystemLoopUpdateHardware()
		{
			UpdateAll();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_InitialValue
	> class TexasInstruments74HC595_Channel :
		public T_InitialValue
	{
//		typedef BasicMicrochip_PCF85X_Channel inherited;
		
	public:
		_V_PROP_( InitialValue )

	public:
		inline void SetMode( int AMode ) // Placeholder for direct pin control!
		{
//			pinMode( FPinNumber, AMode );
		}

		inline bool DigitalRead() // For GPIO compatibility
		{
			return false;
		}

		inline void DigitalWrite( bool AValue )
		{
			C_OWNER.SetChannelValue( C_INDEX, AValue );
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			DigitalWrite( *(bool *)_Data );
		}

	public:
		TexasInstruments74HC595_Channel()
		{
			DigitalWrite( InitialValue() );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif // _MITOV_TEXASINSTRUMENTS_74HC595_h