////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_BINARY_GENERATORS_h
#define _MITOV_BINARY_GENERATORS_h

#include <Mitov.h>

namespace Mitov
{
	template <
		typename T_Asymmetry,
		typename T_Enabled,
		typename T_Frequency,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_Phase
	> class PulseGeneratorSimple :
		public T_Asymmetry,
		public T_Enabled,
		public T_Frequency,
		public T_InitialValue,
		public T_OutputPin,
		public T_Phase
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Frequency )

	protected:
		unsigned long FLastTime = 0;
//		unsigned long FPeriod = 0;

/*
	public:
		void SetFrequency( float AValue )
		{
			if( Frequency == AValue )
				return;

			Frequency = AValue;
			CalculateFields();
		}

	public:
		void CalculateFields()
		{
			if( Frequency == 0 )
				FPeriod = 1000000;

			else
				FPeriod = ( (( 1 / ( Frequency * 2 )) * 1000000 ) + 0.5 ); // 2 times one for each switch

		}
*/
	public:
		inline void SystemLoopBegin()
		{
			if( Frequency() == 0 )
				return;

			unsigned long APeriod = ( (( 1 / ( Frequency() * 2 )) * 1000000 ) + 0.5 ); // 2 times one for each switch
			while( micros() - FLastTime >= APeriod )
			{
				if( Enabled() && ( Frequency() != 0 ))
				{
					InitialValue() = ! InitialValue();
					T_OutputPin::SetPinValue( InitialValue(), true );
				}

				FLastTime += APeriod;
			}
		}

		inline void SystemStart()
		{
			FLastTime = micros();
			T_OutputPin::SetPinValue( InitialValue(), false );
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Asymmetry,
		typename T_Enabled,
		typename T_Frequency,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_Phase
	> class PulseGenerator :
		public T_Asymmetry,
		public T_Enabled,
		public T_Frequency,
		public T_InitialValue,
		public T_OutputPin,
		public T_Phase
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Frequency )

		// -1 - 1
		_V_PIN_( Asymmetry )

		// 0 - 1
		_V_PIN_( Phase )

		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		void SetPhase( float AValue )
		{
			if( Phase() == AValue )
				return;

			Phase() = AValue;
			FPhase = AValue;
		}

	protected:
		float	FPhase = 0.0f;
		unsigned long FLastTime = 0;

	public:
		inline void SystemStart()
		{
			FPhase = Phase();
			T_OutputPin::SetPinValue( InitialValue(), false );
		}

		inline void SystemLoopBegin()
		{
			unsigned long ACurrentMicros = micros();
			if( Enabled() && ( Frequency() != 0 ) )
			{
				float APeriod = 1000000 / Frequency();

				float ATime = ( ACurrentMicros - FLastTime );
				ATime /= APeriod;
				FPhase += ATime;
				FPhase = V_FMOD( FPhase, 1 );

				bool AValue = ( FPhase < 0.5 + Asymmetry() / 2 );
				if( AValue != InitialValue() )
				{
					InitialValue() = AValue;
					T_OutputPin::SetPinValue( InitialValue(), true );
				}
			}

			FLastTime = ACurrentMicros;
//			OutputPin.SendValue( FValue );
		}

	};
//---------------------------------------------------------------------------
}

#endif
