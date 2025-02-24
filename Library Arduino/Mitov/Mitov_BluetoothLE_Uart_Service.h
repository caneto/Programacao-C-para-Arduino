////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_BLUETOOTH_LE_UART_SERVICE_h
#define _MITOV_BLUETOOTH_LE_UART_SERVICE_h

#include <Mitov.h>
#include <Mitov_BluetoothLE_Services.h>
#ifdef VISUINO_ESP32
	#include <BLE2902.h>
#endif

namespace Mitov
{
	namespace ArduinoBluetoothLEUart
	{
		const char *SERVICE_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
		const char *RX_UUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
		const char *TX_UUID = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";
	}

	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_OutputPin
	> class ArduinoBluetoothLEUartService :
#ifdef VISUINO_ESP32
		public BLECharacteristicCallbacks,
#endif //  VISUINO_ESP32
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

	protected:
		BLEService			*FService;
		BLECharacteristic	*FTXCharacteristic;
		BLECharacteristic	*FRXCharacteristic;
//		BLEDescriptor		*FUserDescriptor;

	public:
		void write( uint8_t *AData, uint32_t ASize )
		{
			if( ! C_OWNER.Enabled() )
				return;

			if( ! Enabled() )
				return;

			while( ASize )
			{
				uint8_t ASendSize = MitovMin<uint32_t>( ASize, 20 );
				FTXCharacteristic->setValue( AData, ASendSize );
				FTXCharacteristic->notify();
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

		template<typename T> void PrintChar( T AValue )
		{
			write( AValue );
		}

		void Write( uint8_t *AData, uint32_t ASize )
		{
			write( AData, ASize );
		}

	public:
		virtual void onWrite(BLECharacteristic* pCharacteristic) override
		{
			if( C_OWNER.Enabled() )
				if( Enabled() )
					T_OutputPin::SendPinValue( Mitov::TDataBlock( pCharacteristic->m_value.getLength(), pCharacteristic->getData() ));
		}

	public:
		inline void SystemInit()
		{
#ifdef VISUINO_ESP32
			FService = C_OWNER.GetServer()->createService( ArduinoBluetoothLEUart::SERVICE_UUID );
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

/*
			if( UserDescription != "" )
			{
#ifdef VISUINO_ESP32
//				FUserDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
//				FUserDescriptor->setValue( UserDescription.c_str() );
//				FCharacteristic->addDescriptor( FUserDescriptor );
#else
//				FUserDescriptor = new BLEDescriptor( "2901", UserDescription.c_str() );
//				FCharacteristic->addDescriptor( *FUserDescriptor );
#endif
			}
*/
//			FCharacteristic->setValueLE( InitialValue * 100 + 0.5 );
#ifdef VISUINO_ESP32
			FService->start();
#endif
/*
			FService = new BLEService( "180F" ); // Add -0000-1000-8000-00805F9B34FB for standard
			C_OWNER.FPeripheral.addAttribute( *FService );
			C_OWNER.FPeripheral.setAdvertisedServiceUuid( FService->uuid());
			FCharacteristic = new BLEUnsignedCharCharacteristic( "2A19", BLERead | BLENotify );
			C_OWNER.FPeripheral.addAttribute( *FCharacteristic );
			FCharacteristic->setValue( InitialValue * 100 + 0.5 );
*/
		}

		inline void SystemLoopBegin()
		{
#ifndef VISUINO_ESP32
			if( C_OWNER.IsConnected() )
				if( FRXCharacteristic->written() )
					T_OUTPUT_PIN::SendPinValue( Mitov::TDataBlock( FRXCharacteristic->valueLength(), FRXCharacteristic->value() ));
#endif
		}

	};
//---------------------------------------------------------------------------
}

#endif // _MITOV_BLUETOOTH_LE_UART_SERVICE_h
