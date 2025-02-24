////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_GPRS_h
#define _MITOV_GPRS_h

#include <Mitov.h>
#include <Mitov_BasicEthernet.h>
#include <Mitov_GSMShield.h>
#include <GSM.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_AccessPoints_ResetRetry,
		typename T_AccessPoints_TryConnect,
		uint32_t C_ACESS_POINTS_COUNT,
		typename T_Enabled,
		typename T_FConnected,
		typename T_IsStarted,
		typename T_Sockets_StopSocket
	> class GPRSModule :
//		public T_AccessPoints_TryConnect,
		public T_Enabled,
		public T_FConnected,
		public T_IsStarted,
		public T_Sockets_StopSocket
	{
//		typedef Mitov::GSMModule inherited;

	public:
//		void ( *Sockets_CallChain )() = nullptr;

	public:
//		GPRSAccessPoint *AccessPoints[ C_ACESS_POINTS_COUNT ];

		_V_PROP_( Enabled )

	public:
		_V_PROP_( IsStarted )

	protected:
		_V_PROP_( FConnected )

//	protected:
//		GSMShield &FOwner;

	protected:
		GPRS	FGprs;

	public:
		void SetEnabled( bool AValue )
		{
            if( Enabled == AValue )
                return;

            Enabled = AValue;
			if( ! Enabled() )
				StopEthernet();

//			if( Enabled )
//				StartEthernet();

//			else
//				StopEthernet();

		}

	public:
		inline bool GetIsStarted() { return IsStarted(); }

	protected:
		void StartModule()
		{
			if( Enabled() )
				StartEthernet();

		}

		inline void StopEthernet()
		{
			T_Sockets_StopSocket::ChainCall();
//			LWiFi.end();
			IsStarted = false;
		}

		void StartEthernet()
		{
//			if( ! FOwner.IsStarted() )
//				return;
/*
			if( ! AccessPoints.length() )
				return;

			if( ! Sockets.length() )
				return;
*/
			TryConnect( true );
			if(	FConnected() )
				IsStarted() = true;

//			if( AccessPointName != "" )
//				IsStarted = FGprs.attachGPRS( (char *)AccessPointName.c_str(), (char *)UserName.c_str(), (char *)Password.c_str() );

//			else
//				AConnected = FGprs.attachGPRS();

/*
			if( AConnected )
			{
			}
*/
//			for( int i = 0; i < C_SOCKETS_COUNT; ++i )
//				Sockets[ i ]->TryStartSocket();

		}

		void TryConnect( bool FromStart )
		{
			bool AProccessed = false;
			bool ATryNextConnection = false;
			T_AccessPoints_TryConnect::ChainCall( FromStart, ATryNextConnection, AProccessed );
/*
			for( int i = 0; i < C_ACESS_POINTS_COUNT; i ++ )
				if( AccessPoints[ i ]->Enabled )
				{
					bool ACanRetry = true;
					if( ! FromStart )
						if( ! AccessPoints[ i ]->CanRetry( ACanRetry ) )
							continue;

					FConnected = FGprs.attachGPRS( (char *)AccessPoints[ i ]->AccessPointName.c_str(), (char *)AccessPoints[ i ]->UserName.c_str(), (char *)AccessPoints[ i ]->Password.c_str() );
					if( FConnected )
						break;

					AccessPoints[ i ]->FLastTime = micros();
					++ AccessPoints[ i ]->FRetryCount;
				}
*/
		}

	public:
		inline bool TryConnectToPoint( String AAccessPointName, String AUserName, String APassword )
		{
			 return FGprs.attachGPRS( (char *)AAccessPointName.c_str(), (char *)AUserName.c_str(), (char *)APassword.c_str() );
		}

	public:
		inline void ModuleSystemLoopBegin()
		{
			if( Enabled() )
				if( ! FConnected() )
					TryConnect( false );

		}

	public:
		inline GPRSModule()
		{			
			FConnected() = false;
		}

	};
//---------------------------------------------------------------------------
}

#endif
