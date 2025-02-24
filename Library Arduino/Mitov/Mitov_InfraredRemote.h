////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_INFRARED_REMOTE_h
#define _MITOV_INFRARED_REMOTE_h

#include <Mitov.h>
#include <IRremote.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		int C_TYPE,
		typename T_OutputPin,
		typename T_Value
	> class InfraredRemoteReceiverTypedCommandElement :
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		void TryDecode( decode_results &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.decode_type != C_TYPE )
				return;

			if( AResult.value != Value() )
				return;

			T_OutputPin::ClockPin();
			ASuccess = true;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_FLastValue,
		typename T_OutputPin,
		typename T_RepeatInterval,
		typename T_RepeatOutputPin,
		typename T_Value
	> class InfraredRemoteReceiverNECCommandElement :
		public T_FLastValue,
		public T_OutputPin,
		public T_RepeatInterval,
		public T_RepeatOutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( RepeatOutputPin )

	public:
		_V_PROP_( Value )
		_V_PROP_( RepeatInterval )

	protected:
		_V_PROP_( FLastValue )

		unsigned long	FLastTime = 0;

	public:
		void TryDecode( decode_results &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.decode_type != NEC )
				return;

			if(( AResult.value == REPEAT ) && ( ALastCode == Value() ))
			{
				FLastTime = millis();
				T_OutputPin::SetPinValue( true, true );
				FLastValue() = true;
				T_RepeatOutputPin::ClockPin();
				ASuccess = true;
				return;
			}

			if( AResult.value == Value() )
			{
				FLastTime = millis();
				T_OutputPin::SetPinValue( false, true );
				T_OutputPin::SetPinValue( true, true );
				FLastValue() = true;
				ASuccess = true;
				return;
			}

			ASuccess = true;
		}

	public:
		inline void SystemLoopBegin()
		{
			if( FLastValue() )
				if( millis() - FLastTime > RepeatInterval() )
				{
					T_OutputPin::SetPinValue( false, true );
					FLastValue() = false;
				}

		}

		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( false, false );
		}

	public:
		InfraredRemoteReceiverNECCommandElement()
		{
			FLastValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_OutputPins_Forward,
		typename T_OutputPins_Back,
		typename T_OutputPins_Left,
		typename T_OutputPins_Right,
		typename T_OutputPins_OK,
		typename T_RepeatInterval,
		typename T_RepeatOutputPin
	> class InfraredRemoteReceiverNECArrowCommandsElement :
		public T_OutputPins_Forward,
		public T_OutputPins_Back,
		public T_OutputPins_Left,
		public T_OutputPins_Right,
		public T_OutputPins_OK,
		public T_RepeatInterval,
		public T_RepeatOutputPin
	{
	public:
		_V_PIN_( OutputPins_Forward )
		_V_PIN_( OutputPins_Back )
		_V_PIN_( OutputPins_Left )
		_V_PIN_( OutputPins_Right )
		_V_PIN_( OutputPins_OK )

		_V_PIN_( RepeatOutputPin )

	public:
		uint32_t RepeatInterval = 300;

	protected:
		uint8_t	FIndex = 0xFF;
		uint32_t FLastValue = 0;

	protected:
		unsigned long	FLastTime = 0;

	protected:
		void SetOutputPinValue( uint8_t AIndex, bool AValue )
		{
			switch( AIndex )
			{
				case 0: T_OutputPins_Forward::SetPinValue( AValue, true );
				case 1: T_OutputPins_Back::SetPinValue( AValue, true );
				case 2: T_OutputPins_Left::SetPinValue( AValue, true );
				case 3: T_OutputPins_Right::SetPinValue( AValue, true );
				case 4: T_OutputPins_OK::SetPinValue( AValue, true );
			}
		}

		bool TrySendValue( uint32_t AValue )
		{
			uint8_t AOldIndex = FIndex;
			FIndex = 0xFF;
			switch( AValue )
			{
				case 16736925: // Forward
					FIndex = 0;
					break;

				case 16720605: // Left
					FIndex = 2;
					break;

				case 16754775: // Back
					FIndex = 1;
					break;

				case 16761405: // Right
					FIndex = 3;
					break;

				case 16712445: // OK
					FIndex = 4;
					break;

				default:
					if( AOldIndex <= 5 )
						SetOutputPinValue( AOldIndex, false );

					return false;
			}

			FLastTime = millis();
			if( AOldIndex == FIndex )
				return true;

			if( AOldIndex <= 5 )
				SetOutputPinValue( AOldIndex, false );

			SetOutputPinValue( FIndex, true );
			return true;
		}

	public:
		void TryDecode( decode_results &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.decode_type != NEC )
				return;

			if(( AResult.value == REPEAT ) && ( ALastCode == FLastValue ))
			{
				if( FLastValue )
				{
					TrySendValue( FLastValue );
					T_RepeatOutputPin::ClockPin();
					ASuccess = true;
					return;
				}
			}

			if( TrySendValue( AResult.value ))
				FLastValue = AResult.value;
/*
			if( AResult.value == Value )
			{
				FLastTime = millis();
				OutputPin.SetValue( false, true );
				OutputPin.SetValue( true, true );
				return true;
			}
*/
			ASuccess = true;
		}

	public:
		inline void SystemLoopBegin()
		{
/*
			if( OutputPin.Value )
				if( millis() - FLastTime > RepeatInterval )
					OutputPin.SetValue( false, true );
*/
			if( FIndex <= 5 )
				if( millis() - FLastTime > RepeatInterval )
				{
					SetOutputPinValue( FIndex, false );
					FIndex = 0xFF;
				}
		}

		inline void SystemStart()
		{
			T_OutputPins_Forward::SetPinValue( false, false );
			T_OutputPins_Back::SetPinValue( false, false );
			T_OutputPins_Left::SetPinValue( false, false );
			T_OutputPins_Right::SetPinValue( false, false );
			T_OutputPins_OK::SetPinValue( false, false );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Address,
		typename T_OutputPin,
		typename T_Value
	> class InfraredRemoteReceiverPanasonicCommandElement :
		public T_Address,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )
		_V_PROP_( Address )

	public:
		void TryDecode( decode_results &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.decode_type != PANASONIC )
				return;

			if( AResult.value != Value() )
				return;

			if( AResult.address != Address() )
				return;

			T_OutputPin::ClockPin();
			ASuccess = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Code,
		typename T_OutputPin
	> class InfraredRemoteReceiverAiwaRCT501CommandElement :
		public T_Code,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Code )

	public:
		void TryDecode( decode_results &AResult, uint32_t ALastCode, bool &ASuccess )
		{
			if( ASuccess )
				return;

			if( AResult.decode_type != PANASONIC )
				return;

			if( AResult.value != Code() )
				return;

			T_OutputPin::ClockPin();
			ASuccess = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_AddressOutputPin,
		typename T_ClearInterval,
		typename T_DataTypeOutputPin,
		typename T_Elements_TryDecode,
		typename T_Enabled,
		typename T_OutputPin,
		typename T_RawOutputPin,
		typename T_ReplaceNECRepeatCode
	> class InfraredRemoteReceiver :
		public T_AddressOutputPin,
		public T_ClearInterval,
		public T_DataTypeOutputPin,
		public T_Elements_TryDecode,
		public T_Enabled,
		public T_OutputPin,
		public T_RawOutputPin,
		public T_ReplaceNECRepeatCode
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( AddressOutputPin )
		_V_PIN_( DataTypeOutputPin )
		_V_PIN_( RawOutputPin )

	public:
		_V_PROP_( ClearInterval )
		_V_PROP_( Enabled )
		_V_PROP_( ReplaceNECRepeatCode )

//	public:
//		Mitov::SimpleList<InfraredRemoteReceiverBasicCommandElementIntf *> FElements;

	public:
		void SetEnabled( bool AValue )
		{
			if( Enabled() == AValue )
				return;

			Enabled() = AValue;
			if( Enabled() )
				FSensor.enableIRIn();

		}

	public:
		inline void ResumeSensor()
		{
			if( Enabled() )
				FSensor.enableIRIn();
		}

	protected:
		IRrecv		FSensor;
		uint32_t	FLastCode = 0;
		
	protected:
		unsigned long	FLastTime = 0;

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				FSensor.enableIRIn();

		}

		inline void SystemLoopBegin()
		{
			if( FLastCode )
				if( millis() - FLastTime > ClearInterval().GetValue() )
					FLastCode = 0;

			if( Enabled() )
			{
				decode_results AResults;
				if ( FSensor.decode( &AResults ))
				{
					uint32_t *ARawData;
					uint32_t ARawDataLength = 0;
					if( T_RawOutputPin::GetPinIsConnected() )
						{
							ARawDataLength = AResults.rawlen;
							if( ARawDataLength )
							{
								ARawData = new uint32_t[ ARawDataLength ];
								for( int i = 0; i < ARawDataLength; ++ i )
									ARawData[ i ] = AResults.rawbuf[ i ];
							}
						}

//					Serial.print( "Len: " ); Serial.println( AResults.rawlen );
					FSensor.resume();

					bool ASuccess = false;
					T_Elements_TryDecode::ChainCall( AResults, FLastCode, ASuccess );
/*
					for( int i = 0; i < FElements.size(); ++i )
						if( FElements[ i ]->TryDecode( AResults, FLastCode ))
							break;
*/
					uint32_t AValue = AResults.value;

					if( AResults.decode_type == NEC )
					{
						if( ReplaceNECRepeatCode() )
							if( AValue == REPEAT )
							{
								AValue = FLastCode;
								if( FLastCode != 0 )
									FLastTime = millis();
							}

						if( AValue != REPEAT )
						{
							FLastCode = AValue;
							FLastTime = millis();
						}
					}

					T_DataTypeOutputPin::SetPinValue( AResults.decode_type );
					T_AddressOutputPin::SetPinValue( AResults.address );
					if( T_RawOutputPin::GetPinIsConnected() )
						if( ARawDataLength )
						{
							T_RawOutputPin::SetPinValue( TArray<uint32_t>( ARawDataLength, ARawData ));
							delete [] ARawData;
						}

					T_OutputPin::SetPinValue( AValue );
				}				
			}
		}

	public:
		InfraredRemoteReceiver( int APinNumber ) :
			FSensor( APinNumber )
		{
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FSentOneJVC
	> class InfraredRemoteTransmitter :
		public T_Enabled,
		public T_FSentOneJVC
	{
	public:
		IRsend	FSensor;

	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FSentOneJVC )

		uint32_t	FLastValueJVC = 0;

	public:
		bool RepeatedJVCCommand( uint32_t AValue )
		{
			bool AResult = ( FLastValueJVC == AValue );
			FLastValueJVC = AValue;
			FSentOneJVC() = true;

			return AResult;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
			{
				TArray<uint32_t> *AData = (TArray<uint32_t>*)_Data;
				unsigned int * code = new unsigned int[ AData->Size ];
				for( int i = 0; i < AData->Size; ++i )
					code[ i ] = AData->Data[ i ];

				FSensor.sendRaw( code, AData->Size, 38 ); // 38KHz
				delete [] code;
			}
		}

	public:
		inline InfraredRemoteTransmitter()
		{
			FSentOneJVC() = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendRC5CommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendRC5( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendRC6CommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendRC6( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendNECCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendNEC( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendSonyCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendSony( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Address,
		typename T_Value
	> class InfraredRemoteTransmitterSendPanasonicCommandElement :
		public T_Address,
		public T_Value
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Value )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendPanasonic( Address(), Value() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_UseRepeat,
		typename T_Value
	> class InfraredRemoteTransmitterSendJVCCommandElement :
		public T_NumberBits,
		public T_UseRepeat,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )
		_V_PROP_( UseRepeat )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			bool ARepeat = C_OWNER.RepeatedJVCCommand( Value() );
			if( UseRepeat() && ARepeat )
				C_OWNER.FSensor.sendJVC( Value(), NumberBits(), true );

			else
				C_OWNER.FSensor.sendJVC( Value(), NumberBits(), false );

			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendSAMSUNGCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendSAMSUNG( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendWhynterCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendWhynter( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Code
	> class InfraredRemoteTransmitterSendAiwaRCT501CommandElement :
		public T_Code
	{
	public:
		_V_PROP_( Code )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendAiwaRCT501( Code() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendLGCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendLG( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendDISHCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendDISH( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendSharpRawCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendSharpRaw( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Address,
		typename T_Command
	> class InfraredRemoteTransmitterSendSharpCommandElement :
		public T_Address,
		public T_Command
	{
	public:
		_V_PROP_( Address )
		_V_PROP_( Command )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendSharp( Address(), Command() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendDenonCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendDenon( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Code,
		typename T_Fallback,
		typename T_Repeat
	> class InfraredRemoteTransmitterSendProntoCommandElement :
		public T_Code,
		public T_Fallback,
		public T_Repeat
	{
	public:
		_V_PROP_( Code )
		_V_PROP_( Repeat )
		_V_PROP_( Fallback )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendPronto( (char *)Code.c_str(), Repeat(), Fallback() );
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
/*
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_NumberBits,
		typename T_Value
	> class InfraredRemoteTransmitterSendLEGOPowerFunctionsCommandElement :
		public T_NumberBits,
		public T_Value
	{
	public:
		_V_PROP_( Value )
		_V_PROP_( NumberBits )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendLegoPowerFunctions( Value(), NumberBits() );
			C_OWNER.ResumeSensor();
		}

	};
*/
//---------------------------------------------------------------------------
	template<typename T_OWNER, T_OWNER &C_OWNER> class InfraredRemoteTransmitterSendRawCommandElement
	{
	public:
		uint16_t *_Values;
		uint32_t _Count;

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FSensor.sendRaw( _Values, _Count, 38 ); // 38KHz
			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
	template<typename T_OWNER, T_OWNER &C_OWNER> class InfraredRemoteTransmitterSendConstRawCommandElement
	{
	public:
		const /*PROGMEM*/ uint16_t *_Values;
		uint32_t _Count;

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "TEST" );
			uint16_t *AValues = new uint16_t[ _Count ];
			for( int i = 0; i < _Count; ++i )
			{
				AValues[ i ] = pgm_read_word( _Values + i ) * USECPERTICK;
//				Serial.println( AValues[ i ] );
			}

			C_OWNER.FSensor.sendRaw( AValues, _Count, 38 ); // 38KHz

			delete [] _Values;

			C_OWNER.ResumeSensor();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
