////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_PWM_SERVO_h
#define _MITOV_PWM_SERVO_h

#include <Mitov.h>

#include <PWMServo.h>

namespace Mitov
{
	template<
		typename T_Enabled,
		typename T_InitialValue,
		typename T_MaxPulse,
		typename T_MinPulse,
		int PIN_NUMBER
	> class MitovPWMServo :
		public T_Enabled,
		public T_InitialValue,
		public T_MaxPulse,
		public T_MinPulse
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( MinPulse )
		_V_PROP_( MaxPulse )

	public:
		inline void SetEnabled( bool AValue )
		{
			if( Enabled() == AValue )
				return;

			Enabled() = AValue;
			if( Enabled() )
			{
				FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
				UpdatePosition();
//				InputPin_o_Receive( &InitialValue );
			}
			else
				FServo.detach();
		}

	protected:
		PWMServo	FServo;
		
	protected:
		inline void UpdatePosition()
		{
			float AValue = constrain( InitialValue().GetValue(), 0.0f, 1.0f ) * 180;
			FServo.write( AValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *((float *)_Data);
			if( Enabled() )
				UpdatePosition();

		}

	public:
		inline void SystemStart()
		{
			if( Enabled() )
			{
				FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
				UpdatePosition();
			}
		}

	};
//---------------------------------------------------------------------------
	template<
//		typename T_Enabled,
		typename T_InitialValue,
		typename T_MaxPulse,
		typename T_MinPulse,
		int PIN_NUMBER
	> class PWMServoShieldElement :
//		public T_Enabled,
		public T_InitialValue,
		public T_MaxPulse,
		public T_MinPulse
	{
	public:
//		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( MinPulse )
		_V_PROP_( MaxPulse )

	protected:
		PWMServo	FServo;

	public:
/*
		inline void SetEnabled( bool AValue )
		{
			if( Enabled().GetValue() == AValue )
				return;

			Enabled() = AValue;
			if( Enabled() )
			{
				FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
				InputPin_o_Receive( &InitialValue );
			}
			else
				FServo.detach();
		}
*/
	public:
		inline void SystemInit( float AInitialValue )
		{
			FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
			InputPin_o_Receive( &AInitialValue );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = constrain( *((float *)_Data), 0.0f, 1.0f ) * 180;
			FServo.write( AValue );
		}
	};
//---------------------------------------------------------------------------
}

#endif
