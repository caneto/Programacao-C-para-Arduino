////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_TIMING_h
#define _MITOV_TIMING_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template<
		typename T_OutputPin, 
		typename T_Enabled, 
		typename T_Frequency
	> class ClockGenerator : 
		public T_OutputPin, 
		public T_Enabled, 
		public T_Frequency
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Frequency )
//		T_Enabled	Enabled;
//		T_FREQUENCY	Frequency;

	protected:
		unsigned long FLastTime = 0;
//		unsigned long FPeriod = 0;

	public:
		inline void CalculateFields() {} // Left for backward compatibility

	public:
		inline void SystemLoopBegin()
		{
			if( Frequency() == 0 )
				return;

			unsigned long APeriod = ( (( 1 / Frequency() ) * 1000000 ) + 0.5 );

			while( micros() - FLastTime >= APeriod )
			{
				if( Enabled() && ( Frequency() != 0 ))
					T_OutputPin::ClockPin();

				FLastTime += APeriod;
			}

		}

		inline void SystemInit()
		{
			CalculateFields();
		}

		inline void SystemStart()
		{
			FLastTime = micros();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_AutoRepeat,
		typename T_CanRestart,
		typename T_Enabled,
		typename T_FValue,
		typename T_Interval,
		typename T_OutputPin
	> class Timer :
		public T_OutputPin,
		public T_Enabled,
		public T_Interval,
		public T_AutoRepeat,
		public T_CanRestart,
		protected T_FValue
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Interval )
		_V_PROP_( AutoRepeat )
		_V_PROP_( CanRestart )
		_V_PROP_( Enabled )

	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
				if( AutoRepeat() )
					StartInputPin_o_Receive(nullptr);
			}

			else
			{
				if( FValue() )
				{
					FValue() = false;
					T_OutputPin::SetPinValue( FValue().GetValue() );
				}
			}
		}

	protected:
		_V_PROP_( FValue );
//		bool		FValue = false;

		uint32_t	FStartTime;

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( FValue() );
		}

		inline void SystemLoopBegin()
		{
			if( Enabled() )
			{
				if (!AutoRepeat() )
					if ( ! FValue() )
						return;

				if( FValue() )
					if (!Enabled() )
					{
						FValue() = false;
						T_OutputPin::SetPinValue( FValue() );
						return;
					}

				if (micros() - FStartTime >= Interval() )
				{
					FValue() = ! FValue();
					T_OutputPin::SetPinValue( FValue() );
					if (AutoRepeat() )
						FStartTime += Interval();
				}
			}
		}

	public:
		void StartInputPin_o_Receive(void *_Data)
		{
			if ( ! Enabled().GetValue() )
				return;

			if ( ! CanRestart().GetValue() )
				if ( FValue() )
					return;

			FStartTime = micros();
			if ( FValue() )
				return;

			FValue() = true;
			T_OutputPin::SetPinValue( FValue() );
		}

		void ResetInputPin_o_Receive(void *_Data)
		{
			if( FValue() )
			{
				FValue() = false;
				T_OutputPin::SetPinValue( FValue() );

				if( AutoRepeat() )
					FStartTime = micros();
			}
		}

	public:
		inline Timer()
		{
			FValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OutputPin, 
		typename T_Enabled
	> class Repeat :
		public T_OutputPin,
		public T_Enabled
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		inline void SystemLoopBegin()
		{
			if( Enabled() )
				T_OutputPin::ClockPin();

		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OutputPin
	> class Start :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemStart()
		{
			T_OutputPin::ClockPin();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Count, 
		typename T_Enabled, 
		typename T_OutputPin
	> class RepeatSource :
		public T_OutputPin,
		public T_Enabled,
		public T_Count
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Count )
//		bool	Enabled = true;
//		uint32_t	Count = 1;

	public:
		void InputPin_o_Receive(void *_Data)
		{
			T_OutputPin::Notify( _Data );
			if ( ! Enabled().GetValue() )
				return;

			for (uint32_t i = 0; i < Count(); ++i)
				T_OutputPin::Notify( _Data );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Count, 
		typename T_Enabled, 
		typename T_FRemainingCount, // Ignored
		typename T_Interval, // Ignored
		typename T_OutputPin
	> class RepeatSourceClock :
		public T_OutputPin,
		public T_Enabled,
		public T_Count
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Count )
//		bool	Enabled = true;
//		uint32_t	Count = 1;

	public:
		void InputPin_o_Receive(void *_Data)
		{
			T_OutputPin::Notify( _Data );
			if (!Enabled())
				return;

			for (uint32_t i = 0; i < Count(); ++i)
				T_OutputPin::Notify( _Data );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Count, 
		typename T_Enabled,
		typename T_FRemainingCount,
		typename T_Interval,
		typename T_OutputPin
	> class RepeatSourceClock_Interval :
		public T_OutputPin,
		public T_Enabled,
		protected T_FRemainingCount,
		public T_Interval,
		public T_Count
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Count )
		_V_PROP_( Interval )

	protected:
		_V_PROP_( FRemainingCount )
		uint32_t	FStartTime;

	public:
		void InputPin_o_Receive(void *_Data)
		{
			T_OutputPin::ClockPin();
			if ( ! Enabled().GetValue() )
				return;

			if ( ! Interval().GetValue() )
			{
				for (uint32_t i = 0; i < Count(); ++i)
					T_OutputPin::ClockPin();

				return;
			}

			FStartTime = micros();
			FRemainingCount() = Count();
		}

	public:
		inline void SystemLoopBegin()
		{
			if ( ! Enabled() )
				return;

			if ( ! FRemainingCount() )
				return;

			uint32_t ACurrentTime = micros();
			if ( ACurrentTime - FStartTime < Interval() )
				return;

			T_OutputPin::ClockPin();
			FRemainingCount() = FRemainingCount() - 1;
//			--FRemainingCount();
			FStartTime = ACurrentTime;
		}

	public:
		RepeatSourceClock_Interval()
		{
			FRemainingCount() = 0;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_CanRestart,
		typename T_Enabled, 
		typename T_FValue,
		typename T_Interval, 
		typename T_OutputPin
	> class Delay : 
	    public T_CanRestart,
		public T_Enabled, 
		protected T_FValue, 
		public T_Interval,
		public T_OutputPin 
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Interval )
		_V_PROP_( CanRestart )
		_V_PROP_( Enabled )

	protected:
//		bool			FValue = false;
		_V_PROP_( FValue );
		unsigned long	FStartTime;

	public:
		inline void SystemLoopBegin()
		{
			if (FValue())
			{
				if (! T_Enabled::GetValue() )
					FValue() = false;

				else
				{
					unsigned long ACurrentMicros = micros(); // Use micros(); to make sure the DoReceiveStart is not from the same clock event!
					if (ACurrentMicros - FStartTime >= T_Interval::GetValue() )
					{
						FValue() = false;
						T_OutputPin::ClockPin();
					}
				}
			}
		}

	public:
		void StartInputPin_o_Receive(void *_Data)
		{
			if (! T_Enabled::GetValue() )
				return;

			if ( ! T_CanRestart::GetValue() )
				if (FValue())
					return;

			FStartTime = micros();
			if (FValue())
				return;

			FValue() = true;
		}

		inline void ResetInputPin_o_Receive(void *_Data)
		{
			FValue() = false;
		}


	public:
		Delay()
		{
			FValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_FInputValue,
		typename T_InitialValue,
		typename T_OffInterval,
		typename T_OnInterval,
		typename T_OutputPin
	> class OnOffDelay :
		public T_Enabled,
		public T_FInputValue,
		public T_InitialValue,
		public T_OffInterval,
		public T_OnInterval,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( OnInterval )
		_V_PROP_( OffInterval )
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	protected:
		_V_PROP_( FInputValue ); // Initialized at Start!
		unsigned long	FStartTime;

	public:
		void InputPin_o_Receive(void *_Data)
		{
			bool AValue = *(bool *)_Data;
			if (FInputValue() == AValue)
				return;

			FInputValue() = AValue;
			FStartTime = micros();
		}

	public:
		inline void SystemStart()
		{
			FInputValue() = InitialValue();
			T_OutputPin::SetPinValue( FInputValue() );
		}

		inline void SystemLoopBegin()
		{
			if( FInputValue() == InitialValue() )
				return;

			unsigned long AInterval = (FInputValue()) ? OnInterval() : OffInterval();
			if (micros() - FStartTime < AInterval) // Use micros() to avoid problems when started from the same loop!
				return;

			InitialValue() = FInputValue();
			T_OutputPin::SetPinValue( FInputValue() );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_FLastValue,
		typename T_OutputPin,
		typename T_Rising
	>	class DetectEdge :
		public T_Enabled,
		protected T_FLastValue,
		public T_OutputPin,
		public T_Rising		
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Rising )

	protected:
		_V_PROP_( FLastValue )

	public:
		void InputPin_o_Receive(void *_Data)
		{
			bool AValue = *(bool *)_Data;
			if (AValue == FLastValue() )
				return;

			FLastValue() = AValue;
			if (Rising() == AValue)
				T_OutputPin::ClockPin();

		}

	public:
		DetectEdge()
		{
			FLastValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_MicroSecondsOutputPin,
		typename T_MilliSecondsOutputPin
	> class CPUTimeMicrosMillis :
		public T_Enabled,
		public T_MicroSecondsOutputPin,
		public T_MilliSecondsOutputPin
	{
	public:
		_V_PIN_(MicroSecondsOutputPin )
		_V_PIN_(MilliSecondsOutputPin )

	protected:
		void SendOutputs()
		{
			T_MicroSecondsOutputPin::SetPinValue(micros());
			T_MilliSecondsOutputPin::SetPinValue(millis());
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SendOutputs();
		}

	public:
		inline void SystemStart()
		{
			SendOutputs();
		}

		inline void SystemLoopBegin()
		{
			SendOutputs();
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_MicroSecondsOutputPin,
		typename T_MilliSecondsOutputPin
	> class CPUTimeMicros :
		public T_Enabled,
		public T_MicroSecondsOutputPin,
		public T_MilliSecondsOutputPin
	{
	public:
		_V_PIN_(MicroSecondsOutputPin )

	protected:
		inline void SendOutputs()
		{
			T_MicroSecondsOutputPin::SetPinValue(micros());
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SendOutputs();
		}

	public:
		inline void SystemStart()
		{
			SendOutputs();
		}

		inline void SystemLoopBegin()
		{
			SendOutputs();
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_MicroSecondsOutputPin,
		typename T_MilliSecondsOutputPin
	> class CPUTimeMillis :
		public T_Enabled,
		public T_MicroSecondsOutputPin,
		public T_MilliSecondsOutputPin
	{
	public:
		_V_PIN_(MilliSecondsOutputPin )

	protected:
		inline void SendOutputs()
		{
			T_MilliSecondsOutputPin::SetPinValue(millis());
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SendOutputs();
		}

	public:
		inline void SystemStart()
		{
			SendOutputs();
		}

		inline void SystemLoopBegin()
		{
			SendOutputs();
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_MicroSecondsOutputPin,
		typename T_MilliSecondsOutputPin
	> class CPUTimeMicrosMillis_Enable :
		public T_Enabled,
		public T_MicroSecondsOutputPin,
		public T_MilliSecondsOutputPin
	{
	public:
		_V_PIN_(MicroSecondsOutputPin )
		_V_PIN_(MilliSecondsOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		void SendOutputs()
		{
			if( Enabled() )
			{
				T_MicroSecondsOutputPin::SetPinValue(micros());
				T_MilliSecondsOutputPin::SetPinValue(millis());
			}
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SendOutputs();
		}

	public:
		inline void SystemStart()
		{
			SendOutputs();
		}

		inline void SystemLoopBegin()
		{
			SendOutputs();
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_MicroSecondsOutputPin,
		typename T_MilliSecondsOutputPin
	> class CPUTimeMicros_Enable :
		public T_Enabled,
		public T_MicroSecondsOutputPin,
		public T_MilliSecondsOutputPin
	{
	public:
		_V_PIN_(MicroSecondsOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		inline void SendOutputs()
		{
			if( Enabled() )
				T_MicroSecondsOutputPin::SetPinValue(micros());

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SendOutputs();
		}

	public:
		inline void SystemStart()
		{
			SendOutputs();
		}

		inline void SystemLoopBegin()
		{
			SendOutputs();
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_MicroSecondsOutputPin,
		typename T_MilliSecondsOutputPin
	> class CPUTimeMillis_Enable :
		public T_Enabled,
		public T_MicroSecondsOutputPin,
		public T_MilliSecondsOutputPin
	{
	public:
		_V_PIN_( MilliSecondsOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		inline void SendOutputs()
		{
			if( Enabled() )
				T_MilliSecondsOutputPin::SetPinValue(millis());

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SendOutputs();
		}

	public:
		inline void SystemStart()
		{
			SendOutputs();
		}

		inline void SystemLoopBegin()
		{
			SendOutputs();
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class CPUClockElement :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void SetInputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(Mitov::TDateTime *)_Data;
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SystemStart();
		}

	public:
		inline Mitov::TDateTime GetDateTime()
		{
			return InitialValue();
		}

		inline void SetDateTime( const Mitov::TDateTime &ADateTime )
		{
			InitialValue() = ADateTime;
		}

		void SetMilliSecond( uint8_t AValue )
		{
			uint16_t AHour;
			uint16_t AMin;
			uint16_t ASec;
			uint16_t AMSec;
			InitialValue().GetReference().DecodeTime( AHour, AMin, ASec, AMSec );
			InitialValue().GetReference().TryEncodeTime( AHour, AMin, ASec, AValue );
		}

		void SetSecond( uint8_t AValue )
		{
			uint16_t AHour;
			uint16_t AMin;
			uint16_t ASec;
			uint16_t AMSec;
			InitialValue().GetReference().DecodeTime( AHour, AMin, ASec, AMSec );
			InitialValue().GetReference().TryEncodeTime( AHour, AMin, AValue, AMSec );
		}

		void SetMinute( uint8_t AValue )
		{
			uint16_t AHour;
			uint16_t AMin;
			uint16_t ASec;
			uint16_t AMSec;
			InitialValue().GetReference().DecodeTime( AHour, AMin, ASec, AMSec );
			InitialValue().GetReference().TryEncodeTime( AHour, AValue, ASec, AMSec );
		}

		void SetHour( uint8_t AValue )
		{
			uint16_t AHour;
			uint16_t AMin;
			uint16_t ASec;
			uint16_t AMSec;
			InitialValue().GetReference().DecodeTime( AHour, AMin, ASec, AMSec );
			InitialValue().GetReference().TryEncodeTime( AValue, AMin, ASec, AMSec );
		}

		void SetWeekDay( uint8_t AValue )
		{
		}

		void SetYear( uint16_t AValue )
		{
			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t ADOW;
			InitialValue().GetReference().DecodeDateFully( AYear, AMonth, ADay, ADOW );
			InitialValue().GetReference().TryEncodeDate( AValue, AMonth, ADay );
		}

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue().GetValue(), false );
		}

		inline void SystemLoopBegin()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue().GetValue(), true );
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMillis )
		{
			InitialValue().GetReference().AddMilliSeconds( AElapsedMillis );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
