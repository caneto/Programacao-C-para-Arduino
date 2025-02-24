////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ESP32_BLE_h
#define _MITOV_ESP32_BLE_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

#define private public
	#include <BLECharacteristic.h>
#undef private

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <esp_bt.h>            // ESP32 BLE

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace TArduinoESP32BluetoothLESecurity
	{
		enum TArduinoESP32BluetoothLESecurity {	Default, Encrypt, Encrypt_No_MITM, Encrypt_MITM };
	}
//---------------------------------------------------------------------------
	class ESP32ServerImp_NoServer
	{
	public:
		BLEServer *GetServer() { return nullptr; }

	};
//---------------------------------------------------------------------------
	class ESP32ServerImp
	{
	public:
		BLEServer *FServer;

	public:
		BLEServer *GetServer() { return FServer; }

	};
//---------------------------------------------------------------------------
	template<
		typename T_ConnectedCountOutputPin,
		typename T_DeviceName,
		typename T_MACAddressOutputPin,
		typename T_MaximumTransmissionUnit,
		typename T_Security,
		typename T_Server,
		typename T_TransmitPower
	> class ESP32BluetoothLE :
		public T_ConnectedCountOutputPin,
		public T_DeviceName,
		public T_MACAddressOutputPin,
		public T_MaximumTransmissionUnit,
		public T_Security,
		public T_Server,
		public T_TransmitPower
	{
	public:
		_V_PIN_( ConnectedCountOutputPin )
		_V_PIN_( MACAddressOutputPin )
		
	public:
		_V_PROP_( DeviceName )
		_V_PROP_( MaximumTransmissionUnit )
		_V_PROP_( TransmitPower )
		_V_PROP_( Security )

	public:
		inline void SystemInit()
		{
			BLEDevice::init( DeviceName().c_str() );
		}

		inline void SystemStart()
		{
			T_ConnectedCountOutputPin::SetPinValue( 0, false );

//			Serial.println( "TEST" );
//			BLEDevice::init( DeviceName.c_str() );

//			Serial.println( "TEST1" );
			BLEDevice::setMTU( MaximumTransmissionUnit() );
			BLEDevice::setPower( esp_power_level_t( TransmitPower().GetValue() ));
			BLEDevice::setEncryptionLevel( esp_ble_sec_act_t( Security().GetValue() ));

			T_MACAddressOutputPin::SetPinValue( BLEDevice::getAddress().toString().c_str() );
		}

		inline void SystemLoopBeginConnectionCount()
		{
			T_ConnectedCountOutputPin::SetPinValue( T_Server::GetServer()->getConnectedCount(), true );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif // _MITOV_ESP32_BLE_h
