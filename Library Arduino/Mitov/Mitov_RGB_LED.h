////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_RGB_LED_h
#define _MITOV_RGB_LED_h

#include <Mitov.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_BlueOutputPin,
		typename T_GreenOutputPin,
		typename T_InitialValue,
		typename T_Inverted,
		typename T_RedOutputPin
	> class RGBLed :
		public T_BlueOutputPin,
		public T_GreenOutputPin,
		public T_InitialValue,
		public T_Inverted,
		public T_RedOutputPin
	{
	public:
		_V_PIN_( RedOutputPin )
		_V_PIN_( GreenOutputPin )
		_V_PIN_( BlueOutputPin )

	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Inverted )

	protected:
		inline float Convert( float AValue )
		{
			if( Inverted() )
				return 1 - AValue;

			return AValue;
		}

		void UpdateColor()
		{
			T_RedOutputPin::SetPinValue( Convert( InitialValue().GetValue().Red / 255.0 ));
			T_GreenOutputPin::SetPinValue( Convert( InitialValue().GetValue().Green / 255.0 ));
			T_BlueOutputPin::SetPinValue( Convert( InitialValue().GetValue().Blue / 255.0 ));
		}

	public:
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
	template<
		typename T_BlueOutputPin,
		typename T_GreenOutputPin,
		typename T_InitialValue,
		typename T_Inverted,
		typename T_RedOutputPin,
		typename T_WhiteOutputPin
	> class RGBWLed :
		public T_BlueOutputPin,
		public T_GreenOutputPin,
		public T_InitialValue,
		public T_Inverted,
		public T_RedOutputPin,
		public T_WhiteOutputPin
	{
	public:
		_V_PIN_( RedOutputPin )
		_V_PIN_( GreenOutputPin )
		_V_PIN_( BlueOutputPin )

	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Inverted )

	protected:
		inline float Convert( float AValue )
		{
			if( Inverted() )
				return 1 - AValue;

			return AValue;
		}

	protected:
		void UpdateColor()
		{
			T_RedOutputPin::SetPinValue( Convert( InitialValue().GetValue().Red / 255.0 ));
			T_GreenOutputPin::SetPinValue( Convert( InitialValue().GetValue().Green / 255.0 ));
			T_BlueOutputPin::SetPinValue( Convert( InitialValue().GetValue().Blue / 255.0 ));
			T_WhiteOutputPin::SetPinValue( Convert( InitialValue().GetValue().White / 255.0 ));
		}

	public:
		inline void SystemStart()
		{
			UpdateColor();
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TRGBWColor &AColor = *(TRGBWColor *)_Data;
			if( AColor == InitialValue() )
				return;

			InitialValue() = AColor;
			UpdateColor();
		}
	};
//---------------------------------------------------------------------------
}

#endif
