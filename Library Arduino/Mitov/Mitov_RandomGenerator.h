////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_RANDOM_GENERATOR_h
#define _MITOV_RANDOM_GENERATOR_h

#include <Mitov.h>

namespace Mitov
{
#define Min Min
#define Max Max
	template<
		typename T, 
		typename T_BASE,
		typename T_Enabled,
		typename T_Max,
		typename T_Min,
		typename T_OutputPin,
		typename T_Seed
	> class CommonRandomGenerator : 
		public T_BASE,
		public T_Enabled,
		public T_Max,
		public T_Min,
		public T_OutputPin,
		public T_Seed
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public: // Needs to be public due to compiler bug :-(
		T	FValue = T( 0 );

	public:
		_V_PROP_( Min )
		_V_PROP_( Max )
		_V_PROP_( Seed )

	public:
		inline void SystemStartInit()
		{
			randomSeed( Seed() );
		}

		inline void SystemStartGenerate()
		{
			SystemStartInit();
			Generate( false );
		}

		inline void SystemLoopBegin()
		{
			Generate( true );
		}

	protected:
		void Generate( bool AChangeOnly )
		{
            if( Enabled() )
            {
				if( Min().GetValue() == Max().GetValue() )
					FValue = Min();

				else
					T_BASE::GenerateValue( this );
			}

			T_OutputPin::SetPinValue( FValue, AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

	};
//---------------------------------------------------------------------------
	class RandomAnalogGenerator
	{
	protected:
		template<typename T> void GenerateValue( T *AInherited )
		{
			float AMin = MitovMin( AInherited->Min().GetValue(), AInherited->Max().GetValue() );
			float AMax = MitovMax( AInherited->Min().GetValue(), AInherited->Max().GetValue() );
//			double ARandom = random( -2147483648, 2147483647 );
			double ARandom = random( -1147483648, 1147483647 );
//			FValue = ARandom;
			ARandom += 1147483648;
			AInherited->FValue = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);
		}

	};
//---------------------------------------------------------------------------
	template<typename T_DATA> class RandomIntegerGenerator
	{
	protected:
		template<typename T> void GenerateValue( T *AInherited )
		{
			T_DATA AMin = MitovMin( AInherited->Min().GetValue(), AInherited->Max().GetValue() );
			T_DATA AMax = MitovMax( AInherited->Min().GetValue(), AInherited->Max().GetValue() );
			AInherited->FValue = random( AMin, AMax + 1 );
		}

	};
//---------------------------------------------------------------------------
	class RandomComplexImplementation
	{
	protected:
		template<typename T> void GenerateValue( T *AInherited )
		{
			int AMin = MitovMin( AInherited->Min().GetValue().Real, AInherited->Max().GetValue().Real );
			int AMax = MitovMax( AInherited->Min().GetValue().Real, AInherited->Max().GetValue().Real );

			double ARandom = random( -1147483648, 1147483647 );
			ARandom += 1147483648;
			AInherited->FValue.Real = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);

			AMin = MitovMin( AInherited->Min().GetValue().Imaginary, AInherited->Max().GetValue().Imaginary );
			AMax = MitovMax( AInherited->Min().GetValue().Imaginary, AInherited->Max().GetValue().Imaginary );

			ARandom = random( -1147483648, 1147483647 );
			ARandom += 1147483648;
			AInherited->FValue.Imaginary = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_FValue,
		typename T_OutputPin,
		typename T_Seed
	>class RandomDigitalGenerator :
		public T_Enabled,
		public T_FValue,
		public T_OutputPin,
		public T_Seed
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FValue )

	public:
		_V_PROP_( Seed )

	public:
		inline void SystemStartInit()
		{
			randomSeed( Seed() );
		}

		inline void SystemStartGenerate()
		{
			SystemStartInit();
			Generate( false );
		}

		inline void SystemLoopBegin()
		{
			Generate( true );
		}

	protected:
		inline void GenerateValue()
		{
			FValue() = random( 0, 2 );
		}

		inline void Generate( bool AChangeOnly )
		{
            if( Enabled() )
				GenerateValue();

			T_OutputPin::SetPinValue( FValue(), AChangeOnly );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Generate( false );
		}

	public:
		RandomDigitalGenerator()
		{
			FValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_MaxFrequency,
		typename T_MinFrequency,
		typename T_OutputPin,
		typename T_Seed
	> class RandomClockGenerator :
		public T_Enabled,
		public T_MaxFrequency,
		public T_MinFrequency,
		public T_OutputPin,
		public T_Seed
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		unsigned long FLastTime = 0;
		unsigned long FPeriod = 0;

	public:
		_V_PROP_( Seed )

		_V_PROP_( Enabled )
		_V_PROP_( MinFrequency )
		_V_PROP_( MaxFrequency )

	protected:
		void GeneratePeriod()
		{
			float AMin = MitovMin( MinFrequency().GetValue(), MaxFrequency().GetValue() );
			float AMax = MitovMax( MinFrequency().GetValue(), MaxFrequency().GetValue() );
			double ARandom = random( -1147483648, 1147483647 );
			ARandom += 1147483648;
			ARandom = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);
			ARandom = MitovMax( (double)AMin, MitovMin( (double)AMax, (double)ARandom ));

			if( ARandom == 0 )
				FPeriod = 1000000;

			else
				FPeriod = ( (( 1.0 / ARandom ) * 1000000 ) + 0.5 );

		}

	public:
		inline void SystemLoopBegin()
		{
			while( micros() - FLastTime >= FPeriod )
			{
				if( Enabled() )
					T_OutputPin::ClockPin();

				FLastTime += FPeriod;
				GeneratePeriod();
			}
		}

		inline void SystemStart()
		{
			GeneratePeriod();
			FLastTime = micros();
		}

	};
//---------------------------------------------------------------------------
	class RandomDateTimeGenerator
	{
	protected:
		template<typename T> void GenerateValue( T *AInherited )
		{
			int32_t ARandomDate = random( AInherited->Min().GetValue().Date, AInherited->Max().GetValue().Date );
			int32_t ARandomTime;
			if( ARandomDate == AInherited->Min().GetValue().Date )
				ARandomTime = random( AInherited->Min().GetValue().Time, MSecsPerDay );

			else if( ARandomDate == AInherited->Max().GetValue().Date )
				ARandomTime = random( 0, AInherited->Max().GetValue().Time );

			else
				ARandomTime = random( 0, MSecsPerDay );

			AInherited->FValue.Date = ARandomDate;
			AInherited->FValue.Time = ARandomTime;
/*
			float AMin = MitovMin( Min.Value, Max.Value );
			float AMax = MitovMax( Min.Value, Max.Value );
//			double ARandom = random( -2147483648, 2147483647 );
			double ARandom = random( -1147483648, 1147483647 );
//			FValue = ARandom;
			ARandom += 1147483648;
			FValue.Value = AMin + ( ARandom / ( (double)1147483647 + (double)1147483648 )) * (AMax - AMin);
*/
		}

	};
//---------------------------------------------------------------------------
#undef Min
#undef Max
}

#endif
