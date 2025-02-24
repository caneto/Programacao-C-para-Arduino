////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_VS1053_SHIELD_STANDARD_h
#define _MITOV_VS1053_SHIELD_STANDARD_h

#include <Mitov.h>
#include <SdFat.h>
//#include <SdFatUtil.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

SdFat	sd;

namespace Mitov
{
	template <
		typename T_Left,
		typename T_Right
	> class TArduinoVS1053BMP3Volume :
		public T_Left,
		public T_Right
	{
	public:
		_V_PROP_( Left )
		_V_PROP_( Right )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_OutputPins_Left,
		typename T_OutputPins_Right
	> class TArduinoVS1053BMP3VUMeter :
		public T_Enabled,
		public T_OutputPins_Left,
		public T_OutputPins_Right
	{
	public:
		_V_PIN_( OutputPins_Left )
		_V_PIN_( OutputPins_Right )

	public:
		_V_PROP_( Enabled )

	};
//---------------------------------------------------------------------------
	template <
		typename T_Paused,
		typename T_PositionOutputPin,
		typename T_Stereo,
		typename T_VUMeter,
		typename T_Volume
	> class VS1053ShieldStandard :
		public T_Paused,
		public T_PositionOutputPin,
		public T_Stereo,
		public T_VUMeter,
		public T_Volume
	{
	public:
		_V_PIN_( PositionOutputPin )
/*
#ifdef _MITOV_VS1053_SHIELD_CLASIC_PAUSE_PIN_
		OpenWire::SinkPin	PauseInputPin;
#endif
#ifdef _MITOV_VS1053_SHIELD_CLASIC_RESUME_PIN_
		OpenWire::SinkPin	ResumeInputPin;
#endif
*/
	public:
		_V_PROP_( Volume )
		_V_PROP_( VUMeter )
		_V_PROP_( Paused )
		_V_PROP_( Stereo )

	public:
		SFEMP3Shield FPlayer;

	public:
		void UpdateVolume()
		{
			FPlayer.setVolume( ( 1.0 - Volume().Left() ) * 254 + 0.5, ( 1.0 - Volume().Right() ) * 254 + 0.5 );
//			FPlayer.setVolume( 1, 1 );
		}

		void UpdatePaused()
		{
			if( Paused() )
				FPlayer.pauseMusic();

			else
				FPlayer.resumeMusic();
		}

		void UpdateMono()
		{
			if( Stereo() )
				FPlayer.setMonoMode( 1 );

			else
				FPlayer.setMonoMode( 0 );
		}

	public:
		inline void StopInputPin_o_Receive( void *_Data )
		{
			FPlayer.stopTrack();
		}

/*
#ifdef _MITOV_VS1053_SHIELD_CLASIC_PAUSE_PIN_
		void DoReceivePause( void *_Data )
		{
			FPlayer.pauseMusic();
		}
#endif

#ifdef _MITOV_VS1053_SHIELD_CLASIC_RESUME_PIN_
		void DoReceiveResume( void *_Data )
		{
			FPlayer.resumeMusic();
		}
#endif
*/
	public:
		inline void SystemLoopBegin()
		{
			if( VUMeter().Enabled() )
			{
/*
				FPlayer.setVUmeter( 1 );
				int16_t AValue = FPlayer.getVUlevel();
				
				Serial.println( AValue );
*/
				//				VUMeter.OutputPins[ 0 ]
			}

		}

		inline void SystemInit()
		{
			sd.begin( SD_SEL, SPI_FULL_SPEED );
			FPlayer.begin();
		}

		inline void SystemStart()
		{
			UpdatePaused();
			UpdateMono();
			UpdateVolume();
//			FPlayer.setVUmeter( 1 );
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FileName
	> class VS1053PlayFile :
		public T_FileName
	{
	public:
		_V_PROP_( FileName )

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			C_OWNER.FPlayer.stopTrack();
			C_OWNER.FPlayer.playMP3( (char *)FileName.c_str() );
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
