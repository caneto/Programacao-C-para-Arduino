////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_MAXIM_MAX521X_h
#define _MITOV_MAXIM_MAX521X_h

#include <Mitov.h>
#include <SPI.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	enum TArduinoMaximMAX521XShutDownMode { msdmHighImpedance, msdm100K, msdm1K };
//---------------------------------------------------------------------------
	template <typename T_SPI, T_SPI &C_SPI> class Maxim_MAX5214
	{
	public:
		static void SPI_transfer( byte AAddress, word AData )
		{
			AData >>= 2;
			AData |= ((word)AAddress) << 14;
			C_SPI.transfer16( AData );
		}

	};
//---------------------------------------------------------------------------
	template <typename T_SPI, T_SPI &C_SPI> class Maxim_MAX5216
	{
	public:
		static void SPI_transfer( byte AAddress, word AData )
		{
			byte AHighByte = AData >> 10;
			AHighByte |= AAddress << 6;

			AData <<= 6;

			C_SPI.transfer( AHighByte );
			C_SPI.transfer16( AData );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_IMPLEMENTATION_CLASS,
		typename T_ChipSelectOutputPin,
		typename T_ClockInputPin_o_IsConnected,
		typename T_Enabled,
		typename T_ShutDownMode
	> class Maxim_MAX521X :
		public T_ChipSelectOutputPin,
		public T_ClockInputPin_o_IsConnected,
		public T_Enabled,
		public T_ShutDownMode
	{
	public:
		_V_PIN_( ChipSelectOutputPin )

	public:
		_V_PROP_( ClockInputPin_o_IsConnected )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( ShutDownMode )

	public:
		void SetEnabled( bool AValue )
		{
			if( Enabled() == AValue )
				return;

			Enabled() = AValue;
			if( Enabled )
				SPI_write( 2, 0 );

			else
				SPI_write( 2, ( ((word)ShutDownMode()) + 1 ) << 10 );

		}

	protected:
		word FValue = 0;

	protected:
		inline void UpdateValue()
		{
			SPI_write( 1, FValue );
		}

	protected:
		void SPI_write( byte AAddress, word AData )
		{
			T_ChipSelectOutputPin::SetPinValueLow();
			T_IMPLEMENTATION_CLASS::SPI_transfer( AAddress, AData );
			T_ChipSelectOutputPin::SetPinValueHigh();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			UpdateValue();
		}

		inline void InputPin_o_Receive( void *_Data )
        {
			FValue = constrain( *(float*)_Data, 0, 1 ) * 65536 + 0.5;
			if( ! ClockInputPin_o_IsConnected() )
				UpdateValue();
        }

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
