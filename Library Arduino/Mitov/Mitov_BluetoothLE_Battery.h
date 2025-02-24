////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_BLUETOOTH_LE_BATTERY_h
#define _MITOV_BLUETOOTH_LE_BATTERY_h

#include <Mitov.h>
#include <Mitov_BluetoothLE_Services.h>
#ifdef VISUINO_ESP32
	#include <BLE2902.h>
#endif

namespace Mitov
{
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_InitialValue,
		typename T_UserDescription
	> class ArduinoBluetoothLEBatteryService :
		public T_InitialValue,
		public T_UserDescription
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( UserDescription )

	protected:
		BLEService						*FService;
		BLEUnsignedCharCharacteristic	*FCharacteristic;
		BLEDescriptor					*FUserDescriptor;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float *)_Data;
			if( InitialValue() == AValue )
				return;

			InitialValue() = AValue;
			FCharacteristic->setValueLE( InitialValue() * 100 + 0.5 );
		}

	public:
		void SystemInit()
		{
			FService = C_OWNER.AddStandardService( 0x180F );
			C_OWNER.AddveriseService( FService );
			FCharacteristic = C_OWNER.template AddStandardCharacteristic<BLEUnsignedCharCharacteristic>( FService, 0x2A19, BLERead | BLENotify );
#ifdef VISUINO_ESP32
			FCharacteristic->addDescriptor(new BLE2902());
#endif

			if( UserDescription().GetValue() != "" )
			{
#ifdef VISUINO_ESP32
				FUserDescriptor = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
				FUserDescriptor->setValue( UserDescription().c_str() );
				FCharacteristic->addDescriptor( FUserDescriptor );
#else
				FUserDescriptor = new BLEDescriptor( "2901", UserDescription().c_str() );
				FCharacteristic->addDescriptor( *FUserDescriptor );
#endif
			}

			FCharacteristic->setValueLE( InitialValue() * 100 + 0.5 );
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
	};
//---------------------------------------------------------------------------
}

#endif
