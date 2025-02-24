////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_BASIC_GENERATOR_h
#define _MITOV_BASIC_GENERATOR_h

#include <Mitov.h>

namespace Mitov
{
	template<
		typename T_Amplitude,
		typename T_Asymmetry,
		typename T_BASE,
//		Mitov::TElapsedMicros &C_ElapsedMicros,
		typename T_Enabled, 
		typename T_Frequency,
		typename T_Offset,
		typename T_OutputPin,
		typename T_Phase,
		typename T
	> class TypedFrequencyGenerator :
		public T_BASE,
		public T_Amplitude,
		public T_Asymmetry,
		public T_Offset,
		public T_Frequency,
		public T_Phase,
		public T_Enabled, 
		public T_OutputPin
	{
	public:
		typedef T T_DATA;

	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Frequency )
		_V_PROP_( Amplitude )
		_V_PROP_( Asymmetry )
		_V_PROP_( Offset )

		// 0 - 1
		_V_PROP_( Phase )

	public: // Needs to be public for the base class
//		T	FValue = T( 0 );
		float	FPhase = 0.0f;
//		unsigned long FLastTime = 0;

	public:
		void SetPhase( float AValue )
		{
			if( Phase() == AValue )
				return;

			Phase() = AValue;
			FPhase = AValue;
		}

	public:
		inline void SystemStartInit()
		{
			FPhase = Phase();
		}

		inline void SystemStartGenerate()
		{
			SystemStartInit();
			Generate( false );
		}

		inline void SystemLoopBeginPhase( unsigned long AElapsedMicros )
		{
			if( Enabled() && ( Frequency() != 0 ) )
			{
				float APeriod = 1000000 / Frequency();
				float ATime = AElapsedMicros; //C_ElapsedMicros.GetElapsedTime();
				ATime /= APeriod;
				FPhase += ATime;
				FPhase = V_FMOD( FPhase, 1 );
			}

//			Generate( true );
		}

		inline void SystemLoopBegin()
		{
			Generate( true );
		}

	protected:
		void Generate( bool AChangeOnly )
		{
//			unsigned long ACurrentMicros = micros();
			if( Enabled() && ( Frequency() != 0 ) )
			{
/*
				float APeriod = 1000000 / Frequency();

				float ATime = ( ACurrentMicros - FLastTime );
				ATime /= APeriod;
				FPhase += ATime;
				FPhase = V_FMOD( FPhase, 1 );
*/
			}

			T AValue = T_BASE::CalculateValue( this );
//			FLastTime = ACurrentMicros;
			T_OutputPin::SetPinValue( AValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

	};
//---------------------------------------------------------------------------
/*
	template<
		typename T_Amplitude,
		typename T_Asymmetry,
		typename T_BASE,
		typename T_Enabled, 
		typename T_Frequency,
		typename T_Offset,
		typename T_OutputPin,
		typename T_Phase,
		typename T
	> class TypedAsymmetricFrequencyGenerator : 
		public TypedFrequencyGenerator<
			T_Amplitude,
			T_BASE,
			T_Enabled, 
			T_Frequency,
			T_Offset,
			T_OutputPin,
			T_Phase,
			T>,
		public T_Asymmetry
	{
	public:
		_V_PROP_( Asymmetry )

	};
*/
//---------------------------------------------------------------------------
}

#endif
