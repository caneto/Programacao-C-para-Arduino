////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_DISPLAY_SSD1306_SPI_h
#define _MITOV_DISPLAY_SSD1306_SPI_h

#include <Mitov_Display_SSD1306.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_SPI, T_SPI &C_SPI, 
		typename T_BackgroundColor,
		typename T_ChipSelectOutputPin,
		typename T_DataCommandOutputPin,
		typename T_Elements_Render,
        typename T_Font,
		typename T_PowerOn,
		typename T_ResetOutputPin,
		typename T_Text
	> class Display_SPI : 
		public T_BackgroundColor,
		public T_ChipSelectOutputPin,
		public T_DataCommandOutputPin,
		public T_PowerOn,
		public T_ResetOutputPin
//		public T_Text
	{
	public:
		typedef T_BackgroundColor TYPE_T_BACKGROUND_COLOR;
		typedef T_Text TYPE_T_TEXT;
		typedef T_Font TYPE_T_FONT;

	public:
		_V_PIN_( ChipSelectOutputPin )
		_V_PIN_( DataCommandOutputPin )

	public:
		_V_PROP_( PowerOn )
		_V_PROP_( Text )

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::ChainCall();
		}

	public:
		void SendCommand6( uint8_t ACommand1, uint8_t ACommand2, uint8_t ACommand3, uint8_t ACommand4, uint8_t ACommand5, uint8_t ACommand6 )
		{
			T_DataCommandOutputPin::SetPinValueLow();
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( ACommand1 );
			C_SPI.transfer( ACommand2 );
			C_SPI.transfer( ACommand3 );
			C_SPI.transfer( ACommand4 );
			C_SPI.transfer( ACommand5 );
			C_SPI.transfer( ACommand6 );

			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		void SendCommand( uint8_t ACommand )
		{
//			T_ChipSelectOutputPin::SetPinValue( true );
			T_DataCommandOutputPin::SetPinValueLow();
			T_ChipSelectOutputPin::SetPinValueLow();

			C_SPI.transfer( ACommand );

			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		void SendData16( uint16_t AData )
		{
			StartSendData();

//			C_SPI.transfer( AData >> 8 );
			C_SPI.transfer16( AData );

			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void spiwrite( uint8_t AHi, uint8_t ALo) 
		{
			C_SPI.transfer( AHi );
			C_SPI.transfer( ALo );
		}

		inline void transfer( uint8_t ACommand )
		{
			C_SPI.transfer( ACommand );
		}

		inline void StartSendData()
		{
			T_DataCommandOutputPin::SetPinValueHigh();
			T_ChipSelectOutputPin::SetPinValueLow();
		}

		inline void SetChipSelectHigh()
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void SetChipSelectLow()
		{
			T_ChipSelectOutputPin::SetPinValueLow();
		}

		inline void IntSystemInitStart() 
		{
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

		inline void SetResetHigh()
		{
			T_ResetOutputPin::SetPinValueHigh();
		}

		inline void SetResetLow()
		{
			T_ResetOutputPin::SetPinValueLow();
		}

		inline void SetRegisterSelectHigh()
		{
			T_DataCommandOutputPin::SetPinValueHigh();
		}

		inline void SetRegisterSelectLow()
		{
			T_DataCommandOutputPin::SetPinValueLow();
		}

		inline void IntSystemInitEnd() {} // Placeholder
	};
//---------------------------------------------------------------------------
	template<
		typename T_SPI, T_SPI &C_SPI,
		typename T_BackgroundColor,
		typename T_ChipSelectOutputPin,
		typename T_Contrast,
		typename T_DataCommandOutputPin,
		typename T_Elements_Render,
		typename T_Font,
		uint8_t HEIGHT,
		typename T_PowerOn,
		typename T_PreChargePeriod,
		typename T_ResetOutputPin,
		typename T_Text,
		typename T_UseChargePump,
		uint8_t WIDTH
	> class Display_odtSSD1306_SPI : 
		public Display_SPI<
				T_SPI, C_SPI,
				T_BackgroundColor,
				T_ChipSelectOutputPin,
				T_DataCommandOutputPin,
				T_Elements_Render,
                T_Font,
				T_PowerOn,
				T_ResetOutputPin,
				T_Text
			>,
		public T_PreChargePeriod,
		public T_UseChargePump,
		public T_Contrast
	{
		typedef Display_SPI<
				T_SPI, C_SPI,
				T_BackgroundColor,
				T_ChipSelectOutputPin,
				T_DataCommandOutputPin,
				T_Elements_Render,
                T_Font,
				T_PowerOn,
				T_ResetOutputPin,
				T_Text
			> inherited;

	public:
		_V_PROP_( PreChargePeriod )
		_V_PROP_( UseChargePump )
		_V_PROP_( Contrast )
/*
		inline void SendPinsConfigCommand()
		{
			inherited::SendCommand( C_COMPinsConfig );
		}
*/
		void SendBuffer( uint8_t *ABuffer )
		{
/*
			inherited::SendCommand(SSD1306Const::SSD1306_COLUMNADDR);
			inherited::SendCommand(0);   // Column start address (0 = reset)
			inherited::SendCommand( WIDTH - 1 ); // Column end address (127 = reset)

			inherited::SendCommand(SSD1306Const::SSD1306_PAGEADDR);
			inherited::SendCommand(0); // Page start address (0 = reset)
*/
			inherited::DataCommandOutputPin().SetPinValueHigh();
			inherited::ChipSelectOutputPin().SetPinValueLow();
//			T_ChipSelectOutputPin::StartSendData();
//			T_ChipSelectOutputPin::SetPinValue( true );

			for (uint16_t i = 0; i < ( WIDTH * HEIGHT / 8 ); ++i )
				C_SPI.transfer( ABuffer[ i ] );

			inherited::ChipSelectOutputPin().SetPinValueHigh();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_SPI, T_SPI &C_SPI,
		typename T_BackgroundColor,
		typename T_ChipSelectOutputPin,
		typename T_Contrast,
		typename T_DataCommandOutputPin,
		typename T_Elements_Render,
        typename T_Font,
		uint8_t HEIGHT,
		typename T_PowerOn,
		typename T_PreChargePeriod,
		typename T_ResetOutputPin,
		typename T_Text,
		typename T_UseChargePump,
		uint8_t WIDTH
	> class Display_odtSH1106_SPI :
		public Display_odtSSD1306_SPI<
				T_SPI, C_SPI,
				T_BackgroundColor,
				T_ChipSelectOutputPin,
				T_Contrast,
				T_DataCommandOutputPin,
				T_Elements_Render,
                T_Font,
				HEIGHT,
				T_PowerOn,
				T_PreChargePeriod,
				T_ResetOutputPin,
				T_Text,
				T_UseChargePump,
				WIDTH
			> 
	{
		typedef Display_odtSSD1306_SPI<
				T_SPI, C_SPI,
				T_BackgroundColor,
				T_ChipSelectOutputPin,
				T_Contrast,
				T_DataCommandOutputPin,
				T_Elements_Render,
                T_Font,
				HEIGHT,
				T_PowerOn,
				T_PreChargePeriod,
				T_ResetOutputPin,
				T_Text,
				T_UseChargePump,
				WIDTH
			>  inherited;

	public:
		void SendBuffer( uint8_t *ABuffer )
		{
			inherited::SendCommand(SSD1306Const::SSD1306_SETLOWCOLUMN | 0x0);  // low col = 0
			inherited::SendCommand(SSD1306Const::SSD1306_SETHIGHCOLUMN | 0x0);  // hi col = 0
			inherited::SendCommand(SSD1306Const::SSD1306_SETSTARTLINE | 0x0); // line #0

			byte height=64;
			byte width=132; 
			height >>= 3;
			width >>= 3;
			// I2C
			byte m_row = 0;
			byte m_col = 2;
			int p = 0;
			for ( byte i = 0; i < height; i++)
			{
				// send a bunch of data in one xmission
				inherited::SendCommand(0xB0 + i + m_row);//set page address
				inherited::SendCommand(m_col & 0xf);//set lower column address
				inherited::SendCommand(0x10 | (m_col >> 4));//set higher column address

				for( byte j = 0; j < 8; j++)
				{
//					T_ChipSelectOutputPin::SetPinValue( true );
//					T_DataCommandOutputPin::SetPinValue( true );
//					T_ChipSelectOutputPin::SetPinValue( false );

					inherited::DataCommandOutputPin().SetPinValueHigh();
					inherited::ChipSelectOutputPin().SetPinValueLow();
					for ( byte k = 0; k < width; k++, p++)
						C_SPI.transfer( ABuffer[ p ] );

					inherited::ChipSelectOutputPin().SetPinValueHigh();
				}
			}
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
