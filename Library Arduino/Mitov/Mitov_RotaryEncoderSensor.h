////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ROTARY_ENCODER_SENSOR_h
#define _MITOV_ROTARY_ENCODER_SENSOR_h

#include <Mitov.h>

namespace Mitov
{
	template<
		typename T_B_PIN, T_B_PIN &C_B_PIN,
		typename T_DebounceInterval,
		typename T_DownOutputPin,
		typename T_Enabled,
		typename T_FAValue,
		typename T_UpOutputPin
	> class RotaryEncoderSensor :
		public T_DebounceInterval,
		public T_DownOutputPin,
		public T_Enabled,
		public T_FAValue,
		public T_UpOutputPin
	{
	public:
		_V_PIN_( UpOutputPin )
		_V_PIN_( DownOutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FAValue )
//		bool	FBValue : 1;
//		bool    FAPopulated : 1;
//		bool    FBPopulated : 1;

	public:
		inline void AInputPin_o_Receive( void *_Data )
		{
			bool AValue = *( bool *)_Data;

//			FAPopulated = true;
			if( FAValue() == AValue )
				return;

			FAValue() = AValue;
			if( Enabled() )
			{
				if( AValue )
				{
					if( C_B_PIN.DigitalRead() )
						T_DownOutputPin::ClockPin();

					else
						T_UpOutputPin::ClockPin();
				}
				else
				{
					if( C_B_PIN.DigitalRead() )
						T_UpOutputPin::ClockPin();

					else
						T_DownOutputPin::ClockPin();
				}
			}
		}

/*
		void DoReceiveB( bool AValue )
		{
			FBPopulated = true;
			if( FBValue == AValue )
				return;

			FBValue = AValue;
		}
*/
	public:
		inline void SystemLoopBegin() {} // Placeholder

	public:
		inline RotaryEncoderSensor()
//			FAPopulated( false ),
//			FBPopulated( false ),
//			FBValue( false )
		{
			FAValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_B_PIN, T_B_PIN &C_B_PIN,
		typename T_DebounceInterval,
		typename T_DownOutputPin,
		typename T_Enabled,
//		typename T_FAValue,
		typename T_UpOutputPin
	> class RotaryEncoderSensor_Interrupt : public OpenWire::InterruptObject,
		public T_DebounceInterval,
		public T_DownOutputPin,
		public T_Enabled,
//		public T_FAValue,
		public T_UpOutputPin
	{
	public:
		_V_PIN_( UpOutputPin )
		_V_PIN_( DownOutputPin )

	public:
		_V_PROP_( Enabled )

//	protected:
//		_V_PROP_( FAValue )

	protected:
		uint8_t FUpCount = 0;

//		bool	FBValue : 1;
//		bool    FAPopulated : 1;
//		bool    FBPopulated : 1;
		uint8_t FDownCount = 0;

	public:
		void InterruptHandlerA( bool AValue )
		{
//			FAPopulated = true;
//			if( FAValue() == AValue )
//				return;

//			FAValue() = AValue;
			if( Enabled() )
			{
				if( AValue )
				{
					if( C_B_PIN.DigitalRead() )
						++FDownCount;

					else
						++FUpCount;
				}
				else
				{
					if( C_B_PIN.DigitalRead() )
						++FUpCount;

					else
						++FDownCount;
				}
			}
		}

/*
	protected:
		void DoReceiveB( void *_Data )
		{
			bool AValue = *( bool *)_Data;
			FBPopulated = true;
			if( FBValue == AValue )
				return;

			FBValue = AValue;
		}
*/
	public:
		inline void SystemLoopBegin()
		{
			noInterrupts();
			uint8_t ADownCount = FDownCount;
			FDownCount = 0;

			uint8_t AUpCount = FUpCount;
			FUpCount = 0;

			interrupts();

			while( ADownCount )
			{
				--ADownCount;
				T_DownOutputPin::ClockPin();
			}

			while( AUpCount )
			{
				--AUpCount;
				T_UpOutputPin::ClockPin();
			}

		}

/*
	public:
		inline RotaryEncoderSensor_Interrupt()
//			FAPopulated( false ),
//			FBPopulated( false ),
//			FBValue( false )
		{
			FAValue() = false;
		}
*/
	};
//---------------------------------------------------------------------------
	template<
		typename T_B_PIN, T_B_PIN &C_B_PIN,
		typename T_DebounceInterval,
		typename T_DownOutputPin,
		typename T_Enabled,
		typename T_FALastValue,
		typename T_FAValue,
		typename T_UpOutputPin
	> class RotaryEncoderSensor_Debounce :
		public T_DebounceInterval,
		public T_DownOutputPin,
		public T_Enabled,
		public T_FALastValue,
		public T_FAValue,
		public T_UpOutputPin
	{
	public:
		_V_PIN_( UpOutputPin )
		_V_PIN_( DownOutputPin )

	public:
		_V_PROP_( DebounceInterval )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FAValue )
//		bool	FBValue : 1;
//		bool    FAPopulated : 1;
//		bool    FBPopulated : 1;
		_V_PROP_( FALastValue )
//		bool	FInTimer : 1;
//		bool	FBLastValue : 1;

		unsigned long	FALastTime = 0;
//		unsigned long	FBLastTime = 0;

	protected:
		void DoValueReceiveA( bool AValue )
		{
//			FAPopulated = true;
//			FInTimer = false;
			if( FAValue() == AValue )
				return;

			FAValue() = AValue;
			if( Enabled() )
			{
				if( AValue )
				{
					if( C_B_PIN.DigitalRead() )
						T_DownOutputPin::ClockPin();

					else
						T_UpOutputPin::ClockPin();
				}
				else
				{
					if( C_B_PIN.DigitalRead() )
						T_UpOutputPin::ClockPin();

					else
						T_DownOutputPin::ClockPin();
				}
			}
		}

/*
		void DoValueReceiveB( bool AValue )
		{
			FBPopulated = true;
			if( FBValue == AValue )
				return;

			FBValue = AValue;
		}
*/
	public:
		inline void AInputPin_o_Receive( void *_Data )
		{
			bool AValue = *( bool *)_Data;
			if( AValue != FALastValue() )
			{
				FALastTime = millis();
//				FInTimer = true;
			}

			FALastValue() = AValue;
		}

/*
		void DoReceiveB( void *_Data )
		{
			bool AValue = *( bool *)_Data;
			if( AValue != FBLastValue )
				FBLastTime = millis();

			FBLastValue = AValue;
		}
*/

	public:
		inline void SystemLoopBegin()
		{
			if( FAValue().GetValue() != FALastValue().GetValue() )
//				if(( ! FInTimer ) || ( millis() - FALastTime > DebounceInterval ))
				if( millis() - FALastTime > DebounceInterval() )
					DoValueReceiveA( FALastValue() );

/*
			if( FBValue != FBLastValue )
				if( millis() - FBLastTime > DebounceInterval )
					DoValueReceiveB( FBLastValue );
*/
		}

	public:
		inline RotaryEncoderSensor_Debounce()
//			FInTimer( false )
		{
			FAValue() = false;
			FALastValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_B_PIN, T_B_PIN &C_B_PIN,
		typename T_DebounceInterval,
		typename T_DownOutputPin,
		typename T_Enabled,
		typename T_FInTimer,
//		typename T_FAValue,
		typename T_UpOutputPin
	> class RotaryEncoderSensor_Debounce_Interrupt : public OpenWire::InterruptObject,
		public T_DebounceInterval,
		public T_DownOutputPin,
		public T_Enabled,
		public T_FInTimer,
//		public T_FAValue,
		public T_UpOutputPin
	{
	public:
		_V_PIN_( UpOutputPin )
		_V_PIN_( DownOutputPin )

	public:
		_V_PROP_( DebounceInterval )
		_V_PROP_( Enabled )

	protected:
//		_V_PROP_( FAValue )

		_V_PROP_( FInTimer )
		uint8_t FUpCount = 0;

		uint8_t FDownCount = 0;

		unsigned long	FALastTime = 0;

	public:
		void InterruptHandlerA( bool AValue )
		{
			if( ! AValue )
				return;

//			if( FAValue == AValue )
//				return;

			unsigned long	ATime = millis();
			if( FInTimer() )
				if(( ATime - FALastTime ) <= DebounceInterval() )
					return;

//			FAValue = AValue;
			if( Enabled() )
			{
				FInTimer() = true;
				FALastTime = ATime;
				if( AValue )
				{
					if( C_B_PIN.DigitalRead() )
						++FDownCount;

					else
						++FUpCount;
				}

				else
				{
					if( C_B_PIN.DigitalRead() )
						++FUpCount;

					else
						++FDownCount;
				}
			}
		}

	public:
		inline void SystemLoopBegin()
		{
			unsigned long ATime = millis();

			noInterrupts();
			if( FInTimer() )
				if( ATime - FALastTime > DebounceInterval() )
					FInTimer() = false;

			uint8_t ADownCount = FDownCount;
			FDownCount = 0;

			uint8_t AUpCount = FUpCount;
			FUpCount = 0;

			interrupts();

			while( ADownCount )
			{
				--ADownCount;
				T_DownOutputPin::ClockPin();
			}

			while( AUpCount )
			{
				--AUpCount;
				T_UpOutputPin::ClockPin();
			}

		}

	public:
		inline RotaryEncoderSensor_Debounce_Interrupt()
		{
			FInTimer() = false;
		}

	};
//---------------------------------------------------------------------------
}

#endif
