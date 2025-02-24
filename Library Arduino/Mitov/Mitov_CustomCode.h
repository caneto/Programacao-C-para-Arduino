////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_CUSTOM_CODE_h
#define _MITOV_CUSTOM_CODE_h

#include <Mitov.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin
	> class CustomClockOutput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void Clock()
		{
			T_OutputPin::ClockPin();
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPin,
		typename T_DATA
	> class CustomOutput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline CustomOutput<T_OutputPin, T_DATA> & operator = (T_DATA AData ) { Send( AData ); return *this; }

	public:
		inline void Send( T_DATA AData )
		{
			T_OutputPin::SetPinValue( AData );
		}
	};
//---------------------------------------------------------------------------
}

#endif