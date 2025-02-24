////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_MICRO_SD_CARD_h
#define _MITOV_MICRO_SD_CARD_h

#include <Mitov.h>

#include <SPI.h>
#include <SD.h>

#ifdef VISUINO_ESP32
	#include "vfs_api.h"
	#include "sd_diskio.h"
	#include "ff.h"
	#include "FS.h"
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
#ifdef VISUINO_ESP32
	typedef SDFS SDClass;
#endif
//---------------------------------------------------------------------------
	template<
		typename T_OutputPin
	> class MicroSDCardElementOperation :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_CheckExists,
		typename T_Create,
		typename T_PathName,
		typename T_Remove
	> class MicroSDCardElementDirectory :
		public T_CheckExists,
		public T_Create,
		public T_Remove,
		public T_PathName
	{
	public:
		_V_PROP_( PathName )

	public:
		_V_PROP_( CheckExists )
		_V_PROP_( Create )
		_V_PROP_( Remove )

	public:
		inline bool DoExists( Mitov::String APathName )
		{
			return C_OWNER.DoExists( JoinPath( APathName ));
		}

		inline bool DoCreate( Mitov::String APathName )
		{
			return C_OWNER.DoCreate( JoinPath( APathName ));
		}

		inline bool DoRemove( Mitov::String APathName )
		{
			return C_OWNER.DoRemove( JoinPath( APathName ));
		}

		inline bool DoOpen( Mitov::String APathName, MITOV_FILE_MODE mode, File &AFile )
		{
			return C_OWNER.DoOpen( JoinPath( APathName ), mode, AFile );
		}

	protected:
		Mitov::String JoinPath( Mitov::String AValue )
		{
			Mitov::String APath = PathName();
			if( ! APath.endsWith( "/" ) )
				APath = APath + "/";

			if( AValue.startsWith( "/" ) )
				AValue.remove( 0, 1 );

			return APath + AValue;
		}

	public:
		void CheckExists_ClockInputPin_o_Receive( void *_Data )
		{
			CheckExists().SetPinValue( C_OWNER.DoExists( PathName() ));
		}

		void Create_ClockInputPin_o_Receive( void *_Data )
		{
			Create().SetPinValue( C_OWNER.DoCreate( PathName() ));
		}

		void Remove_ClockInputPin_o_Receive( void *_Data )
		{
			Remove().SetPinValue( C_OWNER.DoRemove( PathName() ));
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FailedOutputPin,
		typename T_OutputPin,
		typename T_PathName,
		typename T_Recursive
	> class MicroSDCardElementList :
		public T_OutputPin,
		public T_FailedOutputPin,
		public T_Recursive,
		public T_PathName
	{
	public:
		_V_PIN_( OutputPin )
//#ifdef MICROSD_LIST_USE_FAILED_OUTPUT_PIN
		_V_PIN_( FailedOutputPin )
//#endif

	public:
		_V_PROP_( PathName )
		_V_PROP_( Recursive )

	protected:
		void ProcessLevel( Mitov::String APath, Mitov::String APrefix )
		{
			File AFile;
			if( ! C_OWNER.DoOpen( APath, FILE_READ, AFile ))
			{
//#ifdef MICROSD_LIST_USE_FAILED_OUTPUT_PIN
				T_FailedOutputPin::ClockPin();
//#endif
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
				C_OWNER.SendErrorInfo( "Missing Path" );
#endif
				return;
			}

			if( APath.endsWith( "/" ))
				APath = APath.substring( 0, APath.length() - 1 );

			while (true) 
			{
				File AEntry =  AFile.openNextFile();
				if (! AEntry) 
				  // no more files
				  break;

				Mitov::String AItemName = APrefix + AEntry.name(); 
				T_OutputPin::SetPinValue( AItemName.c_str() );

				if( Recursive() )
					if( AEntry.isDirectory())
						ProcessLevel( APath + '/' + AEntry.name(), AItemName + '/' );

				AEntry.close();
			  }

			AFile.close();
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "" );
//			Serial.println( PathName );
//			Serial.println( "" );
			if( PathName() == "" )
				ProcessLevel( "/", "" );

			else
				ProcessLevel( PathName(), "" );
/*
			File AFile;
			if( ! C_OWNER.DoOpen( PathName, FILE_READ, AFile ))
			{
				FailedOutputPin.Notify( nullptr );
				return;
			}

			while (true) 
			{
				File AEntry =  AFile.openNextFile();
				if (! AEntry) 
				  // no more files
				  break;

				OutputPin.Notify( AEntry.name() );
				AEntry.close();
			  }

			AFile.close();
*/
		}

	public:
		inline void SystemStart()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_Enabled,
		typename T_FHasWrite,
		typename T_FReadMoved,
		typename T_FWriteMoved,
		typename T_KeepClosed,
		typename T_KeepFlushed,
		typename T_NewLine,
		typename T_PathName,
		typename T_ReadPositionOutputPin,
		typename T_Size,
		typename T_WritePositionOutputPin
	> class MicroSDCardElementFile :
		public T_ReadPositionOutputPin,
		public T_WritePositionOutputPin,
		public T_Enabled,
		public T_Size,
		public T_NewLine,
		public T_KeepFlushed,
		public T_KeepClosed,
		public T_PathName,
		public T_FReadMoved,
		public T_FWriteMoved,
		public T_FHasWrite
	{
	public:
		_V_PIN_( ReadPositionOutputPin )
		_V_PIN_( WritePositionOutputPin )

	public:
		_V_PROP_( PathName )
		_V_PROP_( Enabled )
		_V_PROP_( NewLine )
		_V_PROP_( KeepFlushed )
		_V_PROP_( KeepClosed )
		_V_PROP_( Size )

	public:
		_V_PROP_( FHasWrite )

	protected:
		_V_PROP_( FReadMoved )
		_V_PROP_( FWriteMoved )

		uint16_t	FWriteCount = 0;

		uint32_t	FWritePosition = 0;
		uint32_t	FReadPosition = 0;

	public:
		File FFile;

	public:
		void PrintChar( char AValue )
		{
			if( ! TryOpen() )
				return;

			PrepareWrite();
			FFile.print( AValue );
			if( AValue == 10 )
				UpdateWrite();

		}

		template<typename T> void Print( T AValue )
		{
			if( ! TryOpen() )
				return;

			PrepareWrite();

//			Serial.println( "TEST2" );
//			Serial.println( AValue );
			if( NewLine() )
				FFile.println( AValue );

			else
				FFile.print( AValue );

			UpdateWrite();
		}

		void Write( void *_Data, size_t ASize )
		{
			if( ! TryOpen() )
				return;

			PrepareWrite();
			FFile.write( (uint8_t *)_Data, ASize );
			FWriteCount += ASize;
			if( FWriteCount > 2000 )
			{
				FWriteCount = 0;
				UpdateWrite();
			}

		}

		void BeginRead()
		{
			if( ! FWriteMoved() )
			{
				FWritePosition = FFile.position();
				FWriteMoved() = true;
			}

			if( FReadMoved() )
			{
				FFile.seek( FReadPosition );
				FReadMoved() = false;
			}
		}

		inline void ResetReadPosition()
		{
			FFile.seek( FReadPosition );
		}

	protected:
		void PrepareWrite()
		{
			if( ! FReadMoved() )
			{
				FReadPosition = FFile.position();
//#ifdef MicroSDCardElementFile_ReadPositionOutputPin
				T_ReadPositionOutputPin::SetPinValue( FReadPosition );
//#endif
				FReadMoved() = true;
			}

			if( FWriteMoved() )
			{
				FFile.seek( FWritePosition );
				FWriteMoved() = false;
			}
		}

		void UpdateWrite()
		{
			if( KeepFlushed() )
				FFile.flush();

			if( KeepClosed() )
				FFile.close();
			
//#ifdef MicroSDCardElementFile_WritePositionOutputPin
			if( T_WritePositionOutputPin::GetPinIsConnected() )
				T_WritePositionOutputPin::SetPinValue( FFile.position() );
//#endif
		}

	public:
		bool TryOpen()
		{
			if( FFile )
				return true;

//			Serial.println( "OPEN1" );
			if( ! Enabled() )
				return false;

			if( PathName() == "" )
				return false;

//			Serial.println( PathName );
//			Serial.println( FHasWrite );
			if( FHasWrite() )
				return C_OWNER.DoOpen( PathName(), FILE_WRITE, FFile );

			else
				return C_OWNER.DoOpen( PathName(), FILE_READ, FFile );
		}

	public:
		inline void CloseInputPin_o_Receive( void *_Data )
		{
			FFile.close();
		}

		inline void FlushInputPin_o_Receive( void *_Data )
		{
			FFile.flush();
		}

		inline void Size_ClockInputPin_o_Receive( void *_Data )
		{
//#ifdef MicroSDCardElementFile_Size
			Size().OutputPin().SetPinValue( FFile.size() );
//#endif
		}

	public:
		inline MicroSDCardElementFile( bool AHasWrite )
		{
			FHasWrite() = AHasWrite;
			FReadMoved() = false;
			FWriteMoved() = false;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin
	> class MicroSDCardFileElementReadTextLine :
		public T_OutputPin
	{
    public:
		_V_PIN_( OutputPin )

	protected:
		bool readStringLine( Mitov::String &AResult )
		{
//			AResult = "";
			int c = C_OWNER.FFile.read();
//			Serial.println( c );
			if( c < 0 )
				return false;

//			Serial.println( "R1" );
			while (c >= 0 && c != '\n' && c != '\r' )
			{
				AResult += (char)c;
				c = C_OWNER.FFile.read();
			}

			while ( c >= 0 )
			{
				c = C_OWNER.FFile.peek();
				if( c != '\n' && c != '\r' )
					break;

				C_OWNER.FFile.read();
			}
/*
			while ( c >= 0 && ( c == '\n' || c == '\r' ))
				c = C_OWNER.FFile.read();
*/
			return true;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! C_OWNER.TryOpen() )
				return;

			Mitov::String ALine;			
			C_OWNER.BeginRead();
//			Serial.println( "READ1" );
			if( readStringLine( ALine ))
				T_OutputPin::SetPinValue( ALine );

		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin,
		typename T_TYPE
	> class MicroSDCardFileElementReadTyped :
		public T_OutputPin
	{
    public:
		_V_PIN_( OutputPin )

    public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! C_OWNER.TryOpen() )
				return;

			C_OWNER.BeginRead();

			T_TYPE AData;
#ifdef VISUINO_ESP32
			size_t AReadSize = C_OWNER.FFile.readBytes( (char *)&AData, sizeof( AData ));
#else
			size_t AReadSize = C_OWNER.FFile.readBytes( (uint8_t *)&AData, sizeof( AData ));
#endif
			if( AReadSize == sizeof( AData ) )
				T_OutputPin::SetPinValue( AData );

//			else
//				C_OWNER.ResetReadPosition();
		}

	};
//---------------------------------------------------------------------------
	template<typename T_OWNER, T_OWNER &C_OWNER, typename T> class MicroSDCardFileElementWriteTyped
	{
    public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.Write( _Data, sizeof( T ));
		}

	public:
		MicroSDCardFileElementWriteTyped()
		{
			C_OWNER.FHasWrite() = true;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_OutputPin,
		typename T_PathName
	> class MicroSDCardElementExists :
		public T_OutputPin,
		public T_PathName
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( PathName )

	public:
		inline void SystemStart()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( C_OWNER.DoExists( PathName() ));
		}

	};
//---------------------------------------------------------------------------
	template<
		int PIN_NUMBER, 
		typename T_Enabled,
		typename T_ErrorInfoOutputPin, 
		typename T_FailedOutputPin,
		typename T_SuccessOutputPin
	> class MicroSDCard : 
		public T_Enabled,
		public T_FailedOutputPin,
		public T_SuccessOutputPin,
		public T_ErrorInfoOutputPin
	{
	public:
		_V_PIN_( FailedOutputPin ) 
		_V_PIN_( SuccessOutputPin )
		_V_PIN_( ErrorInfoOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		bool DoExists( Mitov::String APathName )
		{
			if( !FCard )
				return false;

//			Serial.println( "COOL1" );
//			Serial.println( APathName );
			char *ATmp = new char[ APathName.length() + 1 ];
			memcpy( ATmp, APathName.c_str(), APathName.length() + 1 );

			bool AResult = FCard->exists( ATmp );

			delete [] ATmp;

			return AResult;
		}

		bool DoCreate( Mitov::String APathName )
		{
			if( !FCard )
				return false;

			char *ATmp = new char[ APathName.length() + 1 ];
			memcpy( ATmp, APathName.c_str(), APathName.length() + 1 );

			bool AResult = FCard->mkdir( ATmp );

			delete [] ATmp;

			return AResult;
		}

		bool DoRemove( Mitov::String APathName )
		{
			if( !FCard )
				return false;

			char *ATmp = new char[ APathName.length() + 1 ];
			memcpy( ATmp, APathName.c_str(), APathName.length() + 1 );

			bool AResult = FCard->rmdir( ATmp );

			delete [] ATmp;

			return AResult;
		}

		bool DoOpen( Mitov::String APathName, MITOV_FILE_MODE mode, File &AFile )
		{
//			Serial.print( "O1 :" );
			if( !FCard )
				return false;

			if( ! APathName.startsWith( "/" ))
				APathName = Mitov::String( "/" ) + APathName;

//			char *ATmp = new char[ APathName.length() + 1 ];
//			memcpy( ATmp, APathName.c_str(), APathName.length() + 1 );

//			Serial.print( "T1 :" );
//			Serial.println( ATmp );
			AFile = FCard->open( APathName.c_str(), mode );
//			AFile = FCard->open( "/LOG.TXT", mode );
//			delete [] ATmp;

//			Serial.println( AFile );

			return AFile;
		}

	public:
		SDClass	*FCard = nullptr;
		
	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
//				Serial.println( "TEST1" );
#if defined( VISUINO_ESP32 ) || defined( VISUINO_TEENSY_3_6 )
				FCard = &SD;
#else
				FCard = new SDClass;				
#endif
				if( FCard->begin( PIN_NUMBER ) )
					T_SuccessOutputPin::ClockPin();

				else
				{
//					Serial.println( "FAIL" );
					T_FailedOutputPin::ClockPin();
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
					T_ErrorInfoOutputPin::SetPinValue( "Fail to open microSD" );
#endif

					Enabled() = false;
					UpdateEnabled();
				}
//				Serial.println( "COOL" );
			}
			else
			{
#if ! ( defined( VISUINO_ESP32 ) || defined( VISUINO_TEENSY_3_6 ))
				delete FCard;
#endif
				FCard = nullptr;
			}
		}

		inline void SendErrorInfo( const char *AMessage )
		{
			T_ErrorInfoOutputPin::SetPinValue( AMessage );
		}

	public:
		inline void SystemInit()
		{
			UpdateEnabled();
		}

/*
	public:
		virtual ~MicroSDCard()
		{
			if( FCard )
				delete FCard;
		}
*/
	};
//---------------------------------------------------------------------------
#if defined( VISUINO_ESP32 )
	template<
		typename T_SPI, T_SPI &C_SPI, 
		int PIN_NUMBER,
		typename T_Enabled,
		typename T_ErrorInfoOutputPin,
		typename T_FailedOutputPin,
		typename T_SuccessOutputPin
	> class MicroSDCardCustom :
		public T_FailedOutputPin,
		public T_SuccessOutputPin,
		public T_ErrorInfoOutputPin,
		public T_Enabled
	{
	public:
		_V_PIN_( FailedOutputPin ) 
		_V_PIN_( SuccessOutputPin )
		_V_PIN_( ErrorInfoOutputPin )

	public:
		_V_PROP_( Enabled )

	public:
		bool DoExists( Mitov::String APathName )
		{
			if( !FCard )
				return false;

//			Serial.println( "COOL1" );
//			Serial.println( APathName );
			char *ATmp = new char[ APathName.length() + 1 ];
			memcpy( ATmp, APathName.c_str(), APathName.length() + 1 );

			bool AResult = FCard->exists( ATmp );

			delete [] ATmp;

			return AResult;
		}

		bool DoCreate( Mitov::String APathName )
		{
			if( !FCard )
				return false;

			char *ATmp = new char[ APathName.length() + 1 ];
			memcpy( ATmp, APathName.c_str(), APathName.length() + 1 );

			bool AResult = FCard->mkdir( ATmp );

			delete [] ATmp;

			return AResult;
		}

		bool DoRemove( Mitov::String APathName )
		{
			if( !FCard )
				return false;

			char *ATmp = new char[ APathName.length() + 1 ];
			memcpy( ATmp, APathName.c_str(), APathName.length() + 1 );

			bool AResult = FCard->rmdir( ATmp );

			delete [] ATmp;

			return AResult;
		}

		bool DoOpen( Mitov::String APathName, MITOV_FILE_MODE mode, File &AFile )
		{
//			Serial.print( "O1 :" );
			if( !FCard )
				return false;

			if( ! APathName.startsWith( "/" ))
				APathName = Mitov::String( "/" ) + APathName;

//			char *ATmp = new char[ APathName.length() + 1 ];
//			memcpy( ATmp, APathName.c_str(), APathName.length() + 1 );

//			Serial.print( "T1 :" );
//			Serial.println( ATmp );
			AFile = FCard->open( APathName.c_str(), mode );
//			AFile = FCard->open( "/LOG.TXT", mode );
//			delete [] ATmp;

//			Serial.println( AFile );

			return AFile;
		}

	public:
		SDClass	*FCard = nullptr;
		
	public:
		void UpdateEnabled()
		{
			if( Enabled() )
			{
//				Serial.println( "TEST1" );
				FCard = new SDClass( FSImplPtr(new VFSImpl()) );
#if defined( VISUINO_ESP32 )
				if( FCard->begin( PIN_NUMBER, C_SPI ) )
#else
				if( FCard->begin( PIN_NUMBER ) )
#endif
					T_SuccessOutputPin::ClockPin();

				else
				{
//					Serial.println( "FAIL" );
					T_FailedOutputPin::ClockPin();
#ifdef _MICROSD_USE_ERROR_INFO_PIN_
					T_ErrorInfoOutputPin::SetPinValue( "Fail to open microSD" );
#endif
					Enabled() = false;
					UpdateEnabled();
				}
//				Serial.println( "COOL" );
			}
			else
			{
				delete FCard;
				FCard = nullptr;
			}
		}

	public:
		inline void SystemInit()
		{
			UpdateEnabled();
		}

/*
	public:
		virtual ~MicroSDCard()
		{
			if( FCard )
				delete FCard;
		}
*/
	};
#endif // defined( VISUINO_ESP32 )
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
