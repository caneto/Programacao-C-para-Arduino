////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_COUNTER_h
#define _MITOV_COUNTER_h

#include <Mitov.h>

namespace Mitov
{
#define Min Min
#define Max Max
	template<typename T_VALUE, typename T_ROLL_OVER> class CounterLimit : public T_VALUE, public T_ROLL_OVER
	{
	public:
		inline T_VALUE		&Value() { T_VALUE::GetInstance(); }
		inline T_ROLL_OVER	&RollOver() { T_ROLL_OVER::GetInstance(); }

	};
//---------------------------------------------------------------------------
	template<
		typename T_OutputPin, 
		typename T_Enabled,
		typename T_FCount,
		typename T_InitialValue,
		typename T_Min, 
		typename T_Max, 
		typename T_NeedsUpdate
	> class BasicCounter : 
		public T_OutputPin, 
		public T_Enabled,
		public T_FCount,
		public T_InitialValue, 
		public T_Min, 
		public T_Max, 
		protected T_NeedsUpdate
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Min )
		_V_PROP_( Max )

//    public:
//        T_ENABLED Enabled;
//		bool Reversed : 1;		// Added here to save memory

    public:
		_V_PROP_( NeedsUpdate )

	protected:
		_V_PROP_( FCount )

	public:
		inline void SetValue( int32_t AValue )
		{
			FCount() = AValue;
			T_OutputPin::SetPinValue( FCount() );
		}

	public:
		inline void SystemInit()
		{
//			Serial.println( "Counter::SystemInit" );
			FCount() = InitialValue();

			T_OutputPin::SetPinValue( FCount() );
		}

		inline void SystemStart()
		{
			if( FCount() != InitialValue() )
				T_OutputPin::SetPinValue( FCount() );

		}

		inline void SystemLoopBegin() // For compatibility with Interrupt version
		{
			if( NeedsUpdate() )
			{
				NeedsUpdate() = false;
				T_OutputPin::SetPinValue( FCount() );
			}
		}

	protected:
		inline void	CountUp( bool ASendOutput )
		{
			if( T_Max::RollOver() || ( FCount() < T_Max::Value() ))
			{
				FCount() = FCount() + 1;  // Bitfields can't use reference for ++
//				++FCount;

				if( FCount() > T_Max::Value() )
					FCount() = T_Min::Value();

				if( ASendOutput )
					T_OutputPin::SetPinValue( FCount() );

				else
					NeedsUpdate() = true;

			}
		}

		inline void	CountDown( bool ASendOutput )
		{
			if( T_Min::RollOver() || ( FCount() > T_Min::Value() ))
			{
				FCount() = FCount() - 1;  // Bitfields can't use reference for --
//				--FCount();
				if( FCount() < T_Min::Value() )
					FCount() = T_Max::Value();

				if( ASendOutput )
					T_OutputPin::SetPinValue( FCount() );

				else
					NeedsUpdate() = true;

			}
		}

	public:
		void ResetInputPin_o_Receive( void *_Data )
		{
			FCount() = InitialValue();
			T_OutputPin::SetPinValue( FCount() );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Value
	> class ArduinoCounterSetValueElement :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.SetValue( Value() );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled, 
		typename T_FCount,
		typename T_InitialValue, 
		typename T_Max, 
		typename T_Min, 
		typename T_NeedsUpdate, 
		typename T_OutputPin,
		typename T_Reversed
	> class Counter : public Mitov::BasicCounter<T_OutputPin, T_Enabled, T_FCount, T_InitialValue, T_Min, T_Max, T_NeedsUpdate>, 
		public T_Reversed
	{
		typedef Mitov::BasicCounter<T_OutputPin, T_Enabled, T_FCount, T_InitialValue, T_Min, T_Max, T_NeedsUpdate> inherited;

	public:
		_V_PROP_( Reversed )
//		T_REVERSED	Reversed;

	protected:
		void UpdateCount( bool ASendOutput )
		{
            if( ! inherited::Enabled() )
				return;

			if( Reversed() )
				inherited::CountDown( ASendOutput );

			else
				inherited::CountUp( ASendOutput );

		}

	public:
		inline void InterruptHandler()
		{
			UpdateCount( false );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			UpdateCount( true );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled, 
		typename T_FCount,
		typename T_InitialValue, 
		typename T_Max, 
		typename T_Min, 
		typename T_NeedsUpdate, 
		typename T_OutputPin
		> class UpDownCounter :
			public Mitov::BasicCounter <
					T_OutputPin,
					T_Enabled,
					T_FCount,
					T_InitialValue,
					T_Min,
					T_Max,
					T_NeedsUpdate
				>
	{
		typedef Mitov::BasicCounter <
				T_OutputPin,
				T_Enabled,
				T_FCount,
				T_InitialValue,
				T_Min,
				T_Max,
				T_NeedsUpdate
			> inherited;

	public:
		void InterruptHandler_Up()
		{
            if( inherited::Enabled() )
				inherited::CountUp( false );
		}

		void InterruptHandler_Down()
		{
            if( inherited::Enabled() )
				inherited::CountDown( false );
		}

	public:
		inline void UpInputPin_o_Receive( void *_Data )
		{
            if( inherited::Enabled() )
				inherited::CountUp( true );
		}

		inline void DownInputPin_o_Receive( void *_Data )
		{
            if( inherited::Enabled() )
				inherited::CountDown( true );
		}

	};
//---------------------------------------------------------------------------
#undef Min
#undef Max
}

#endif
