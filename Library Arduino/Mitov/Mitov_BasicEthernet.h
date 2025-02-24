////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_BASIC_ETHERNET_h
#define _MITOV_BASIC_ETHERNET_h

#include <Mitov.h>
#include <IPAddress.h>
//#include <Ethernet.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	inline Mitov::String MACAdressToString( uint8_t *AMACAddress )
	{
		char AMACString[ 6 * 3 + 1 ];
		sprintf( AMACString, "%02X-%02X-%02X-%02X-%02X-%02X", AMACString[ 0 ], AMACString[ 1 ], AMACString[ 2 ], AMACString[ 3 ], AMACString[ 4 ], AMACString[ 5 ] );
		return AMACString;
//		MACOutputPin.Notify( AMACString );
	}
//---------------------------------------------------------------------------
	inline Mitov::String IPAdressToString( ::IPAddress AIPAddress )
	{
		char AIPString[ 4 * 4 + 1 ];
		sprintf( AIPString, "%u.%u.%u.%u", AIPAddress[ 0 ], AIPAddress[ 1 ], AIPAddress[ 2 ], AIPAddress[ 3 ] );
		return AIPString;
	}
//---------------------------------------------------------------------------
/*
	class BasicShieldIPAddress
	{
	public:
		bool		Enabled = false;
		::IPAddress	IP;

	};
//---------------------------------------------------------------------------
	class ShieldGatewayAddress : public BasicShieldIPAddress
	{
	public:
		BasicShieldIPAddress	Subnet;
	};
//---------------------------------------------------------------------------
	class ShieldDNSAddress : public BasicShieldIPAddress
	{
	public:
		ShieldGatewayAddress	Gateway;
	};
//---------------------------------------------------------------------------
	class ShieldIPAddress : public BasicShieldIPAddress
	{
	public:
		ShieldDNSAddress	DNS;
	};
//---------------------------------------------------------------------------
	class ShieldIPDNS2Address : public ShieldIPAddress
	{
	public:
		BasicShieldIPAddress	DNS2;
	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_Octet1,
		typename T_Octet2,
		typename T_Octet3,
		typename T_Octet4
	> class TIPAddress :
		public T_Octet1,
		public T_Octet2,
		public T_Octet3,
		public T_Octet4
	{
	public:
		_V_PROP_( Octet1 )
		_V_PROP_( Octet2 )
		_V_PROP_( Octet3 )
		_V_PROP_( Octet4 )

	public:
		void operator = ( uint32_t AValue )
		{
			Octet4() = AValue;
			Octet3() = AValue >> 8;
			Octet2() = AValue >> 16;
			Octet1() = AValue >> 24;
		}

		inline uint32_t GetUInt32Value()
		{
			return ( uint32_t( Octet1() ) << 24 ) | ( uint32_t( Octet2() ) << 16 ) | ( uint32_t( Octet3() ) << 8 ) | Octet4();
		}

/*
		bool operator == ( uint32_t AValue )
		{
			if( Octet4 != AValue & 0xFF )
				return false;

			if( Octet3 != ( AValue >> 8 ) & 0xFF )
				return false;

			if( Octet2 != ( AValue >> 16 ) & 0xFF )
				return false;

			if( Octet1 != ( AValue >> 24 ) & 0xFF )
				return false;

			return true;
		}
*/
	public:
		inline ::IPAddress GetIPAddress()
		{
			return ::IPAddress( Octet1(), Octet2(), Octet3(), Octet4() );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IP
	> class TArduinoBasicIPV4Address :
		public T_Enabled,
		public T_IP
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( IP )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IP,
		typename T_Subnet
	> class TArduinoIPV4GatewayDNSAddress :
		public T_Enabled,
		public T_IP,
		public T_Subnet
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( IP )
		_V_PROP_( Subnet )
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Gateway,
		typename T_IP
	> class TArduinoIPV4DNSAddress :
		public T_Enabled,
		public T_Gateway,
		public T_IP
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Gateway )
		_V_PROP_( IP )
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_IP
	> class TArduinoSubnetIPV4Address :
		public T_Enabled,
		public T_IP
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( IP )
	};
//---------------------------------------------------------------------------
	template <
		typename T_DNS,
		typename T_Enabled,
		typename T_IP
	> class TArduinoIPV4DNS1Address :
		public T_DNS,
		public T_Enabled,
		public T_IP
	{
	public:
		_V_PROP_( DNS )
		_V_PROP_( Enabled )
		_V_PROP_( IP )
	};
//---------------------------------------------------------------------------
	template <
		typename T_DNS,
		typename T_DNS2,
		typename T_Enabled,
		typename T_IP
	> class TArduinoIPV4DNS2Address :
		public T_DNS,
		public T_DNS2,
		public T_Enabled,
		public T_IP
	{
	public:
		_V_PROP_( DNS )
		_V_PROP_( DNS2 )
		_V_PROP_( Enabled )
		_V_PROP_( IP )
	};
//---------------------------------------------------------------------------
/*
	class BasicSocket
	{
//	public:
//		OpenWire::SourcePin	OutputPin;

	public:
		unsigned int	Port = 80;
		bool			Enabled : 1;

	protected:
		bool	FRunning : 1; // = false; // In base class to save memory
		bool	FConnecting : 1; // = false; // In base class to save memory
		bool	FIsConnected : 1; // = false; // In base class to save memory

	protected:
		virtual void StartSocket() = 0;

	public:
		virtual bool IsEnabled()
		{
			return Enabled;
		}

		virtual bool CanSend()
		{
			return Enabled;
		}

		inline void SetRemotePort( uint32_t APort )
		{
		}

		virtual void BeginPacket()
		{
		}

		virtual void EndPacket()
		{
		}

		virtual void StopSocket() = 0;

		inline void SystemLoopBegin() 
		{
			StartSocket();
		}

	public:
		inline void TryStartSocket() // For direct socket access components
		{
			if( Enabled )
				StartSocket();
		}

		virtual void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				TryStartSocket();

		}

	public:
		BasicSocket() :
			Enabled( true ),
			FRunning( false ),
			FConnecting( false ),
			FIsConnected( false )
		{
		}

	};
//---------------------------------------------------------------------------
	template<typename T_OWNER> class BasicEthernetSocket : public Mitov::BasicSocket
	{
		typedef Mitov::BasicSocket inherited;

	public:
		virtual Print *GetPrint() = 0;

//	public:
//		BasicEthernetSocket( T_OWNER &AOwner, uint32_t AIndex )
//		{
//			AOwner.Sockets[ AIndex ] = this;
//		}

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_CLIENT, 
		typename T_ConnectedOutputPin,
		typename T_Enabled,
//		typename T_FConnecting,
		typename T_FIsConnected,
//		typename T_FResolved,
//		typename T_FRunning,
		typename T_Host,
		typename T_IPAddress,
		typename T_OutputPin,
		typename T_Port
	> class TCPClientSocket : 
		public T_ConnectedOutputPin,
		public T_Enabled,
//		public T_FConnecting,
		public T_FIsConnected,
//		public T_FResolved,
//		public T_FRunning,
		public T_Host,
		public T_IPAddress,
		public T_OutputPin,
		public T_Port
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectedOutputPin )

	public:
		_V_PROP_( Port )
		_V_PROP_( Enabled )

	protected:
//		_V_PROP_( FRunning )
//		_V_PROP_( FConnecting )
		_V_PROP_( FIsConnected )
//		_V_PROP_( FResolved )

	public:
		_V_PROP_( Host )
		_V_PROP_( IPAddress )

	protected:
		T_CLIENT	FClient;

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

//			Serial.println( "SetEnabled" );
            Enabled() = AValue;
			if( IsEnabled() )
				StartSocket();

			else
				StopSocket();

		}

	public:
		inline void SetRemotePort( uint32_t APort )
		{
		}

		inline void BeginPacket()
		{
		}

		inline void EndPacket()
		{
		}

	public:
		inline void TryStartSocket() // For direct socket access components
		{
			if( Enabled() )
				StartSocket();
		}

		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				TryStartSocket();

		}

/*
	public:
		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				if( Enabled )
					StartSocket();

		}
*/
	public:
		inline void FlushInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
		}

	public:
		inline bool IsEnabled()
		{
			return Enabled() && C_OWNER.Enabled();
		}

		inline bool CanSend()
		{
			return Enabled() && C_OWNER.Enabled() && FIsConnected() && C_OWNER.GetIsStarted();
		}

		inline Print *GetPrint()
		{
			return &FClient;
		}

	public:
		inline void SystemInit()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
		}

	public:
		void DisconnectInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
			Enabled() = false;
//			Serial.println( "DoDisconnect" );
//			Serial.println( FClient.connected() );
		}

	public:
		void StartSocket()
		{
			if( ! C_OWNER.GetIsStarted() )
				return;

//			Serial.println( "StartSocket1" );
			if( FIsConnected() )
				return;

//			Serial.println( "StartSocket" );
			if( Host().GetValue().length() )
				FIsConnected() = FClient.connect( Host().c_str(), Port() );

			else
			{
//				IPAddress.printTo( Serial );
				FIsConnected() = FClient.connect( IPAddress().GetIPAddress(), Port() );
			}
		}

		void StopSocket()
		{
//			Serial.println( "StopSocket" );
			if( ! FIsConnected() )
				return;

			FIsConnected() = false;

			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
//			inherited::Enabled = false;
		}

	public:
		inline void SystemLoopBegin() 
		{
			StartSocket();
			if ( FClient.available() )
			{
				unsigned char AByte = FClient.read();
				T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
//				OutputPin.SendValue( Mitov::TDataBlock( 1, &AByte ));
			}

			bool AIsConnected = FClient.connected();

			T_ConnectedOutputPin::SetPinValue( AIsConnected, true );

			if ( ! AIsConnected ) 
			{
				FClient.stop(); // Do we need this?
				T_ConnectedOutputPin::SetPinValue( false, true );

				if( FIsConnected() )
				{
					if( C_OWNER.Enabled() )
						Enabled() = false;

					FIsConnected() = false;
				}

			}
		}

	public:
		TCPClientSocket()
		{
//			FRunning() = false;
//			FConnecting() = false;
			FIsConnected() = false;
//			FResolved() = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CLIENT,
		typename T_Certificate,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
		typename T_FConnecting,
		typename T_FIsConnected,
		typename T_FRunning,
		typename T_Host,
		typename T_IPAddress,
		typename T_OutputPin,
		typename T_Port,
		typename T_PrivateKey
	> class TCPSecureClientSocket :
		public T_Certificate,
		public T_ConnectedOutputPin,
		public T_Enabled,
		public T_FConnecting,
		public T_FIsConnected,
		public T_FRunning,
		public T_Host,
		public T_IPAddress,
		public T_OutputPin,
		public T_Port,
		public T_PrivateKey
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectedOutputPin )

	public:
		_V_PROP_( Port )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FRunning )
		_V_PROP_( FConnecting )
		_V_PROP_( FIsConnected )
//		bool			FResolved : 1;

	public:
		_V_PROP_( Host )
		_V_PROP_( IPAddress )

	public:
		_V_PROP_( Certificate )
		_V_PROP_( PrivateKey )

	protected:
		T_CLIENT	FClient;

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

//			Serial.println( "SetEnabled" );
            Enabled() = AValue;
			if( IsEnabled() )
				StartSocket();

			else
				StopSocket();

		}

	public:
		inline void SetRemotePort( uint32_t APort )
		{
		}

		inline void BeginPacket()
		{
		}

		inline void EndPacket()
		{
		}

/*
	public:
		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				if( Enabled )
					StartSocket();

		}
*/
	public:
		inline void FlushInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
		}

	public:
		inline bool IsEnabled()
		{
			return Enabled() && C_OWNER.Enabled();
		}

		inline bool CanSend()
		{
			return Enabled() && C_OWNER.Enabled() && FIsConnected() && C_OWNER.GetIsStarted();
		}

		inline Print *GetPrint()
		{
			return &FClient;
		}

	public:
		inline void SystemInit()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
		}

	public:
		void DisconnectInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
			Enabled() = false;

//			Serial.println( "DoDisconnect" );
//			Serial.println( FClient.connected() );
		}

	public:
		void StopSocket()
		{
			if( ! FIsConnected() )
				return;

			FIsConnected() = false;

			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
//			inherited::Enabled = false;
		}

	public:
		inline void SystemLoopBegin() 
		{
			StartSocket();
			if ( FClient.available() )
			{
				unsigned char AByte = FClient.read();
				T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
//				OutputPin.SendValue( Mitov::TDataBlock( 1, &AByte ));
//				OutputPin.Notify( &AByte );
			}

			bool AIsConnected = FClient.connected();

			T_ConnectedOutputPin::SetPinValue( AIsConnected, true );

			if ( ! AIsConnected ) 
			{
				FClient.stop(); // Do we need this?
				T_ConnectedOutputPin::SetPinValue( false, true );

				if( FIsConnected() )
				{
					if( C_OWNER.Enabled() )
						Enabled() = false;

					FIsConnected() = false;
				}
			}
		}

	protected:
		inline void StartSocket()
		{
			if( ! C_OWNER.GetIsStarted() )
				return;

			if( FIsConnected() )
				return;

			if( Certificate().GetValue() != "" )
			{
#if defined( VISUINO_ESP8266 )
				FIsConnected() = FClient.setCertificate( (uint8_t*)Certificate().c_str(), Certificate().length() );
#else
				FIsConnected() = FClient.setCertificate( Certificate().c_str() );
#endif
				return;
			}

			if( PrivateKey != "" )
			{
#if defined( VISUINO_ESP8266 )
				FIsConnected() = FClient.setPrivateKey( (uint8_t*)PrivateKey().c_str(), PrivateKey().length() );
#else
				FIsConnected() = FClient.setPrivateKey( PrivateKey().c_str() );
#endif
				return;
			}

//			Serial.println( "StartSocket1" );
			if( FIsConnected() )
				return;

//			Serial.println( "StartSocket" );
			if( Host().length() )
				FIsConnected() = FClient.connect( Host().c_str(), Port() );

			else
			{
//				IPAddress.printTo( Serial );
				FIsConnected() = FClient.connect( IPAddress().GetIPAddress(), Port() );
			}
		}

	public:
		inline TCPSecureClientSocket()
		{
			FRunning() = false;
			FConnecting() = false;
			FIsConnected() = false;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_CACert,
		typename T_CLIENT,
		typename T_Certificate,
		typename T_ConnectedOutputPin,
		typename T_Enabled,
		typename T_FConnecting,
		typename T_FIsConnected,
		typename T_FRunning,
		typename T_Host,
		typename T_IPAddress,
		typename T_OutputPin,
		typename T_Port,
		typename T_PrivateKey
	> class TCPCACertSecureClientSocket : 
		public T_CACert,
		public T_Certificate,
		public T_ConnectedOutputPin,
		public T_Enabled,
		public T_FConnecting,
		public T_FIsConnected,
		public T_FRunning,
		public T_Host,
		public T_IPAddress,
		public T_OutputPin,
		public T_Port,
		public T_PrivateKey
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectedOutputPin )

	public:
		_V_PROP_( Port )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FRunning )
		_V_PROP_( FConnecting )
		_V_PROP_( FIsConnected )
//		bool			FResolved : 1;

	public:
		_V_PROP_( Host )
		_V_PROP_( IPAddress )

	public:
		_V_PROP_( Certificate )
		_V_PROP_( PrivateKey )
		_V_PROP_( CACert )

	protected:
		T_CLIENT	FClient;

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

//			Serial.println( "SetEnabled" );
            Enabled() = AValue;
			if( IsEnabled() )
				StartSocket();

			else
				StopSocket();

		}

	public:
		inline void SetRemotePort( uint32_t APort )
		{
		}

		inline void BeginPacket()
		{
		}

		inline void EndPacket()
		{
		}

/*
	public:
		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				if( Enabled )
					StartSocket();

		}
*/
	public:
		inline void FlushInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
		}

	public:
		inline bool IsEnabled()
		{
			return Enabled() && C_OWNER.Enabled();
		}

		inline bool CanSend()
		{
			return Enabled() && C_OWNER.Enabled() && FIsConnected() && C_OWNER.GetIsStarted();
		}

		inline Print *GetPrint()
		{
			return &FClient;
		}

	public:
		inline void SystemInit()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
		}

	public:
		void DisconnectInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
			Enabled() = false;

//			Serial.println( "DoDisconnect" );
//			Serial.println( FClient.connected() );
		}

	public:
		void StopSocket()
		{
			if( ! FIsConnected() )
				return;

			FIsConnected() = false;

			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
//			inherited::Enabled = false;
		}

	public:
		inline void SystemLoopBegin() 
		{
			StartSocket();
			if ( FClient.available() )
			{
				unsigned char AByte = FClient.read();
				T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
//				OutputPin.SendValue( Mitov::TDataBlock( 1, &AByte ));
//				OutputPin.Notify( &AByte );
			}

			bool AIsConnected = FClient.connected();

			T_ConnectedOutputPin::SetPinValue( AIsConnected, true );

			if ( ! AIsConnected ) 
			{
				FClient.stop(); // Do we need this?
//				T_ConnectedOutputPin::SetPinValue( false, true );

				if( FIsConnected() )
				{
//					Serial.println( "DISCONNECT" );

					if( C_OWNER.Enabled() )
						Enabled() = false;

					FIsConnected() = false;
				}
			}
		}

	protected:
		inline void StartSocket()
		{
//			Serial.println( "StartSocket 1" );
			if( ! C_OWNER.GetIsStarted() )
				return;

//			Serial.println( "StartSocket 2" );
			if( FIsConnected() )
				return;

			if( CACert().GetValue() != "" )
#if defined( VISUINO_ESP8266 )
				FClient.setCACert( (uint8_t *)CACert().c_str(), CACert().GetValue().length() );
#else
				FClient.setCACert( (char *)CACert().c_str() );
#endif

			if( Certificate().GetValue() != "" )
			{
#if defined( VISUINO_ESP8266 )
				FClient.setCertificate( (uint8_t*)Certificate().c_str(), Certificate().GetValue().length() );
#else
//				FIsConnected() = FClient.setCertificate( Certificate().c_str() );
				FClient.setCertificate( Certificate().c_str() );
#endif
//				return;
			}

			if( PrivateKey().GetValue() != "" )
			{
#if defined( VISUINO_ESP8266 )
				FClient.setPrivateKey( (uint8_t*)Certificate().c_str(), Certificate().GetValue().length() );
#else
//				FIsConnected() = FClient.setPrivateKey( PrivateKey().c_str() );
				FClient.setPrivateKey( PrivateKey().c_str() );
#endif
//				return;
			}

//			Serial.println( "StartSocket1" );
//			if( FIsConnected() )
//				return;

//			Serial.println( "StartSocket" );
			if( Host().GetValue().length() )
				FIsConnected() = FClient.connect( Host().c_str(), Port() );

			else
			{
//				IPAddress.printTo( Serial );
				FIsConnected() = FClient.connect( IPAddress().GetIPAddress(), Port() );
			}
		}

	public:
		inline TCPCACertSecureClientSocket()
		{
			FRunning() = false;
			FConnecting() = false;
			FIsConnected() = false;
		}


	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_CLIENT, 
		typename T_ConnectedOutputPin,
		typename T_Enabled,
//		typename T_FConnecting,
		typename T_FIsConnected,
//		typename T_FResolved,
//		typename T_FRunning,
#if defined( ESP8266 ) || defined( ESP32 )
		typename T_NoDelay,
#endif
		typename T_OutputPin,
		typename T_Port,
		typename T_SERVER
	> class TCPServerSocket : 
		public T_ConnectedOutputPin,
		public T_Enabled,
//		public T_FConnecting,
		public T_FIsConnected,
//		public T_FResolved,
//		public T_FRunning,
#if defined( ESP8266 ) || defined( ESP32 )
		public T_NoDelay,
#endif
		public T_OutputPin,
		public T_Port
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( ConnectedOutputPin )

	public:
		_V_PROP_( Port )
		_V_PROP_( Enabled )
#if defined( ESP8266 ) || defined( ESP32 )
		_V_PROP_( NoDelay )
#endif

	protected:
//		_V_PROP_( FRunning )
//		_V_PROP_( FConnecting )
		_V_PROP_( FIsConnected )
//		_V_PROP_( FResolved )

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

//			Serial.println( "SetEnabled" );
            Enabled() = AValue;
			if( IsEnabled() )
				StartSocket();

			else
				StopSocket();

		}

	public:
		inline void SetRemotePort( uint32_t APort )
		{
		}

		inline void BeginPacket()
		{
		}

		inline void EndPacket()
		{
		}

	public:
		inline void UpdateNoDelay()
		{
#if defined( ESP8266 ) || defined( ESP32 )
			FServer->setNoDelay( NoDelay().GetValue() );
#endif
		}

	public:
		inline void TryStartSocket() // For direct socket access components
		{
			if( Enabled() )
				StartSocket();
		}

		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				TryStartSocket();

		}

	protected:
		T_CLIENT	FClient;
		T_SERVER	*FServer = nullptr;

	public:
		void DisconnectInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
			FClient.stop();
			T_ConnectedOutputPin::SetPinValue( false, true );
//			inherited::Enabled = false;
//			Serial.println( "DoDisconnect" );
//			Serial.println( FClient.connected() );
		}

		inline void FlushInputPin_o_Receive( void *_Data )
		{
			FClient.flush();
		}

	public:
		inline bool IsEnabled()
		{
			return Enabled() && C_OWNER.Enabled();
		}

	public:
		inline void SystemInit()
		{
			T_ConnectedOutputPin::SetPinValue( false, false );
		}

	protected:
		void StartSocket()
		{
//			Serial.println( "StartSockect 1" );
			if( ! C_OWNER.GetIsStarted() )
				return;

//			Serial.println( "StartSockect 2" );
			if( FIsConnected() )
				return;

//			Serial.println( "StartSockect" );
			if( FServer )
			{
				FIsConnected() = true;
				return;
			}

//			Serial.println( Port );
			FServer = new T_SERVER( Port() );
			FServer->begin();
			UpdateNoDelay();
			FIsConnected() = true;
//			Serial.println( "Start Server Sockect" );
//			Serial.println( inherited::Port );
		}

	public:
		inline void SystemLoopBegin() 
		{
			StartSocket();
			if( FServer )
			{
				if( ! FClient )
				{
//					Serial.println( inherited::Port );
//					Serial.println( "TRY CLIENT" );
					FClient = FServer->available();

//					if( inherited::FClient )
//						Serial.println( "CLIENT" );

//					Serial.println( "TRY CLIENT OUT" );
				}

/*
				if( inherited::FClient )
 					if (! inherited::FClient.connected()) 
					{
						Serial.println( "STOP" );
						inherited::FClient.stop(); // Do we need this?
					}
*/
				if( FClient )
				{
//					Serial.println( "CLIENT" );
					if( FClient.available() )
					{
						unsigned char AByte = FClient.read();
//				Serial.println( "RECEIVED" );
						T_OutputPin::SendPinValue( Mitov::TDataBlock( 1, &AByte ));
//						OutputPin.SendValue( Mitov::TDataBlock( 1, &AByte ));
					}

 					if( ! FClient.connected() ) 
					{
//						Serial.println( "STOP" );
						FClient.stop(); // Do we need this?
						T_ConnectedOutputPin::SetPinValue( false, true );
					}
				}
			}

//			Serial.println( inherited::FClient.connected() );

			bool AIsConnected = FClient.connected();

			T_ConnectedOutputPin::SetPinValue( AIsConnected, true );

			if( ! AIsConnected )
				FIsConnected() = false;

//			if( ! AIsConnected )
//				if( C_OWNER.Enabled )
//					inherited::Enabled = false;

//			Serial.println( "SYSTEM_LOOP" );
//			Serial.println( "SYSTEM_LOOP_OUT" );
		}

	public:
		inline bool IsConnected() { return FClient; }

		inline bool CanSend()
		{
/*
			Serial.print( Enabled() ); Serial.print( " " );
			Serial.print( C_OWNER.Enabled() ); Serial.print( " " );
			Serial.print( FIsConnected().GetValue() ); Serial.print( " " );
			Serial.println( C_OWNER.GetIsStarted() );

			Serial.print( "IS_STARTED : " );
			Serial.println( C_OWNER.IsStarted().GetValue() );
*/
//			return inherited::Enabled && C_OWNER.Enabled && inherited::FClient;
			return Enabled() && C_OWNER.Enabled() && FIsConnected() && C_OWNER.GetIsStarted();
		}

		inline void StopSocket()
		{
			FIsConnected() = false;

			if( FServer )
			{
				delete FServer;
				FServer = nullptr;
			}
		}

	public:
		inline Print *GetPrint()
		{
//			Serial.println( "GetPrint" );
			if( FClient )
				return &FClient;

			return FServer;
		}

/*
		void EndPacket()
		{
			Serial.println( "EndPacket" );
//			inherited::FClient.flush();
//			delay(1000);
//			inherited::FClient.stop();
		}
*/
	public:
		inline TCPServerSocket()
		{
//			FRunning() = false;
//			FConnecting() = false;
			FIsConnected() = false;
//			FResolved() = false;
		}


/*
		virtual ~TCPServerSocket()
		{
			if( FServer )
				delete FServer;
		}
*/
	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_UDP, 
		typename T_Enabled,
		typename T_FIsConnected,
//		typename T_FResolved,
		typename T_OutputPin,
		typename T_Port,
		typename T_RemoteHost,
		typename T_RemoteIPAddress,
		typename T_RemoteIPOutputPin,
		typename T_RemotePort,
		typename T_RemotePortOutputPin
	> class UDPSocket : 
		public T_Enabled,
		public T_FIsConnected,
//		public T_FResolved,
		public T_OutputPin,
		public T_Port,
		public T_RemoteHost,
		public T_RemoteIPAddress,
		public T_RemoteIPOutputPin,
		public T_RemotePort,
		public T_RemotePortOutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( RemoteIPOutputPin )
		_V_PIN_( RemotePortOutputPin )

	public:
		_V_PROP_( RemotePort )
		_V_PROP_( Port )
		_V_PROP_( RemoteHost )
		_V_PROP_( RemoteIPAddress )
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FIsConnected )
//		_V_PROP_( FResolved )

	protected:
		T_UDP FSocket;
//		Mitov::String FHostName;

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled() == AValue )
                return;

//			Serial.println( "SetEnabled" );
            Enabled() = AValue;
			if( IsEnabled() )
				StartSocket();

			else
				StopSocket();

		}

	public:
		inline void TryStartSocket() // For direct socket access components
		{
			if( Enabled() )
				StartSocket();
		}

		void TryStartSocket( uint32_t AIndex, uint32_t ACurrentIndex )
		{
//			Serial.println( Enabled );
			if( AIndex == ACurrentIndex )
				TryStartSocket();
//				if( Enabled )
//					StartSocket();

		}

	public:
		inline bool IsEnabled()
		{
			return Enabled() && C_OWNER.Enabled();
		}

		inline bool CanSend()
		{
			return Enabled() && C_OWNER.Enabled() && FIsConnected() && C_OWNER.GetIsStarted();
		}

	protected:
		void StartSocket()
		{
			if( FIsConnected() )
				return;

			if( ! C_OWNER.GetIsStarted() )
				return;

//			Serial.println( "TEST11111" );
/*
			if( FHostName != "" )
			{
//				Serial.println( "TEST1" );
				FResolved = C_OWNER.GetIPFromHostName( FHostName, RemoteIPAddress );
			}
*/
//			Serial.print( "StartSocket: " );
//			Serial.println( inherited::Port );
			FIsConnected() = FSocket.begin( Port() );
		}

	public:
		inline void SetRemotePort( uint32_t APort )
		{
			RemotePort() = APort;
		}

		inline void BeginPacket()
		{
			if( ! FIsConnected() )
				return;

//			Serial.print( "BeginPacket: " );
//			Serial.print( RemoteHost );
//			Serial.print( RemoteIPAddress );
//			Serial.print( " " );
//			Serial.println( RemotePort );
			if( RemoteHost().GetValue().length() )
				FSocket.beginPacket( RemoteHost().c_str(), RemotePort() );

			else
				FSocket.beginPacket( RemoteIPAddress().GetIPAddress(), RemotePort() );

////  FSocket.beginPacket( RemoteIPAddress, 8888 );
//  FSocket.println( "Hello1" );
////  FSocket.endPacket();
		}

		inline void EndPacket()
		{
			if( ! FIsConnected() )
				return;

//			Serial.println( "EndPacket" );
//			FSocket.println( "Hello" );
			FSocket.endPacket();
//			delay( 1000 );
		}

		inline void StopSocket()
		{
			FIsConnected() = false;

//			Serial.println( "StopSocket" );
			FSocket.stop();
		}

		inline Print *GetPrint()
		{
//  FSocket.println( "Hello2" );
//			return &Serial;
//			Serial.println( "GetPrint" );
			return &FSocket;
		}

	public:
		inline void SystemLoopBegin() 
		{
			StartSocket();
			int APacketSize = FSocket.parsePacket();
			if( APacketSize )
			{
//				Serial.println( APacketSize );
				byte *Adata = new byte[ APacketSize ];
				FSocket.read( Adata, APacketSize );

				T_OutputPin::SendPinValue( Mitov::TDataBlock( APacketSize, Adata ));
//				OutputPin.SendValue( Mitov::TDataBlock( APacketSize, Adata ));
				T_RemoteIPOutputPin::SetPinValue( IPAdressToString( FSocket.remoteIP() ));
//				RemoteIPOutputPin.SendValue( IPAdressToString( FSocket.remoteIP() ));
				T_RemotePortOutputPin::SetPinValue( FSocket.remotePort() );
//				RemotePortOutputPin.SendValue( FSocket.remotePort() );

				delete []Adata;
			}
/*
			if ( FSocket.available() )
			{
				unsigned char AByte = FSocket.read();
				inherited::OutputPin.Notify( &AByte );
			}
*/
		}

	public:
		inline UDPSocket()
		{
			FIsConnected() = false;
//			FResolved() = true;
		}
/*
		UDPSocket( ::IPAddress ARemoteIPAddress ) :
			Enabled( true ),
			FIsConnected( false ),
			FResolved( true ),
			RemoteIPAddress( ARemoteIPAddress )
			
		{
		}

		UDPSocket( Mitov::String AHostName ) :
			Enabled( true ),
			FIsConnected( false ),
			FResolved( false ),
			FHostName( AHostName )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	template<typename T_ROOT, T_ROOT &C_ROOT, typename T> class EthernetSocketInput
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
//			Serial.print( "DoReceive" );
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();

//				Serial.print( "PRINT: " );
//				Serial.println( *(T*)_Data );

				C_ROOT.GetPrint()->println( *(T*)_Data );
				C_ROOT.EndPacket();
			}
		}

	};
//---------------------------------------------------------------------------
	template<typename T_ROOT, T_ROOT &C_ROOT> class EthernetSocketBinaryInput
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
//			Serial.println( "InputPin_o_Receive" );
			if( C_ROOT.CanSend() )
			{
//				Serial.println( "TEST11" );
				C_ROOT.BeginPacket();
				Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;

//				Serial.println( ADataBlock.Size );
//				Serial.print( ADataBlock.ToString() );

				C_ROOT.GetPrint()->write((uint8_t *) ADataBlock.Data, ADataBlock.Size );
				C_ROOT.EndPacket();
			}
		}
	};
//---------------------------------------------------------------------------
	template<typename T_ROOT, T_ROOT &C_ROOT> class EthernetSocketBufferInput
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();
				Mitov::TArray<float> ADataBlock = *(Mitov::TArray<float>*)_Data;

//				Serial.println( ADataBlock.Size );
				C_ROOT.GetPrint()->write((uint8_t *) ADataBlock.Data, ADataBlock.Size );
//				C_ROOT.GetPrint()->write( *(unsigned char*)_Data );
				C_ROOT.EndPacket();
			}
		}
	};
//---------------------------------------------------------------------------
	template<typename T_ROOT, T_ROOT &C_ROOT> class EthernetSocketStringInput
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();
				C_ROOT.GetPrint()->println( (char*)_Data );
				C_ROOT.EndPacket();
			}
		}
	};
//---------------------------------------------------------------------------
	template<typename T_ROOT, T_ROOT &C_ROOT> class EthernetSocketClockInput
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();
				C_ROOT.GetPrint()->println( "(Clock)" );
				C_ROOT.EndPacket();
			}
		}
	};
//---------------------------------------------------------------------------
	template<typename T_ROOT, T_ROOT &C_ROOT, typename T_OBJECT> class EthernetSocketObjectInput
	{
	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( C_ROOT.CanSend() )
			{
				C_ROOT.BeginPacket();
				Print *APrint = C_ROOT.GetPrint();
				APrint->println( ((T_OBJECT *)_Data)->ToString().c_str() );
				C_ROOT.EndPacket();
			}
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Channel,
		typename T_Enabled
	> class ESPWiFiModuleOptionalChannel :
		public T_Channel,
		public T_Enabled
	{
	public:
		_V_PROP_( Channel )
		_V_PROP_( Enabled )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Gateway,
		typename T_IP,
		typename T_Subnet
	> class ESPWiFiModuleAccessPointConfig :
		public T_Enabled,
		public T_Gateway,
		public T_IP,
		public T_Subnet
	{
	public:
		_V_PROP_( Enabled )

		_V_PROP_( IP )
		_V_PROP_( Gateway )
		_V_PROP_( Subnet )

	};
//---------------------------------------------------------------------------
	template <
		typename T_DNS1,
		typename T_DNS2,
		typename T_Enabled,
		typename T_Gateway,
		typename T_IP,
		typename T_Subnet
	> class ESPWiFiModuleRemoteConfig  :
		public T_DNS1,
		public T_DNS2,
		public T_Enabled,
		public T_Gateway,
		public T_IP,
		public T_Subnet
	{
	public:
		_V_PROP_( Enabled )

		_V_PROP_( IP )
		_V_PROP_( Gateway )
		_V_PROP_( Subnet )

		 _V_PROP_( DNS1 )
		 _V_PROP_( DNS2 )
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_MacAddress
	> class ESPWiFiModuleOptionalMacAddress :
		public T_Enabled,
		public T_MacAddress
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( MacAddress )

	};
//---------------------------------------------------------------------------
}	

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
