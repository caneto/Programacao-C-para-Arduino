////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_SERVO_h
#define _MITOV_SERVO_h

#include <Mitov.h>

#ifdef VISUINO_ESP32
#else
#include <Servo.h>
#endif

namespace Mitov
{
	template<
		typename T_Enabled,
		typename T_InitialValue,
		typename T_MaxPulse,
		typename T_MinPulse,
		int PIN_NUMBER
	> class MitovServo :
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
		void SetEnabled( bool AValue )
		{
			if( Enabled() == AValue )
				return;

			Enabled() = AValue;
			if( Enabled() )
			{
#ifdef VISUINO_ESP32
				Mitov::ServoAttach( PIN_NUMBER );
#else
				FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
#endif
				UpdatePosition();
			}
			else
#ifdef VISUINO_ESP32
				Mitov::ServoDetach( PIN_NUMBER );
#else
				FServo.detach();
#endif
		}

	protected:
#ifndef VISUINO_ESP32
		Servo	FServo;
#endif
		
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *((float *)_Data);
			if( Enabled() )
				UpdatePosition();

		}

	protected:
		void UpdatePosition()
		{
#ifdef VISUINO_ESP32
			float AValue = constrain( InitialValue().GetValue(), 0.0f, 1.0f ) * 180;
			Mitov::ServoWrite( PIN_NUMBER, AValue, MinPulse().GetValue(), MaxPulse().GetValue() );
#else
//			FServo.write( AValue );
			float AValue = (constrain( InitialValue().GetValue(), 0.0f, 1.0f ) * (MaxPulse().GetValue() - MinPulse().GetValue())) + MinPulse().GetValue(); // Scale input to microseconds
			FServo.writeMicroseconds( AValue );
#endif
		}

	public:
		inline void SystemStart()
		{
			if( Enabled() )
			{
#ifdef VISUINO_ESP32
				Mitov::ServoAttach( PIN_NUMBER );
#else
				FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
#endif
				UpdatePosition();
			}
//			float AValue = constrain( InitialValue, 0.0f, 1.0f ) * 180;
//			FServo.write( AValue );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_InitialValue,
		typename T_MaxPulse,
		typename T_MinPulse,
		int PIN_NUMBER
	> class ServoShieldElement :
		public T_InitialValue,
		public T_MaxPulse,
		public T_MinPulse
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( MinPulse )
		_V_PROP_( MaxPulse )

	protected:
#ifndef VISUINO_ESP32
		Servo	FServo;
#endif

	public:
		inline void SystemInit( float AInitialValue )
		{
#ifdef VISUINO_ESP32
			Mitov::ServoAttach( PIN_NUMBER );
#else
			FServo.attach( PIN_NUMBER, MinPulse().GetValue(), MaxPulse().GetValue() );
#endif
			InputPin_o_Receive( &AInitialValue );
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
#ifdef VISUINO_ESP32
			float AValue = constrain( *((float *)_Data), 0.0f, 1.0f ) * 180;
			Mitov::ServoWrite( PIN_NUMBER, AValue, MinPulse().GetValue(), MaxPulse().GetValue() );
#else
			float AValue = (constrain( *((float *)_Data), 0.0f, 1.0f ) * (MaxPulse().GetValue() - MinPulse().GetValue())) + MinPulse().GetValue(); // Scale input to microseconds
			FServo.writeMicroseconds( AValue );
//			FServo.write( AValue );
#endif
		}

	};
//---------------------------------------------------------------------------
}

#endif
