////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_GUOYUN_ESP32_ST7735_DISPLAY_h
#define _MITOV_GUOYUN_ESP32_ST7735_DISPLAY_h

#include <Mitov_Display_ST7735.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template<
		typename T_BackgroundColor,
		uint8_t	C_Brightness,
		typename T_Elements_Render,
		typename T_Font,
		typename T_Inverted,
		typename T_IMPLEMENTATION,
		typename T_Text
	> class DisplayST7735Guoyun :
		public DisplayST7735Basic<
				T_BackgroundColor,
				T_Elements_Render,
				T_Font,
				T_Inverted,
				T_IMPLEMENTATION,
				T_Text
			>
	{
		typedef DisplayST7735Basic<
				T_BackgroundColor,
				T_Elements_Render,
				T_Font,
				T_Inverted,
				T_IMPLEMENTATION,
				T_Text
			> inherited;

	public:
		inline void SetBrightness( float AValue )
		{
			analogWrite( 13, AValue * 255 + 0.5 );
		}

	public:
		inline void SystemStart()
		{
			inherited::SystemStart();
			analogWrite( 13, C_Brightness );
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
