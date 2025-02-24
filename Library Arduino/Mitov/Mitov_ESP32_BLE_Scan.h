////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ESP32_BLE_SCAN_h
#define _MITOV_ESP32_BLE_SCAN_h

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	namespace ESP32ModuleScanBLE
	{
		bool InScanning = false;
	}
//---------------------------------------------------------------------------
	template<
		typename T_Active,
		typename T_ActiveScanWindow,
		typename T_ClearStored,
		typename T_CountDevicesOutputPin,
		typename T_DeviceNameOutputPin,
		typename T_Duration,
		typename T_FScanRequest,
		typename T_FoundDeviceOutputPin,
		typename T_MACAddressOutputPin,
		typename T_ManufacturerDataOutputPin,
		typename T_PayloadOutputPin,
		typename T_ScanningOutputPin,
		typename T_ServiceDataOutputPin,
		typename T_ServiceUUIDOutputPin,
		typename T_SignalStrengthOutputPin,
		typename T_TransmitPowerOutputPin
	> class ESP32ModuleScanBLEOperation :
		public T_Active,
		public T_ActiveScanWindow,
		public T_ClearStored,
		public T_CountDevicesOutputPin,
		public T_DeviceNameOutputPin,
		public T_Duration,
		public T_FScanRequest,
		public T_FoundDeviceOutputPin,
		public T_MACAddressOutputPin,
		public T_ManufacturerDataOutputPin,
		public T_PayloadOutputPin,
		public T_ScanningOutputPin,
		public T_ServiceDataOutputPin,
		public T_ServiceUUIDOutputPin,
		public T_SignalStrengthOutputPin,
		public T_TransmitPowerOutputPin
	{
	public:
		_V_PIN_( ScanningOutputPin )
		_V_PIN_( CountDevicesOutputPin )
		_V_PIN_( DeviceNameOutputPin )
		_V_PIN_( MACAddressOutputPin )
		_V_PIN_( ServiceDataOutputPin )
		_V_PIN_( ServiceUUIDOutputPin )
		_V_PIN_( ManufacturerDataOutputPin )
		_V_PIN_( SignalStrengthOutputPin )
		_V_PIN_( TransmitPowerOutputPin )
		_V_PIN_( PayloadOutputPin )
		_V_PIN_( FoundDeviceOutputPin )

	public:
		_V_PROP_( Duration )
		_V_PROP_( ActiveScanWindow )
		_V_PROP_( Active )
		_V_PROP_( ClearStored )

	protected:
		_V_PROP_( FScanRequest )

		static	void *FInstance;

	protected:
		static void SharedScanCompleteCB( BLEScanResults foundDevices )
		{
			ESP32ModuleScanBLE::InScanning = false;
			((ESP32ModuleScanBLEOperation *)FInstance )->ScanCompleteCB( foundDevices );
		}

		void ScanCompleteCB( BLEScanResults &foundDevices )
		{
			int ACount = foundDevices.getCount();
			for( int i = 0; i < ACount; ++i )
			{
				BLEAdvertisedDevice ADevice = foundDevices.getDevice( i );
				T_DeviceNameOutputPin::SetPinValue( ADevice.getName().c_str() );
				T_MACAddressOutputPin::SetPinValue( ADevice.getAddress().toString().c_str() );
				T_ServiceDataOutputPin::SetPinValue( ADevice.getServiceData().c_str() );
				T_ServiceUUIDOutputPin::SetPinValue( ADevice.getServiceUUID().toString().c_str() );
				T_ManufacturerDataOutputPin::SetPinValue( ADevice.getManufacturerData().c_str() );
				T_SignalStrengthOutputPin::SetPinValue( ADevice.getRSSI() );
				T_TransmitPowerOutputPin::SetPinValue( ADevice.getTXPower() );
				T_PayloadOutputPin::SendPinValue( Mitov::TDataBlock( ADevice.getPayloadLength(), ADevice.getPayload() ));
				T_FoundDeviceOutputPin::ClockPin();
			}

			T_CountDevicesOutputPin::SetPinValue( ACount );
			T_ScanningOutputPin::SetPinValue( false, true );
		}

	public:
		inline void SystemStart()
		{
			T_ScanningOutputPin::SetPinValue( false, false );
			ScanInputPin_o_Receive( nullptr );
		}

		inline void SystemStartNoScan()
		{
			T_ScanningOutputPin::SetPinValue( false, false );
		}

		inline void SystemLoopBegin()
		{
			if( FScanRequest() )
				ScanInputPin_o_Receive( nullptr );
		}

	public:
		inline void ScanInputPin_o_Receive( void *_Data )
		{
			if( ESP32ModuleScanBLE::InScanning )
			{
				FScanRequest() = true;
				return;
			}

			ESP32ModuleScanBLE::InScanning = true;
			T_ScanningOutputPin::SetPinValue( true, true );

			FInstance = this;
			BLEScan* AScan = BLEDevice::getScan();
			AScan->setActiveScan( Active() );
//			AScan->setInterval( Duration );
//			AScan->setWindow( MitovMin( Duration, ActiveScanWindow ));
			AScan->start( Duration(), SharedScanCompleteCB, false );
		}
		
	};
//---------------------------------------------------------------------------
template<
		typename T_Active,
		typename T_ActiveScanWindow,
		typename T_ClearStored,
		typename T_CountDevicesOutputPin,
		typename T_DeviceNameOutputPin,
		typename T_Duration,
		typename T_FScanRequest,
		typename T_FoundDeviceOutputPin,
		typename T_MACAddressOutputPin,
		typename T_ManufacturerDataOutputPin,
		typename T_PayloadOutputPin,
		typename T_ScanningOutputPin,
		typename T_ServiceDataOutputPin,
		typename T_ServiceUUIDOutputPin,
		typename T_SignalStrengthOutputPin,
		typename T_TransmitPowerOutputPin
	> void * ESP32ModuleScanBLEOperation<
		T_Active,
		T_ActiveScanWindow,
		T_ClearStored,
		T_CountDevicesOutputPin,
		T_DeviceNameOutputPin,
		T_Duration,
		T_FScanRequest,
		T_FoundDeviceOutputPin,
		T_MACAddressOutputPin,
		T_ManufacturerDataOutputPin,
		T_PayloadOutputPin,
		T_ScanningOutputPin,
		T_ServiceDataOutputPin,
		T_ServiceUUIDOutputPin,
		T_SignalStrengthOutputPin,
		T_TransmitPowerOutputPin
	>::FInstance;
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif // _MITOV_ESP32_BLE_SCAN_h
