////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_BLUETOOTH_LE_SERVICES_h
#define _MITOV_BLUETOOTH_LE_SERVICES_h

#include <Mitov.h>

#ifdef VISUINO_ESP32
	#define private public
		#include <BLECharacteristic.h>
	#undef private

	#include <BLEDevice.h>
	#include <BLEUtils.h>
	#include <BLE2902.h>
//	#include <byteswap.h>
#else // VISUINO_ESP32
	#ifdef VISUINO_ARDUINO_101
		#include <CurieBLE.h>
	#else // VISUINO_ARDUINO_101
		#include <BLEPeripheral.h>
	#endif // VISUINO_ARDUINO_101
#endif // VISUINO_ESP32

#ifdef VISUINO_ESP32
//#define    ESP_GATT_CHAR_PROP_BIT_BROADCAST    (1 << 0)       /* 0x01 */    /* relate to BTA_GATT_CHAR_PROP_BIT_BROADCAST in bta/bta_gatt_api.h */
//#define    ESP_GATT_CHAR_PROP_BIT_READ         (1 << 1)       /* 0x02 */    /* relate to BTA_GATT_CHAR_PROP_BIT_READ in bta/bta_gatt_api.h */
//#define    ESP_GATT_CHAR_PROP_BIT_WRITE_NR     (1 << 2)       /* 0x04 */    /* relate to BTA_GATT_CHAR_PROP_BIT_WRITE_NR in bta/bta_gatt_api.h */
//#define    ESP_GATT_CHAR_PROP_BIT_WRITE        (1 << 3)       /* 0x08 */    /* relate to BTA_GATT_CHAR_PROP_BIT_WRITE in bta/bta_gatt_api.h */
//#define    ESP_GATT_CHAR_PROP_BIT_NOTIFY       (1 << 4)       /* 0x10 */    /* relate to BTA_GATT_CHAR_PROP_BIT_NOTIFY in bta/bta_gatt_api.h */
//#define    ESP_GATT_CHAR_PROP_BIT_INDICATE     (1 << 5)       /* 0x20 */    /* relate to BTA_GATT_CHAR_PROP_BIT_INDICATE in bta/bta_gatt_api.h */
//#define    ESP_GATT_CHAR_PROP_BIT_AUTH         (1 << 6)       /* 0x40 */    /* relate to BTA_GATT_CHAR_PROP_BIT_AUTH in bta/bta_gatt_api.h */
//#define    ESP_GATT_CHAR_PROP_BIT_EXT_PROP     (1 << 7)       /* 0x80 */    /* relate to BTA_GATT_CHAR_PROP_BIT_EXT_PROP in bta/bta_gatt_api.h */

/*
	const uint8_t BLEBroadcast = ESP_GATT_CHAR_PROP_BIT_BROADCAST;
	const uint8_t BLERead = ESP_GATT_CHAR_PROP_BIT_READ;
	const uint8_t BLEWrite = ESP_GATT_CHAR_PROP_BIT_WRITE;
	const uint8_t BLEWriteWithoutResponse = ESP_GATT_CHAR_PROP_BIT_WRITE_NR;
	const uint8_t BLENotify = ESP_GATT_CHAR_PROP_BIT_NOTIFY;
	const uint8_t BLEIndicate = ESP_GATT_CHAR_PROP_BIT_INDICATE;
*/
	const uint8_t BLEBroadcast = BLECharacteristic::PROPERTY_BROADCAST;
	const uint8_t BLERead = BLECharacteristic::PROPERTY_READ;
	const uint8_t BLEWrite = BLECharacteristic::PROPERTY_WRITE;
	const uint8_t BLEWriteWithoutResponse = BLECharacteristic::PROPERTY_WRITE_NR;
	const uint8_t BLENotify = BLECharacteristic::PROPERTY_NOTIFY;
	const uint8_t BLEIndicate = BLECharacteristic::PROPERTY_INDICATE;

template<typename T> class BLETypedCharacteristic : public BLECharacteristic
{
	typedef BLECharacteristic inherited;

public:
	void setValueLE( T AValue )
	{
		uint8_t temp[1];
		temp[0] = AValue;
		inherited::setValue( temp, 1 );
	}

	void setValueBE( T AValue )
	{
		uint8_t temp[1];
		temp[0] = AValue;
		inherited::setValue( temp, 1 );
	}

	T valueBE()
	{		
		return *getData();
	}

	T valueLE()
	{		
		return *getData();
	}

public:
	using inherited::inherited;

};

class BLEBoolCharacteristic : public BLETypedCharacteristic<bool> 
{
	typedef BLETypedCharacteristic<bool> inherited;

public:
	using inherited::inherited;

};

class BLEByteCharacteristic : public BLETypedCharacteristic<byte> 
{
	typedef BLETypedCharacteristic<byte> inherited;

public:
	using inherited::inherited;

};

class BLECharCharacteristic : public BLETypedCharacteristic<char> 
{
	typedef BLETypedCharacteristic<char> inherited;

public:
	using inherited::inherited;

};

class BLEUnsignedCharCharacteristic : public BLETypedCharacteristic<unsigned char> 
{
	typedef BLETypedCharacteristic<unsigned char> inherited;

public:
	using inherited::inherited;

};

class BLEShortCharacteristic : public BLETypedCharacteristic<short> 
{
	typedef BLETypedCharacteristic<short> inherited;

public:
	void setValueLE( short AValue ) 
	{
		uint8_t temp[2];
		temp[0] = AValue;
		temp[1] = AValue >> 8;
		inherited::setValue(temp, 2);
	}

	void setValueBE( short AValue )
	{
		uint8_t temp[2];
		temp[1] = AValue;
		temp[0] = AValue >> 8;
		inherited::setValue(temp, 2);
	}

	short valueLE()
	{		
		uint8_t *temp = getData();
		return ( short( temp[ 1 ] ) << 8 ) | temp[ 0 ];
	}

	short valueBE()
	{		
		uint8_t *temp = getData();
		return ( short( temp[ 0 ] ) << 8 ) | temp[ 1 ];
	}

public:
	using inherited::inherited;

};

class BLEUnsignedShortCharacteristic : public BLETypedCharacteristic<unsigned short> 
{
	typedef BLETypedCharacteristic<unsigned short> inherited;

public:
	void setValueLE( unsigned short AValue ) 
	{
		uint8_t temp[2];
		temp[0] = AValue;
		temp[1] = AValue >> 8;
		inherited::setValue(temp, 2);
	}

	void setValueBE( unsigned short AValue )
	{
		uint8_t temp[2];
		temp[1] = AValue;
		temp[0] = AValue >> 8;
		inherited::setValue(temp, 2);
	}

	unsigned short valueLE()
	{		
		uint8_t *temp = getData();
		return ( ((unsigned short) temp[ 1 ] ) << 8 ) | temp[ 0 ];
	}

	unsigned short valueBE()
	{		
		uint8_t *temp = getData();
		return ( ((unsigned short) temp[ 0 ] ) << 8 ) | temp[ 1 ];
	}

public:
	using inherited::inherited;

};

/*
class BLEIntCharacteristic : public BLETypedCharacteristic<int> 
{
	typedef BLETypedCharacteristic<int> inherited;

public:
	using inherited::inherited;

};

class BLEUnsignedIntCharacteristic : public BLETypedCharacteristic<unsigned int> 
{
	typedef BLETypedCharacteristic<unsigned int> inherited;

public:
	void setValueBE( unsigned int AValue )
	{
		inherited::setValue( bswap16( AValue ) );
	}

public:
	using inherited::inherited;

};
*/

class BLELongCharacteristic : public BLETypedCharacteristic<long> 
{
	typedef BLETypedCharacteristic<long> inherited;

public:
	void setValueLE( long AValue ) 
	{
		uint8_t temp[4];
		temp[0] = AValue;
		temp[1] = AValue >> 8;
		temp[2] = AValue >> 16;
		temp[3] = AValue >> 24;
		inherited::setValue(temp, 4);
	}

	void setValueBE( long AValue )
	{
		uint8_t temp[4];
		temp[3] = AValue;
		temp[2] = AValue >> 8;
		temp[1] = AValue >> 16;
		temp[0] = AValue >> 24;
		inherited::setValue(temp, 4);
	}

	long valueLE()
	{		
		uint8_t *temp = getData();
		return ( (((long) temp[ 3 ] ) << 24 ) | (((long) temp[ 2 ] ) << 16 ) | (((long) temp[ 1 ] ) << 8 ) | temp[ 0 ] );
	}

	long valueBE()
	{		
		uint8_t *temp = getData();
		return ( (((long) temp[ 0 ] ) << 24 ) | (((long) temp[ 1 ] ) << 16 ) | (((long) temp[ 2 ] ) << 8 ) | temp[ 3 ] );
	}

public:
	using inherited::inherited;

};

class BLEUnsignedLongCharacteristic : public BLETypedCharacteristic<unsigned long> 
{
	typedef BLETypedCharacteristic<unsigned long> inherited;

public:
	void setValueLE( unsigned long AValue ) 
	{
		uint8_t temp[4];
		temp[0] = AValue;
		temp[1] = AValue >> 8;
		temp[2] = AValue >> 16;
		temp[3] = AValue >> 24;
		inherited::setValue(temp, 4);
	}

	void setValueBE( unsigned long AValue )
	{
		uint8_t temp[4];
		temp[3] = AValue;
		temp[2] = AValue >> 8;
		temp[1] = AValue >> 16;
		temp[0] = AValue >> 24;
		inherited::setValue(temp, 4);
	}

	unsigned long valueLE()
	{		
		uint8_t *temp = getData();
		return ( (((unsigned long) temp[ 3 ] ) << 24 ) | (((unsigned long) temp[ 2 ] ) << 16 ) | (((unsigned long) temp[ 1 ] ) << 8 ) | temp[ 0 ] );
	}

	unsigned long valueBE()
	{		
		uint8_t *temp = getData();
		return ( (((unsigned long) temp[ 0 ] ) << 24 ) | (((unsigned long) temp[ 1 ] ) << 16 ) | (((unsigned long) temp[ 2 ] ) << 8 ) | temp[ 3 ] );
	}

public:
	using inherited::inherited;

};

class BLEFloatCharacteristic : public BLETypedCharacteristic<float> 
{
	typedef BLETypedCharacteristic<float> inherited;

public:
	void setValueLE( float AValue )
	{
//		Serial.println( AValue );
		unsigned long data32 = *(unsigned long *)&AValue;
		uint8_t temp[4];
		temp[0] = data32;
		temp[1] = data32 >> 8;
		temp[2] = data32 >> 16;
		temp[3] = data32 >> 24;
		inherited::setValue(temp, 4);
	}

	void setValueBE( float AValue )
	{
		unsigned long data32 = *(unsigned long *)&AValue;
		uint8_t temp[4];
		temp[3] = data32;
		temp[2] = data32 >> 8;
		temp[1] = data32 >> 16;
		temp[0] = data32 >> 24;
		inherited::setValue(temp, 4);
	}

	float valueLE()
	{		
		uint8_t *temp = getData();
		unsigned long AValue = ( (((unsigned long) temp[ 3 ] ) << 24 ) | (((unsigned long) temp[ 2 ] ) << 16 ) | (((unsigned long) temp[ 1 ] ) << 8 ) | temp[ 0 ] );
		return *(float *)&AValue;
	}

	float valueBE()
	{		
		uint8_t *temp = getData();
		unsigned long AValue = ( (((unsigned long) temp[ 0 ] ) << 24 ) | (((unsigned long) temp[ 1 ] ) << 16 ) | (((unsigned long) temp[ 2 ] ) << 8 ) | temp[ 3 ] );
		return *(float *)&AValue;
	}

public:
	using inherited::inherited;

};

/*
class BLEDoubleCharacteristic : public BLETypedCharacteristic<double> 
{
	typedef BLETypedCharacteristic<double> inherited;

public:
	using inherited::inherited;

};
*/

#endif // VISUINO_ESP32

namespace Mitov
{
//---------------------------------------------------------------------------
	enum TArduinoBluetoothSensorLocation { slNone, slOther, slChest, slWrist, slFinger, slHand, slEarLobe, slFoot };
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_UUID,
		typename T_Advertise
	> class ArduinoBluetoothLEService :
		public T_UUID,
		public T_Advertise
	{
	public:
		_V_PROP_( UUID )
		_V_PROP_( Advertise )

	public:
		BLEService	*FService;

	public:
		inline bool IsConnected()
		{
			return C_OWNER.IsConnected();
		}

/*
	public:
		inline void AddCharacteristic( BLECharacteristic *ACharacteristic )
		{
			FService->addCharacteristic( *ACharacteristic );
		}
*/
	public:
		inline void SystemInit()
		{
#ifdef VISUINO_ESP32
			FService = C_OWNER.GetServer()->createService( UUID().c_str() );
			if( Advertise() )
			{
				  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
				  pAdvertising->addServiceUUID( UUID().c_str() );
			}
#else
			FService = new BLEService( UUID().c_str() );
			C_OWNER.FPeripheral.addAttribute( *FService );
			if( Advertise() )
				C_OWNER.FPeripheral.setAdvertisedServiceUuid( FService->uuid());
#endif
		}

		inline void SystemStart()
		{
#ifdef VISUINO_ESP32
			FService->start();
#endif
		}
	};
//---------------------------------------------------------------------------
	template <typename T_INSTANCE> uint8_t GetBLECharacteristicProperties( T_INSTANCE *AInstance )
	{
			uint8_t AResult = AInstance->Notify() ? BLENotify : 0;
			if( AInstance->Indicate() )
				AResult |= BLEIndicate;

			if( AInstance->WriteWithoutResponse() )
				AResult |= BLEWriteWithoutResponse;

			if( AInstance->Write() )
				AResult |= BLEWrite;

			if( AInstance->Notify() )
				AResult |= BLENotify;

			if( AInstance->Read() )
				AResult |= BLERead;

			if( AInstance->Broadcast() )
				AResult |= BLEBroadcast;

			return AResult;
	}
//---------------------------------------------------------------------------
/*
	class ArduinoBluetoothLEBasicCharacteristic
	{
	public:
		Mitov::String	UUID;

		bool	Broadcast : 1;
		bool	Notify : 1;
		bool	Indicate : 1;
		bool	WriteWithoutResponse : 1;
		bool	Write : 1;
		bool	Read : 1;
		bool	BigEndian : 1;

	protected:
		inline uint8_t GetProperties()
		{
			uint8_t AResult = Notify ? BLENotify : 0;
			if( Indicate )
				AResult |= BLEIndicate;

			if( WriteWithoutResponse )
				AResult |= BLEWriteWithoutResponse;

			if( Write )
				AResult |= BLEWrite;

			if( Notify )
				AResult |= BLENotify;

			if( Read )
				AResult |= BLERead;

			if( Broadcast )
				AResult |= BLEBroadcast;

			return AResult;
		}

	public:
		ArduinoBluetoothLEBasicCharacteristic() :
			Broadcast( false ),
			Notify( false ),
			Indicate( false ),
			WriteWithoutResponse( false ),
			Write( false ),
			Read( false ),
			BigEndian( false )
		{
		}
	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CHARACT,
		typename T_BigEndian,
		typename T_Broadcast,
		typename T_Indicate,
		typename T_Notify,
		typename T_OutputPin,
		uint8_t C_PROPERTIES,
		typename T_Read,
		typename T_ReadRequestOutputPin,
		typename TYPE,
		typename T_UUID,
		typename T_Write,
		typename T_WriteWithoutResponse
	> class ArduinoBluetoothLETypedCharacteristic : 
#ifdef VISUINO_ESP32
		public BLECharacteristicCallbacks, 
#endif //  VISUINO_ESP32
//		public ArduinoBluetoothLEBasicCharacteristic,
		public T_BigEndian,
		public T_Broadcast,
		public T_Indicate,
		public T_Notify,
		public T_OutputPin,
		public T_Read,
		public T_ReadRequestOutputPin,
		public T_UUID,
		public T_Write,
		public T_WriteWithoutResponse
	{
//		typedef ArduinoBluetoothLEBasicCharacteristic inherited;

	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ReadRequestOutputPin )

	public:
		_V_PROP_( BigEndian )
		_V_PROP_( Broadcast )
		_V_PROP_( Indicate )
		_V_PROP_( Notify )
		_V_PROP_( Read )
		_V_PROP_( UUID )
		_V_PROP_( Write )
		_V_PROP_( WriteWithoutResponse )

	protected:
		T_CHARACT	*FCharacteristic;

#ifdef VISUINO_ESP32
	public:
		virtual void onRead(BLECharacteristic* pCharacteristic) override
		{
			T_ReadRequestOutputPin::ClockPin();
		}

		virtual void onWrite(BLECharacteristic* pCharacteristic) override
		{
			uint32_t ALength = FCharacteristic->m_value.getLength();
			if( ALength < sizeof( TYPE ) )
				return;

			if( BigEndian() )
				T_OutputPin::SetPinValue( FCharacteristic->valueBE(), true );

			else
				T_OutputPin::SetPinValue( FCharacteristic->valueLE(), true );

//			T_OutputPin::SendPinValue( Mitov::TDataBlock( FCharacteristic->m_value.getLength(), FCharacteristic->getData() ));
		}
#endif //  VISUINO_ESP32

	public:
		inline void SystemInit()
		{
			FCharacteristic = new T_CHARACT( UUID().c_str(), C_PROPERTIES | GetBLECharacteristicProperties( this ) );
#ifdef VISUINO_ESP32
			FCharacteristic->addDescriptor( new BLE2902() );
			C_OWNER.FService->addCharacteristic( FCharacteristic );
			FCharacteristic->setCallbacks( this );
#else //  VISUINO_ESP32
			C_OWNER.FService->addCharacteristic( *FCharacteristic );
#endif //  VISUINO_ESP32
//			C_OWNER.AddCharacteristic( FCharacteristic );
//			C_OWNER.FOwner.FPeripheral.addAttribute( *FCharacteristic );
		}

		inline void SystemLoopBegin()
		{
#ifdef VISUINO_ESP32
#else //  VISUINO_ESP32
			if( C_OWNER.IsConnected() )
				if( FCharacteristic->written() )
				{
					if( BigEndian() )
						T_OutputPin::SetPinValue( FCharacteristic->valueBE(), true );

					else
						T_OutputPin::SetPinValue( FCharacteristic->valueLE(), true );

				}

#endif //  VISUINO_ESP32
		}


	public:
		inline void InputPin_o_Receive( void *_Data )
		{
//			Serial.println( "Receive" );
			TYPE AValue = *(TYPE*)_Data;
			if( BigEndian() )
				FCharacteristic->setValueBE( AValue );

			else
				FCharacteristic->setValueLE( AValue );

#ifdef VISUINO_ESP32
			if( Notify() || ( C_PROPERTIES | BLENotify ))
				FCharacteristic->notify();

			if( Indicate() )
				FCharacteristic->indicate();
#endif //  VISUINO_ESP32
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Broadcast,
		typename T_Indicate,
		int C_MaxSize,
		typename T_Notify,
		typename T_OutputPin,
		uint8_t C_PROPERTIES,
		typename T_Read,
		typename T_ReadRequestOutputPin,
		typename T_UUID,
		typename T_Write,
		typename T_WriteWithoutResponse
	> class ArduinoBluetoothLEBinaryCharacteristic : 
#ifdef VISUINO_ESP32
		public BLECharacteristicCallbacks, 
#endif //  VISUINO_ESP32
//		public ArduinoBluetoothLEBasicCharacteristic, 
		public T_Broadcast,
		public T_Indicate,
		public T_Notify,
		public T_OutputPin,
		public T_Read,
		public T_ReadRequestOutputPin,
		public T_UUID,
		public T_Write,
		public T_WriteWithoutResponse
	{
//		typedef ArduinoBluetoothLEBasicCharacteristic inherited;

	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ReadRequestOutputPin )

	public:
		_V_PROP_( Broadcast )
		_V_PROP_( Indicate )
		_V_PROP_( Notify )
		_V_PROP_( Read )
		_V_PROP_( UUID )
		_V_PROP_( Write )
		_V_PROP_( WriteWithoutResponse )

	protected:
		BLECharacteristic	*FCharacteristic;

#ifdef VISUINO_ESP32
	public:
		virtual void onRead(BLECharacteristic* pCharacteristic) override
		{
			T_ReadRequestOutputPin::ClockPin();
		}

		virtual void onWrite(BLECharacteristic* pCharacteristic) override
		{
			T_OutputPin::SendPinValue( Mitov::TDataBlock( pCharacteristic->m_value.getLength(), pCharacteristic->getData() ));
		}
#endif //  VISUINO_ESP32

	public:
		inline void SystemInit()
		{
#ifdef VISUINO_ESP32
			FCharacteristic = new BLECharacteristic( UUID().c_str(), C_PROPERTIES | GetBLECharacteristicProperties( this ) );
			FCharacteristic->addDescriptor(new BLE2902());
			C_OWNER.FService->addCharacteristic( FCharacteristic );
			FCharacteristic->setCallbacks( this );
#else //  VISUINO_ESP32
			FCharacteristic = new BLECharacteristic( UUID().c_str(), C_PROPERTIES | GetBLECharacteristicProperties( this ), C_MaxSize );
			C_OWNER.FService->addCharacteristic( *FCharacteristic );
#endif //  VISUINO_ESP32
//			C_OWNER.AddCharacteristic( FCharacteristic );
//			C_OWNER.FOwner.FPeripheral.addAttribute( *FCharacteristic );
		}

		inline void SystemLoopBegin()
		{
#ifdef VISUINO_ESP32
#else //  VISUINO_ESP32
			if( C_OWNER.IsConnected() )
				if( FCharacteristic->written() )
					T_OutputPin::SendPinValue( Mitov::TDataBlock( FCharacteristic->valueLength(), FCharacteristic->value() ));

#endif //  VISUINO_ESP32
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock &ABlock = *(Mitov::TDataBlock *)_Data;
			FCharacteristic->setValue( ABlock.Data, ABlock.Size );
#ifdef VISUINO_ESP32
			if( Notify() || ( C_PROPERTIES | BLENotify ))
				FCharacteristic->notify();

			if( Indicate() )
				FCharacteristic->indicate();
#endif //  VISUINO_ESP32
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Broadcast,
		typename T_Indicate,
		int C_MaxSize,
		typename T_Notify,
		typename T_OutputPin,
		uint8_t C_PROPERTIES,
		typename T_Read,
		typename T_ReadRequestOutputPin,
		typename T_UUID,
		typename T_Write,
		typename T_WriteWithoutResponse
	> class ArduinoBluetoothLETextCharacteristic : 
#ifdef VISUINO_ESP32
		public BLECharacteristicCallbacks, 
#endif //  VISUINO_ESP32
		public T_Broadcast,
		public T_Indicate,
		public T_Notify,
		public T_OutputPin,
		public T_Read,
		public T_ReadRequestOutputPin,
		public T_UUID,
		public T_Write,
		public T_WriteWithoutResponse
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ReadRequestOutputPin )

	public:
		_V_PROP_( Broadcast )
		_V_PROP_( Indicate )
		_V_PROP_( Notify )
		_V_PROP_( Read )
		_V_PROP_( UUID )
		_V_PROP_( Write )
		_V_PROP_( WriteWithoutResponse )

	protected:
		BLECharacteristic	*FCharacteristic;

#ifdef VISUINO_ESP32
	public:
		virtual void onRead(BLECharacteristic* pCharacteristic) override
		{
			T_ReadRequestOutputPin::ClockPin();
		}

		virtual void onWrite(BLECharacteristic* pCharacteristic) override
		{
			T_OutputPin::SetPinValue( pCharacteristic->getValue().c_str(), false );
		}
#endif //  VISUINO_ESP32

	public:
		inline void SystemInit()
		{
#ifdef VISUINO_ESP32
			FCharacteristic = new BLECharacteristic( UUID().c_str(), C_PROPERTIES | GetBLECharacteristicProperties( this ) );
			FCharacteristic->addDescriptor(new BLE2902());
			C_OWNER.FService->addCharacteristic( FCharacteristic );
			FCharacteristic->setCallbacks( this );
#else //  VISUINO_ESP32
			FCharacteristic = new BLECharacteristic( UUID().c_str(), C_PROPERTIES | GetBLECharacteristicProperties( this ), C_MaxSize );
			C_OWNER.FService->addCharacteristic( *FCharacteristic );
#endif //  VISUINO_ESP32
//			C_OWNER.AddCharacteristic( FCharacteristic );
//			C_OWNER.FOwner.FPeripheral.addAttribute( *FCharacteristic );
		}

		inline void SystemLoopBegin()
		{
#ifdef VISUINO_ESP32
#else //  VISUINO_ESP32
			if( C_OWNER.IsConnected() )
				if( FCharacteristic->written() )
					T_OutputPin::SendPinValue( StringEx( (const char*)FCharacteristic->value(), FCharacteristic->valueLength() ) );

#endif //  VISUINO_ESP32
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::String AData = (char *)_Data;
			FCharacteristic->setValue( (unsigned char *)AData.c_str(), AData.length() );
#ifdef VISUINO_ESP32
			if( Notify() || ( C_PROPERTIES | BLENotify ))
				FCharacteristic->notify();

			if( Indicate() )
				FCharacteristic->indicate();
#endif //  VISUINO_ESP32
		}

	};
//---------------------------------------------------------------------------
}

#endif // _MITOV_BLUETOOTH_LE_SERVICES_h
