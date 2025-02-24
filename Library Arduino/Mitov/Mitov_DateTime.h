////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_DATE_DIME_h
#define _MITOV_DATE_DIME_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FModified,
		typename T_InitialValue,
		typename T_OutputPin
    > class EncodeDateTime :
		public T_Enabled,
		public T_FModified,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( InitialValue )

	protected:
		_V_PROP_( FModified )

		int16_t	FValues[ 7 ] = { 0 };

	public:
		inline void InputPins_o_Receive( int AIndex, void *_Data )
		{
			int32_t	AValue = *(int32_t *)_Data;
			if( FValues[ AIndex ] == AValue )
				return;

			FValues[ AIndex ] = AValue;
			FModified() = true;
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			TDateTime AValue = CalculateOutput();
			T_OutputPin::SetPinValue( AValue, false );
		}

	protected:
		inline void CalculateSendOutput( bool AFromStart )
		{
			TDateTime AValue = CalculateOutput();
			T_OutputPin::SetPinValue( AValue, ! AFromStart );
			FModified() = false;
		}

	public:
		inline void SystemStart()
		{
			CalculateSendOutput( true );
		}

		inline void SystemLoopEnd()
		{
			if( FModified() )
				CalculateSendOutput( false );

		}

	protected:
		inline TDateTime CalculateOutput()
		{
			TDateTime ADate;
			ADate.TryEncodeDateTime( FValues[ 0 ], FValues[ 1 ], FValues[ 2 ], FValues[ 3 ], FValues[ 4 ], FValues[ 5 ], FValues[ 6 ] );
			return ADate;
		}

	public:
		EncodeDateTime()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_Year,
		typename T_OutputPins_Month,
		typename T_OutputPins_Day,
		typename T_OutputPins_Hour,
		typename T_OutputPins_Minute,
		typename T_OutputPins_Second,
		typename T_OutputPins_MilliSecond,
		typename T_OutputPins_WeekDay
	> class DecodeDateTime :
		public T_OutputPins_Year,
		public T_OutputPins_Month,
		public T_OutputPins_Day,
		public T_OutputPins_Hour,
		public T_OutputPins_Minute,
		public T_OutputPins_Second,
		public T_OutputPins_MilliSecond,
		public T_OutputPins_WeekDay
	{
	public:
		_V_PIN_( OutputPins_Year )
		_V_PIN_( OutputPins_Month )
		_V_PIN_( OutputPins_Day )
		_V_PIN_( OutputPins_Hour )
		_V_PIN_( OutputPins_Minute )
		_V_PIN_( OutputPins_Second )
		_V_PIN_( OutputPins_MilliSecond )
		_V_PIN_( OutputPins_WeekDay )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TDateTime &ADate = *(TDateTime *)_Data;
			uint16_t AItem16[ 8 ];
			ADate.DecodeDateTime( AItem16[ 0 ], AItem16[ 1 ], AItem16[ 2 ], AItem16[ 7 ], AItem16[ 3 ], AItem16[ 4 ], AItem16[ 5 ], AItem16[ 6 ] );
			T_OutputPins_Year::SetPinValue( AItem16[ 0 ] );
			T_OutputPins_Month::SetPinValue( AItem16[ 1 ] );
			T_OutputPins_Day::SetPinValue( AItem16[ 2 ] );
			T_OutputPins_Hour::SetPinValue( AItem16[ 3 ] );
			T_OutputPins_Minute::SetPinValue( AItem16[ 4 ] );
			T_OutputPins_Second::SetPinValue( AItem16[ 5 ] );
			T_OutputPins_MilliSecond::SetPinValue( AItem16[ 6 ] );
			T_OutputPins_WeekDay::SetPinValue( AItem16[ 7 ] );
		}
	};	
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPin
	> class CompileDateTime :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		TDateTime	FValue;

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( FValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( FValue );

		}

	public:
		inline CompileDateTime()
		{
			const char *monthName[12] = 
			{
			  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
			  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
			};

			char AMonth[12];
			int ADay, AYear;
			uint8_t AMonthIndex;
			sscanf( __DATE__, "%s %d %d", AMonth, &ADay, &AYear);

			int AHour, AMin, ASec;
			sscanf( __TIME__, "%d:%d:%d", &AHour, &AMin, &ASec);

			for (AMonthIndex = 0; AMonthIndex < 12; AMonthIndex++) 
				if (strcmp(AMonth, monthName[AMonthIndex]) == 0)
					break;

			FValue.TryEncodeDateTime( AYear, AMonthIndex + 1, ADay, AHour, AMin, ASec, 0 );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Days,
		typename T_Enabled,
		typename T_Hours,
		typename T_MilliSeconds,
		typename T_Minutes,
		typename T_Months,
		typename T_OutputPin,
		typename T_Seconds,
		typename T_Years
	> class AddDateTime :
		public T_Days,
		public T_Enabled,
		public T_Hours,
		public T_MilliSeconds,
		public T_Minutes,
		public T_Months,
		public T_OutputPin,
		public T_Seconds,
		public T_Years
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

		_V_PROP_( Years )
		_V_PROP_( Months )
		_V_PROP_( Days )
		_V_PROP_( Hours )
		_V_PROP_( Minutes )
		_V_PROP_( Seconds )
		_V_PROP_( MilliSeconds )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			TDateTime ADate = *(TDateTime *)_Data;
			if( Years() )
				ADate.AddYears( Years() );

			if( Months() )
				ADate.AddMonths( Months() );

			if( Days() )
				ADate.AddDays( Days() );

			if( Hours() )
				ADate.AddHours( Hours() );

			if( Minutes() )
				ADate.AddMinutes( Minutes() );

			if( Seconds() )
				ADate.AddSeconds( Seconds() );

			if( MilliSeconds() )
				ADate.AddMilliSeconds( MilliSeconds() );

			T_OutputPin::SetPinValue( ADate );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif