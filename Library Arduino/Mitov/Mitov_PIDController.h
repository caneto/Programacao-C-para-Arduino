////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_PID_CONTROLLER_h
#define _MITOV_PID_CONTROLLER_h

#include <Mitov.h>

namespace Mitov
{
	template <
		typename T_ClockInputPin_o_IsConnected,
		typename T_DerivativeGain,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_IntegralGain,
		typename T_OutputPin,
		typename T_ProportionalGain,
		typename T_SetPoint
	> class PIDController :
		public T_ClockInputPin_o_IsConnected,
		public T_DerivativeGain,
		public T_Enabled,
		public T_InitialValue,
		public T_IntegralGain,
		public T_OutputPin,
		public T_ProportionalGain,
		public T_SetPoint
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( ClockInputPin_o_IsConnected )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( ProportionalGain )
		_V_PROP_( IntegralGain )
		_V_PROP_( DerivativeGain )

		_V_PROP_( SetPoint )

		_V_PROP_( InitialValue )

	protected:
		unsigned long	FLastTime;
		float	FOutput = 0.0f;
		float	FLastInput;
		float	FITerm;

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

            Enabled() = AValue;
			if( Enabled() )
				Initialize();

		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(float *)_Data;
		}

		void ManualControlInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				return;

			FOutput = *(float *)_Data;
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FOutput );
		}

	protected:
		void Initialize()
		{
			FITerm = FOutput;
			FLastInput = InitialValue();
			FLastTime = micros();
		}

	public:
		inline void SystemStart()
		{
			Initialize();
		}

		inline void SystemLoopBegin()
		{
			if( ! Enabled() ) 
				return;

			unsigned long ACurrentMicros = micros();
			unsigned long timeChange = ( ACurrentMicros - FLastTime );
			float ANormalizedTime = float( timeChange ) / 1000000;

			// Compute all the working error variables
			double error = SetPoint() - InitialValue();
//			ITerm += ( ki * error ) * ANormalizedTime;
//			Serial.println( FITerm + ( IntegralGain * error ) * ANormalizedTime );

			FITerm = constrain( FITerm + ( IntegralGain() * error ) * ANormalizedTime, -10000.0f, 10000.0f );
//			FITerm += ( IntegralGain * error ) * ANormalizedTime;

			double dInput = ( InitialValue() - FLastInput ) * ANormalizedTime;
 
			// Compute PID Output
			float AOutput = constrain( ProportionalGain() * error + FITerm - DerivativeGain() * dInput, 0.0f, 1.0f );
	  
			// Remember some variables for next time
			FLastInput = InitialValue();
			FLastTime = ACurrentMicros;

			if( AOutput == FOutput )
				return;

			FOutput = AOutput;

			if( ! ClockInputPin_o_IsConnected() )
				T_OutputPin::SetPinValue( FOutput );
		}

	};
//---------------------------------------------------------------------------
}

#endif
