////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_RGB_PEGUINO_LED_h
#define _MITOV_RGB_PEGUINO_LED_h

#include <Mitov.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_InitialValue,
		uint8_t C_RED_PIN,
		uint8_t C_GREEN_PIN,
		uint8_t C_BLUE_PIN
	> class PeguinoRGBLed :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
            analogWrite( C_RED_PIN, ( ( InitialValue().GetValue().Red / 255.0 ) * PWMRANGE ) + 0.5 );
            analogWrite( C_GREEN_PIN, ( ( InitialValue().GetValue().Green / 255.0 ) * PWMRANGE ) + 0.5 );
            analogWrite( C_BLUE_PIN, ( ( InitialValue().GetValue().Blue / 255.0 ) * PWMRANGE ) + 0.5 );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_RED_PIN, OUTPUT );
			pinMode( C_GREEN_PIN, OUTPUT );
			pinMode( C_BLUE_PIN, OUTPUT );
		}

		inline void SystemStart()
		{
			UpdateColor();
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue() )
				return;

			InitialValue() = AColor;
			UpdateColor();
		}
	};
//---------------------------------------------------------------------------
}

#endif // _MITOV_RGB_PEGUINO_LED_h
