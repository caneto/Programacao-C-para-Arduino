////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_LED_BAR_DISPLAY_h
#define _MITOV_LED_BAR_DISPLAY_h

#include <Mitov.h>

namespace Mitov
{
	template<
		typename T_BarsOutputPins_SetPinValue,
		uint32_t COUNT_BarsOutputPins,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_InvertedBars,
		typename T_SingleBar
	> class LEDBarDisplayInteger :
		public T_Enabled,
		public T_InitialValue,
		public T_InvertedBars,
		public T_SingleBar
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( InvertedBars )
		_V_PROP_( SingleBar )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(int32_t*)_Data;
			UpdateDisplay();
		}

	protected:
		inline void UpdateDisplay()
		{
			int32_t AValue = ( Enabled() ) ? InitialValue() : 0;
			DisplayValue( AValue );
		}

	protected:
		void DisplayValue( long AValue )
		{
			for( int i = 0; i < COUNT_BarsOutputPins; ++i )
			{
				bool ABitValue;
				if( SingleBar() )
					ABitValue = ( AValue == 0 ) ^ InvertedBars();

				else
					ABitValue = ( AValue > 0 ) ^ InvertedBars();

				T_BarsOutputPins_SetPinValue::ChainCall( i, ABitValue );
				--AValue;
			}
		}

	public:
		inline void SystemInit()
		{
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_BarsOutputPins_SetPinValue,
		uint32_t COUNT_BarsOutputPins,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_InvertedBars,
		typename T_SingleBar
	> class LEDBarDisplayAnalog :
		public T_Enabled,
		public T_InitialValue,
		public T_InvertedBars,
		public T_SingleBar
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Enabled )

	public:
		_V_PROP_( InvertedBars )
		_V_PROP_( SingleBar )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(float*)_Data;
			UpdateDisplay();
		}

	protected:
		void UpdateDisplay()
		{
			int AValue;
			if( Enabled() )
			{
				if( SingleBar() )
					AValue = InitialValue() * COUNT_BarsOutputPins;

				else
					AValue = InitialValue() * COUNT_BarsOutputPins + 0.5;
			}

			else
				AValue = 0;

			DisplayValue( AValue );
		}

	protected:
		void DisplayValue( long AValue )
		{
			for( int i = 0; i < COUNT_BarsOutputPins; ++i )
			{
				bool ABitValue;
				if( SingleBar() )
					ABitValue = ( AValue == 0 ) ^ InvertedBars();

				else
					ABitValue = ( AValue > 0 ) ^ InvertedBars();

				T_BarsOutputPins_SetPinValue::ChainCall( i, ABitValue );
				--AValue;
			}
		}

	public:
		inline void SystemInit()
		{
			UpdateDisplay();
		}

	};
//---------------------------------------------------------------------------
}

#endif
