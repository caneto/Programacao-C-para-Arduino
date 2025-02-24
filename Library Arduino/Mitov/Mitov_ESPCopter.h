////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ESP_COPTER_h
#define _MITOV_ESP_COPTER_h

#include <Mitov.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		uint8_t C_BLUE_PIN,
		uint8_t C_GREEN_PIN,
		uint8_t C_RED_PIN,
		typename T_InitialValue
	> class ArduinoRGBESPCopterModule :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
			analogWrite( C_RED_PIN, int( 255 - InitialValue().GetValue().Red ) * PWMRANGE / 255 );
			analogWrite( C_GREEN_PIN, int( 255 - InitialValue().GetValue().Green ) * PWMRANGE / 255 );
			analogWrite( C_BLUE_PIN, int( InitialValue().GetValue().Blue ) * PWMRANGE / 255 );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetValue() )
				return;

			InitialValue() = AColor;
			UpdateColor();
		}

	public:
		inline void SystemStart()
		{
			UpdateColor();
		}

	};
//---------------------------------------------------------------------------
}

#endif // _MITOV_ESP_COPTER_h