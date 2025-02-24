////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_SHIELD_JOYSTICK_DISPLAY_h
#define _MITOV_SHIELD_JOYSTICK_DISPLAY_h

#include <Mitov.h>
#include <Mitov_SoftwareSPI.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------

Mitov::VisuinoSoftwareSPI<
  10, // 0
  9 // 1
  > GSoftwareSPI_Nokia;

//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif // _MITOV_SHIELD_JOYSTICK_DISPLAY_h
