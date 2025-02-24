////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_DFROBOT_LCD_SHIELD_h
#define _MITOV_DFROBOT_LCD_SHIELD_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template<
		int C_NAVKEYMOVE_RIGHT,
		int C_NAVKEYMOVE_UP,
		int C_NAVKEYMOVE_DOWN,
		int C_NAVKEYMOVE_LEFT,
		int C_NAVKEYMOVE_SELECT,
		typename T_DirectionOutputPins_Up,
		typename T_DirectionOutputPins_Down,
		typename T_DirectionOutputPins_Left,
		typename T_DirectionOutputPins_Right,
		typename T_SelectOutputPin
	> class ArduinoDFRobotLCDKeypadKeys :
		public T_DirectionOutputPins_Up,
		public T_DirectionOutputPins_Down,
		public T_DirectionOutputPins_Left,
		public T_DirectionOutputPins_Right,
		public T_SelectOutputPin
	{
	public:
		_V_PIN_( DirectionOutputPins_Up )
		_V_PIN_( DirectionOutputPins_Down )
		_V_PIN_( DirectionOutputPins_Left )
		_V_PIN_( DirectionOutputPins_Right )
		_V_PIN_( SelectOutputPin )

	protected:
		void UpdatePins( bool AChangeOnly )
		{
			int AAnalogValue = analogRead( 0 );
//	    	AnalogOutputPin.SetValue( AAnalogValue / 1023.0, AChangeOnly );

			bool AUp = false;
			bool ADown = false;
			bool ALeft = false;
			bool ARight = false;
			bool ASelect = false;

			if( AAnalogValue < C_NAVKEYMOVE_RIGHT )
				ARight = true;

			else if( AAnalogValue < C_NAVKEYMOVE_UP )
				AUp = true;

			else if( AAnalogValue < C_NAVKEYMOVE_DOWN )
				ADown = true;

			else if( AAnalogValue < C_NAVKEYMOVE_LEFT )
				ALeft = true;

			else if( AAnalogValue < C_NAVKEYMOVE_SELECT )
				ASelect = true;

			T_SelectOutputPin::SetPinValue( ASelect, AChangeOnly );
			T_DirectionOutputPins_Up::SetPinValue( AUp, AChangeOnly );
			T_DirectionOutputPins_Down::SetPinValue( ADown, AChangeOnly );
			T_DirectionOutputPins_Left::SetPinValue( ALeft, AChangeOnly );
			T_DirectionOutputPins_Right::SetPinValue( ARight, AChangeOnly );
		}

	public:
		inline void SystemLoopBegin()
		{
			UpdatePins( true );
		}

		inline void SystemStart()
		{
			UpdatePins( false );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
