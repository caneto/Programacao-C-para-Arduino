////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_MULTI_MERGER_h
#define _MITOV_MULTI_MERGER_h

#include <Mitov.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		int C_NUM_INPUTS,
		typename T_OutputPin
	> class MultiMerger :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			T_OutputPin::PinNotify( _Data );
		}

	};
}

#endif
