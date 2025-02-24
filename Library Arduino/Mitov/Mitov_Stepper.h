////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_STEPPER_h
#define _MITOV_STEPPER_h

#include <Mitov.h>

namespace Mitov
{

/*
	enum StepperMotorType =
	{
		smtDirectional,
		smtFull2Wire,
		smtFull3Wire,
		smtFull4Wire,
		smtHalf3Wire,
		smtHalf4Wire
	};
*/
	const uint8_t C_StepperBits4[] =
	{
			0b1100,
			0b0110,
			0b0011,
			0b1001
	};

	const uint8_t C_StepperBits8[] =
	{
			0b1000,
			0b1100,
			0b0100,
			0b0110,
			0b0010,
			0b0011,
			0b0001,
			0b1001
	};
//---------------------------------------------------------------------------
	const uint8_t C_Stepper3Bits3[] =
	{
			0b110,
			0b011,
			0b101
	};

	const uint8_t C_Stepper3Bits6[] =
	{
			0b100,
			0b110,
			0b010,
			0b011,
			0b001,
			0b101
	};
//---------------------------------------------------------------------------
/*
	class StepperMotor4Wire
	{
	public:
		template<typename T_INSTANCE> static inline void UpdatePinsValue( T_INSTANCE *AInstance )
		{
			uint8_t AValue = AInstance->HalfStep() ? C_StepperBits8[ AInstance->FStep() & 0b0111 ] : C_StepperBits4[ AInstance->FStep() & 0b0011 ];
			for( int i = 0; i < 4; ++i )
			{
				bool APinValue = ( AValue & 1 );

				AInstance->SetChannelValue( i, APinValue );

				AValue >>= 1;
			}
		}

	};
//---------------------------------------------------------------------------
	class StepperMotor3Wire
	{
	public:
		template<typename T_INSTANCE> static inline void UpdatePinsValue( T_INSTANCE *AInstance )
		{
			uint8_t AValue = AInstance->HalfStep() ? C_Stepper3Bits3[ AInstance->FStep() % 3 ] : C_Stepper3Bits6[ AInstance->FStep() % 6 ];
			for( int i = 0; i < 4; ++i )
			{
				bool APinValue = ( AValue & 1 );

				AInstance->SetChannelValue( i, APinValue );

				AValue >>= 1;
			}
		}

	};
*/
//---------------------------------------------------------------------------
	class StepperMotorSpeed
	{
	protected:
		unsigned long	FLastTime = 0;

	protected:
		inline void SystemStart()
		{
			FLastTime = micros();
		}

		template<typename T> inline void SystemLoopBegin( T *AInstance )
		{
			unsigned long ACurrentMicros = micros();
			if( ! AInstance->Enabled() )
				FLastTime = ACurrentMicros;

			else
			{
				unsigned long APeriod = ( 1000000 / fabs( AInstance->StepsPerSecond() )) + 0.5;
				if( ( ACurrentMicros - FLastTime ) >= APeriod )
				{
					FLastTime += APeriod;
					if( AInstance->StepsPerSecond() )
					{
						if(( AInstance->StepsPerSecond() > 0 ) ^ AInstance->Reversed() )
							AInstance->FStep() = AInstance->FStep() + 1;

						else
							AInstance->FStep() = AInstance->FStep() - 1;

						AInstance->UpdatePins();
					}
				}
			}
		}
	};
//---------------------------------------------------------------------------
	class StepperMotorNoSpeed
	{
	protected:
		inline void SystemStart()
		{
		}

		template<typename T> inline void SystemLoopBegin( T *AInstance )
		{
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		int C_NUMOUTPINS,
		typename T_Enabled,
		typename T_FStep,
		typename T_HalfStep,
		typename T_IMPLEMENTATION,
		typename T_Reversed,
		typename T_Speed_Implementation,
		typename T_StepsPerSecond
	> class StepperMotorBasic :
		public T_Enabled,
		public T_FStep,
		public T_HalfStep,
		public T_OutputPins0,
		public T_OutputPins1,
		public T_OutputPins2,
		public T_OutputPins3,
		public T_Reversed,
		public T_Speed_Implementation,
		public T_StepsPerSecond
	{
	public:
//		unsigned long	FLastTime = 0;
		_V_PROP_( FStep )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reversed )
		_V_PROP_( HalfStep )
		_V_PROP_( StepsPerSecond )

	public:
		inline void StepInputPin_o_Receive( void *_Data )
		{
			Step();
		}

	public:
		void UpdatePins()
		{
			T_IMPLEMENTATION::UpdatePinsValue( this );
/ *
			uint8_t AValue = T_IMPLEMENTATION_CLASS::GetUpdatePinsValue( HalfStep, FStep );
			for( int i = 0; i < C_NUMOUTPINS; ++i )
			{
				bool APinValue = ( AValue & 1 );

				T_IMPLEMENTATION_CLASS::SetChannelValue( i, APinValue );

				AValue >>= 1;
			}
* /
		}

	public:
		inline void SystemStart()
		{
			T_SPEED::SystemStart();
		}

		inline void SystemLoopBegin()
		{
			T_SPEED::SystemLoopBegin( this );
		}

	public:
		void Step()
		{
			if( Reversed() ) //^ ( StepsPerSecond > 0 ) )
				FStep() = FStep() - 1;

			else
				FStep() = FStep() + 1;

			UpdatePins();
		}

	public:
		inline StepperMotorBasic()
		{
			FStep() = 0;
		}

	};
*/
//---------------------------------------------------------------------------
	template <
//		int C_NUMOUTPINS,
		typename T_Enabled,
		typename T_FStep,
		typename T_HalfStep,
		typename T_OutputPins0,
		typename T_OutputPins1,
		typename T_OutputPins2,
		typename T_OutputPins3,
		typename T_Reversed,
		typename T_Speed_Implementation,
		typename T_StepsPerSecond
	> class StepperMotor4Wire :
		public T_Enabled,
		public T_FStep,
		public T_HalfStep,
		public T_OutputPins0,
		public T_OutputPins1,
		public T_OutputPins2,
		public T_OutputPins3,
		public T_Reversed,
		public T_Speed_Implementation,
		public T_StepsPerSecond
	{
	public:
		_V_PIN_( OutputPins0 )
		_V_PIN_( OutputPins1 )
		_V_PIN_( OutputPins2 )
		_V_PIN_( OutputPins3 )

	public:
		_V_PROP_( FStep )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reversed )
		_V_PROP_( HalfStep )
		_V_PROP_( StepsPerSecond )

	public:
		inline void StepInputPin_o_Receive( void *_Data )
		{
			Step();
		}

	public:
		void UpdatePins()
		{
			uint8_t AValue = HalfStep() ? C_StepperBits8[ FStep() & 0b0111 ] : C_StepperBits4[ FStep() & 0b0011 ];
			for( int i = 0; i < 4; ++i )
			{
				bool APinValue = ( AValue & 1 );

				switch( i )
				{
					case 0 : T_OutputPins0::SetPinValue( APinValue ); break;
					case 1 : T_OutputPins1::SetPinValue( APinValue ); break;
					case 2 : T_OutputPins2::SetPinValue( APinValue ); break;
					case 3 : T_OutputPins3::SetPinValue( APinValue ); break;
				}

				AValue >>= 1;
			}
		}

	public:
		inline void SystemStart()
		{
			T_Speed_Implementation::SystemStart();
		}

		inline void SystemLoopBegin()
		{
			T_Speed_Implementation::SystemLoopBegin( this );
		}

	public:
		void Step()
		{
			if( Reversed() ) //^ ( StepsPerSecond > 0 ) )
				FStep() = FStep() - 1;

			else
				FStep() = FStep() + 1;

			UpdatePins();
		}

	public:
		inline StepperMotor4Wire()
		{
			FStep() = 0;
		}

	};
//---------------------------------------------------------------------------
	template <
//		int C_NUMOUTPINS,
		typename T_Enabled,
		typename T_FStep,
		typename T_HalfStep,
		typename T_OutputPins0,
		typename T_OutputPins1,
		typename T_OutputPins2,
		typename T_Reversed,
		typename T_Speed_Implementation,
		typename T_StepsPerSecond
	> class StepperMotor3Wire :
		public T_Enabled,
		public T_FStep,
		public T_HalfStep,
		public T_OutputPins0,
		public T_OutputPins1,
		public T_OutputPins2,
		public T_Reversed,
		public T_Speed_Implementation,
		public T_StepsPerSecond
	{
	public:
		_V_PIN_( OutputPins0 )
		_V_PIN_( OutputPins1 )
		_V_PIN_( OutputPins2 )

	public:
		_V_PROP_( FStep )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Reversed )
		_V_PROP_( HalfStep )
		_V_PROP_( StepsPerSecond )

	public:
		inline void StepInputPin_o_Receive( void *_Data )
		{
			Step();
		}

	public:
		void UpdatePins()
		{
			uint8_t AValue = HalfStep() ? C_Stepper3Bits3[ FStep() % 3 ] : C_Stepper3Bits6[ FStep() % 6 ];
			for( int i = 0; i < 3; ++i )
			{
				bool APinValue = ( AValue & 1 );

				switch( i )
				{
					case 0 : T_OutputPins0::SetPinValue( APinValue ); break;
					case 1 : T_OutputPins1::SetPinValue( APinValue ); break;
					case 2 : T_OutputPins2::SetPinValue( APinValue ); break;
				}

				AValue >>= 1;
			}
		}

	public:
		inline void SystemStart()
		{
			T_Speed_Implementation::SystemStart();
		}

		inline void SystemLoopBegin()
		{
			T_Speed_Implementation::SystemLoopBegin( this );
		}

	public:
		void Step()
		{
			if( Reversed() ) //^ ( StepsPerSecond > 0 ) )
				FStep() = FStep() - 1;

			else
				FStep() = FStep() + 1;

			UpdatePins();
		}

	public:
		inline StepperMotor3Wire()
		{
			FStep() = 0;
		}

	};
//---------------------------------------------------------------------------
}

#endif
