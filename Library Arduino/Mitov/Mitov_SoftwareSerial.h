////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_SOFTWARE_SERIAL_h
#define _MITOV_SOFTWARE_SERIAL_h

#include <Mitov.h>
#include <SoftwareSerial.h>

namespace Mitov
{
	template<
		int RX_PIN_NUMBER, 
		int TX_PIN_NUMBER,
		typename T_AfterSendingDelay,
		typename T_Enabled,
		typename T_FEndTime,
		typename T_FSending,
		typename T_OutputPin,
		typename T_SendingOutputPin,
		typename T_Speed
	> class VisuinoSoftwareSerial : 
		public T_AfterSendingDelay,
		public T_Enabled,
		public T_FEndTime,
		public T_FSending,
		public T_OutputPin,
		public T_SendingOutputPin,
		public T_Speed
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( SendingOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		_V_PROP_( AfterSendingDelay )

	public:
		_V_PROP_( Speed )

	protected:
		_V_PROP_( FSending )

	protected:
		_V_PROP_( FEndTime )
//#ifdef _MITOV_SERIAL_SENDING_PIN_
//		uint32_t	FEndTime;
//#endif

	protected:
		SoftwareSerial *FSerial = nullptr;

	public:
		void SetEnabled( bool AValue )
		{
			if( Enabled() == AValue )
				return;

			Enabled() = AValue;
			UpdateSerial();
		}

	public:
		inline Stream &GetStream()
		{ 
			return *FSerial; 
		}

#ifdef _MITOV_SERIAL_SENDING_PIN_
		inline uint32_t GetByteSpeed()
		{ 
			return Speed().GetValue() / 11; 
		}
#endif

	public:
		template<typename T> void Print( T AValue )
		{
			if( Enabled() )
			{
#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( 1 );
#endif
				GetStream().println( AValue );
#ifdef __AVR
//				SendingCount( 1 );
#endif
			}
		}

#ifdef _MITOV_SERIAL_SENDING_PIN_
		void Print( Mitov::String AValue )
		{
			if( Enabled() )
			{
#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( AValue.length() );
#endif
				GetStream().println( AValue );
#ifdef _MITOV_SERIAL_SENDING_PIN_
#ifdef __AVR
//				SendingCount( 1 );
#endif
#endif
			}
		}

		void Print( char *AValue )
		{
			if( Enabled() )
			{
#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( strlen( AValue ) );
#endif
				GetStream().println( AValue );
#ifdef _MITOV_SERIAL_SENDING_PIN_
#ifdef __AVR
//				SendingCount( 1 );
#endif
#endif
			}
		}
#endif

		void PrintChar( char AValue )
		{
			if( Enabled() )
			{
#ifdef _MITOV_SERIAL_SENDING_PIN_
				SendingCount( 1 );
#endif
				GetStream().print( AValue );
			}
		}

		void Write( uint8_t *AData, uint32_t ASize )
		{
#ifdef _MITOV_SERIAL_SENDING_PIN_
			SendingCount( ASize );
#endif
			GetStream().write( AData, ASize );
#ifdef _MITOV_SERIAL_SENDING_PIN_
#ifdef __AVR
//			SendingCount( 1 );
#endif
#endif
		}

#ifdef _MITOV_SERIAL_SENDING_PIN_
	protected:
		inline void UpdateSending()
		{
			T_SendingOutputPin::SetPinValue( FSending(), true );
		}

		void SendingCount( uint32_t ACount )
		{
//			Serial.println( ACount );

			ACount = 1; // Most conrtrollers can send only one character at a time.
#ifdef __AVR
			ACount = 1; // The AVR can send only one character at a time.
#endif
			if( ! FSending() )
			{
				FSending() = true;
				UpdateSending();
				uint32_t ATime = micros();
				FEndTime() = ATime + AfterSendingDelay() + ACount * ( 10000000 / GetByteSpeed() );
//				Serial.println( 10000000 / GetByteSpeed() );
//				Serial.println( int32_t( FEndTime - ATime ));
			}

			else
			{
				UpdateSending();
				uint32_t ATime = micros();
				if( int32_t( FEndTime().GetValue() - ATime ) >= AfterSendingDelay() )
					FEndTime() = FEndTime().GetValue() + ACount * ( 10000000 / GetByteSpeed() );

				else
					FEndTime() = ATime + AfterSendingDelay() + ACount * ( 10000000 / GetByteSpeed() );

//					Serial.println( 10000000 / GetByteSpeed() );
//					Serial.println( int32_t( FEndTime - ATime ));
			}

//			Serial.println( 10000000 / GetByteSpeed() );
//			Serial.println( int32_t( FEndTime - ATime ));
		}
#endif

	protected:
		void UpdateSerial()
		{
			if( Enabled() )
			{
				if( ! FSerial )
				{
					FSerial = new SoftwareSerial( RX_PIN_NUMBER, TX_PIN_NUMBER );
					FSerial->begin( Speed() );
				}
			}
			else
			{
				if( FSerial )
				{
					delete FSerial;
					FSerial = nullptr;
				}
			}
		}

	public:
		inline void SystemInit()
		{
			UpdateSerial();
		}

		inline void SystemStart()
		{
			T_SendingOutputPin::SetPinValue( true, false );
		}

		inline void SystemLoopBegin()
		{
			if( Enabled() )
			{
				int AData = FSerial->read();
				if( AData >= 0 )
				{
					unsigned char AByte = AData;
					T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
//					OutputPin.Notify( &AByte );
				}
			}

#ifdef _MITOV_SERIAL_SENDING_PIN_
			if( FSending() )
			{
				if( int32_t( FEndTime().GetValue() - micros() ) < 0 )
				{
//					Serial.print( "STOP: " );
//					Serial.println( int32_t( FEndTime - micros() ) );
					FSending() = false;
					UpdateSending();
				}
			}
#endif // _MITOV_SERIAL_SENDING_PIN_
		}


/*
		virtual ~VisuinoSoftwareSerial()
		{
			if( FSerial )
				delete FSerial;
		}
*/
	};
}

#endif
