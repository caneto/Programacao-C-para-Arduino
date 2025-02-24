////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_SPEED_TO_CLOCK_h
#define _MITOV_SPEED_TO_CLOCK_h

#include <Mitov.h>

namespace Mitov
{
	template <
		typename T_ClockOutputPin,
		typename T_DirectionOutputPin,
		typename T_Enabled,
		typename T_FSendOutput,
		typename T_MaxFrequency
	> class SpeedToClock :
		public T_ClockOutputPin,
		public T_DirectionOutputPin,
		public T_Enabled,
		public T_FSendOutput,
		public T_MaxFrequency
	{
	public:
		_V_PIN_( DirectionOutputPin )
		_V_PIN_( ClockOutputPin )

	public:
		_V_PROP_( MaxFrequency )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FSendOutput )
		unsigned long FLastTime = 0;
		unsigned long FPeriod = 0;

	public:
		inline void SystemLoopBegin()
		{
			if( ! FSendOutput() )
				FLastTime = micros();

			else
				while( micros() - FLastTime >= FPeriod )
				{
					if( Enabled() )
						T_ClockOutputPin::ClockPin();

					FLastTime += FPeriod;
				}

		}

		inline void SystemStart()
		{
			FLastTime = micros();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = constrain( *(float *)_Data, 0.0, 1.0 );
			float AFrequency = abs( ASpeed - 0.5 ) * 2;
			FSendOutput() = ( AFrequency != 0 );
			if( FSendOutput() )
			{
				AFrequency *= MaxFrequency();
				FPeriod = ( (( 1 / AFrequency ) * 1000000 ) + 0.5 );
			}

			else
				FPeriod = 0;

			bool ADirection = ASpeed > 0.5;

			T_DirectionOutputPin::SetPinValue( ADirection );
		}

	public:
		inline SpeedToClock()
		{
			FSendOutput() = false;
		}
	};
}

#endif
