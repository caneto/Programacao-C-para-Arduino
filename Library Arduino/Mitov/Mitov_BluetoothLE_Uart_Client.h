////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_BLUETOOTH_LE_UART_CLIENT_h
#define _MITOV_BLUETOOTH_LE_UART_CLIENT_h

#include <Mitov.h>
#include <Mitov_BluetoothLE_Uart_Service.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template<
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_Enabled,
		typename T_OutputPin
	> class ArduinoBluetoothLEUartClient :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		BLERemoteService		*FService;
		BLERemoteCharacteristic	*FTXCharacteristic;
		BLERemoteCharacteristic	*FRXCharacteristic;

	protected:
		static	void *FInstance;

	protected:
		static void NotifyCallback( BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify )
		{
			((ArduinoBluetoothLEUartClient *)FInstance )->InstanceNotifyCallback( pData, length );
		}

		inline void InstanceNotifyCallback( uint8_t* pData, size_t length ) 
		{
			if( ! Enabled() )
				return;

			T_OutputPin::SendPinValue( Mitov::TDataBlock( length, pData ));
		}
/*
	public:
		virtual void onWrite(BLECharacteristic* pCharacteristic) override
		{
			T_OUTPUT_PIN::SendPinValue( Mitov::TDataBlock( pCharacteristic->m_value.getLength(), pCharacteristic->getData() ));
		}
*/
	public:
		void write( uint8_t *AData, uint32_t ASize )
		{
			if( ! Enabled() )
				return;

			if( ! FTXCharacteristic )
				return;

//			if( ! C_OWNER.IsConnected() )
//				return;

			while( ASize )
			{
				uint8_t ASendSize = MitovMin<uint32_t>( ASize, 20 );
				FTXCharacteristic->writeValue( AData, ASendSize );
//				FTXCharacteristic->setValue( AData, ASendSize );
//				FTXCharacteristic->notify();
				AData += ASendSize;
				ASize -= ASendSize;
			}
		}

		void Print( const Mitov::String AValue )
		{
			write( (uint8_t *)AValue.c_str(), AValue.length());
			write( (uint8_t *)"\r\n", 2 );
		}

		void Print( float AValue )
		{
			char AText[ 16 ];
			dtostrf( AValue,  1, 2, AText );
			Print( Mitov::String( AText ));
		}

		void Print( int32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		void Print( uint32_t AValue )
		{
			char AText[ 16 ];
			ltoa( AValue, AText, 10 );
			Print( Mitov::String( AText ));
		}

		template<typename T> inline void PrintChar( T AValue )
		{
			write( AValue );
		}

		inline void Write( uint8_t *AData, uint32_t ASize )
		{
			write( AData, ASize );
		}

	public:
		inline void TryStart( bool AValue )
		{
			if( AValue )
			{
#ifdef VISUINO_ESP32
				FService = C_OWNER.GetClient()->getService( ArduinoBluetoothLEUart::SERVICE_UUID );
				FTXCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::RX_UUID ); // TX and RX are reversed!

				FRXCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::TX_UUID ); // TX and RX are reversed!
				if( FRXCharacteristic )
					FRXCharacteristic->registerForNotify( NotifyCallback );

#else // VISUINO_ESP32
#endif // VISUINO_ESP32
			}

			else
			{
				FRXCharacteristic = nullptr;
				FTXCharacteristic = nullptr;
				FService = nullptr;
			}
		}

	public:
		inline void SystemInit()
		{
/*
#ifdef VISUINO_ESP32
			FService = C_OWNER.GetClient()->getService( ArduinoBluetoothLEUart::SERVICE_UUID );
			FTXCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::RX_UUID ); // TX and RX are reversed!

			FRXCharacteristic = FService->getCharacteristic( ArduinoBluetoothLEUart::TX_UUID ); // TX and RX are reversed!
			FRXCharacteristic->registerForNotify( NotifyCallback );

#else // VISUINO_ESP32
#endif // VISUINO_ESP32
*/


/*
#ifdef VISUINO_ESP32
			FService = C_OWNER.FServer->createService( ArduinoBluetoothLEUart::SERVICE_UUID );
#else // VISUINO_ESP32
			FService = new BLEService( ArduinoBluetoothLEUart::SERVICE_UUID );
			C_OWNER.FPeripheral.addAttribute( *FService );
#endif // VISUINO_ESP32

			C_OWNER.AddveriseService( FService );

#ifdef VISUINO_ESP32
// Create a BLE Characteristic
			FTXCharacteristic = FService->createCharacteristic( ArduinoBluetoothLEUart::TX_UUID, BLENotify );

			FService->addCharacteristic( FTXCharacteristic );
//			FTXCharacteristic->setCallbacks( this );
			FTXCharacteristic->addDescriptor(new BLE2902());

			FRXCharacteristic = FService->createCharacteristic( ArduinoBluetoothLEUart::RX_UUID, BLEWriteWithoutResponse );
//			FService->addCharacteristic( FRXCharacteristic );
			FRXCharacteristic->setCallbacks( this );

#else
			FTXCharacteristic = new BLECharacteristic( ArduinoBluetoothLEUart::TX_UUID, BLENotify, 20 );
			FService->addCharacteristic( *FTXCharacteristic );

			FRXCharacteristic = new BLECharacteristic( ArduinoBluetoothLEUart::RX_UUID, BLEWriteWithoutResponse, 20 );
			FService->addCharacteristic( *FRXCharacteristic );
#endif

#ifdef VISUINO_ESP32
			FService->start();
#endif
*/
		}

		inline void SystemLoopBegin()
		{
/*
#ifndef VISUINO_ESP32
			if( C_OWNER.IsConnected() )
				if( FRXCharacteristic->written() )
					T_OUTPUT_PIN::SendPinValue( Mitov::TDataBlock( FRXCharacteristic->valueLength(), FRXCharacteristic->value() ));
#endif
*/
		}

	public:
		ArduinoBluetoothLEUartClient()
		{
			FInstance = this;
		}

	};
//---------------------------------------------------------------------------
template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_OutputPin
	> void * ArduinoBluetoothLEUartClient<
			T_OWNER, C_OWNER,
			T_Enabled,
			T_OutputPin
		>::FInstance;
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif // _MITOV_BLUETOOTH_LE_UART_CLIENT_h
