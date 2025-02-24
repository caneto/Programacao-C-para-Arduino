////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_SQUARE_GENERATOR_h
#define _MITOV_SQUARE_GENERATOR_h

#include <Mitov.h>
#include "Mitov_BasicGenerator.h"

namespace Mitov
{
	class TypedSquareGenerator
	{
//	public:
		// -1 - 1
//		_V_PROP_( Asymmetry )
//		float	Asymmetry = 0.0f;

	protected:
		template<typename T> typename T::T_DATA CalculateValue( T *AInherited )
		{
			if( AInherited->FPhase < 0.5 + AInherited->Asymmetry() / 2 )
				return AInherited->Offset() - AInherited->Amplitude();

			return AInherited->Offset() + AInherited->Amplitude();
		}
	};
//---------------------------------------------------------------------------
}

#endif
