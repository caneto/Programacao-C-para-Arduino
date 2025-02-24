////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_SPEED_CHANGE_METER_h
#define _MITOV_SPEED_CHANGE_METER_h

#include <Mitov.h>

namespace Mitov
{
	template <
		typename T_Enabled,
		typename T_FHasTime,
		typename T_OutputPin
	> class SpeedChangeMeter :
		public T_Enabled,
		public T_FHasTime,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FHasTime )
		unsigned long	FLastTime = 0;
		float			FLastValue;

	public:
		void InputPin_o_Receive( void *_Data )
		{
            if( ! Enabled() )
				return;

			float AValue = *(float *)_Data;
			unsigned long	ATime = micros();
			if( ! FHasTime() )
				FHasTime() = true;

			else
			{
				float ASpeed = AValue - FLastValue;
				ASpeed /= ( ATime - FLastTime ) / 1000000.0f;
				T_OutputPin::SetPinValue( ASpeed );
			}

			FLastValue = AValue;
			FLastTime = ATime;
		}

	public:
		inline SpeedChangeMeter()
		{
			FHasTime() = false;
		}
	};
}

#endif
