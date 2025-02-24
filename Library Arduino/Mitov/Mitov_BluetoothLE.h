////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_BLUETOOTH_LE_h
#define _MITOV_BLUETOOTH_LE_h

#include <Mitov.h>

#ifdef VISUINO_ESP32
	#define private public
		#include <BLECharacteristic.h>
	#undef private

	#include <BLEDevice.h>
	#include <BLEUtils.h>
#else // VISUINO_ESP32
	#ifdef VISUINO_ARDUINO_101
		#include <CurieBLE.h>
	#else // VISUINO_ARDUINO_101
		#include <BLEPeripheral.h>
	#endif // VISUINO_ARDUINO_101
#endif // VISUINO_ESP32

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_ConnectedOutputPin,
		typename T_DeviceName,
		typename T_FConnected,
		typename T_LocalName,
		typename T_MACAddressOutputPin
	>	class ArduinoBluetoothLE :
		public T_ConnectedOutputPin,
		public T_DeviceName,
		public T_FConnected,
		public T_LocalName,
		public T_MACAddressOutputPin
	{
	public:
		_V_PIN_( ConnectedOutputPin )
		_V_PIN_( MACAddressOutputPin )

	public:
		_V_PROP_( LocalName )
		_V_PROP_( DeviceName )

	public:
		BLEPeripheral	FPeripheral;

/*
	public:
		void RegisterUpdateElement( ArduinoBlutoothUpdateIntf *AElement )
		{
			FElements.push_back( AElement );
		}
*/
	protected:
//		Mitov::SimpleList<ArduinoBlutoothUpdateIntf *>	FElements;
		_V_PROP_( FConnected ) // = false;

	public:
		inline bool IsConnected()
		{
			return FConnected();
		}

	public:
		void AddveriseService( BLEService *AService )
		{
			FPeripheral.setAdvertisedServiceUuid( AService->uuid());
		}

		BLEService *AddStandardService( uint16_t ASubUUID )
		{
			Mitov::String AText( ASubUUID, HEX );
			while( AText.length() < 4 )
				AText = '0' + AText;

			BLEService *AService = new BLEService( AText.c_str() ); // Add -0000-1000-8000-00805F9B34FB for standard
			FPeripheral.addAttribute( *AService );
			return AService;
		}

		template<typename T_CHARACTERISTIC> T_CHARACTERISTIC *AddStandardCharacteristic( BLEService *AService, uint16_t ASubUUID, uint8_t AProperties )
		{
			Mitov::String AText( ASubUUID, HEX );
			while( AText.length() < 4 )
				AText = '0' + AText;

			T_CHARACTERISTIC *ACharacteristic = new BLEUnsignedCharCharacteristic( AText.c_str(), AProperties );
			FPeripheral.addAttribute( *ACharacteristic );
			return ACharacteristic;
		}

	public:
		inline void SystemLoopBegin()
		{
			BLECentral ACentral = FPeripheral.central();
			if( FConnected() )
			{
				if( ! ACentral )
				{
					FConnected() = false;
					T_ConnectedOutputPin::SetPinValue( FConnected(), false );
				}
			}
			else
			{
				if( ACentral )
				{
					FConnected() = true;
					T_ConnectedOutputPin::SetPinValue( FConnected(), false );
					Mitov::String AAddress = ACentral.address();
					T_MACAddressOutputPin::SetPinValue( AAddress, false );
				}
			}

/*
			if( FConnected )
			{
				for( int i = 0; i < FElements.size(); ++ i )
					FElements[ i ]->UpdateValues();
			}
*/
		}

		inline void SystemInit()
		{
			if( LocalName().GetValue() != "" )
				FPeripheral.setLocalName( LocalName().c_str() );

			if( DeviceName().GetValue() != "" )
				FPeripheral.setDeviceName( DeviceName().c_str() );
		}

		inline void SystemStart()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
			FPeripheral.begin();
		}

	public:
		inline ArduinoBluetoothLE()
		{
			FConnected() = false;
		}

	};
//---------------------------------------------------------------------------
}

#endif
