////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_EVIVE_DISPLAY_h
#define _MITOV_EVIVE_DISPLAY_h

#include <Mitov.h>
#include <Mitov_Display_ST7735.h>
#include <Mitov_Display_ST7735_Const_R.h>
#include <Mitov_Display_ST7735_Const_R2.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
//	template<typename T_SPI, T_SPI &C_SPI, typename T_BACKGROUND_COLOR, typename T_TEXT, typename T_INVERTED, uint8_t C_WIDTH, uint8_t C_HEIGHT, uint8_t C_PIXEL_MODE> class ArduinoEVIVEDisplay : public BasicSPI_ST7735<T_SPI, C_SPI, Mitov::ST7735_Reset_DirectBoardPinImplementation<47>,Mitov::DigitalPin_DirectBoardPinImplementation<49>,Mitov::DigitalPin_DirectBoardPinImplementation<48>, 0, 0>
	template<
		typename T_SPI, T_SPI &C_SPI,
		uint8_t C_WIDTH,
		uint8_t C_HEIGHT,
		uint8_t C_PIXEL_MODE
	> class ArduinoEVIVEDisplay : public BasicSPI_ST7735<T_SPI, C_SPI, Mitov::ST7735_Reset_DirectBoardPinImplementation<47>,Mitov::DigitalPin_DirectBoardPinImplementation<49>,Mitov::DigitalPin_DirectBoardPinImplementation<48>, 0, 0>
	{
		typedef BasicSPI_ST7735<T_SPI, C_SPI, Mitov::ST7735_Reset_DirectBoardPinImplementation<47>,Mitov::DigitalPin_DirectBoardPinImplementation<49>,Mitov::DigitalPin_DirectBoardPinImplementation<48>, 0, 0> inherited;

//	protected:
//		static const uint16_t WIDTH = 128;
//		static const uint16_t HEIGHT = 160;

	public:
		void UpdateOrientation() // Used by the bnding setters
		{
			inherited::writecommand( ST7735::MADCTL );

			inherited::writedata( C_PIXEL_MODE );
/*
			switch( inherited::Orientation )
			{
				case goUp:		inherited::writedata(ST7735::MADCTL_MX | ST7735::MADCTL_MY | ST7735::MADCTL_RGB); break;
				case goLeft:	inherited::writedata(ST7735::MADCTL_MX | ST7735::MADCTL_MV | ST7735::MADCTL_RGB); break;
				case goDown: 	inherited::writedata(ST7735::MADCTL_RGB); break;
				case goRight: 	inherited::writedata(ST7735::MADCTL_MY | ST7735::MADCTL_MV | ST7735::MADCTL_RGB); break;
			}
*/
		}

	public:
		int16_t width(void) const
		{
			return C_WIDTH;
		}

		int16_t height(void) const
		{
			return C_HEIGHT;
		}

	public:
		inline void SystemInit()
		{
			inherited::commonInit( ST7735::Rcmd1 );
			inherited::commandList( ST7735::Rcmd2red );
			inherited::commandList( ST7735::Rcmd3 );
			inherited::writecommand( ST7735::MADCTL );
			inherited::writedata( 0xC0 );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
