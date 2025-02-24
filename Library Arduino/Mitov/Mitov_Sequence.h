////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_SEQUENCE_h
#define _MITOV_SEQUENCE_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

// #define __SEQUENCE__DEBUG__


namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Delay,
		typename T_Enabled,
		uint32_t C_INDEX, 
		typename T_OutputPin
	> class SequenceElement : 
		public T_OutputPin,
		public T_Delay,
		public T_Enabled
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Delay )
		_V_PROP_( Enabled )

	public:
		void MoveToNextIndex( int32_t *& ACurrentIndex )
		{
#ifdef __SEQUENCE__DEBUG__
			Serial.println( "MoveToNextIndex" );
			Serial.println( C_INDEX );
			Serial.println( *ACurrentIndex );
#endif // __SEQUENCE__DEBUG__

			if( Enabled() )
				if( ACurrentIndex )
					if( int32_t( C_INDEX ) > *ACurrentIndex )
					{
	//					Serial.println( "TEST2" );
						*ACurrentIndex = C_INDEX;
						ACurrentIndex = nullptr;
					}

		}

		void SetElementValue( bool AValue )
		{
#ifdef __SEQUENCE__DEBUG__
			Serial.println( "SetElementValue" );
			Serial.print( "Value: " );
			Serial.println( AValue );
#endif // __SEQUENCE__DEBUG__

			if( ! AValue )
				T_OutputPin::ClockPin();

		}

		inline void GetElementDelay( uint32_t &ADelay )
		{
			ADelay = Delay();
		}

	public:
		inline void StartInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			C_OWNER.StartElement( C_INDEX );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SignalElement( C_INDEX );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Delay,
		typename T_Enabled,
		uint32_t C_INDEX, 
		typename T_OutputPin
	> class SequenceElementDigital : 
		public T_OutputPin,
		public T_Delay,
		public T_Enabled
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Delay )
		_V_PROP_( Enabled )

	public:
		inline void InitElementValue()
		{
			T_OutputPin::SetPinValue( false );
		}

		void MoveToNextIndex( int32_t *& ACurrentIndex )
		{
			if( Enabled() )
				if( ACurrentIndex )
					if( int32_t( C_INDEX ) > *ACurrentIndex )
					{
						*ACurrentIndex = C_INDEX;
						ACurrentIndex = nullptr;
					}

		}

		inline void SetElementValue( bool AValue )
		{
			T_OutputPin::SetPinValue( AValue );
		}

		inline void GetElementDelay( uint32_t &ADelay )
		{
			ADelay = Delay();
		}

	public:
		inline void StartInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

			C_OWNER.StartElement( C_INDEX );
		}

		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.SignalElement( C_INDEX );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_CanRestart, 
		uint32_t C_CHANNELS_COUNT,
		typename T_Elements_GetElementDelay,
		typename T_Elements_InitElementValue,
		typename T_Elements_MoveToNextIndex,
		typename T_Elements_SetElementValue,
		typename T_Enabled,
		typename T_FCurrentIndex,
		typename T_IsEnabled,
		typename T_Repeat
	> class Sequence : 
		public T_Enabled, 
		public T_FCurrentIndex,
		public T_IsEnabled,
		public T_Repeat, 
		public T_CanRestart
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Repeat )
		_V_PROP_( CanRestart )

	protected:
		_V_PROP_( IsEnabled )
//		SequenceClockedElement *FCurrentElement = nullptr;
		_V_PROP_( FCurrentIndex )
		uint32_t	FStartTime;

	protected:
		void MoveToNextIndex()
		{
#ifdef __SEQUENCE__DEBUG__
			Serial.println( "MoveToNextIndex" );
			Serial.println( FCurrentIndex() );
#endif // __SEQUENCE__DEBUG__
			int32_t ACurrentIndex = FCurrentIndex();
//			if( ACurrentIndex >= C_CHANNELS_COUNT )
//				ACurrentIndex = -1;

			int32_t *ACurrentIndexPrt = &ACurrentIndex; // Will be nulled once it moves to next
			T_Elements_MoveToNextIndex::ChainCall( ACurrentIndexPrt );
//			SequenceOperation::TMoveToNextIndexOperation AOperation( ACurrentIndex );
//			ProcessCallChain( -1, AOperation );
#ifdef __SEQUENCE__DEBUG__
			Serial.println( FCurrentIndex() );
#endif // __SEQUENCE__DEBUG__
			if( ACurrentIndexPrt )
				FCurrentIndex() = C_CHANNELS_COUNT;

			else
				FCurrentIndex() = ACurrentIndex;
				
		}

		inline uint32_t GetElementDelay( uint32_t AIndex )
		{
			uint32_t ADelay;
			T_Elements_GetElementDelay::ChainCall( AIndex, ADelay );
			return ADelay;
		}

		inline void SetElementToTrue( uint32_t AIndex )
		{
			T_Elements_SetElementValue::ChainCall( AIndex, true );
		}

		inline void SetElementToFalse( uint32_t AIndex )
		{
			T_Elements_SetElementValue::ChainCall( AIndex, false );
		}

	public:
		void StartElement( uint32_t AIndex )
		{
			if( ! Enabled() )
				return;

			FCurrentIndex() = AIndex;
		}

		void SignalElement( uint32_t AIndex )
		{
			if( ! Enabled() )
				return;

			if( FCurrentIndex() < 0 )
				return;

			if( AIndex != FCurrentIndex() )
				return;

//			SequenceElement<Sequence<C_CHANNELS_COUNT>> *AElementItem = FElements[ FCurrentIndex ];

#ifdef __SEQUENCE__DEBUG__
			Serial.print( "SignalElement: " );
			Serial.println( FCurrentIndex() );
#endif // __SEQUENCE__DEBUG__

//			Serial.print( "SignalElement1: " );
//			Serial.println( FCurrentIndex );

			SetElementToFalse( FCurrentIndex() );

//			FElements[ FCurrentIndex ]->DoClockReceive( nullptr );
//			AElementItem->OutputPin.template SendValue<bool>( false );

			MoveToNextIndex();

//			while( ++FCurrentIndex < C_CHANNELS_COUNT )
//				if( FElements[ FCurrentIndex ]->Enabled )
//				if( GetIsElementEnabled( FCurrentIndex ))
//					break;

			if( FCurrentIndex() < C_CHANNELS_COUNT )
				FStartTime = millis();

			else if( Repeat() )
				StartInputPin_o_Receive( nullptr );

			else
			{
				FCurrentIndex() = -1;
				return;
			}

/*
#ifdef _SEQUENCE_DIGIAL_ELEMENT_
			SetElementToTrue( FCurrentIndex );

//			AElementItem = FElements[ FCurrentIndex ];
//			if( AElementItem->FIsDigital )
//				AElementItem->OutputPin.template SendValue<bool>( true );
#endif // _SEQUENCE_DIGIAL_ELEMENT_
*/

//			Serial.print( "FCurrentIndex: " ); Serial.println( FCurrentIndex );
		}

	public:
		void StartInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "----" );
/*
#ifdef _SEQUENCE_DIGIAL_ELEMENT_
			SetElementToFalse( -1 );
//			for( int i = 0; i < C_CHANNELS_COUNT; ++i )
//				SetElementToFalse( i );

//				if( FElements[ i ]->FIsDigital )
//					FElements[ i ]->OutputPin.template SendValue<bool>( false );
#endif // _SEQUENCE_DIGIAL_ELEMENT_
*/
#ifdef __SEQUENCE__DEBUG__
			Serial.println( "START" );
			Serial.println( FCurrentIndex() );
			Serial.println( CanRestart() );
#endif // __SEQUENCE__DEBUG__
			if( Enabled() )
				if( C_CHANNELS_COUNT )
					if( CanRestart() || ( FCurrentIndex() == -1 ))
					{
						FCurrentIndex() = -1;
						MoveToNextIndex();

						if( FCurrentIndex() < C_CHANNELS_COUNT )
						{
//#ifdef _SEQUENCE_DIGIAL_ELEMENT_
//							SetElementToTrue( FCurrentIndex );
//#endif // _SEQUENCE_DIGIAL_ELEMENT_

							FStartTime = millis();
						}

/*
						for( int i = 0; i < C_CHANNELS_COUNT; ++i )
							if( FElements[ i ]->Enabled )
							{	
								FCurrentIndex = i;
#ifdef _SEQUENCE_DIGIAL_ELEMENT_
								SetElementToTrue( FCurrentIndex );
//								if( FElements[ i ]->FIsDigital )
//									FElements[ i ]->OutputPin.template SendValue<bool>( true );
#endif // _SEQUENCE_DIGIAL_ELEMENT_

								FStartTime = millis();
//								Serial.println( "START1" );
								break;
							}
*/
					}

		}

	public:
		inline void SystemStart()
		{
//			Serial.println( "TEST2" );
			StartInputPin_o_Receive( nullptr );
		}

		inline void SystemLoopBegin()
		{
/*
#ifdef _SEQUENCE_DIGIAL_ELEMENT_
			if( IsEnabled != Enabled )
			{
				SetElementValue( FCurrentIndex, Enabled, true );
//				SequenceElement<Sequence<C_CHANNELS_COUNT>> *AElementItem = FElements[ FCurrentIndex ];
//				if( AElementItem->FIsDigital )
//					AElementItem->OutputPin.template SendValue<bool>( Enabled );

				IsEnabled = Enabled;
			}
#endif // _SEQUENCE_DIGIAL_ELEMENT_
*/
			if( Enabled() )
				if( FCurrentIndex() >= 0 )
					if( ( millis() - FStartTime ) >= GetElementDelay( FCurrentIndex() ))
					{
#ifdef __SEQUENCE__DEBUG__
						Serial.println( "Signal" );
						Serial.println( FCurrentIndex() );
#endif // __SEQUENCE__DEBUG__
						SignalElement( FCurrentIndex() );
					}

/*
			if( FDetectTimerRunning )
				if( ( millis() - FDetectStartTime ) >= PacketDetectInterval )
					TryProcessAcumulattor();

			if( FTimeoutTimerRunning )
				if( ( millis() - FTimeoutStartTime ) >= ResponseTimeout )
				{
					// Timeout
					FDetectTimerRunning = false;

					if( FCurrentTransaction )
						FCurrentTransaction->Release();

					FCurrentTransaction = nullptr;
					FReceivedData.clear();
					TrySend();
				}
*/
		}
		
	public:
		Sequence()
		{
			IsEnabled() = true;
			FCurrentIndex() = -1;
		}
/*
	public:
		Sequence() :
			Enabled( true ),
			IsEnabled( true ),
			Repeat( false ),
			CanRestart( true )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	template<
		typename T_CanRestart, 
		uint32_t C_CHANNELS_COUNT,
		typename T_Elements_GetElementDelay,
		typename T_Elements_InitElementValue,
		typename T_Elements_MoveToNextIndex,
		typename T_Elements_SetElementValue,
		typename T_Enabled,
		typename T_FCurrentIndex,
		typename T_IsEnabled,
		typename T_Repeat
	> class DigitalSequence : public Sequence<
			T_CanRestart, 
			C_CHANNELS_COUNT,
			T_Elements_GetElementDelay,
			T_Elements_InitElementValue,
			T_Elements_MoveToNextIndex,
			T_Elements_SetElementValue,
			T_Enabled,
			T_FCurrentIndex,
			T_IsEnabled,
			T_Repeat
		>
	{
		typedef Sequence<
			T_CanRestart, 
			C_CHANNELS_COUNT,
			T_Elements_GetElementDelay,
			T_Elements_InitElementValue,
			T_Elements_MoveToNextIndex,
			T_Elements_SetElementValue,
			T_Enabled,
			T_FCurrentIndex,
			T_IsEnabled,
			T_Repeat
		> inherited;

	public:
		void SignalElement( uint32_t AIndex )
		{
			if( ! inherited::Enabled() )
				return;

			if( inherited::FCurrentIndex() < 0 )
				return;

			if( AIndex != inherited::FCurrentIndex() )
				return;

//			SequenceElement<Sequence<C_CHANNELS_COUNT>> *AElementItem = FElements[ FCurrentIndex ];

//			Serial.print( "SignalElement: " );
//			Serial.println( FCurrentIndex );

//			Serial.print( "SignalElement1: " );
//			Serial.println( FCurrentIndex );

			inherited::SetElementToFalse( inherited::FCurrentIndex() );

//			FElements[ FCurrentIndex ]->DoClockReceive( nullptr );
//			AElementItem->OutputPin.template SendValue<bool>( false );

			inherited::MoveToNextIndex();

//			while( ++FCurrentIndex < C_CHANNELS_COUNT )
//				if( FElements[ FCurrentIndex ]->Enabled )
//				if( GetIsElementEnabled( FCurrentIndex ))
//					break;

			if( inherited::FCurrentIndex() < C_CHANNELS_COUNT )
				inherited::FStartTime = millis();

			else if( inherited::Repeat() )
				StartInputPin_o_Receive( nullptr );

			else
			{
				inherited::FCurrentIndex() = -1;
				return;
			}

			inherited::SetElementToTrue( inherited::FCurrentIndex() );
		}

	public:
		void StartInputPin_o_Receive( void *_Data )
		{
			T_Elements_InitElementValue::ChainCall();

//			Serial.println( "START" );
//			Serial.println( FCurrentIndex );
//			Serial.println( CanRestart );
			if( inherited::Enabled() )
				if( C_CHANNELS_COUNT )
					if( inherited::CanRestart() || ( inherited::FCurrentIndex() == -1 ))
					{
						inherited::FCurrentIndex() = -1;
						inherited::MoveToNextIndex();

						if( inherited::FCurrentIndex() < C_CHANNELS_COUNT )
						{
							inherited::SetElementToTrue( inherited::FCurrentIndex() );
							inherited::FStartTime = millis();
						}

					}

		}

	public:
		inline void SystemStart()
		{
			StartInputPin_o_Receive( nullptr );
		}

		inline void SystemLoopBegin()
		{
			if( inherited::IsEnabled() != inherited::Enabled() )
			{
				T_Elements_SetElementValue::ChainCall( inherited::FCurrentIndex(), inherited::Enabled() );
//				SequenceElement<Sequence<C_CHANNELS_COUNT>> *AElementItem = FElements[ FCurrentIndex ];
//				if( AElementItem->FIsDigital )
//					AElementItem->OutputPin.template SendValue<bool>( Enabled );

				inherited::IsEnabled() = inherited::Enabled();
			}

			if( inherited::Enabled() )
				if( inherited::FCurrentIndex() >= 0 )
					if( ( millis() - inherited::FStartTime ) >= inherited::GetElementDelay( inherited::FCurrentIndex() ))
					{
//						Serial.println( "Signal" );
//						Serial.println( FCurrentIndex );
						SignalElement( inherited::FCurrentIndex() );
					}

		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
