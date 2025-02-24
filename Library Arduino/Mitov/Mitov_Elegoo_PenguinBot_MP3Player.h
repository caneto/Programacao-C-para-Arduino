////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ELEGOO_PENGUIN_BOT_MP3PLAYER_h
#define _MITOV_ELEGOO_PENGUIN_BOT_MP3PLAYER_h

#include <Mitov.h>
#include <Mitov_SoftwareSerial.h>
#include <Mitov_Maiyout_Serial_MP3.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

//---------------------------------------------------------------------------
namespace PinCalls
{
	class MP3SerialPinCallerReceive
	{
	public:
		void Notify( void *_Data );

	};
} // PinCalls
//---------------------------------------------------------------------------
namespace Mitov
{
//---------------------------------------------------------------------------
	typedef Mitov::VisuinoSoftwareSerial<
		16, 17,
		Mitov::ConstantProperty<9, uint32_t, 0>,
		Mitov::ConstantProperty<7, bool, true>, // Enabled
		Mitov::ConstantProperty<12, bool, false>, // FSending
		Mitov::GenericPin_EmbeddedPinImplementation<5, ::PinCalls::MP3SerialPinCallerReceive>, // OutputPin
		Mitov::DigitalPin_NoImplementation<4>, // SendingOutputPin
		Mitov::ConstantProperty<8, uint32_t, 9600> // Speed
	> TElegooPenguinBotMP3PlayerSerial;
//---------------------------------------------------------------------------
	TElegooPenguinBotMP3PlayerSerial GSoftwareSerial;
//---------------------------------------------------------------------------
	template<
		typename T_Element_Response,
		typename T_Elements_UpdateOutput,
		typename T_Enabled,
		typename T_EqualizerMode,
		typename T_ErrorOutputPin,
		typename T_FExpectFormatResult,
		typename T_FExpectReset,
		typename T_FFirstTry,
		typename T_FIsSending,
		typename T_Playback,
		typename T_SDCard,
		typename T_TimeOutPeriod,
		typename T_TimeoutErrorOutputPin,
		typename T_Track,
		typename T_Volume
	> class ElegooPenguinBotMP3Player :
		public MitovMaiyoutSerialMP3<
			TElegooPenguinBotMP3PlayerSerial, GSoftwareSerial,
			T_Element_Response,
			T_Elements_UpdateOutput,
			T_Enabled,
			T_EqualizerMode,
			T_ErrorOutputPin,
			T_FExpectFormatResult,
			T_FExpectReset,
			T_FFirstTry,
			T_FIsSending,
			T_Playback,
			T_SDCard,
			T_TimeOutPeriod,
			T_TimeoutErrorOutputPin,
			T_Track,
			T_Volume
		>
	{
	public:
		typedef MitovMaiyoutSerialMP3<
				TElegooPenguinBotMP3PlayerSerial, GSoftwareSerial,
				T_Element_Response,
				T_Elements_UpdateOutput,
				T_Enabled,
				T_EqualizerMode,
				T_ErrorOutputPin,
				T_FExpectFormatResult,
				T_FExpectReset,
				T_FFirstTry,
				T_FIsSending,
				T_Playback,
				T_SDCard,
				T_TimeOutPeriod,
				T_TimeoutErrorOutputPin,
				T_Track,
				T_Volume
			> inherited;

	public:
		inline void SetEnabled( bool AValue )
		{
			digitalWrite( 7, (AValue) ? HIGH : LOW );
		}

//	protected:
//		static inherited	*FInstance;

	protected:
//		static void PinCallerReceive( void *_Data )
//		{
//			FInstance->_DirectPinReceive( _Data );
//		}

	public:
		inline void SystemInit( bool AEnabled )
		{
//			FInstance = this;
//			GSoftwareSerial.OutputPin().SetCallback( PinCallerReceive );

			pinMode( 7, OUTPUT );
			digitalWrite( 7, (AEnabled) ? HIGH : LOW );

			GSoftwareSerial.SystemInit();
			inherited::SystemInit();
		}

		inline void SystemLoopBegin()
		{
			GSoftwareSerial.SystemLoopBegin();
			inherited::SystemLoopBegin();
		}

	};
//---------------------------------------------------------------------------
/*
	template<
		typename T_Element_Response,
		typename T_Elements_UpdateOutput,
		typename T_Enabled,
		typename T_EqualizerMode,
		typename T_ErrorOutputPin,
		typename T_FExpectFormatResult,
		typename T_FExpectReset,
		typename T_FFirstTry,
		typename T_FIsSending,
		typename T_Playback,
		typename T_SDCard,
		typename T_TimeOutPeriod,
		typename T_TimeoutErrorOutputPin,
		typename T_Track,
		typename T_Volume
	> MitovMaiyoutSerialMP3<
			TElegooPenguinBotMP3PlayerSerial, GSoftwareSerial,
			T_Element_Response,
			T_Elements_UpdateOutput,
			T_Enabled,
			T_EqualizerMode,
			T_ErrorOutputPin,
			T_FExpectFormatResult,
			T_FExpectReset,
			T_FFirstTry,
			T_FIsSending,
			T_Playback,
			T_SDCard,
			T_TimeOutPeriod,
			T_TimeoutErrorOutputPin,
			T_Track,
			T_Volume
		> *ElegooPenguinBotMP3Player<
				T_Element_Response,
				T_Elements_UpdateOutput,
				T_Enabled,
				T_EqualizerMode,
				T_ErrorOutputPin,
				T_FExpectFormatResult,
				T_FExpectReset,
				T_FFirstTry,
				T_FIsSending,
				T_Playback,
				T_SDCard,
				T_TimeOutPeriod,
				T_TimeoutErrorOutputPin,
				T_Track,
				T_Volume
			>::FInstance = nullptr;
*/
//---------------------------------------------------------------------------
/*
namespace MitovMaiyoutSerialMP3_PinCalls
{
void PinCallerReceive0::Notify( void *_Data )
{
	ElegooPenguinBotMP3Player<
			T_Element_Response,
			T_Elements_UpdateOutput,
			T_Enabled,
			T_EqualizerMode,
			T_ErrorOutputPin,
			T_FExpectFormatResult,
			T_FExpectReset,
			T_FFirstTry,
			T_FIsSending,
			T_Playback,
			T_SDCard,
			T_TimeOutPeriod,
			T_TimeoutErrorOutputPin,
			T_Track,
			T_Volume
		>::PinCallerReceive( _Data );
}
} // PinCalls
*/
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif // _MITOV_ELEGOO_PENGUIN_BOT_MP3PLAYER_h
