////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ESP32_WIFI_h
#define _MITOV_ESP32_WIFI_h

#include <Mitov.h>
#include <Mitov_BasicEthernet.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <lwip/dns.h>
//#include "ip_addr.h"
//#include "espconn.h"

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	class ESP32WiFiModuleOptionalChannel
	{
	public:
		bool	Enabled : 1;
		unsigned long	Channel : 4;

	public:
		ESP32WiFiModuleOptionalChannel() :
			Enabled( false ),
			Channel( 1 )
		{
		}

	};
//---------------------------------------------------------------------------
/*
	class ESP32WiFiModuleAccessPointConfig
	{
	public:
		bool		Enabled = false;

		::IPAddress	IP;
		::IPAddress	Gateway;
		::IPAddress	Subnet;

	};
//---------------------------------------------------------------------------
	class ESP32WiFiModuleRemoteConfig : public ESP32WiFiModuleAccessPointConfig
	{
	public:
		 ::IPAddress	DNS1;
		 ::IPAddress	DNS2;
	};
//---------------------------------------------------------------------------
	class ESP32WiFiModuleOptionalMacAddress
	{
	public:
		bool	Enabled = false;
		TMACAddress MacAddress;

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Channel,
		typename T_Config,
		typename T_Enabled,
		typename T_MacAddress,
		typename T_Password,
		typename T_SSID
	> class ESP32ModuleRemoteAccessPoint :
		public T_Channel,
		public T_Config,
		public T_Enabled,
		public T_MacAddress,
		public T_Password,
		public T_SSID
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( SSID )
	    _V_PROP_( Password )
		_V_PROP_( Channel )
		_V_PROP_( MacAddress )
		_V_PROP_( Config )

	public:
		void Connect()
		{
			if( ! Enabled() )
				return;

			if( SSID().GetValue() == "" )
				return;

			if( Config().Enabled() )
				WiFi.config( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress(), Config().DNS1().GetIPAddress(), Config().DNS2().GetIPAddress() );

//			Serial.println( SSID );
//			Serial.println( APassword );

			Begin();

//			if( C_OWNER.HostName != "" )
//				WiFi.setHostname( (char *)C_OWNER.HostName.c_str() );			

/*
			while (WiFi.status() != WL_CONNECTED) {
			WL_CONNECT_FAILED

			return ( ARes == WL_CONNECTED );
*/
//			return ( WiFi.begin( (char *)SSID.c_str(), APassword, channel, bssid ) == WL_CONNECTED );


//			return ( WiFi.begin( (char *)SSID.c_str() ) == WL_CONNECTED );


/*
			if( Password == "" )
				return ( LWiFi.connect( SSID.c_str() ) > 0 );

			switch( Encription )
			{
				case liweAuto:
				{
					if( LWiFi.connectWPA( SSID.c_str(), Password.c_str() ) > 0 )
						return true;

					return( LWiFi.connectWEP( SSID.c_str(), Password.c_str() ) > 0 );
				}

				case liweWEP:
					return( LWiFi.connectWEP( SSID.c_str(), Password.c_str() ) > 0 );

				case liweWPA:
					return( LWiFi.connectWPA( SSID.c_str(), Password.c_str() ) > 0 );
			}
*/
		}

		void Begin()
		{

			const char *APassword;
			if( Password().GetValue() == "" )
				APassword = nullptr;

			else
				APassword = Password().c_str();

			int32_t channel;

			if( Channel().Enabled() )
				channel = Channel().Channel();

			else
				channel = 0;

			uint8_t AMacAddress[ 6 ];
			const uint8_t* bssid;

			if( MacAddress().Enabled() )
			{
				MacAddress().MacAddress().GetMacAddress( AMacAddress );
				bssid = AMacAddress;
			}

			else
				bssid = nullptr;

			WiFi.begin( (char *)SSID().c_str(), APassword, channel, bssid, false );

			if( C_OWNER.HostName().GetValue() != "" )
				WiFi.setHostname( (char *)C_OWNER.HostName().c_str() );			

			WiFi.reconnect();
		}

	public:
		inline void IsEnabled( bool &AIsEnabled )
		{
			if( ( ! Enabled() ) || ( SSID() == "" ))
				return;

			AIsEnabled = true;
		}

		inline void TryConnect( bool &AIsEnabled )
		{
			if( ( ! Enabled() ) || ( SSID() == "" ))
				return;

			AIsEnabled = true;
			Connect();
		}

		inline void AssignDNS()
		{
//				Serial.println( Config.DNS.IP.toString() );
			if( Config().DNS1().GetUInt32Value() == 0 )
			{
				IPAddress AIP = WiFi.gatewayIP();
				WiFi.disconnect();

//					delay( 10000 );

//					Serial.println( AIP.toString() );
				if( Config().Enabled() )
					WiFi.config( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress(), AIP, Config().DNS2().GetIPAddress() );

				else
					WiFi.config( ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 255, 255, 255, 0 ), AIP );

/*
				ip_addr_t d;
				d.type = IPADDR_TYPE_V4;

				// Set DNS1-Server
				d.u_addr.ip4.addr = static_cast<uint32_t>( AIP );

				dns_setserver( 0, &d );
*/
				Begin();
//					WiFi.reconnect();
//					if(esp_wifi_disconnect() == ESP_OK)
//						esp_wifi_connect();
			}
		}

/*
		void TryConnect( uint32_t AIndex, uint32_t ACurrentIndex, int AOption, bool &AIsEnabled )
		{
			if( ( ! Enabled ) || ( SSID == "" ))
				return;

			AIsEnabled = true;

			if( AIndex != ACurrentIndex )
				return;

			if( ! AOption )
				return;

//			Serial.println( "TEST" );

			if( AOption == 2 )
			{
//				Serial.println( Config.DNS.IP.toString() );
				if( static_cast<uint32_t>( Config.DNS1 ) == 0 )
				{
					IPAddress AIP = WiFi.gatewayIP();
					WiFi.disconnect();

//					delay( 10000 );

//					Serial.println( AIP.toString() );
					if( Config.Enabled )
						WiFi.config( Config.IP, Config.Gateway, Config.Subnet, AIP, Config.DNS2 );

					else
						WiFi.config( ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 255, 255, 255, 0 ), AIP );

/ *
					ip_addr_t d;
					d.type = IPADDR_TYPE_V4;

					// Set DNS1-Server
					d.u_addr.ip4.addr = static_cast<uint32_t>( AIP );

					dns_setserver( 0, &d );
* /
					Begin();
//					WiFi.reconnect();
//					if(esp_wifi_disconnect() == ESP_OK)
//						esp_wifi_connect();
				}
			}

			else
				Connect();

		}
*/
	};
//---------------------------------------------------------------------------
	template<
		typename T_Channel,
		typename T_Config,
		typename T_Enabled,
		typename T_IsHidden,
		typename T_Password,
		typename T_SSID
	> class ESP32WiFiModuleAccessPoint :
		public T_Channel,
		public T_Config,
		public T_Enabled,
		public T_IsHidden,
		public T_Password,
		public T_SSID
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( IsHidden )
		_V_PROP_( Channel )

		_V_PROP_( SSID )
		_V_PROP_( Password )

		_V_PROP_( Config )

	public:
		inline void TryStart()
		{
//			if( AOwner->Enabled )
				if( Enabled() )
					if( SSID().GetValue() != "" )
					{
	//Serial.println( "TEST1" );
	//Serial.println( SSID );
	//Serial.println( IsHidden );
						if( Config().Enabled() )
							WiFi.softAPConfig( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress() );

						if( Password().GetValue() != "" )
							WiFi.softAP( SSID().c_str(), Password().c_str(), Channel(), IsHidden() );

						else
	//						WiFi.softAP( SSID().c_str() );
							WiFi.softAP( SSID().c_str(), nullptr, Channel(), IsHidden() );

//						if( AOwner->HostName != "" )
//							WiFi.setHostname( (char *)AOwner->HostName.c_str() );

						WiFi.mode( WIFI_AP );
						WiFi.softAPIP();
					}

		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_ChannelOutputPin,
		typename T_CountOutputPin,
		typename T_EncryptionOutputPin,
		typename T_FScanRequest,
		typename T_FailedOutputPin,
		typename T_FoundNetworkOutputPin,
		typename T_MACAddressOutputPin,
		typename T_SSIDOutputPin,
		typename T_ScanningOutputPin,
		typename T_ShowHidden,
		typename T_SignalStrengthOutputPin
	> class ESP32ModuleScanNetworksOperation :
		public T_ChannelOutputPin,
		public T_CountOutputPin,
		public T_EncryptionOutputPin,
		public T_FScanRequest,
		public T_FailedOutputPin,
		public T_FoundNetworkOutputPin,
		public T_MACAddressOutputPin,
		public T_SSIDOutputPin,
		public T_ScanningOutputPin,
		public T_ShowHidden,
		public T_SignalStrengthOutputPin
	{
	public:
		_V_PIN_( ScanningOutputPin )
		_V_PIN_( FailedOutputPin )
		_V_PIN_( CountOutputPin )
		_V_PIN_( SignalStrengthOutputPin )
		_V_PIN_( SSIDOutputPin )
		_V_PIN_( MACAddressOutputPin )
		_V_PIN_( EncryptionOutputPin )
		_V_PIN_( ChannelOutputPin )
		_V_PIN_( FoundNetworkOutputPin )

//		OpenWire::SourcePin	IsHiddenOutputPin ;
		
	public:
		_V_PROP_( ShowHidden )

	protected:
		_V_PROP_( FScanRequest )

	public:
		inline void SystemStart()
		{
			T_ScanningOutputPin::SetPinValue( FScanRequest(), false );
			ScanInputPin_o_Receive( nullptr );
		}

		inline void SystemLoopBegin()
		{
			T_ScanningOutputPin::SetPinValue( FScanRequest(), true );
			if( FScanRequest() )
			{
				int8_t AComplete = WiFi.scanComplete();
				if( AComplete == WIFI_SCAN_FAILED )
				{
					T_FailedOutputPin::ClockPin();
					T_CountOutputPin::SetPinValue( 0 );
					FScanRequest() = false;
				}

				else if( AComplete >= 0 )
				{
					T_CountOutputPin::SetPinValue( (int)AComplete );
					for( int i = 0; i < AComplete; ++ i )
					{
						Mitov::String ssid;
						uint8_t encryptionType;
						int32_t ASetrength;
						uint8_t* BSSID;
						int32_t channel;
//						bool isHidden;
						if( WiFi.getNetworkInfo( i, ssid, encryptionType, ASetrength, BSSID, channel )) //, isHidden ))
						{
							Mitov::String BSSIDStr = WiFi.BSSIDstr( i );

							T_SSIDOutputPin::SetPinValue( ssid.c_str() );
							T_SignalStrengthOutputPin::SetPinValue( ASetrength );
							T_EncryptionOutputPin::SetPinValue( encryptionType );
							T_MACAddressOutputPin::SetPinValue( BSSIDStr.c_str() );
							T_ChannelOutputPin::SetPinValue( channel );
//							IsHiddenOutputPin.Notify( &isHidden );
						}

						T_FoundNetworkOutputPin::ClockPin();
					}

					FScanRequest() = false;
				}
			}
		}

	public:
		inline void ScanInputPin_o_Receive( void *_Data )
		{
			if( WiFi.scanComplete() != WIFI_SCAN_RUNNING )
			{
				WiFi.scanNetworks( true, ShowHidden() );
//				WiFi.scanNetworks( false, ShowHidden );
				FScanRequest() = true;
			}
/*
			for( int i = 0; i < nearbyAccessPointCount; i ++ )
			{
				FoundSSIDOutputPin.Notify( LWiFi.SSID( i ));

				int32_t ASetrength = LWiFi.RSSI( i );
				FoundSignalStrengthOutputPin.Notify( &ASetrength );
			}
*/
		}

	public:
		inline ESP32ModuleScanNetworksOperation()
		{
			FScanRequest() = false;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_AccessPoint,
		typename T_AccessPoints_AssignDNS,
		typename T_AccessPoints_IsEnabled,
		typename T_AccessPoints_TryConnect,
		typename T_AddressOutputPin,
		typename T_AutoReconnect,
		typename T_BSSIDOutputPin,
		uint32_t C_ACESS_POINTS_COUNT,
		uint32_t C_SOCKETS_COUNT,
		typename T_DNSUpdated,
		typename T_Enabled,
		typename T_FIsStarted,
		typename T_GatewayIPOutputPin,
		typename T_HostName,
		typename T_MACOutputPin,
		typename T_RemoteConnectedOutputPin,
		typename T_RemoteStarted,
		typename T_Sockets_StopSocket,
		typename T_SubnetMaskIPOutputPin
	> class ESP32WiFiModule : 
		public T_AccessPoint,
		public T_AddressOutputPin,
		public T_AutoReconnect,
		public T_BSSIDOutputPin,
		public T_DNSUpdated,
		public T_Enabled,
		public T_FIsStarted,
		public T_GatewayIPOutputPin,
		public T_HostName,
		public T_MACOutputPin,
		public T_RemoteConnectedOutputPin,
		public T_RemoteStarted,
		public T_SubnetMaskIPOutputPin
	{
//		typedef ESP32WiFiModule<T_AddressOutputPin, T_MACOutputPin, T_BSSIDOutputPin, T_GatewayIPOutputPin, T_SubnetMaskIPOutputPin, T_RemoteConnectedOutputPin, C_ACESS_POINTS_COUNT, C_SOCKETS_COUNT>	SELF;

	public:
		_V_PIN_( AddressOutputPin )
		_V_PIN_( MACOutputPin )
		_V_PIN_( BSSIDOutputPin )
		_V_PIN_( GatewayIPOutputPin )
		_V_PIN_( SubnetMaskIPOutputPin )
		_V_PIN_( RemoteConnectedOutputPin )

	public:
		_V_PROP_( HostName )
		_V_PROP_( AutoReconnect )

	public:
		_V_PROP_( FIsStarted )
		_V_PROP_( RemoteStarted )
		_V_PROP_( DNSUpdated )

	public:
		_V_PROP_( Enabled )

	public:
//		bool    FIsConnected : 1;
		uint8_t	FAccessPointIndex = 255;

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

            Enabled() = AValue;
			if( Enabled() )
				StartEthernet();

			else
				StopEthernet();

		}

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				StartEthernet();

		}

	public:
		_V_PROP_( AccessPoint )

	public:
		inline bool GetIsStarted() { return FIsStarted(); }

	public:
		void UpdateAccessPoint()
		{
			if( AccessPoint().Enabled() )
			{
				if( Enabled() )
					AccessPoint().TryStart();
			}

			else
				WiFi.softAPdisconnect();

		}

        void RestartEthernet()
		{
			if( ! Enabled() )
				return;

//			C_SERIAL.end();
//			Ethernet.end();
			StartEthernet();
		}

		bool GetIPFromHostName( Mitov::String AHostName, ::IPAddress &AAdress )
		{
//			Serial.println( "GetIPFromHostName" );
//			Serial.println( AHostName );
			bool AResult = ( WiFi.hostByName( AHostName.c_str(), AAdress ) == 1 );
			if( ! AResult )
				AAdress = INADDR_NONE;

//			Serial.println( AAdress.toString());
			return AResult;
		}

	protected:
		void StopEthernet()
		{
			T_Sockets_StopSocket::ChainCall();

//			for( int i = 0; i < C_SOCKETS_COUNT; ++i )
//				Sockets[ i ]->StopSocket();

			WiFi.disconnect( true, true );
			FIsStarted() = false;
			RemoteStarted() = false;
			DNSUpdated() = false;
		}

		void StartEthernet()
		{
/*
			if( ! AccessPoints.length() )
				return;

			if( ! Sockets.length() )
				return;
*/
//			Serial.println( "TRY CONNECT" );

/*
			bool AConnected = false;
			for( int i = 0; i < C_ACESS_POINTS_COUNT; ++i )
				if( AccessPoints[ i ]->Connect( this ) )
				{
					RemoteStarted = true;
//					WiFi.setHostname( (char *)HostName.c_str() );
//					WiFi.reconnect();
					AConnected = true;
//					Serial.println( "CONNECT" );

					if( AddressOutputPin.IsConnected() )
					{
						Mitov::String IPAddress = IPAdressToString( WiFi.localIP());
						AddressOutputPin.Notify( (void *)IPAddress.c_str() );
					}

					if( MACOutputPin.IsConnected() )
					{
						Mitov::String AAddress = WiFi.macAddress();
						MACOutputPin.Notify( (void *)AAddress.c_str() );
					}

					FIsStarted = true;
					break;
				}

			if( ! AConnected )
				WiFi.begin();

			if( HostName != "" )
			{
				WiFi.setHostname( (char *)HostName.c_str() );
//				tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, (char *)HostName.c_str() );
//				WiFi.setHostname( "robot" );
//				tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_ETH, "robot" );
//				espconn_mdns_set_hostname( (char *) HostName.c_str() ); 
			}

			WiFi.reconnect();
			RemoteConnectedOutputPin.SetValue( AConnected, true );
*/
			if( HostName().GetValue() != "" )
				WiFi.setHostname( (char *)HostName().c_str() );

			WiFi.begin();
			WiFi.disconnect( true, true );
//			esp_wifi_disconnect();

//			WiFi.setAutoReconnect( AutoReconnect ); // For some reason not yet implemented!

			if( HostName().GetValue() != "" )
			{
				WiFi.setHostname( (char *)HostName().c_str() );
//				tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, (char *)HostName.c_str() );
//				WiFi.setHostname( "robot" );
//				tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_ETH, "robot" );
//				espconn_mdns_set_hostname( (char *) HostName.c_str() ); 
			}

			WiFi.mode( WIFI_OFF );
			if( Enabled() )
				AccessPoint().TryStart();

			if( T_BSSIDOutputPin::GetPinIsConnected() )
				T_BSSIDOutputPin::SetPinValue( WiFi.BSSIDstr().c_str() );

			if( T_GatewayIPOutputPin::GetPinIsConnected() )
				T_GatewayIPOutputPin::SetPinValue( IPAdressToString( WiFi.gatewayIP() ).c_str() );

			if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
				T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( WiFi.subnetMask() ).c_str() );

/*
			for( int i = 0; i < C_SOCKETS_COUNT; ++i )
				Sockets[ i ]->TryStartSocket();
*/
		}

	public:
		inline void SystemStart()
		{
//WiFi.mode( WIFI_STA );
//WiFi.mode( WIFI_OFF );
//esp_wifi_set_mode( (wifi_mode_t)WIFI_OFF );
//delay( 20000 );
//Serial.println( "SystemStart" );

//WiFi.printDiag( Serial );

//    wifi_mode_t mode;
//    esp_wifi_get_mode(&mode);
//	Serial.println( "TEST1" );
//	Serial.println( (int)mode );
//			return;
			T_RemoteConnectedOutputPin::SetPinValue( WiFi.isConnected(), false );
		}

		inline void SystemLoopBegin() // Called only for RemoteConnectedOutputPin.IsConnected
		{
//			return;
			int ARes = WiFi.status();

//			Serial.print( "STATUS: " );
//			Serial.println( ARes );

			bool AIsEnabled = false; //( C_ACESS_POINTS_COUNT == 0 );

			if( ! WiFi.isConnected() )
			{
//				Serial.println( "WiFi.isConnected()" );
//				FIsStarted() = false;
				RemoteStarted() = false;
				DNSUpdated() = false;
			}

			if( C_ACESS_POINTS_COUNT > 0 )
				if( ! RemoteStarted() )
				{
					if( ( ARes != WL_CONNECTED ) && ( ARes != WL_DISCONNECTED ) && ( ARes != WL_IDLE_STATUS ) )
					{
//			Serial.print( "STATUS: " );
//			Serial.println( ARes );
//						Serial.println( FAccessPointIndex );
//						AccessPoints[ FAccessPointIndex ]->Connect( this );
						++ FAccessPointIndex;
						if( FAccessPointIndex >= C_ACESS_POINTS_COUNT )
							FAccessPointIndex = 0;

						T_AccessPoints_TryConnect::ChainCall( FAccessPointIndex, AIsEnabled );
					}

					else
						T_AccessPoints_IsEnabled::ChainCall( FAccessPointIndex, AIsEnabled );

				}

			T_RemoteConnectedOutputPin::SetPinValue( WiFi.isConnected(), true );

//			Serial.println( AIsEnabled );

			if( ! AIsEnabled )
				FIsStarted() = true;

			else if( WiFi.isConnected() )
				if( ! RemoteStarted() )
				{
					if( ! DNSUpdated() )
					{
//						Serial.println( "TEST11" );
						DNSUpdated() = true;
//							bool AIsEnabled = false;
						T_AccessPoints_AssignDNS( FAccessPointIndex );

						return;
					}

					else
					{
						FIsStarted() = true;
						RemoteStarted() = true;
					}

/*
					if( HostName != "" )
					{
						WiFi.setHostname( (char *)HostName.c_str() );
//						WiFi.disconnect();
//						WiFi.reconnect();
//						ESP.restart();
//						Serial.println( HostName );
					}
*/
					if( T_AddressOutputPin::GetPinIsConnected() )
						T_AddressOutputPin::SetPinValue( IPAdressToString( WiFi.localIP()).c_str() );

					if( T_MACOutputPin::GetPinIsConnected() )
						T_MACOutputPin::SetPinValue( WiFi.macAddress().c_str() );

					if( T_BSSIDOutputPin::GetPinIsConnected() )
						T_BSSIDOutputPin::SetPinValue( WiFi.BSSIDstr().c_str() );

					if( T_GatewayIPOutputPin::GetPinIsConnected() )
						T_GatewayIPOutputPin::SetPinValue( IPAdressToString( WiFi.gatewayIP() ).c_str() );

					if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
						T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( WiFi.subnetMask() ).c_str() );

				}

//			for( int i = 0; i < C_SOCKETS_COUNT; ++i )
//				Sockets[ i ]->TryStartSocket();

//			WiFi.setHostname( (char *)HostName.c_str() );
		}

/*
	void DoCheckSignalStrength( void *_Data )
		{
			if( FIsStarted )
				if( SignalStrengthOutputPin.IsConnected() )
				{
					int32_t ASetrength = WiFi.RSSI();
					SignalStrengthOutputPin.Notify( &ASetrength );
				}

		}
*/
	public:
		inline ESP32WiFiModule()
		{			
//			FIsConnected() = false;
			FIsStarted() = false;
			RemoteStarted() = false;
			DNSUpdated() = false;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_SignalStrengthOutputPin
	> class ESP32ModuleSignalStrengthOperation :
		public T_SignalStrengthOutputPin
	{
	public:
		_V_PIN_( SignalStrengthOutputPin )

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( C_OWNER.FIsStarted() )
				if( T_SignalStrengthOutputPin::GetPinIsConnected() )
				{
					int32_t ASetrength = WiFi.RSSI();
					T_SignalStrengthOutputPin::SetPinValue( ASetrength );
				}

		}

	};
//---------------------------------------------------------------------------
	class ESP32ModuleReconnectOperation
	{
	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			WiFi.reconnect();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
