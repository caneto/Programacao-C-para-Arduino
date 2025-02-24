////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ESP32_PROCESSOR_h
#define _MITOV_ESP32_PROCESSOR_h

#include <Mitov.h>
#include <Esp.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	class ESP32ProcessorReset
	{
	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ESP.restart();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Period
	> class ESP32ProcessorDeepSleep :
		public T_Period
	{
	public:
		_V_PROP_( Period )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ESP.deepSleep( Period() );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_OutputPin
	> class ESP32GetArduinoAvailableRAM : 
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue( false );
		}

	protected:
		inline void ReadValue( bool AChangeOnly )
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( ESP.getFreeHeap(), AChangeOnly );

		}

	public:
		inline void SystemStart()
		{
			ReadValue( false );
		}

		inline void SystemLoopBegin()
		{
			ReadValue( true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
