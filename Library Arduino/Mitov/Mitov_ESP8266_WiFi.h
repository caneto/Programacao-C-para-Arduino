////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ESP8266_WIFI_h
#define _MITOV_ESP8266_WIFI_h

#include <Mitov.h>
#include <Mitov_BasicEthernet.h>
#include <ESP8266WiFi.h>
//#include "ip_addr.h"
//#include "espconn.h"

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
/*
	class ESP8266WiFiModuleOptionalIPConfig
	{
	public:
		bool	Enabled = false;
		::IPAddress	IP;

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
	> class ESP8266ModuleRemoteAccessPoint :
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

	protected:
		void Begin()
		{
			const char *APassword;
			if( Password() == "" )
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

/*
			if( Config.Enabled )
				WiFi.config( Config.IP, Config.Gateway, Config.Subnet, Config.DNS1, Config.DNS2 );

			else
				WiFi.config( ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 0, 0, 0, 0 ), ::IPAddress( 255, 255, 255, 0 ) );
*/
//			Serial.println( SSID );
//			Serial.println( APassword );
			WiFi.begin( (char *)SSID().c_str(), APassword, channel, bssid );

			if( C_OWNER.HostName().GetValue() != "" )
				WiFi.hostname( C_OWNER.HostName() );

			WiFi.reconnect();
		}

	public:
		void Connect()
		{
			if( ! Enabled() )
				return;

			if( SSID() == "" )
				return;

//			Serial.println( SSID );
//			Serial.println( Password );

			if( Config().Enabled() )
				WiFi.config( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress(), Config().DNS1().GetIPAddress(), Config().DNS2().GetIPAddress() );

			Begin();

/*
			for(;;)
			{
//				Serial.print( "." );
				int ARes = WiFi.status();
//				Serial.print( ";" );
//				Serial.println( ARes );
				if( ARes == WL_CONNECTED )
					return true;

//				Serial.println( "+" );
				if( ARes == WL_CONNECT_FAILED )
					return false;

//				Serial.println( "++" );
				if( ARes == WL_NO_SSID_AVAIL )
					return false;

//				Serial.println( "+++" );
				delay(500);
			}
*/
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
//			return false;
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
		inline void TryConnect( uint32_t AIndex, uint32_t ACurrentIndex, int AOption, bool &AIsEnabled )
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

/*
	public:
		ESP8266ModuleRemoteAccessPoint( TMACAddress AMacAddress )
		{
			MacAddress.MacAddress = AMacAddress;
		}

		ESP8266ModuleRemoteAccessPoint()
		{
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
	> class ESP8266WiFiModuleAccessPoint :
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
//Serial.println( "TEST1" );
//Serial.println( SSID().GetValue() );
//Serial.println( IsHidden().GetValue() );
//			if( AOwner->Enabled )
				if( Enabled() )
					if( SSID().GetValue() != "" )
					{
						if( Config().Enabled() )
						{
//							Serial.print( "IP : " );
//							Serial.println( IPAdressToString( Config().IP().GetIPAddress() ) );
							WiFi.softAPConfig( Config().IP().GetIPAddress(), Config().Gateway().GetIPAddress(), Config().Subnet().GetIPAddress() );
						}

						if( Password().GetValue() != "" )
							WiFi.softAP( SSID().c_str(), Password().c_str(), Channel(), IsHidden() );

						else
//								WiFi.softAP( SSID.c_str() );
							WiFi.softAP( SSID().c_str(), nullptr, Channel(), IsHidden() );

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
		typename T_IsHiddenOutputPin,
		typename T_MACAddressOutputPin,
		typename T_SSIDOutputPin,
		typename T_ScanningOutputPin,
		typename T_SignalStrengthOutputPin
	> class ESP8266ModuleScanNetworksOperation :
		public T_ChannelOutputPin,
		public T_CountOutputPin,
		public T_EncryptionOutputPin,
		public T_FScanRequest,
		public T_FailedOutputPin,
		public T_FoundNetworkOutputPin,
		public T_IsHiddenOutputPin,
		public T_MACAddressOutputPin,
		public T_SSIDOutputPin,
		public T_ScanningOutputPin,
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
		_V_PIN_( IsHiddenOutputPin )
		_V_PIN_( FoundNetworkOutputPin )

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
					T_CountOutputPin::SetPinValue( AComplete );
					for( int i = 0; i < AComplete; i ++ )
					{
						Mitov::String ssid;
						uint8_t encryptionType;
						int32_t ASetrength;
						uint8_t* BSSID;
						int32_t channel;
						bool isHidden;
						if( WiFi.getNetworkInfo( i, ssid, encryptionType, ASetrength, BSSID, channel, isHidden ))
						{
							Mitov::String BSSIDStr = WiFi.BSSIDstr( i );

							T_SSIDOutputPin::SetPinValue( ssid.c_str() );
							T_SignalStrengthOutputPin::SetPinValue( ASetrength );
							T_EncryptionOutputPin::SetPinValue( encryptionType );
							T_MACAddressOutputPin::SetPinValue( BSSIDStr.c_str() );
							T_ChannelOutputPin::SetPinValue( channel );
							T_IsHiddenOutputPin::SetPinValue( isHidden );
						}

						T_FoundNetworkOutputPin::ClockPin();
					}

					FScanRequest() = false;
				}
			}
		}

	public:
		void ScanInputPin_o_Receive( void *_Data )
		{
			if( WiFi.scanComplete() != WIFI_SCAN_RUNNING )
			{
				WiFi.scanNetworks( true );
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
		inline ESP8266ModuleScanNetworksOperation()
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
		typename T_GatewayIPOutputPin,
		typename T_HostName,
		typename T_IsStarted,
		typename T_MACOutputPin,
//		typename T_NoDelay,
		typename T_RemoteConnectedOutputPin,
		typename T_RemoteStarted,
		typename T_Sockets_StopSocket,
		typename T_SubnetMaskIPOutputPin
	> class ESP8266WiFiModule :
		public T_AccessPoint,
		public T_AddressOutputPin,
		public T_AutoReconnect,
		public T_BSSIDOutputPin,
		public T_DNSUpdated,
		public T_Enabled,
		public T_GatewayIPOutputPin,
		public T_HostName,
		public T_IsStarted,
		public T_MACOutputPin,
//		public T_NoDelay,
		public T_RemoteConnectedOutputPin,
		public T_RemoteStarted,
		public T_Sockets_StopSocket,
		public T_SubnetMaskIPOutputPin
	{
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
		_V_PIN_( AddressOutputPin )
		_V_PIN_( MACOutputPin )
		_V_PIN_( BSSIDOutputPin )
		_V_PIN_( GatewayIPOutputPin )
		_V_PIN_( SubnetMaskIPOutputPin )
		_V_PIN_( RemoteConnectedOutputPin )

	public:
		_V_PROP_( AccessPoint )
//		ESP8266ModuleRemoteAccessPoint< ESP8266WiFiModule<C_ACESS_POINTS_COUNT, C_SOCKETS_COUNT> > *AccessPoints[ C_ACESS_POINTS_COUNT ];

	public:
		_V_PROP_( HostName )
		_V_PROP_( Enabled )
		_V_PROP_( AutoReconnect )
//		_V_PROP_( NoDelay )

	public:
		_V_PROP_( IsStarted )
		_V_PROP_( RemoteStarted )
		_V_PROP_( DNSUpdated )

	protected:
		uint8_t	FAccessPointIndex = 255;
		unsigned int	FLastTime;

//	public:
//		void ( *AccessPoints_CallChain )( uint32_t AIndex, int AOption, bool &AIsEnabled ) = nullptr;
//		void ( *Sockets_CallChain )() = nullptr;

	public:
		inline void SetAutoReconnect( bool AValue )
		{
			WiFi.setAutoReconnect( AValue );
		}

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

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				StartEthernet();

		}

	public:
		bool GetIPFromHostName( Mitov::String AHostName, ::IPAddress &AAdress )
		{
			bool AResult = ( WiFi.hostByName( AHostName.c_str(), AAdress ) == 1 );
			if( ! AResult )
				AAdress = INADDR_NONE;

			return AResult;
		}

	public:
        void RestartEthernet()
		{
			if( ! Enabled() )
				return;

//			C_SERIAL.end();
//			Ethernet.end();
			StartEthernet();
		}

	public:
		bool GetIsStarted() 
		{ 
//			Serial.print( "IS_STARTED : " );
//			Serial.println( IsStarted().GetValue() );
//			Serial.println( uint32_t( this ));
			return IsStarted().GetValue(); 
		}

/*
		bool GetIsStarted1() 
		{ 

			Serial.print( "IS_STARTED1 : " );
			Serial.println( IsStarted().GetValue() );
			Serial.println( uint32_t( this ));
			return IsStarted().GetValue(); 
		}
*/
	protected:
		void StopEthernet()
		{
//			Serial.println( "StopEthernet" );
			T_Sockets_StopSocket::ChainCall();

//			for( int i = 0; i < C_SOCKETS_COUNT; ++i )
//				Sockets[ i ]->StopSocket();

			WiFi.disconnect( true );
			IsStarted() = false;
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
			if( HostName != "" )
			{
				WiFi.hostname( HostName );
//				espconn_mdns_set_hostname( (char *) HostName.c_str() ); 
			}

//			Serial.println( "TRY CONNECT1" );

			bool AConnected = false;
			for( int i = 0; i < C_ACESS_POINTS_COUNT; ++i )
				if( AccessPoints[ i ]->Connect() )
				{
					RemoteStarted = true;
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

					IsStarted = true;
					break;
				}

			if( ! AConnected )
				WiFi.begin();
*/
//			WiFi.begin();
//			WiFi.disconnect( true );

			if( HostName().GetValue() != "" )
			{
				WiFi.hostname( HostName() );
//				espconn_mdns_set_hostname( (char *) HostName.c_str() ); 
			}

//			RemoteConnectedOutputPin.SetValue( AConnected, true );

			WiFi.setAutoReconnect( AutoReconnect() );

			WiFi.mode( WIFI_OFF );
			if( Enabled() )
				AccessPoint().TryStart();

			if( T_BSSIDOutputPin::GetPinIsConnected() )
				T_BSSIDOutputPin::SetPinValue( WiFi.BSSIDstr().c_str() );

			if( T_GatewayIPOutputPin::GetPinIsConnected() )
				T_GatewayIPOutputPin::SetPinValue( IPAdressToString( WiFi.gatewayIP() ).c_str() );

			if( T_SubnetMaskIPOutputPin::GetPinIsConnected() )
				T_SubnetMaskIPOutputPin::SetPinValue( IPAdressToString( WiFi.subnetMask() ).c_str() );

//			for( int i = 0; i < C_SOCKETS_COUNT; ++i )
//				Sockets[ i ]->TryStartSocket();

//			WiFi.setNoDelay( NoDelay() );
		}

	public:
		inline void SystemStart()
		{
//			Serial.println( "SystemStart" );
			T_RemoteConnectedOutputPin::SetPinValue( WiFi.isConnected(), false );

			FLastTime = micros() + 10000000;
/*
			if( RemoteStarted )
			{
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
			}

			if( BSSIDOutputPin.IsConnected() )
				BSSIDOutputPin.SendValue( WiFi.BSSIDstr() );

			if( GatewayIPOutputPin.IsConnected() )
				GatewayIPOutputPin.SendValue( IPAdressToString( WiFi.gatewayIP() ));

			if( SubnetMaskIPOutputPin.IsConnected() )
				SubnetMaskIPOutputPin.SendValue( IPAdressToString( WiFi.subnetMask() ));
*/
		}

		inline void SystemLoopBegin() // Called only for RemoteConnectedOutputPin.IsConnected
		{
//			return;
			int ARes = WiFi.status();

//			Serial.print( "STATUS: " );
//			Serial.println( ARes );

			bool AIsEnabled;

			if( ! WiFi.isConnected() )
			{
//				Serial.println( "WiFi.isConnected()" );
//				Serial.println( "DISCONNECTED" );

//				IsStarted() = false;
				RemoteStarted() = false;
				DNSUpdated() = false;
			}

			AIsEnabled = false;
			if( C_ACESS_POINTS_COUNT > 0 )
			{
				AIsEnabled = false;
//				Serial.print( "STATUS: " );
//				Serial.println( RemoteStarted );

				if( ! RemoteStarted() )
				{
					unsigned long ACurrentMicros = micros();
					if( (( ACurrentMicros - FLastTime ) > 10000000 ) || ( ARes != WL_CONNECTED ) && ( ARes != WL_DISCONNECTED ) && ( ARes != WL_IDLE_STATUS ) )
					{
//			WiFi.printDiag( Serial );
//			Serial.print( "STATUS: " );
//				Serial.println( ARes );
						FLastTime = ACurrentMicros;
//						AccessPoints[ FAccessPointIndex ]->Connect( this );
						++ FAccessPointIndex;
						if( FAccessPointIndex >= C_ACESS_POINTS_COUNT )
							FAccessPointIndex = 0;

//							Serial.println( FAccessPointIndex );
						T_AccessPoints_TryConnect::ChainCall( FAccessPointIndex, AIsEnabled );
					}

					else
						T_AccessPoints_IsEnabled::ChainCall( FAccessPointIndex, AIsEnabled );

				}

			}

			T_RemoteConnectedOutputPin::SetPinValue( WiFi.isConnected(), true );

//			Serial.println( AIsEnabled );

			if( ! AIsEnabled )
			{
//				WiFi.begin();
//				Serial.println( "IsStarted" );
				IsStarted() = true;
			}

			else if( WiFi.isConnected() )
			{
				if( ! RemoteStarted() )
				{
					if( ! DNSUpdated() )
					{
//			WiFi.printDiag( Serial );
//						Serial.println( "TEST11" );
						DNSUpdated() = true;
/*
						if( AccessPoints_CallChain )
						{
							bool AIsEnabled = false;
							AccessPoints_CallChain( FAccessPointIndex, 2, AIsEnabled );
						}
*/
						return;
					}

					else
					{
						IsStarted() = true;
//						Serial.println( "RemoteStarted = true" );
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
//					WiFi.setNoDelay( NoDelay() );

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
			}
//			for( int i = 0; i < C_SOCKETS_COUNT; ++i )
//				Sockets[ i ]->TryStartSocket();

//			WiFi.setHostname( (char *)HostName.c_str() );
//			Serial.print( "IS_STARTED : " );
//			Serial.println( IsStarted().GetValue() );
		}

/*
	void DoCheckSignalStrength( void *_Data )
		{
			if( IsStarted )
				if( SignalStrengthOutputPin.IsConnected() )
				{
					int32_t ASetrength = WiFi.RSSI();
					SignalStrengthOutputPin.Notify( &ASetrength );
				}

		}
*/
	public:
		inline ESP8266WiFiModule()
		{			
			IsStarted() = false;
			RemoteStarted() = false;
			DNSUpdated() = false;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_SignalStrengthOutputPin
	> class ESP8266ModuleSignalStrengthOperation :
		public T_SignalStrengthOutputPin
	{
	public:
		_V_PIN_( SignalStrengthOutputPin )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( C_OWNER.IsStarted() )
				if( T_SignalStrengthOutputPin::GetPinIsConnected() )
				{
					int32_t ASetrength = WiFi.RSSI();
					T_SignalStrengthOutputPin::SetPinValue( ASetrength );
				}

		}

	};
//---------------------------------------------------------------------------
	class ESP8266ModuleReconnectOperation
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
