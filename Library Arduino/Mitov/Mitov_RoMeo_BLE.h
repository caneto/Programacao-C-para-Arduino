////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ROMEO_BLE_h
#define _MITOV_ROMEO_BLE_h

#include <Mitov.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_S1,
		typename T_OutputPins_S2,
		typename T_OutputPins_S3,
		typename T_OutputPins_S4,
		typename T_OutputPins_S5
	> class ArduinoRoMeoBLEButtons :
		public T_OutputPins_S1,
		public T_OutputPins_S2,
		public T_OutputPins_S3,
		public T_OutputPins_S4,
		public T_OutputPins_S5
	{
	public:
		_V_PIN_( OutputPins_S1 )
		_V_PIN_( OutputPins_S2 )
		_V_PIN_( OutputPins_S3 )
		_V_PIN_( OutputPins_S4 )
		_V_PIN_( OutputPins_S5 )

	protected:
		void ReadButtons( bool AOnlyChanged )
		{
			const int  adc_key_val[ 5 ] = { 30, 150, 360, 535, 760 };

			int input = analogRead( 7 );
			int AButtonIndex = -1;
			for( int i = 0; i < 5; ++ i )
				if (input < adc_key_val[i])
				{
					AButtonIndex = i;
					break;
				}
		
			T_OutputPins_S1::SetPinValue( ( 0 == AButtonIndex ), AOnlyChanged );
			T_OutputPins_S2::SetPinValue( ( 1 == AButtonIndex ), AOnlyChanged );
			T_OutputPins_S3::SetPinValue( ( 2 == AButtonIndex ), AOnlyChanged );
			T_OutputPins_S4::SetPinValue( ( 3 == AButtonIndex ), AOnlyChanged );
			T_OutputPins_S5::SetPinValue( ( 4 == AButtonIndex ), AOnlyChanged );

//			for( int i = 0; i < 5; ++ i )
//				OutputPins[ i ].SetValue( ( i == AButtonIndex ), AOnlyChanged );

		}

	public:
		inline void SystemStart()
		{
			ReadButtons( false );
		}

		inline void SystemLoopBegin()
		{
			ReadButtons( true );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_FMotor1_Modified,
		typename T_FMotor1_Populated,
		typename T_FMotor2_Modified,
		typename T_FMotor2_Populated
	> class ArduinoRoMeoBLEMotors :
		public T_FMotor1_Modified,
		public T_FMotor1_Populated,
		public T_FMotor2_Modified,
		public T_FMotor2_Populated
	{
	protected:
		_V_PROP_( FMotor1_Populated )
		_V_PROP_( FMotor2_Populated )
		_V_PROP_( FMotor1_Modified )
		_V_PROP_( FMotor2_Modified )

		float	FMotor1_Value = 0.0f;
		float	FMotor2_Value = 0.0f;

	protected:
		void SetSpeed( int ADirectionPin, int ASpeedPin, float AValue )
		{
			AValue = constrain( AValue, 0.0, 1.0 );
			if( AValue > 0.5 )
			{
				digitalWrite( ADirectionPin, HIGH );
				AValue -= 0.5;
			}

			else
			{
				digitalWrite( ADirectionPin, LOW );
				AValue = 0.5 - AValue;
			}

			AValue = AValue * 511 + 0.5;
			analogWrite( ASpeedPin, AValue );
		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			float AValue = *(float*)_Data;
			if( AIndex )
			{
				if( FMotor2_Populated() )
					if( AValue == FMotor2_Value )
						return;

				FMotor2_Populated() = true;
				FMotor2_Value = AValue;
				FMotor2_Modified() = true;
			}

			else
			{
				if( FMotor1_Populated() )
					if( AValue == FMotor1_Value )
						return;

				FMotor1_Populated() = true;
				FMotor1_Value = AValue;
				FMotor1_Modified() = true;
			}
		}

	public:
		inline void SystemLoopBegin()
		{
			if( FMotor1_Modified() )
				SetSpeed( 4, 5, FMotor1_Value );

			if( FMotor2_Modified() )
				SetSpeed( 7, 6, FMotor2_Value );
		}

		inline void SystemStart()
		{
			SetSpeed( 4, 5, FMotor1_Value );
			SetSpeed( 7, 6, FMotor2_Value );
		}

	public:
		inline ArduinoRoMeoBLEMotors()
		{
			FMotor1_Populated() = false;
			FMotor2_Populated() = false;
			FMotor1_Modified() = false;
			FMotor2_Modified() = false;
		}
	};
//---------------------------------------------------------------------------
}

#endif
