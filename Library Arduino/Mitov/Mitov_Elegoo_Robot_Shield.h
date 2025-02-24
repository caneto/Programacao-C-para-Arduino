////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ELEGOO_MOTOR_SHIELD_h
#define _MITOV_ELEGOO_MOTOR_SHIELD_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
/*
#ifdef __MITOV_ELEGOO_MOTOR_SHIELD_V3__
	typedef	MotorShieldChannel<5,8,7, 6,9,11> ElegooRobotMotorShieldChannel;
	typedef	MotorShieldChannel_Slopped<5,8,7, 6,9,11> ElegooRobotMotorShieldChannel_Slopped;
#else
	#ifdef __MITOV_ELEGOO_MOTOR_SHIELD_V2_REMAPPED__
		typedef	MotorShieldChannel<5,7,11, 6,8,9> ElegooRobotMotorShieldChannel;
		typedef	MotorShieldChannel_Slopped<5,7,11, 6,8,9> ElegooRobotMotorShieldChannel_Slopped;
	#else // __MITOV_ELEGOO_MOTOR_SHIELD_V2_REMAPPED__
		typedef	MotorShieldChannel<5,7,6, 11,8,9> ElegooRobotMotorShieldChannel;
		typedef	MotorShieldChannel_Slopped<5,7,6, 11,8,9> ElegooRobotMotorShieldChannel_Slopped;
	#endif // __MITOV_ELEGOO_MOTOR_SHIELD_V2_REMAPPED__
#endif // __MITOV_ELEGOO_MOTOR_SHIELD_V3__
*/
	template<
		typename T_OutputPins_Left,
		typename T_OutputPins_Center,
		typename T_OutputPins_Right
	> class ElegooRobotLineFollowingSensors :
		public T_OutputPins_Left,
		public T_OutputPins_Center,
		public T_OutputPins_Right
	{
	public:
		_V_PIN_( OutputPins_Left )
		_V_PIN_( OutputPins_Center )
		_V_PIN_( OutputPins_Right )

	protected:
		void ReadSensor( bool AChangeOnly )
		{
			T_OutputPins_Left::SetPinValue( digitalRead( 10 ) == HIGH, AChangeOnly );
			T_OutputPins_Center::SetPinValue( digitalRead( 4 ) == HIGH, AChangeOnly );
			T_OutputPins_Right::SetPinValue( digitalRead( 2 ) == HIGH, AChangeOnly );
		}

	public:
		inline void SystemStart()
		{
			ReadSensor( false );
		}

		inline void SystemLoopBegin()
		{
			ReadSensor( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
