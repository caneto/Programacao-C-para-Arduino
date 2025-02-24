////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_COLOR_h
#define _MITOV_COLOR_h

#include <Mitov.h>
#include "Mitov_RandomGenerator.h"

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
#define Min Min
#define Max Max
//---------------------------------------------------------------------------
    class RandomColorImplementation
    {
	protected:
		template<typename T> void GenerateValue( T *AInherited )
		{
			int AMin = MitovMin( AInherited->Min().GetValue().Red, AInherited->Max().GetValue().Red );
			int AMax = MitovMax( AInherited->Min().GetValue().Red, AInherited->Max().GetValue().Red );

			AInherited->FValue.Red = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Min().GetValue().Green, AInherited->Max().GetValue().Green );
			AMax = MitovMax( AInherited->Min().GetValue().Green, AInherited->Max().GetValue().Green );

			AInherited->FValue.Green = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Min().GetValue().Blue, AInherited->Max().GetValue().Blue );
			AMax = MitovMax( AInherited->Min().GetValue().Blue, AInherited->Max().GetValue().Blue );

			AInherited->FValue.Blue = random( AMin, AMax + 1 );
		}
    };
//---------------------------------------------------------------------------
/*
	template<
		typename T_Enabled,
		typename T_Max,
		typename T_Min,
		typename T_OutputPin,
		typename T_Seed
	> class RandomColor : 
		public CommonRandomGenerator<
			Mitov::TColor, 
			RandomColorImplementation,
			T_Enabled,
			T_Max,
			T_Min,
			T_OutputPin,
			T_Seed
		>
	{
		typedef CommonRandomGenerator<
			Mitov::TColor, 
			RandomColorImplementation,
			T_Enabled,
			T_Max,
			T_Min,
			T_OutputPin,
			T_Seed
		> inherited;

	};
*/
//---------------------------------------------------------------------------
	class RandomRGBWColorImplementation
    {
	protected:
		template<typename T> void GenerateValue( T *AInherited )
		{
			int AMin = MitovMin( AInherited->Min().GetValue().Red, AInherited->Max().GetValue().Red );
			int AMax = MitovMax( AInherited->Min().GetValue().Red, AInherited->Max().GetValue().Red );

			AInherited->FValue.Red = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Min().GetValue().Green, AInherited->Max().GetValue().Green );
			AMax = MitovMax( AInherited->Min().GetValue().Green, AInherited->Max().GetValue().Green );

			AInherited->FValue.Green = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Min().GetValue().Blue, AInherited->Max().GetValue().Blue );
			AMax = MitovMax( AInherited->Min().GetValue().Blue, AInherited->Max().GetValue().Blue );

			AInherited->FValue.Blue = random( AMin, AMax + 1 );

			AMin = MitovMin( AInherited->Min().GetValue().White, AInherited->Max().GetValue().White );
			AMax = MitovMax( AInherited->Min().GetValue().White, AInherited->Max().GetValue().White );

			AInherited->FValue.White = random( AMin, AMax + 1 );
		}
	};
//---------------------------------------------------------------------------
/*
	class RandomRGBWColor : public Mitov::CommonRandomGenerator<Mitov::TRGBWColor, RandomRGBWColorImplementation>
    {
		typedef Mitov::CommonRandomGenerator<Mitov::TRGBWColor, RandomRGBWColorImplementation> inherited;

	public:
		inline void SystemInit()
		{
			int AMin = MitovMin( inherited::Min.Red, inherited::Max.Red );
			int AMax = MitovMax( inherited::Min.Red, inherited::Max.Red );

			inherited::Min.Red = AMin;
			inherited::Max.Red = AMax;

			AMin = MitovMin( inherited::Min.Green, inherited::Max.Green );
			AMax = MitovMax( inherited::Min.Green, inherited::Max.Green );

			inherited::Min.Green = AMin;
			inherited::Max.Green = AMax;

			AMin = MitovMin( inherited::Min.Blue, inherited::Max.Blue );
			AMax = MitovMax( inherited::Min.Blue, inherited::Max.Blue );

			inherited::Min.Blue = AMin;
			inherited::Max.Blue = AMax;

			AMin = MitovMin( inherited::Min.White, inherited::Max.White );
			AMax = MitovMax( inherited::Min.White, inherited::Max.White );

			inherited::Min.White = AMin;
			inherited::Max.White = AMax;
		}

	public:
		RandomRGBWColor() :
			inherited( 0, 0xFFFFFF )
		{
		}

    };
*/
//---------------------------------------------------------------------------
	template<
		typename T_InitialValue,
		typename T_OutputPin
	> class AnalogToColor :
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PIN_( InitialValue )

	public:
		void RedInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetValue().Red == AValue )
				return;

			InitialValue().GetReference().Red = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void GreenInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetValue().Green == AValue )
				return;

			InitialValue().GetReference().Green = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void BlueInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetValue().Blue == AValue )
				return;

			InitialValue().GetReference().Blue = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( InitialValue().GetValue(), false );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_InitialValue,
		typename T_OutputPin
	> class AnalogToRGBWColor :
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PIN_( InitialValue )

	public:
		void RedInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetValue().Red == AValue )
				return;

			InitialValue().GetReference().Red = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void GreenInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetValue().Green == AValue )
				return;

			InitialValue().GetReference().Green = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void BlueInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetValue().Blue == AValue )
				return;

			InitialValue().GetReference().Blue = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		void WhiteInputPin_o_Receive( void *_Data )
		{
			unsigned char AValue = (*(float *)_Data ) * 255;
			if( InitialValue().GetValue().White == AValue )
				return;

			InitialValue().GetReference().White = AValue;
			T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( InitialValue().GetValue(), false );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_BlueOutputPin,
		typename T_GreenOutputPin,
		typename T_RedOutputPin
	> class ColorToAnalog : 
		public T_BlueOutputPin,
		public T_GreenOutputPin,
		public T_RedOutputPin
	{
	public:
		_V_PIN_( RedOutputPin )
		_V_PIN_( GreenOutputPin )
		_V_PIN_( BlueOutputPin )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;

			T_RedOutputPin::SetPinValue( AColor.Red / 255.0 );
			T_GreenOutputPin::SetPinValue( AColor.Green / 255.0 );
			T_BlueOutputPin::SetPinValue( AColor.Blue / 255.0 );
		}

	};	
//---------------------------------------------------------------------------
	template<
		typename T_BlueOutputPin,
		typename T_GreenOutputPin,
		typename T_RedOutputPin,
		typename T_WhiteOutputPin
	> class RGBWColorToAnalog : 
		public T_BlueOutputPin,
		public T_GreenOutputPin,
		public T_RedOutputPin,
		public T_WhiteOutputPin
	{
	public:
		_V_PIN_( RedOutputPin )
		_V_PIN_( GreenOutputPin )
		_V_PIN_( BlueOutputPin )
		_V_PIN_( WhiteOutputPin )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TRGBWColor &AColor = *(TRGBWColor *)_Data;

			T_RedOutputPin::SetPinValue( AColor.Red / 255.0 );
			T_GreenOutputPin::SetPinValue( AColor.Green / 255.0 );
			T_BlueOutputPin::SetPinValue( AColor.Blue / 255.0 );
			T_WhiteOutputPin::SetPinValue( AColor.White / 255.0 );
		}

	};	
//---------------------------------------------------------------------------
#undef Min
#undef Max
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
