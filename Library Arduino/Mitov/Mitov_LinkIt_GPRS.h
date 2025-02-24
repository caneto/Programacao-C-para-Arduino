////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_LINKIT_GPRS_h
#define _MITOV_LINKIT_GPRS_h

#include <Mitov.h>
#include <Mitov_BasicEthernet.h>
#include <LGPRS.h>
#include <LGPRSClient.h>
#include <LGPRSServer.h>
#include <LGPRSUdp.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_AccessPoints_ResetRetry,
		typename T_AccessPoints_TryConnect,
		uint32_t COUNT_Sockets,
		typename T_Enabled,
		typename T_FConnected,
		typename T_Sockets_StartSocket,
		typename T_Sockets_StopSocket
	> class LinkItGPRSModule :
//		public T_AccessPoints_ResetRetry,
//		public T_AccessPoints_TryConnect,
		public T_Enabled,
		public T_FConnected,
		public T_Sockets_StartSocket,
		public T_Sockets_StopSocket
	{
//	public:
//		Mitov::SimpleList<BasicEthernetSocket<LinkItGPRSModule>*>	Sockets;

	public:
//		Mitov::SimpleObjectList<GPRSAccessPoint *>	AccessPoints;

		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FConnected )

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled().GetValue() == AValue )
                return;

            Enabled() = AValue;
			if( Enabled() )
				StartEthernet();

			else
				StopEthernet();

		}

	public:
		inline bool GetIsStarted() { return FConnected(); }

	public:
		virtual bool GetIPFromHostName( Mitov::String AHostName, ::IPAddress &AAdress )
		{
			bool AResult = ( LGPRS.hostByName( AHostName.c_str(), AAdress ) == 1 );
			if( ! AResult )
				AAdress = INADDR_NONE;

			return AResult;
		}

	protected:
		void StopEthernet()
		{
			T_Sockets_StopSocket::ChainCall();
//			for( int i = 0; i < Sockets.size(); ++i )
//				Sockets[ i ]->StopSocket();

//			LWiFi.end();
		}

		void StartEthernet()
		{
			TryConnect( true );
			if( FConnected() )
				T_Sockets_StartSocket::ChainCall();
//				for( int i = 0; i < Sockets.size(); ++i )
//					Sockets[ i ]->TryStartSocket();
		}

		void TryConnect( bool FromStart )
		{
			bool AProccessed = false;
			bool ATryNextConnection = false;
			T_AccessPoints_TryConnect::ChainCall( FromStart, ATryNextConnection, AProccessed );
/*
			for( int i = 0; i < AccessPoints.size(); i ++ )
				if( AccessPoints[ i ]->Enabled )
				{
					bool ACanRetry = true;
					if( ! FromStart )
						if( ! AccessPoints[ i ]->CanRetry( ACanRetry ) )
							continue;

					if( AccessPoints[ i ]->AccessPointName != "" )
						FConnected() = LGPRS.attachGPRS( AccessPoints[ i ]->AccessPointName.c_str(), AccessPoints[ i ]->UserName.c_str(), AccessPoints[ i ]->Password.c_str() );

					else
						FConnected() = LGPRS.attachGPRS();

					if( FConnected() )
						break;
				}
*/
		}

	public:
		inline bool TryConnectToPoint( String AAccessPointName, String AUserName, String APassword )
		{
			 return LGPRS.attachGPRS( (char *)AAccessPointName.c_str(), (char *)AUserName.c_str(), (char *)APassword.c_str() );
		}

	public:
		inline void SystemInit()
		{
			if( Enabled() )
				StartEthernet();

		}

		inline void SystemLoopBegin()
		{
			if( ! FConnected() )
				TryConnect( false );

		}

	public:
		inline LinkItGPRSModule()
		{			
			FConnected() = false;
		}

	};
//---------------------------------------------------------------------------
}

#endif
