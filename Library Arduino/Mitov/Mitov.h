////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_h
#define _MITOV_h

#include <OpenWire.h>

#define _VISUINO_MAX_PIN_NO_ 0xFFFF

#ifdef _MITOV_SOFTWARE_SERIAL_SENDING_PIN_
	#define _MITOV_SERIAL_SENDING_PIN_
#endif

#ifdef FSPI
  #undef FSPI
#endif

#if defined(__AVR_ATmega168__) ||defined(__AVR_ATmega168P__) ||defined(__AVR_ATmega328P__)
  #define Mitov_ATmega_168_168P_328P
#endif

#ifdef VISUINO_ESP32
	typedef const char *MITOV_FILE_MODE;
	#define __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
#else
	typedef uint8_t MITOV_FILE_MODE;
#endif


#ifndef INPUT_PULLDOWN
	#ifndef VISUINO_ESP32
		#define INPUT_PULLDOWN INPUT
	#endif
#endif

#if defined( VISUINO_CHIPKIT_MAX32 ) || defined( VISUINO_CHIPKIT_UC32 ) || defined( VISUINO_CHIPKIT_DP32 ) || defined( VISUINO_CHIPKIT_LENNY )
	#define VISUINO_CHIPKIT

#endif

#if defined( VISUINO_FEATHER_M0_BASIC ) || defined( VISUINO_FEATHER_M0_RFM9X ) || defined( VISUINO_FEATHER_M0_RFM69 ) || defined( VISUINO_TRINKET_M0 )
	#define VISUINO_M0
	#define VISUINO_MEMORY_ALLIGN
	#define VISUINO_FEATHER_M0
#endif

#if defined( VISUINO_WEMOS_D1_R1 ) || defined( VISUINO_WEMOS_D1_R2 ) || defined( VISUINO_WEMOS_D1_MINI ) || defined( VISUINO_FEATHER_HUZZAH_ESP8266 )
	#define VISUINO_ESP8266
#endif

#ifdef VISUINO_ESP8266
	#define VISUINO_MEMORY_ALLIGN
    #undef pgm_read_float
    #define pgm_read_float(addr)            (*reinterpret_cast<const float *>(addr))
    #undef pgm_read_ptr
    #define pgm_read_ptr(addr)              (*(void **)(addr))
#endif

#if defined( VISUINO_STMDUINO )

#if defined( _WIRISH_WPROGRAM_H_ ) // For the olr Mapple compatibility
  #define PIN_MODE WiringPinMode
#endif

  #ifndef OUTPUT_OPEN_DRAIN
	#define OUTPUT_OPEN_DRAIN	OUTPUT
  #endif // OUTPUT_OPEN_DRAIN

#else
  #ifndef OUTPUT_OPEN_DRAIN
    #define OUTPUT_OPEN_DRAIN	OUTPUT
  #endif // OUTPUT_OPEN_DRAIN

#endif

#ifndef PWMRANGE
  #ifdef VISUINO_STMDUINO
    #define PWMRANGE 0xFFFF
 #else // VISUINO_STMDUINO
    #define PWMRANGE 0xFF
  #endif // VISUINO_STMDUINO

#endif // PWMRANGE

//#ifdef VISUINO_ESP32
//	inline void analogWrite( uint8_t APin, uint16_t AValue ) {}
//#endif

#ifdef VISUINO_ESP32
	#define VISUINO_ANALOG_IN_RANGE 4096
	#define VISUINO_ANALOG_IN_THRESHOLD 2000
#else
	#define VISUINO_ANALOG_IN_RANGE 1023
	#define VISUINO_ANALOG_IN_THRESHOLD 409
#endif

#if defined(ESP8266)
  #define FEATHER_PIN_9  0
  #define FEATHER_PIN_6 16
  #define FEATHER_PIN_5  2
#elif defined(ESP32)
  #define FEATHER_PIN_9 15
  #define FEATHER_PIN_6 32
  #define FEATHER_PIN_5 14
#elif defined(ARDUINO_STM32_FEATHER)
  #define FEATHER_PIN_9 PA15
  #define FEATHER_PIN_6 PC7
  #define FEATHER_PIN_5 PC5
#elif defined(TEENSYDUINO)
  #define FEATHER_PIN_9  4
  #define FEATHER_PIN_6  3
  #define FEATHER_PIN_5  8
#elif defined(ARDUINO_FEATHER52832)
  #define FEATHER_PIN_9 31
  #define FEATHER_PIN_6 30
  #define FEATHER_PIN_5 27
#else // 32u4, M0, M4, nrf52840 and 328p
  #define FEATHER_PIN_9  9
  #define FEATHER_PIN_6  6
  #define FEATHER_PIN_5  5
#endif

#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
 #define mitov_pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
 #define mitov_pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif

//#define _VPROP_(TYPE,NAME) inline typename TYPE::T_TYPE		NAME() { return TYPE::GetValue(); }
#define _VPROP_(TYPE,NAME) inline TYPE		&NAME() { return TYPE::GetInstance(); }
#define _V_PROP_(NAME) inline T_##NAME		&NAME() { return T_##NAME::GetInstance(); }
//#define _VPROP_(TYPE,NAME) inline TYPE		&NAME() { TYPE *AResult; TYPE::GetInstance( AResult ); return *AResult; }
//#define _V_PROP_(NAME) inline T_##NAME		&NAME() { T_##NAME *AResult; T_##NAME::GetInstance( AResult ); return *AResult; }

#define _VPIN_(TYPE,NAME) TYPE	&NAME() { return TYPE::GetInstance(); }
#define _V_PIN_(NAME) T_##NAME	&NAME() { return T_##NAME::GetInstance(); }

#ifdef VISUINO_ESP32
//  #define PWMRANGE 255

namespace Mitov
{
	HardwareSerial Serial1(1);
	HardwareSerial Serial2(2);
}

namespace Mitov
{
	uint8_t	CESP32PWMTimers[ 2 ] = {0};
	uint8_t	CESP32PWMPinMap[ 40 ] = {0};

	void ServoAttach( uint8_t APin )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] == 0 )
		{
			uint8_t AChannel = 0;
			for( AChannel = 0; AChannel < 16; ++AChannel )
				if( ! ( Mitov::CESP32PWMTimers[ AChannel >> 3 ] & ( 1 << ( AChannel & 0b111 ) ) ))
					break;

			if( AChannel == 16 )
				return; // Can't allocate channel

			Mitov::CESP32PWMPinMap[ APin ] = AChannel + 1;
			Mitov::CESP32PWMTimers[ AChannel >> 3 ] |= ( 1 << ( AChannel & 0b111 ) );
			ledcSetup( AChannel, 50, 16 ); // 50Hz 16 bit
//			ledcWrite( AChannel, AValue );
			ledcAttachPin( APin, AChannel );
		}
	}

	void ServoDetach( uint8_t APin )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
		{
			ledcDetachPin( APin );
			uint8_t AChannel = Mitov::CESP32PWMPinMap[ APin ] - 1;
			Mitov::CESP32PWMTimers[ AChannel >> 3 ] &= ~ ( 1 << ( AChannel & 0b111 ) );
			Mitov::CESP32PWMPinMap[ APin ] = 0;
		}
	}

	void ServoWrite( uint8_t APin, float AValue, uint16_t APulseMin, uint16_t APulseMax )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
		{
			APulseMin *= 0xFFFF / 20000;
			APulseMax *= 0xFFFF / 20000;
			APulseMin += 512; //255;
			APulseMax += 512; //255;
			uint16_t AIntValue = ( AValue / 180 ) * ( APulseMax - APulseMin ) + 0.5;
			AIntValue += APulseMin;
			ledcWrite( Mitov::CESP32PWMPinMap[ APin ] - 1, AIntValue );
		}
	}

	void ToneAttach( uint8_t APin )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] == 0 )
		{
			uint8_t AChannel = 0;
			for( AChannel = 0; AChannel < 16; ++AChannel )
				if( ! ( Mitov::CESP32PWMTimers[ AChannel >> 3 ] & ( 1 << ( AChannel & 0b111 ) ) ))
					break;

			if( AChannel == 16 )
				return; // Can't allocate channel

			Mitov::CESP32PWMPinMap[ APin ] = AChannel + 1;
			Mitov::CESP32PWMTimers[ AChannel >> 3 ] |= ( 1 << ( AChannel & 0b111 ) );
			ledcSetup( AChannel, 50, 16 ); // 50Hz 16 bit
//			ledcWrite( AChannel, AValue );
			ledcAttachPin( APin, AChannel );
		}
	}

	void ToneDetach( uint8_t APin )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
		{
			ledcDetachPin( APin );
			uint8_t AChannel = Mitov::CESP32PWMPinMap[ APin ] - 1;
			Mitov::CESP32PWMTimers[ AChannel >> 3 ] &= ~ ( 1 << ( AChannel & 0b111 ) );
			Mitov::CESP32PWMPinMap[ APin ] = 0;
		}
	}

	void PlayTone( uint8_t APin, double freq )
	{
		if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
		{
			ledcWriteTone( Mitov::CESP32PWMPinMap[ APin ] - 1, freq );
		}
	}
}
	void analogWrite( uint8_t APin, uint16_t AValue )
	{
//		Serial.print( "analogWrite : " ); Serial.println( AValue );

		if( AValue == 0 || AValue == 255 )
		{
			if( Mitov::CESP32PWMPinMap[ APin ] != 0 )
			{
				ledcWrite( Mitov::CESP32PWMPinMap[ APin ] - 1, AValue );
//				Serial.println( "ledcDetachPin" );
				digitalWrite( APin, (AValue == 255 ) ? HIGH : LOW );
				ledcDetachPin( APin );
				uint8_t AChannel = Mitov::CESP32PWMPinMap[ APin ] - 1;
				Mitov::CESP32PWMTimers[ AChannel >> 3 ] &= ~ ( 1 << ( AChannel & 0b111 ) );
				Mitov::CESP32PWMPinMap[ APin ] = 0;
			}
			
			pinMode( APin, OUTPUT );
			digitalWrite( APin, (AValue == 255 ) ? HIGH : LOW );
			return;
		}

		if( Mitov::CESP32PWMPinMap[ APin ] == 0 )
		{
			uint8_t AChannel = 0;
			for( AChannel = 0; AChannel < 16; ++AChannel )
				if( ! ( Mitov::CESP32PWMTimers[ AChannel >> 3 ] & ( 1 << ( AChannel & 0b111 ) ) ))
					break;

			if( AChannel == 16 )
				return; // Can't allocate channel

			Mitov::CESP32PWMPinMap[ APin ] = AChannel + 1;
			Mitov::CESP32PWMTimers[ AChannel >> 3 ] |= ( 1 << ( AChannel & 0b111 ) );
			ledcSetup( AChannel, 1000, 8 ); // 1KHz 8 bit
			ledcWrite( AChannel, AValue );
			ledcAttachPin( APin, AChannel );
//			Serial.print( "ledcAttachPin: " ); Serial.print( APin ); Serial.print( " " ); Serial.println( AChannel );
			return;
		}

		ledcWrite( Mitov::CESP32PWMPinMap[ APin ] - 1, AValue );
//		Serial.print( "ledcWrite: " ); Serial.print( Mitov::CESP32PWMPinMap[ APin ] - 1 ); Serial.print( " " ); Serial.println( AValue );
	}

#endif

#if defined( VISUINO_FREESOC2 )
  #include <itoa.h>
#endif

#if defined( VISUINO_ARDUINO_DUE ) || defined( VISUINO_NRF52832 ) || defined( VISUINO_BBC_MICRO_BIT ) || defined( VISUINO_FEATHER_M0 )
  #include <avr/dtostrf.h> 
#endif

#if defined( VISUINO_TEENSY_3_0 ) || defined( VISUINO_TEENSY_3_1 ) || defined( VISUINO_TEENSY_LC ) || defined( VISUINO_TEENSY_3_5 ) || defined( VISUINO_TEENSY_3_6 )
  #define VISUINO_TEENSY
#endif

#if defined( VISUINO_ARDUINO_DUE ) || defined( VISUINO_LINKIT_ONE ) || defined( VISUINO_TEENSY ) || defined( VISUINO_TEENSY_2_0 ) || defined( VISUINO_TEENSY_2_0_PP ) || defined( VISUINO_ARDUINO_RASPBERRY_PI )
  #define NO_SERIAL_FORMAT
#endif

#if defined( VISUINO_ESP8266 )
  #define V_FMOD(a,b) (a - b * floor(a / b))
  #define sscanf os_sprintf
#else
  #define V_FMOD(a,b) fmod(a,b)
#endif


float posmod( float a, float b )
{
	a = V_FMOD(a,b);
	if( a < 0 )
		a += b;

	return a;
}

#define MAKE_CALLBACK_3_1(T1,T2,A,P) (T2)( void (T1::*) ( P ) )&A
#define MAKE_CALLBACK_3(T1,T2,A,P) (T1 *)this, (T2)( void (T1::*) ( P ) )&A
#define MAKE_CALLBACK_2(T1,T2,A) (T1 *)this, (T2)( void (T1::*) ( void *_Data ) )&A

#define MITOV_ARRAY_SIZE(A) ( sizeof(A) / sizeof(A[0]) )
#define ARRAY_PARAM(A) A,MITOV_ARRAY_SIZE(A)

namespace Mitov
{
//---------------------------------------------------------------------------
	struct FloatPoint2D
	{
		float X;
		float Y;
	};
//---------------------------------------------------------------------------
	struct FloatRect2D // : public FloatPoint2D
	{
		float X;
		float Y;
		float Width;
		float Height;
	};
//---------------------------------------------------------------------------
	template<typename T> class TAsBytes
	{
		union
		{
			T Value;
			uint8_t  Bytes[sizeof( T )];
		}	Value;

	public:
		inline TAsBytes<T>& operator=( const uint32_t AValue )
		{
			Value.Value = AValue;
			return *this;
		}

		inline uint8_t &operator[]( const uint32_t AIndex ) { return Value.Bytes[ AIndex ]; }

		inline operator T() { return Value.Value; }

	public:
		inline TAsBytes() {}
		inline TAsBytes( const uint32_t AValue )
		{
			Value.Value = AValue;
		}

	};
//---------------------------------------------------------------------------
/*
	template<typename T> T log2( T log2_input) 
	{
		return log(float( log2_input)) / log(float( 2 ));  
	}
*/
	uint8_t MitovIlog2( uint32_t AValue )
	{
		for( int i = 0; i < 32; ++i )
		{
			if( !( AValue >> i ))
				return i;
		}

		return 32;
	}
//---------------------------------------------------------------------------
	template<typename T> T Constrain16( uint32_t AValue )
	{
		uint32_t AValue1 = MitovMin( AValue, uint32_t( 0xFFFF ));
		return AValue1;
	}
//---------------------------------------------------------------------------
	template<> int16_t Constrain16<int16_t>( uint32_t AValue )
	{
		int32_t AValue1 = constrain( (*(int32_t *)&AValue ), -0x8000l, 0x7FFFl );
		return AValue1;
	}
//---------------------------------------------------------------------------
	template<typename T> T Constrain16( int32_t AValue )
	{
		int32_t AValue1 = constrain( AValue, -0x8000l, 0x7FFFl );
		return AValue1;
	}
//---------------------------------------------------------------------------
	enum TAngleUnits { auDegree, auRadians, auNormalized };
	typedef TAngleUnits TArduinoAngleUnits ;
//---------------------------------------------------------------------------
	const uint16_t MonthDays[2][12] =
	{
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	};
//---------------------------------------------------------------------------
	static const double HoursPerDay			= 24;
	static const double MinsPerHour			= 60;
	static const double SecsPerMin			= 60;
	static const double MSecsPerSec			= 1000;
	static const double MinsPerDay			= HoursPerDay * MinsPerHour;
	static const double SecsPerDay			= MinsPerDay * SecsPerMin;
	static const double SecsPerHour			= SecsPerMin * MinsPerHour;
	static const double MSecsPerDay			= SecsPerDay * MSecsPerSec;
	static const int32_t	IMSecsPerDay	= MSecsPerDay;
// Days between 1/1/0001 and 12/31/1899
//	const double DateDelta			= 693594;
//---------------------------------------------------------------------------
	template<
		typename T_Days,
		typename T_Hours,
		typename T_MilliSeconds,
		typename T_Minutes,
		typename T_Months,
		typename T_Seconds,
		typename T_WeekDays,
		typename T_Years
	> class TDateTimeCompareItems :
		public T_Days,
		public T_Hours,
		public T_MilliSeconds,
		public T_Minutes,
		public T_Months,
		public T_Seconds,
		public T_WeekDays,
		public T_Years
	{
	public:
		_V_PROP_( Years )
		_V_PROP_( Months )
		_V_PROP_( Days )
		_V_PROP_( WeekDays )
		_V_PROP_( Hours )
		_V_PROP_( Minutes )
		_V_PROP_( Seconds )
		_V_PROP_( MilliSeconds )

	public:
		inline bool AllTrue()
		{ 
			return Years().GetValue() && Months().GetValue() && Days().GetValue() && Hours().GetValue() && Minutes().GetValue() && Seconds().GetValue() && MilliSeconds().GetValue();
		}

		inline bool AllDateTrue()
		{ 
			return Years().GetValue() && Months().GetValue() && Days().GetValue();
		}

		inline bool AllDateFalse()
		{ 
			return ( ! Years().GetValue() ) && ( ! Months().GetValue() ) && ( ! Days().GetValue() );
		}

		inline bool AllTimeTrue()
		{ 
			return Hours().GetValue() && Minutes().GetValue() && Seconds().GetValue() && MilliSeconds().GetValue();
		}

		inline bool AllTimeFalse()
		{ 
			return ( ! Hours().GetValue() ) && ( ! Minutes().GetValue() ) && ( ! Seconds().GetValue() ) && ( ! MilliSeconds().GetValue() );
		}

		inline bool CompareWeekDays()
		{
			return AllDateFalse() && WeekDays().GetValue(); 
		}
	};
//---------------------------------------------------------------------------
	class TComplex
	{
	public:
		float Real;
		float Imaginary;

	public:
		operator Mitov::String()
		{
			return "[" + Mitov::String( Real ) + "," + Mitov::String( Imaginary ) + "]";
		}

	public:
		bool operator == ( const TComplex &AOther ) const
		{
			if( Real != AOther.Real )
				return false;

			return ( Imaginary == AOther.Imaginary );
		}

	public:
		TComplex() :
			Real( 0 ),
			Imaginary( 0 )
		{
		}

		TComplex( float AReal ) :
			Real( AReal ),
			Imaginary( 0 )
		{
		}

		TComplex( float AReal, float AImaginary ) :
			Real( AReal ),
			Imaginary( AImaginary )
		{
		}
	};
//---------------------------------------------------------------------------
	#pragma pack(push, 1)
	struct TCompactColor
	{
	public:
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;

	public:
		inline void SetValue( const int32_t AValue )
		{
#if defined( VISUINO_MEMORY_ALLIGN )
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
#else
			*((int32_t *)this) = AValue & 0xFFFFFF;
#endif
		}

		inline void operator = ( const long AValue )
		{
			SetValue( AValue );
		}

	public:
		inline TCompactColor()
		{
		}

		inline TCompactColor( const long AValue )
		{
			SetValue( AValue );
		}
	};
	#pragma pack(pop)
//---------------------------------------------------------------------------
	#pragma pack(push, 1)
	struct TColor
	{
	public:
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;
		uint8_t Alpha = 0;

	public:
		inline void operator = ( const TCompactColor AValue )
		{
			Red = AValue.Red;
			Green = AValue.Green;
			Blue = AValue.Blue;
		}

		inline TColor( const TCompactColor AValue )
		{
			Red = AValue.Red;
			Green = AValue.Green;
			Blue = AValue.Blue;
		}

	public:
		inline void SetValue( const int32_t AValue )
		{
#if defined( VISUINO_MEMORY_ALLIGN )
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
#else
			*((int32_t *)this) = AValue & 0xFFFFFF;
#endif

//			Blue = ( AValue >> 16 ) & 0xFF;
//			Green = ( AValue >> 8 ) & 0xFF;
//			Red = AValue & 0xFF;
//			Alpha = 0;
		}

		inline void operator = ( const long AValue )
		{
			SetValue( AValue );
		}

		inline bool operator == ( const TColor other )
		{
#if defined( VISUINO_MEMORY_ALLIGN )
			return (( Red == other.Red ) && ( Green == other.Green ) && ( Blue == other.Blue ) && ( Alpha == other.Alpha ));
#else
			return *((long *)this) == *((long *)&other);
#endif
		}

		inline operator long()
		{
			return *((long *)this);
		}

		inline uint32_t AsRGBWord()
		{
			return (( uint32_t( Red ) << 16 ) | ( uint32_t( Green ) << 8 ) | Blue );
		}

	public:
		Mitov::String ToString()
		{
			return	Mitov::String( "(R:" ) + 
					Mitov::String((int)Red ) + ",G:" +
					Mitov::String((int)Green ) + ",B:" +
					Mitov::String((int)Blue ) + ")";
		}

		uint16_t To565Color()
		{
			return (((31*(Red+4))/255)<<11) | 
               (((63*(Green+2))/255)<<5) | 
               ((31*(Blue+4))/255);
		}

		uint16_t BGRTo565Color()
		{
			return (((31*(Blue+4))/255)<<11) | 
               (((63*(Green+2))/255)<<5) | 
               ((31*(Red+4))/255);
		}
/*
		uint32_t GetGBR()
		{
			return ( uint32_t( Red ) << 16 ) | ( uint32_t( Green ) << 8 ) | Blue;
		}
*/
	public:
		TColor( long AValue = 0 )
		{
			SetValue( AValue );
		}

		TColor( unsigned char ARed, unsigned char AGreen, unsigned char ABlue ) :
			Red( ARed ),
			Green( AGreen ),
			Blue( ABlue )
		{
		}

		TColor( long AValue, bool ) // For Windows BGR support
		{
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
		}
	};
	#pragma pack(pop)
//---------------------------------------------------------------------------
	#pragma pack(push, 1)
	struct TRGBWColor
	{
	public:
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;
		uint8_t White;

	public:
		void SetValue( const uint32_t AValue )
		{
//			*((uint32_t *)this) = AValue;
			White = ( AValue >> 24 ) & 0xFF;
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
		}

		void operator =( const long AValue )
		{
			SetValue( AValue );
		}

		bool operator ==( const TRGBWColor other )
		{
			return *((uint32_t *)this) == *((uint32_t *)&other);
		}

		operator long()
		{
			return *((long *)this);
		}

	public:
		Mitov::String ToString()
		{
			return	Mitov::String( "(R:" ) + 
					Mitov::String((int)Red ) + ",G:" +
					Mitov::String((int)Green ) + ",B:" +
					Mitov::String((int)Blue ) + ",W:" +
					Mitov::String((int)White ) + ")";
		}

		uint16_t To565Color()
		{
			return (((31*(Red+4))/255)<<11) | 
               (((63*(Green+2))/255)<<5) | 
               ((31*(Blue+4))/255);
		}

	public:
		TRGBWColor( uint32_t AValue = 0 )
		{
			SetValue( AValue );
		}

		TRGBWColor( TColor &AColor )
		{
			Red = AColor.Red;
			Green = AColor.Green;
			Blue = AColor.Blue;
			White = 0;
		}

		TRGBWColor( unsigned char ARed, unsigned char AGreen, unsigned char ABlue, unsigned char AWhite ) :
			Red( ARed ),
			Green( AGreen ),
			Blue( ABlue ),
			White( AWhite )
		{
		}

		TRGBWColor( uint32_t AValue, bool ) // For Windows BGR support
		{
			White = ( AValue >> 24 ) & 0xFF;
			Red = ( AValue >> 16 ) & 0xFF;
			Green = ( AValue >> 8 ) & 0xFF;
			Blue = AValue & 0xFF;
		}
	};
	#pragma pack(pop)
//---------------------------------------------------------------------------
	typedef TRGBWColor TAlphaColor;
//---------------------------------------------------------------------------
	class TDateTime
	{
	public:
		// Do not change the order! Date Must be after Time for pcket communication!
		int32_t Time; // Number of milliseconds since midnight
		int32_t Date; // One plus number of days since 1/1/0001

	protected:
		inline void DivMod( int32_t Dividend, uint16_t Divisor, uint16_t &Result, uint16_t &Remainder ) const
		{
			Result = Dividend / Divisor;
			Remainder = Dividend % Divisor;
		}

	public:
		void DecodeTime( uint16_t &AHour, uint16_t &AMin, uint16_t &ASec, uint16_t &AMSec ) const
		{
			uint16_t	MinCount, MSecCount;

			DivMod( Time, SecsPerMin * MSecsPerSec, MinCount, MSecCount );
			DivMod( MinCount, MinsPerHour, AHour, AMin );
			DivMod( MSecCount, MSecsPerSec, ASec, AMSec );
		}

		bool IsLeapYear( uint16_t Year ) const
		{
			return (Year % 4 == 0) && ((Year % 100 != 0) || (Year % 400 == 0));
		}

		bool DecodeDateFully( uint16_t &Year, uint16_t &Month, uint16_t &Day, uint16_t &DOW ) const
		{
			const uint32_t TD1 = 365;
			const uint32_t TD4 = TD1 * 4 + 1;
			const uint32_t D100 = TD4 * 25 - 1;
			const uint32_t D400 = D100 * 4 + 1;

			int32_t T = Date;
			if( T <= 0 )
			{
				Year = 0;
				Month = 0;
				Day = 0;
				DOW = 0;
				return( false );
			}

			else
			{
				DOW = T % 7 + 1;
				--T;
				uint16_t Y = 1;
				while( T >= (int32_t)D400 )
				{
					T -= D400;
					Y += 400;
				}

				uint16_t D, I;
				DivMod( T, D100, I, D );
				if( I == 4 )
				{
					-- I;
					D -= D100;
				}

				Y += I * 100;
				DivMod(D, TD4, I, D);

				Y += I * 4;
				DivMod(D, TD1, I, D);

				if( I == 4 )
				{
					--I;
					D += TD1;
				}

				Y += I;
				bool Result = IsLeapYear(Y);

				const uint16_t *DayTable = MonthDays[Result];
				uint16_t M = 0; // The C++ Day table is zero indexed!
				for(;;)
				{
					I = DayTable[M];
					if( D < I )
						break;

					D -= I;
					++ M;
				}

				Year = Y;
				Month = M + 1;
				Day = D + 1;

                return Result;
			}
		}

		bool TryEncodeDate( uint16_t Year, uint16_t Month, uint16_t Day )
		{
			const uint16_t *DayTable = MonthDays[IsLeapYear(Year)];
			if( (Year >= 1) && (Year <= 9999) && (Month >= 1) && (Month <= 12) && (Day >= 1))
				if( Day <= DayTable[Month - 1] )
				{

					for( uint32_t i = 1; i < Month; i ++ )
						Day += DayTable[ i - 1 ];

					int I = Year - 1;
					Date = ((uint32_t)I) * 365 + (int)( I / 4 ) - (int)( I / 100 ) + (int)( I / 400 ) + (uint32_t)Day;
					return true;
				}

			return false;
		}

		bool TryEncodeTime( uint16_t Hour, uint16_t AMin, uint16_t Sec, uint16_t MSec )
		{
			if ((Hour < HoursPerDay) && (AMin < MinsPerHour) && (Sec < SecsPerMin) && (MSec < MSecsPerSec))
			{
				Time =  (Hour * (MinsPerHour * SecsPerMin * MSecsPerSec))
					  + (AMin * SecsPerMin * MSecsPerSec)
					  + (Sec * MSecsPerSec)
					  +  MSec;
//				Date = DateDelta; // This is the "zero" day for a TTimeStamp, days between 1/1/0001 and 12/30/1899 including the latter date
				Date = 0; // This is the "zero" day for a TTimeStamp, days between 1/1/0001 and 12/30/1899 including the latter date
				return true;
			}

			return false;
		}

		bool TryEncodeDateTime( uint16_t AYear, uint16_t AMonth, uint16_t ADay, uint16_t AHour, uint16_t AMinute, uint16_t ASecond, uint16_t AMilliSecond )
		{
			bool Result = TryEncodeDate( AYear, AMonth, ADay );
			if( Result )
			{
				TDateTime LTime;
				Result = LTime.TryEncodeTime( AHour, AMinute, ASecond, AMilliSecond );
				if( Result )
					Time = LTime.Time; 

			}

            return Result;
		}

		void DecodeDateTime( uint16_t &AYear, uint16_t &AMonth, uint16_t &ADay, uint16_t &AHour, uint16_t &AMinute, uint16_t &ASecond, uint16_t &AMilliSecond ) const
		{
			uint16_t AWeekDay;
			DecodeDateTime( AYear, AMonth, ADay, AWeekDay, AHour, AMinute, ASecond, AMilliSecond );
		}

		void DecodeDateTime( uint16_t &AYear, uint16_t &AMonth, uint16_t &ADay, uint16_t &AWeekDay, uint16_t &AHour, uint16_t &AMinute, uint16_t &ASecond, uint16_t &AMilliSecond ) const
		{
			DecodeDateFully( AYear, AMonth, ADay, AWeekDay );
			DecodeTime( AHour, AMinute, ASecond, AMilliSecond );
		}

		Mitov::String ToString() const
		{
			uint16_t AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond;
			DecodeDateTime( AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond );
			char ABuffer[ 6 + 4 + 5 * 2 + 3 + 2 + 1 + 5 ];
			sprintf( ABuffer, "(%04d.%02d.%02d %02d:%02d:%02d.%03d)", AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond );
			return ABuffer;
		}

		uint32_t JulianDate()
		{
			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t DOW;

			DecodeDateFully( AYear, AMonth, ADay, DOW );
//			Serial.println( AYear );
			if( AMonth <= 2 ) 
			{
				AYear--; 
				AMonth += 12;
			}

			int A = AYear / 100; 
			int B = 2 - A + A / 4;
			return uint32_t((365.25*( AYear+4716))+(int)(30.6001*(AMonth+1))+ ADay+B-1524 );
		}

		void AddMilliSeconds( int64_t AValue )
		{
			AllignTime( AValue + Time );
//			int64_t
//			if( int64_t( AValue ) + >= MSecsPerDay )
//			int32_t AOldTime
		}

		void AddSeconds( int64_t AValue )
		{
//			Serial.println( uint32_t( AValue ));
			AllignTime( int64_t( AValue * MSecsPerSec ) + Time );
		}

		void AddMinutes( int64_t AValue )
		{
			AllignTime( int64_t( AValue * MSecsPerSec * SecsPerMin ) + Time );
		}

		void AddHours( int64_t AValue )
		{
			AllignTime( int64_t( AValue * MSecsPerSec * SecsPerMin * MinsPerHour ) + Time );
		}

		void AddDays( int32_t AValue )
		{
			Date += AValue;
		}

		void AddMonths( int32_t AValue )
		{
			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t DOW;
			DecodeDateFully( AYear, AMonth, ADay, DOW );

			IncAMonth( AYear, AMonth, ADay, AValue );

			TryEncodeDate( AYear, AMonth, ADay );
//			Date += AValue;
		}

		void AddYears( int32_t AValue )
		{
			AddMonths( AValue * 12 );
		}

	protected:
		void AllignTime( int64_t AValue )
		{
//			Serial.println( "AllignTime" );
//			Serial.println( int32_t( AValue ));
//			Serial.println( Time );
			Date += AValue / IMSecsPerDay;
			Time = AValue % IMSecsPerDay;
			if( Time < 0 )
			{
				Time += IMSecsPerDay;
				Date --;
			}
				
//			Serial.println( Time );
		}

		void IncAMonth( uint16_t &AYear, uint16_t &AMonth, uint16_t &ADay, int32_t ANumberOfMonths )
		{
			int Sign;

			if( ANumberOfMonths >= 0 )
				Sign = 1;

			else 
				Sign = -1;

			AYear += ANumberOfMonths / 12;
			ANumberOfMonths %= 12;
			int64_t ABigMonth = AMonth;
			ABigMonth += ANumberOfMonths;
			if( uint64_t( ABigMonth - 1 ) > 11 )    // if Month <= 0, word(Month-1) > 11)
			{
				AYear += Sign;
				ABigMonth += -12 * Sign;
			}

			AMonth = ABigMonth;

			const uint16_t *DayTable = MonthDays[IsLeapYear(AYear)];
			if( ADay > DayTable[ AMonth - 1 ] )
				ADay = DayTable[ AMonth - 1 ];

		}


	public:
		template<typename T_ITEMS> bool IsEqual( const TDateTime &AOther, T_ITEMS &AItems ) const
		{
			if( AItems.AllTrue() )
				return ( *this == AOther );

			if( AItems.AllDateTrue() && AItems.AllTimeFalse() )
				return ( Date == AOther.Date );

			if( AItems.AllTimeTrue() && AItems.AllDateFalse() )
				return ( Time == AOther.Time );

			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t AWeekDay;
			uint16_t AHour;
			uint16_t AMinute;
			uint16_t ASecond;
			uint16_t AMilliSecond;

			DecodeDateTime( AYear, AMonth, ADay, AWeekDay, AHour, AMinute, ASecond, AMilliSecond );

			uint16_t AOtherYear;
			uint16_t AOtherMonth;
			uint16_t AOtherDay;
			uint16_t AOtherWeekDay;
			uint16_t AOtherHour;
			uint16_t AOtherMinute;
			uint16_t AOtherSecond;
			uint16_t AOtherMilliSecond;

			AOther.DecodeDateTime( AOtherYear, AOtherMonth, AOtherDay, AOtherWeekDay, AOtherHour, AOtherMinute, AOtherSecond, AOtherMilliSecond );

			if( AItems.CompareWeekDays() )
				if( AWeekDay != AOtherWeekDay ) 
					return false;

			if( AItems.Years() )
				if( AYear != AOtherYear ) 
					return false;

			if( AItems.Months() )
				if( AMonth != AOtherMonth ) 
					return false;

			if( AItems.Days() )
				if( ADay != AOtherDay ) 
					return false;

			if( AItems.Hours() )
				if( AHour != AOtherHour ) 
					return false;

			if( AItems.Minutes() )
				if( AMinute != AOtherMinute ) 
					return false;

			if( AItems.Seconds() )
				if( ASecond != AOtherSecond ) 
					return false;

			if( AItems.MilliSeconds() )
				if( AMilliSecond != AOtherMilliSecond ) 
					return false;

			return true;
		}

		template<typename T_ITEMS> bool IsBigger( const TDateTime &AOther, T_ITEMS &AItems ) const
		{
			if( AItems.AllTrue() )
				return ( *this > AOther );

			if( AItems.AllDateTrue() && AItems.AllTimeFalse() )
				return ( Date > AOther.Date );

			if( AItems.AllTimeTrue() && AItems.AllDateFalse() )
				return ( Time > AOther.Time );

			uint16_t AYear;
			uint16_t AMonth;
			uint16_t ADay;
			uint16_t AWeekDay;
			uint16_t AHour;
			uint16_t AMinute;
			uint16_t ASecond;
			uint16_t AMilliSecond;

			DecodeDateTime( AYear, AMonth, ADay, AWeekDay, AHour, AMinute, ASecond, AMilliSecond );

			uint16_t AOtherYear;
			uint16_t AOtherMonth;
			uint16_t AOtherDay;
			uint16_t AOtherWeekDay;
			uint16_t AOtherHour;
			uint16_t AOtherMinute;
			uint16_t AOtherSecond;
			uint16_t AOtherMilliSecond;

			AOther.DecodeDateTime( AOtherYear, AOtherMonth, AOtherDay, AOtherWeekDay, AOtherHour, AOtherMinute, AOtherSecond, AOtherMilliSecond );

			if( AItems.CompareWeekDays() )
			{
				if( AWeekDay < AOtherWeekDay ) 
					return false;

				if( AWeekDay > AOtherWeekDay ) 
					return true;
			}

			if( AItems.Years().GetValue() )
			{
				if( AYear < AOtherYear ) 
					return false;

				if( AYear > AOtherYear ) 
					return true;
			}

			if( AItems.Months().GetValue() )
			{
				if( AMonth < AOtherMonth ) 
					return false;

				if( AMonth > AOtherMonth ) 
					return true;
			}

			if( AItems.Days().GetValue() )
			{
				if( ADay < AOtherDay ) 
					return false;

				if( ADay > AOtherDay ) 
					return true;
			}

			if( AItems.Hours().GetValue() )
			{
				if( AHour < AOtherHour ) 
					return false;

				if( AHour > AOtherHour ) 
					return true;
			}

			if( AItems.Minutes().GetValue() )
			{
				if( AMinute < AOtherMinute ) 
					return false;

				if( AMinute > AOtherMinute ) 
					return true;
			}

			if( AItems.Seconds().GetValue() )
			{
				if( ASecond < AOtherSecond ) 
					return false;

				if( ASecond > AOtherSecond ) 
					return true;
			}

			if( AItems.MilliSeconds().GetValue() )
			{
				if( AMilliSecond < AOtherMilliSecond ) 
					return false;

				if( AMilliSecond > AOtherMilliSecond ) 
					return true;
			}

			return true;
		}

		template<typename T_ITEMS> bool IsBiggerOrEqual( const TDateTime &AOther, T_ITEMS &AItems ) const
		{
			if( AItems.AllTrue() )
				return ( *this >= AOther );

			return( IsEqual( AOther, AItems ) || IsBigger( AOther, AItems ));
		}

	public:
		bool operator == ( const TDateTime &AOther ) const
		{
			if( Date != AOther.Date )
				return false;

			return ( Time == AOther.Time );
		}

		bool operator != ( const TDateTime &AOther ) const
		{
			if( Date != AOther.Date )
				return true;

			return ( Time != AOther.Time );
		}

		bool operator <= ( const TDateTime &AOther ) const
		{
			if( Date > AOther.Date )
				return false;

			if( Date < AOther.Date )
				return true;

			return ( Time <= AOther.Time );
		}

		bool operator >= ( const TDateTime &AOther ) const
		{
			if( Date > AOther.Date )
				return true;

			if( Date < AOther.Date )
				return false;

			return ( Time >= AOther.Time );
		}

		bool operator < ( const TDateTime &AOther ) const
		{
			if( Date > AOther.Date )
				return false;

			if( Date < AOther.Date )
				return true;

			return ( Time < AOther.Time );
		}

		bool operator > ( const TDateTime &AOther ) const
		{
			if( Date > AOther.Date )
				return true;

			if( Date < AOther.Date )
				return false;

			return ( Time > AOther.Time );
		}

	public:
		TDateTime() :
			Time( 0 ),
			Date( 693594 )
		{
		}

		TDateTime( int32_t ADate, int32_t ATime ) :
			Time( ATime ),
			Date( ADate )
		{
		}

		TDateTime( uint32_t ADummy ) :
			Time( 0 ),
			Date( 693594 )
		{
		}

	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE> class EmptyProperty
	{
	public:
		inline EmptyProperty &GetInstance() { return *this; }

	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T_TYPE, typename T_NESTED> class TypedVariable : public T_NESTED
	{
	public:
		inline TypedVariable &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T_TYPE AValue ) { T_NESTED::SetValue( AValue ); }
		inline operator T_TYPE() const { return T_NESTED::GetValue(); }
		inline T_TYPE GetValue() const { return T_NESTED::GetValue(); }
//		inline T_TYPE &GetReference() const { return T_NESTED::GetReference(); }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_NESTED::SetValue( *( T_TYPE* )_Data );
		}

	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T_TYPE, typename T_NESTED, T_TYPE C_INITIAL_VALUE> class TypedVariableValue : public T_NESTED
	{
	public:
		inline TypedVariableValue &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T_TYPE AValue ) { T_NESTED::SetValue( AValue ); }
		inline operator T_TYPE() const { return T_NESTED::GetValue(); }
		inline T_TYPE GetValue() const { return T_NESTED::GetValue(); }
//		inline T_TYPE &GetReference() const { return T_NESTED::GetReference(); }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			T_NESTED::SetValue( *( T_TYPE* )_Data );
		}

	public:
		TypedVariableValue()
		{
			T_NESTED::SetValue( C_INITIAL_VALUE );
		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T_NESTED> class NestedProperty : public T_NESTED
	{
	public:
		inline NestedProperty &GetInstance() { return *this; }

	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T, T C_VALUE> class VariableProperty
	{
	public:
		typedef T T_TYPE;

	protected:
		T	FValue = C_VALUE;

	public:
//		inline void GetInstance( VariableProperty * &AInstance ) { AInstance = this; }
		inline VariableProperty &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T AValue ) { FValue = AValue; }
		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }

//	public:
//		VariableProperty() {}

//		VariableProperty( T AValue ) :
//			FValue( AValue )
//		{
//		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FValue = *( T* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T, T C_VALUE, T C_MIN> class VariableMinProperty
	{
	public:
		typedef T T_TYPE;

	protected:
		T	FValue = C_VALUE;

	public:
//		inline void GetInstance( VariableProperty * &AInstance ) { AInstance = this; }
		inline VariableMinProperty &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T AValue ) 
		{ 
			if( AValue < C_MIN )
				AValue = C_MIN;

			FValue = AValue; 
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }

//	public:
//		VariableProperty() {}

//		VariableProperty( T AValue ) :
//			FValue( AValue )
//		{
//		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T, T C_VALUE, T C_MAX> class VariableMaxProperty
	{
	public:
		typedef T T_TYPE;

	protected:
		T	FValue = C_VALUE;

	public:
//		inline void GetInstance( VariableProperty * &AInstance ) { AInstance = this; }
		inline VariableMaxProperty &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T AValue ) 
		{ 
			if( AValue > C_MAX )
				AValue = C_MAX;

			FValue = AValue; 
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }

//	public:
//		VariableProperty() {}

//		VariableProperty( T AValue ) :
//			FValue( AValue )
//		{
//		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T, T C_VALUE, T C_MIN, T C_MAX> class VariableMinMaxProperty
	{
	public:
		typedef T T_TYPE;

	protected:
		T	FValue = C_VALUE;

	public:
//		inline void GetInstance( VariableProperty * &AInstance ) { AInstance = this; }
		inline VariableMinMaxProperty &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T AValue ) 
		{ 
			if( AValue < C_MIN )
				AValue = C_MIN;

			else if( AValue > C_MAX )
				AValue = C_MAX;

			FValue = AValue; 
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }

//	public:
//		VariableProperty() {}

//		VariableProperty( T AValue ) :
//			FValue( AValue )
//		{
//		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T, T C_VALUE> class ConstantProperty
	{
//	public:
//		typedef T T_TYPE;

	public:
		inline ConstantProperty &GetInstance() { return *this; }
//		inline void GetInstance( ConstantProperty * &AInstance ) { AInstance = this; }
//		inline void SetValue( T AValue ) {}
		inline void operator = ( T AValue ) {}
		inline operator T() const { return C_VALUE; }
		inline T GetValue() const { return C_VALUE; }

//	public:
//		ConstantProperty() {}
//		ConstantProperty( T AValue ) {}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T, typename T_VALUE> class VariablePropertyFloat
	{
//	public:
//		typedef T T_TYPE;

	protected:
		T	FValue = T_VALUE::GetValue();

	public:
//		inline void GetInstance( VariablePropertyFloat * &AInstance ) { AInstance = this; }
		inline VariablePropertyFloat &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T AValue ) { FValue = AValue; }
		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }

//	public:
//		VariableProperty() {}

//		VariablePropertyFloat( T AValue ) :
//			FValue( AValue )
//		{
//		}
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE> class VariableFieldFloat
	{
//	public:
//		typedef T T_TYPE;

	protected:
		float	FValue;

	public:
//		inline void GetInstance( VariablePropertyFloat * &AInstance ) { AInstance = this; }
		inline VariableFieldFloat &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( float AValue ) { FValue = AValue; }
		inline operator float() const { return FValue; }
		inline float GetValue() const { return FValue; }
		inline float &GetReference() { return FValue; }

//	public:
//		VariableProperty() {}

//		VariablePropertyFloat( T AValue ) :
//			FValue( AValue )
//		{
//		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T, typename T_VALUE, typename T_MIN> class VariableMinPropertyFloat
	{
//	public:
//		typedef T T_TYPE;

	protected:
		T	FValue = T_VALUE::GetValue();

	public:
//		inline void GetInstance( VariablePropertyFloat * &AInstance ) { AInstance = this; }
		inline VariableMinPropertyFloat &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T AValue ) 
		{ 
			if( AValue < T_MIN::GetValue() )
				AValue = T_MIN::GetValue();

			FValue = AValue; 
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }

//	public:
//		VariableProperty() {}

//		VariablePropertyFloat( T AValue ) :
//			FValue( AValue )
//		{
//		}
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T, typename T_VALUE, typename T_MAX> class VariableMaxPropertyFloat
	{
//	public:
//		typedef T T_TYPE;

	protected:
		T	FValue = T_VALUE::GetValue();

	public:
//		inline void GetInstance( VariablePropertyFloat * &AInstance ) { AInstance = this; }
		inline VariableMaxPropertyFloat &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T AValue ) 
		{ 
			if( AValue > T_MAX::GetValue() )
				AValue = T_MAX::GetValue();

			FValue = AValue; 
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }

//	public:
//		VariableProperty() {}

//		VariablePropertyFloat( T AValue ) :
//			FValue( AValue )
//		{
//		}
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T, typename T_VALUE, typename T_MIN, typename T_MAX> class VariableMinMaxPropertyFloat
	{
//	public:
//		typedef T T_TYPE;

	protected:
		T	FValue = T_VALUE::GetValue();

	public:
//		inline void GetInstance( VariablePropertyFloat * &AInstance ) { AInstance = this; }
		inline VariableMinMaxPropertyFloat &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) { FValue = AValue; }
		inline void operator = ( T AValue ) 
		{ 
			if( AValue < T_MIN::GetValue() )
				AValue = T_MIN::GetValue();

			else if( AValue > T_MAX::GetValue() )
				AValue = T_MAX::GetValue();

			FValue = AValue; 
		}

		inline operator T() const { return FValue; }
		inline T GetValue() const { return FValue; }
		inline T &GetReference() { return FValue; }

//	public:
//		VariableProperty() {}

//		VariablePropertyFloat( T AValue ) :
//			FValue( AValue )
//		{
//		}
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( float* )_Data;
		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T, typename C_VALUE> class ConstantPropertyFloat
	{
//	public:
//		typedef T T_TYPE;

	public:
//		inline void GetInstance( ConstantPropertyFloat * &AInstance ) { AInstance = this; }
		inline ConstantPropertyFloat &GetInstance() { return *this; }
//		inline void SetValue( T AValue ) {}
		inline void operator = ( T AValue ) {}
		inline operator T() const { return C_VALUE::GetValue(); }
		inline T GetValue() const { return C_VALUE::GetValue(); }

//	public:
//		ConstantProperty() {}
//		ConstantProperty( T AValue ) {}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T_VALUE> class VariablePropertyString
	{
//	public:
//		typedef Mitov::String T_TYPE;

	protected:
		Mitov::String	FValue;

	public:
//		inline void GetInstance( VariablePropertyString * &AInstance ) { AInstance = this; }
		inline VariablePropertyString &GetInstance() { return *this; }
//		inline void SetValue( const char *AValue ) { FValue = AValue; }
//		inline void SetValue( Mitov::String &AValue ) { FValue = AValue; }
		inline void operator = ( const char *AValue ) { FValue = AValue; }
		inline void operator = ( Mitov::String AValue ) { FValue = AValue; }
//		inline operator const char *() { return FValue.c_str(); }
		inline operator Mitov::String () const { return FValue; }
		inline const char *c_str() const { return FValue.c_str(); }
		inline Mitov::String GetValue() const { return FValue; }
		inline Mitov::String &GetReference() { return FValue; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = ( char * )_Data;
		}

	public:
		inline VariablePropertyString()
		{
			FValue = T_VALUE::GetValue();
		}

	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T_VALUE> class ConstantPropertyString
	{
//	public:
//		typedef Mitov::String T_TYPE;

	public:
//		inline void GetInstance( ConstantPropertyString * &AInstance ) { AInstance = this; }
		inline ConstantPropertyString &GetInstance() { return *this; }
//		inline void SetValue( const char *AValue ) {}
//		inline void SetValue( Mitov::String &AValue ) {}
		inline void operator = ( const char *AValue ) {}
		inline void operator = ( Mitov::String AValue ) {}
//		inline operator const char *() { return T_VALUE::GetValue(); }
		inline bool operator == ( const char * AOther ) { return strcmp( T_VALUE::GetValue(), AOther ) == 0; }
		inline operator Mitov::String () const { return T_VALUE::GetValue(); }
		inline const char *c_str() const { return T_VALUE::GetValue(); }
		inline Mitov::String GetValue() const { return T_VALUE::GetValue(); }

	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T_COLOR, typename T_CONST, T_CONST C_VALUE> class VariablePropertyColor
	{
//	public:
//		typedef Mitov::String T_TYPE;

	protected:
		T_COLOR	FValue;

	public:
//		inline void GetInstance( VariablePropertyString * &AInstance ) { AInstance = this; }
		inline VariablePropertyColor &GetInstance() { return *this; }
		inline void operator = ( T_COLOR AValue ) { FValue = AValue; }
//		inline operator const char *() { return FValue.c_str(); }
		inline operator T_COLOR () const { return FValue; }
		inline T_COLOR GetValue() const { return FValue; }
		inline T_COLOR &GetReference() { return FValue; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( T_COLOR* )_Data;
		}

	public:
		inline VariablePropertyColor() :
			FValue( C_VALUE, true )
		{
		}
	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T_COLOR, typename T_CONST, T_CONST C_VALUE> class ConstantPropertyColor
	{
	public:
		inline ConstantPropertyColor &GetInstance() { return *this; }
		inline void operator = ( T_COLOR AValue ) {}
		inline operator T_COLOR () const { return T_COLOR( C_VALUE, true ); }
		inline T_COLOR GetValue() const { return T_COLOR( C_VALUE, true ); }

	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, int32_t C_DATE, int32_t C_TIME> class VariablePropertyDateTime
	{
	protected:
		TDateTime	FValue;

	public:
		inline VariablePropertyDateTime &GetInstance() { return *this; }
		inline void operator = ( TDateTime AValue ) { FValue = AValue; }
		inline operator TDateTime () const { return FValue; }
		inline TDateTime GetValue() const { return FValue; }
		inline TDateTime &GetReference() { return FValue; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( TDateTime* )_Data;
		}

	public:
		VariablePropertyDateTime () :
			FValue( C_DATE, C_TIME )
		{
		}

	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, int32_t C_DATE, int32_t C_TIME> class ConstantPropertyDateTime
	{
	public:
		inline ConstantPropertyDateTime &GetInstance() { return *this; }
		inline void operator = ( TDateTime AValue ) {}
		inline operator TDateTime () const { return TDateTime( C_DATE, C_TIME ); }
		inline TDateTime GetValue() const { return TDateTime( C_DATE, C_TIME ); }

	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T_REAL, typename T_IMAGINARY> class VariablePropertyComplex
	{
	protected:
		TComplex	FValue;

	public:
		inline VariablePropertyComplex &GetInstance() { return *this; }
		inline void operator = ( TComplex AValue ) { FValue = AValue; }
		inline operator TComplex () const { return FValue; }
		inline TComplex GetValue() const { return FValue; }
		inline TComplex &GetReference() { return FValue; }

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			*this = *( TComplex* )_Data;
		}

	public:
		VariablePropertyComplex () :
			FValue( T_REAL::GetValue(), T_IMAGINARY::GetValue() )
		{
		}

	};
//---------------------------------------------------------------------------
	template<uint32_t C_UNIQUE, typename T_REAL, typename T_IMAGINARY> class ConstantPropertyComplex
	{
	public:
		inline ConstantPropertyComplex &GetInstance() { return *this; }
		inline void operator = ( TComplex AValue ) {}
		inline operator TComplex () const { return TComplex( T_REAL::GetValue(), T_IMAGINARY::GetValue() ); }
		inline TComplex GetValue() const { return TComplex( T_REAL::GetValue(), T_IMAGINARY::GetValue() ); }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T> class NullPropertyArray
	{
	public:
		static constexpr uint32_t GetCount() { return 0; }
		static constexpr const T *GetValue() { return nullptr; }
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T, T const *C_VALUE, uint32_t C_COUNT> class ConstantPropertyArray
	{
	public:
		static constexpr uint32_t GetCount() { return C_COUNT; }
		static constexpr const T *GetValue() { return C_VALUE; }
	};
//---------------------------------------------------------------------------
	template <typename T> struct TArray
	{
	public:
		uint32_t	Size;
		T *Data;

	public:
		bool operator == ( const TArray &other )
		{
			if( Size != other.Size )
				return false;

			return( memcmp( Data, other.Data, Size * sizeof( T ) ) == 0 );
		}

	public:
		Mitov::String ToString( uint32_t AMaxSize = 32 )
		{
//			Serial.println( "ToString" );
			Mitov::String AResult = "(";
			for( uint32_t i = 0; i < MitovMin( Size, uint32_t( AMaxSize ) ); ++i )
			{
				if( i < Size - 1 )
					AResult += Mitov::String( Data[ i ] ) + ",";

				else
					AResult += Mitov::String( Data[ i ] );

			}

			if( Size > AMaxSize )
				AResult += "...";

			return	AResult + ")";
		}

	public:
		TArray( uint32_t ASize, const T *AData ) :
			Size( ASize), 
			Data( (T *)AData )
		{
		}

		TArray( const T &AData ) :
			Size( 1 ), 
			Data( (T *)&AData )
		{
		}
	};
//---------------------------------------------------------------------------
	struct TDataBlock : public TArray<byte>
	{
		typedef TArray<byte> inherited;

	public:
		inline Mitov::String ToString( uint32_t AMaxSize = 32 ) 
		{ 
			return inherited::ToString( AMaxSize ); 
		}

		Mitov::String ToString( uint32_t AMaxSize, uint8_t ABase )
		{
//			Serial.println( "ToString" );
			Mitov::String AResult = "(";
			for( uint32_t i = 0; i < MitovMin( Size, uint32_t( AMaxSize ) ); ++i )
			{
				if( i < Size - 1 )
					AResult += Mitov::String( Data[ i ], ABase ) + ",";

				else
					AResult += Mitov::String( Data[ i ], ABase );

			}

			if( Size > AMaxSize )
				AResult += "...";

			return	AResult + ")";
		}

	public:
		TDataBlock( uint32_t ASize, const void *AData ) :
			inherited( ASize, (byte *)AData )
		{
		}

		TDataBlock() :
			inherited( 0, nullptr )
		{
		}

	};
//---------------------------------------------------------------------------
	template <typename T> struct TValueArray : public TArray<T>
	{
		typedef TArray<T> inherited;

	public:
		TValueArray<T>& operator=(const TArray<T>& other)
		{
			if( inherited::Data )
				delete [] inherited::Data;

			inherited::Size = other.Size;
			inherited::Data = new T[ inherited::Size ];

			for( int i = 0; i < inherited::Size; ++ i )
				inherited::Data[ i ] = other.Data[ i ];

			return *this;
		}

	public:
		void Assign( T *AData, uint32_t ASize )
		{
			delete [] inherited::Data;
			inherited::Data = new T[ ASize ];

			inherited::Size = ASize;

			for( int i = 0; i < ASize; ++i )
				inherited::Data[ i ] = AData[ i ];
		}

		void Copy( T *AData, uint32_t ASize, uint32_t AOffset )
		{
			for( int i = 0; i < MitovMin( ASize, inherited::Size - AOffset ); ++i )
				inherited::Data[ i + AOffset ] = AData[ i ];

		}

		void ResizeFront( uint32_t ASize )
		{
			if( ASize == inherited::Size )
				return;

			T *AOldData = inherited::Data;
			inherited::Data = new T[ ASize ];

			if( ASize > inherited::Size )
			{
				uint32_t AOffset = ASize - inherited::Size;
				for( int i = 0; i < inherited::Size; ++i )
					inherited::Data[ i + AOffset ] = AOldData[ i ];
			}

			else
			{
				uint32_t AOffset = inherited::Size - ASize;
				for( int i = 0; i < inherited::Size; ++i )
					inherited::Data[ i ] = AOldData[ i + AOffset ];
			}

			delete [] AOldData;

			inherited::Size = ASize;
		}

		void ResizeBack( uint32_t ASize, bool APreserveData )
		{
			if( ASize == inherited::Size )
				return;

			T *AOldData = inherited::Data;
			inherited::Data = new T[ ASize ];

			for( int i = 0; i < MitovMin( ASize, inherited::Size ); ++i )
				inherited::Data[ i ] = AOldData[ i ];

			delete [] AOldData;

			inherited::Size = ASize;
		}

		void Resize( uint32_t ASize )
		{
			if( ASize == inherited::Size )
				return;

			delete [] inherited::Data;
			inherited::Data = new T[ ASize ];

			inherited::Size = ASize;
		}

	public:
		TValueArray( T AData ) :
			inherited( 1, new T[ 1 ] )

		{
			*inherited::Data = AData;
		}

		TValueArray( uint32_t ASize, const T *AData ) :
			inherited( ASize, new T[ ASize ] )
		{
			for( int i = 0; i < ASize; ++ i )
				inherited::Data[ i ] = AData[ i ];
		}

		TValueArray( const TArray<T> &AOther ) :
			TValueArray( AOther.Size, AOther.Data )
		{
		}

		TValueArray() :
			inherited( 0, nullptr )
		{
		}

		~TValueArray()
		{
			if( inherited::Data )
				delete [] inherited::Data;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Octet1,
		typename T_Octet2,
		typename T_Octet3,
		typename T_Octet4,
		typename T_Octet5,
		typename T_Octet6
	> class TMACAddress :
		public T_Octet1,
		public T_Octet2,
		public T_Octet3,
		public T_Octet4,
		public T_Octet5,
		public T_Octet6
	{
	public:
		_V_PROP_( Octet1 )
		_V_PROP_( Octet2 )
		_V_PROP_( Octet3 )
		_V_PROP_( Octet4 )
		_V_PROP_( Octet5 )
		_V_PROP_( Octet6 )

	public:
		void GetMacAddress( uint8_t *AMacAddress )
		{
			AMacAddress[ 0 ] = Octet1();
			AMacAddress[ 1 ] = Octet2();
			AMacAddress[ 2 ] = Octet3();
			AMacAddress[ 3 ] = Octet4();
			AMacAddress[ 4 ] = Octet5();
			AMacAddress[ 5 ] = Octet6();
		}

	public:
		bool IsZero()
		{
			if( Octet1() )
				return false;

			if( Octet2() )
				return false;

			if( Octet3() )
				return false;

			if( Octet4() )
				return false;

			if( Octet5() )
				return false;

			if( Octet6() )
				return false;

			return true;
		}

	};
//---------------------------------------------------------------------------
	template<typename T> inline T GetDefaultValue() { return 0; }
	template<> inline bool GetDefaultValue() { return false; }
	template<> inline Mitov::String GetDefaultValue() { return ""; }
	template<> inline TDateTime GetDefaultValue() { return TDateTime(); }
	template<> inline TColor GetDefaultValue() { return TColor(); }
	template<> inline TRGBWColor GetDefaultValue() { return TRGBWColor(); }
//---------------------------------------------------------------------------
	template<typename T> inline T ExtractValue( void *_Data ) { return *( (T *)_Data ); }
	template<> inline Mitov::String ExtractValue( void *_Data ) { return ( (char *)_Data ); }
//---------------------------------------------------------------------------
	template <typename T> void swap ( T& a, T& b )
	{
		T c(a); a=b; b=c;
	}
//---------------------------------------------------------------------------
	class Bytes
	{
	public:
		uint8_t *_Bytes;
		unsigned int  _BytesSize = 0;

	public:
		inline uint32_t GetSize() { return _BytesSize; }

	public:
		bool operator == ( const Bytes &AOther )
		{
			if( _BytesSize != AOther._BytesSize )
				return false;

			return ( memcmp( _Bytes, AOther._Bytes, _BytesSize ) == 0 );
		}
	};
//---------------------------------------------------------------------------
	class ConstBytes
	{
	public:
		uint8_t const * _Bytes;
		unsigned int  _BytesSize = 0;

	public:
		inline uint32_t GetSize() { return _BytesSize; }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class ValueSource : 
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		inline T GetCurrentValue() { return Value(); }

	public:
		void SetValue( T AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}

		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			if( Value() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( Value().GetValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( Value().GetValue() );
		}

	};
//---------------------------------------------------------------------------
/*
	template<
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class BindableValueSource : public Mitov::ValueSource<
			T_Elements_TryActive,
			T_OutputPin,
			T,
			T_Value
		>
	{
		typedef Mitov::ValueSource<
			T_Elements_TryActive,
			T_OutputPin,
			T,
			T_Value
		> inherited;

	protected:
		T OldValue;

	public:
		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			inherited::ExternalSetValue( AIndex, AValue );
			OldValue = inherited::Value();
		}

	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
			OldValue = inherited::Value();
		}

		inline void SystemLoopBegin()
		{
			if( inherited::Value() == OldValue )
				return;

			OldValue = inherited::Value();
			inherited::OutputPin.Notify( &OldValue );
		}

	public:
		BindableValueSource( T AValue ) :
			inherited( AValue ),
			OldValue( AValue )
		{
		}

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_FActiveElement,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class ValueSourceRamp : 
		public T_OutputPin,
		public T_Value,
		public T_FActiveElement
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		_V_PROP_( FActiveElement )

	public:
		inline bool GetCurrentValue() { return Value(); }

	public:
		void SetValue( T AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			if( FActiveElement() == -1 )
				T_OutputPin::SetPinValue( AValue );

		}

		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			if( AIndex != FActiveElement() )
				FActiveElement() = -1;

//			inherited::ExternalSetValue( AIndex, AValue );
			if( Value() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}

	public:
		void RegisterForTimeUpdate( int32_t AIndex, bool ARegister )
		{
			if( ARegister )
				FActiveElement() = AIndex;

			else if( FActiveElement() == AIndex )
				FActiveElement() = -1;

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( Value().GetValue() );
		}

	public:
		inline void SystemStart() 
		{
			T_OutputPin::SetPinValue( Value().GetValue() );
		}

	public:
		inline ValueSourceRamp()
		{
			FActiveElement() = -1;
		}
	};
//---------------------------------------------------------------------------
/*
	template<
		typename T_Elements_TryActive,
		typename T_FActiveElement,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class BindableValueRampSource : public Mitov::ValueRampSource<
			T_Elements_TryActive,
			T_FActiveElement,
			T_OutputPin,
			T,
			T_Value
		>
	{
		typedef Mitov::ValueRampSource<
			T_Elements_TryActive,
			T_FActiveElement,
			T_OutputPin,
			T,
			T_Value
		> inherited;

	protected:
		T OldValue;

	public:
		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			inherited::ExternalSetValue( AIndex, AValue );
			OldValue = inherited::Value;
		}

	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
			OldValue = inherited::Value;
		}

		inline void SystemLoopBegin()
		{
			if( inherited::Value == OldValue )
				return;

			OldValue = inherited::Value;
			inherited::OutputPin.Notify( &OldValue );
		}

	public:
		BindableValueRampSource( T AValue ) :
			inherited( AValue ),
			OldValue( AValue )
		{
		}

	};
*/
//---------------------------------------------------------------------------
/*
	class BasicValueStateElement
	{
	public:
		virtual bool TryActive() = 0;

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_FStateActive,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class ValueSourceState :
		public T_OutputPin,
		public T_Value,
		public T_FStateActive
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		_V_PROP_( FStateActive )

	protected:
		T FCurrentValue;

	public:
		inline T GetCurrentValue() { return FCurrentValue; }

	public:
		void SetValue( T AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			if( FStateActive() )
				T_OutputPin::SetPinValue( AValue );

		}

		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::ChainCall( AResult );
			if( AResult )
				return;

			FCurrentValue = Value();
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = Value();
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline ValueSourceState()
		{
			FStateActive() = false;
		}
	};
//---------------------------------------------------------------------------
/*
	template<
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class BindableValueStateSource : public Mitov::ValueStateSource<
			T_Elements_TryActive,
			T_OutputPin,
			T,
			T_Value
		>
	{
		typedef Mitov::ValueStateSource<
			T_Elements_TryActive,
			T_OutputPin,
			T,
			T_Value
		> inherited;

	protected:
		T OldValue;

	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
			OldValue = inherited::Value;
		}

		inline void SystemLoopBegin()
		{
			if( inherited::Value == OldValue )
				return;

			OldValue = inherited::Value;
			inherited::OutputPin.Notify( &OldValue );
		}

	public:
		BindableValueStateSource( T AValue ) :
			inherited( AValue ),
			OldValue( AValue )
		{
		}

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_FActiveElement,
		typename T_FStateActive,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class ValueSourceStateRamp : 
		public T_OutputPin,
		public T_Value,
		public T_FStateActive,
		public T_FActiveElement
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		_V_PROP_( FStateActive )

	public:
		_V_PROP_( FActiveElement )

	protected:
		T FCurrentValue;

	public:
		inline T GetCurrentValue() { return FCurrentValue; }

	public:
		void SetValue( T AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			if( ! FStateActive() )
				return;

			if( FActiveElement() != -1 )
				return;

			T_OutputPin::SetPinValue( AValue );
		}

		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			if( AIndex != FActiveElement() )
				FActiveElement() = -1;

			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::ChainCall( AResult );
			if( AResult )
				return;

			FCurrentValue = Value();
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		void RegisterForTimeUpdate( int32_t AIndex, bool ARegister )
		{
			if( ARegister )
				FActiveElement() = AIndex;

			else if( FActiveElement() == AIndex )
				FActiveElement() = -1;

		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = Value();
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		ValueSourceStateRamp()
		{
			FActiveElement() = -1;
		}

	};
//---------------------------------------------------------------------------
/*
	template<
		typename T_Elements_TryActive,
		typename T_FActiveElement,
		typename T_OutputPin,
		typename T,
		typename T_Value
	> class BindableValueRampStateSource : public Mitov::ValueRampStateSource<
			T_Elements_TryActive,
			T_FActiveElement,
			T_OutputPin,
			T,
			T_Value
		>
	{
		typedef Mitov::ValueRampStateSource<
			T_Elements_TryActive,
			T_FActiveElement,
			T_OutputPin,
			T,
			T_Value
		> inherited;

	protected:
		T OldValue;

	public:
		void ExternalSetValue( int32_t AIndex, T AValue )
		{
			inherited::ExternalSetValue( AIndex, AValue );
			OldValue = inherited::Value;
		}

	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
			OldValue = inherited::Value;
		}

		inline void SystemLoopBegin()
		{
			if( inherited::Value == OldValue )
				return;

			OldValue = inherited::Value;
			inherited::OutputPin.Notify( &OldValue );
		}

	public:
		BindableValueRampStateSource( T AValue ) :
			inherited( AValue ),
			OldValue( AValue )
		{
		}

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T_Value
	> class ValueSourceDigital : 
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		inline bool GetCurrentValue() { return Value(); }

	public:
		void SetValue( bool AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}

		void ExternalSetValue( int32_t AIndex, bool AValue )
		{
			if( Value() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( Value() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( Value() );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_FCurrentValue,
		typename T_FStateActive,
		typename T_OutputPin,
		typename T_Value
	> class ValueSourceDigitalState :
		public T_OutputPin,
		public T_Value,
		public T_FStateActive,
		public T_FCurrentValue
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		_V_PROP_( FStateActive )

	protected:
		_V_PROP_( FCurrentValue )

	public:
		inline bool GetCurrentValue() { return FCurrentValue(); }

	public:
		void SetValue( bool AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			if( FStateActive() )
				T_OutputPin::SetPinValue( AValue );

		}

		void ExternalSetValue( int32_t AIndex, bool AValue )
		{
			if( FCurrentValue() == AValue )
				return;

			FCurrentValue() = AValue;
			T_OutputPin::SetPinValue( FCurrentValue() );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::ChainCall( AResult );
			if( AResult )
				return;

			FCurrentValue() = Value();
			T_OutputPin::SetPinValue( FCurrentValue() );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue() = Value();
			T_OutputPin::SetPinValue( FCurrentValue() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue() );
		}

	public:
		inline ValueSourceDigitalState()
		{
			FStateActive() = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_OutputPin
	> class BinaryValueSource : 
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		Mitov::Bytes	Value;

	public:
		inline Mitov::Bytes GetCurrentValue() { return Value; }

	public:
		void ExternalSetValue( Mitov::Bytes &AValue )
		{
			if( Value == AValue )
				return;

			Value = AValue;
			T_OutputPin::SendPinValue( Mitov::TDataBlock( Value._BytesSize, Value._Bytes ));
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SendPinValue( Mitov::TDataBlock( Value._BytesSize, Value._Bytes ));
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SendPinValue( Mitov::TDataBlock( Value._BytesSize, Value._Bytes ));
		}

	};
//---------------------------------------------------------------------------
/*
	template<typename T_OWNER, T_OWNER &C_OWNER> class ArduinoBinaryStateSetValueElementBasic
	{
	protected:
		bool	FInputValue = false;

	public:
		Mitov::Bytes	Value;

	public:
		inline Mitov::Bytes GetCurrentValue() { return Value; }

	public:
		bool TryActive( T_OWNER *AOwner )
		{
			if( FInputValue )
			{
				C_OWNER.ExternalSetValue( Value );
				return true;
			}

			return false;
		}

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FInputValue
	> class ArduinoBinaryStateSetValueElement : // : public ArduinoBinaryStateSetValueElementBasic<T_OWNER, C_OWNER>
		public T_FInputValue
	{
//		typedef ArduinoBinaryStateSetValueElementBasic<T_OWNER> inherited;

	protected:
		_V_PROP_( FInputValue )

	public:
		Mitov::Bytes	Value;

	public:
		inline Mitov::Bytes GetCurrentValue() { return Value; }

	public:
		inline void TryActive( bool &AResult )
		{
			if( AResult )
				return;

			if( FInputValue() )
			{
				C_OWNER.ExternalSetValue( Value );
				AResult = true;
			}
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( FInputValue == *(bool *)_Data )
				return;

			FInputValue = *(bool *)_Data;
			C_OWNER.ResetValue();
		}

//	public:
//		ArduinoBinaryStateSetValueElement()
//		{
//			C_OWNER.RegisterElement( this );
//		}

	public:
		ArduinoBinaryStateSetValueElement()
		{
			FInputValue() = false;
		}
	};
//---------------------------------------------------------------------------
    template<
		typename T_Elements_TryActive,
		typename T_OutputPin
	> class BinaryStateValueSource : 
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		Mitov::Bytes	Value;

	protected:
		Mitov::Bytes	*FCurrentValue = &Value;

	public:
		inline Mitov::Bytes *GetCurrentValue() { return FCurrentValue; }

	public:
		void ExternalSetValue( Mitov::Bytes &AValue )
		{
			if( *FCurrentValue == AValue )
				return;

			FCurrentValue = &AValue;
			T_OutputPin::SendPinValue( Mitov::TDataBlock( FCurrentValue->_BytesSize, FCurrentValue->_Bytes ));
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::ChainCall( AResult );
			if( AResult )
				return;

			FCurrentValue = &Value;
			T_OutputPin::SendPinValue( Mitov::TDataBlock( FCurrentValue->_BytesSize, FCurrentValue->_Bytes ));
		}

//		void RegisterElement( ArduinoBinaryStateSetValueElementBasic<BinaryStateValueSource> *AValue )
//		{
//			FElements.push_back( AValue );
//		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SendPinValue( Mitov::TDataBlock( FCurrentValue->_BytesSize, FCurrentValue->_Bytes ));
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SendPinValue( Mitov::TDataBlock( FCurrentValue->_BytesSize, FCurrentValue->_Bytes ));
		}

	};
//---------------------------------------------------------------------------
	typedef void (OpenWire::Object::*TNotifyEvent) ();
//---------------------------------------------------------------------------
	template<
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class DigitalInput_Flexible :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemStart() {} // Placeholder

		inline void SystemInit()
		{
			pinMode(C_PIN_NUMBER, INPUT);
		}

		inline void SystemLoopBegin()
		{
			T_OutputPin::SetPinValue( digitalRead( C_PIN_NUMBER ) );
		}

		inline void SystemLoopUpdateHardware() {} // Placeholder

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template<
//		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class DigitalInput :
//		public T_IsOutput,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemStart() {} // Placeholder

		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT );
		}

		inline void SystemLoopBegin()
		{
			bool AValue = digitalRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopUpdateHardware() {} // Placeholder

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template<
//		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class DigitalPullUpInput :
//		public T_IsOutput,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT_PULLUP );
		}

		inline void SystemStart()
		{
			bool AValue = digitalRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopBegin()
		{
			SystemStart();
		}

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template<
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class SimpleDigitalInput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemStart() {} // Placeholder

		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT );
		}

		inline void SystemLoopBegin()
		{
			bool AValue = digitalRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopUpdateHardware() {} // Placeholder

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template<
		int C_PIN_NUMBER,
		typename T_OutputPin
	> class SimpleDigitalPullUpInput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT_PULLUP );
		}

		inline void SystemStart()
		{
			bool AValue = digitalRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopBegin()
		{
			SystemStart();
		}

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template<
//		typename T_IsOutput,
//		typename T_IsPullDown,
//		typename T_IsPullUp,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class InvertedDigitalPullUpInput :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT_PULLUP );
		}

		inline void SystemStart()
		{
			bool AValue = ! digitalRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemLoopBegin()
		{
			SystemStart();
		}

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

	};
//---------------------------------------------------------------------------
	template<int C_PIN_NUMBER, typename T_OutputPin> class AnalogAsDigitalInput : public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		void SetMode( int AMode ) {} // Placeholder

		bool DigitalRead()
		{
			int AValue = analogRead( C_PIN_NUMBER );
			return ( AValue > VISUINO_ANALOG_IN_THRESHOLD ); // > 2.0 V High < 0.8 V Low
		}

	public:
		inline void SystemLoopBegin()
		{
			bool AValue = DigitalRead();
			T_OutputPin::SetPinValue( AValue );
		}

		inline void SystemInit() {} // Placeholder
		inline void SystemStart() {} // Placeholder

	};
//---------------------------------------------------------------------------
	template<int C_PIN_NUMBER, typename T_OutputPin> class ArduinoAnalogInputChannel : public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
//		float			FLastOutput = 0.0f;

#ifdef VISUINO_ESP8266
		unsigned int	FLastTime;
#endif // VISUINO_ESP8266

#ifdef VISUINO_TEENSY
	protected:
		unsigned int	FResolution = 0;
		float			FDivider = 1023.0;

	public:
		void SetResolution( unsigned int AResolution )
		{
			if( AResolution == FResolution )
				return;

			FResolution = AResolution;
			analogReadRes( FResolution );
			CalculateMultiplier();
		}

	protected:
		void CalculateMultiplier()
		{
			FDivider = pow( 2, FResolution ) - 1;
		}

#endif // VISUINO_TEENSY

	public:
		inline void SystemInit() {} // Placeholder

		inline void SystemLoopUpdateHardware() {} // Placeholder

		inline void SystemStart()
		{
#ifdef VISUINO_TEENSY
			analogReadRes( FResolution );
			float ALastOutput = analogRead( C_PIN_NUMBER ) / float( FDivider );
#else // VISUINO_TEENSY
			float ALastOutput = analogRead( C_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE );
#endif // VISUINO_TEENSY

#ifdef VISUINO_ESP8266
			FLastTime = micros();
#endif // VISUINO_ESP8266
	    	T_OutputPin::SetPinValue( ALastOutput, false );
		}

		inline void SystemLoopBegin()
		{
#ifdef VISUINO_ESP8266
			unsigned long ACurrentMicros = micros();
			if( ACurrentMicros - FLastTime < 10000 )
				return;

			FLastTime = ACurrentMicros;
#endif // VISUINO_ESP8266
#ifdef VISUINO_TEENSY
			float AValue = analogRead( C_PIN_NUMBER ) / float( FDivider );
#else // VISUINO_TEENSY
			float AValue = analogRead( C_PIN_NUMBER ) / float( VISUINO_ANALOG_IN_RANGE );
#endif // VISUINO_TEENSY
//			if( AValue == FLastOutput )
//				return;

//			FLastOutput = AValue;
			T_OutputPin::SetPinValue( AValue, true );
		}

	public:
		ArduinoAnalogInputChannel()
#ifdef VISUINO_TEENSY
			: FResolution( 1023 )
#endif // VISUINO_TEENSY
		{
		}

#ifdef VISUINO_TEENSY
		ArduinoAnalogInputChannel( unsigned int AResolution ) :
			FResolution( AResolution )
		{
			CalculateMultiplier();
		}

#endif // VISUINO_TEENSY
	};
//---------------------------------------------------------------------------
#if ( defined( VISUINO_TEENSY ) || defined( VISUINO_ESP32 ))
//---------------------------------------------------------------------------
	template<
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class ArduinoTouchAnalogInputChannel :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		inline void SystemInit() {} // Form compatibility!

		inline void SystemStart()
		{
			float AValue = touchRead( C_PIN_NUMBER );
	    	T_OutputPin::SetPinValue( AValue, false );
		}

		inline void SystemLoopBegin()
		{
			float AValue = touchRead( C_PIN_NUMBER );
			T_OutputPin::SetPinValue( AValue, true );
		}

	};
//---------------------------------------------------------------------------
/*
	template <int V_PIN> void SetArduinoAnalogInputResolutionChannel( unsigned int AValue )
	{
		analogReadRes( AValue );
	}
*/
//---------------------------------------------------------------------------
#endif // VISUINO_TEENSY
//---------------------------------------------------------------------------
	template<int C_PIN_NUMBER> class AnalogOutput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float*)_Data;
			analogWrite( C_PIN_NUMBER, ( AValue * PWMRANGE ) + 0.5 );
		}

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, OUTPUT);
		}

	};
//---------------------------------------------------------------------------
    template<
		typename T_InitialValue, 
		typename T_IsOpenDrain,
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp, 
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class ArduinoDigitalInputChannel :
		public T_InitialValue, 
		public T_IsOpenDrain
	{
	public:
		_V_PROP_( IsOpenDrain )
		_V_PROP_( InitialValue )
/*
	public:
		class DigitalPin : public OpenWire::ReceivePin
		{
//		public:
//			uint8_t FPinNumber;

		protected:
			virtual void Receive( void *_Data ) override
			{
				bool AValue = *(bool *)_Data;
				digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
			}
		};

	public:
		DigitalPin	DigitalInputPin;
*/
/*
	protected:
		virtual void SystemInit() override
		{
//			inherited::SystemInit();
			pinMode( DigitalInputPin.FPinNumber, OUTPUT );
		}
*/
	public:
		inline void SystemInit() {} // Placeholder
		inline void SystemStart() {} // Placeholder
		inline void SystemLoopBegin() {} // Placeholder

	public:
		inline void UpdatePinOutputValue()
		{
			digitalWrite( C_PIN_NUMBER, InitialValue().GetValue() ? HIGH : LOW );
		}

	public:
		inline void DigitalInputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
			InitialValue() = AValue;
		}

	public:
		inline ArduinoDigitalInputChannel()
		{
			pinMode( C_PIN_NUMBER, ( IsOpenDrain() ) ? OUTPUT_OPEN_DRAIN : OUTPUT );
			digitalWrite( InitialValue().GetValue(), InitialValue().GetValue() ? HIGH : LOW );
		}
	};
//---------------------------------------------------------------------------
    template<
		typename T_IsAnalog,
		typename T_IsCombinedInOut,
		typename T_IsOpenDrain,
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp, 
		typename T_IsRawInput,
		typename T_OutputPin,
		int C_PIN_NUMBER
	> class ArduinoDigitalOutputChannel : // : public BasicPinRead
		public T_OutputPin,
		public T_IsPullUp,
		public T_IsPullDown
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		_V_PROP_( IsPullUp )
		_V_PROP_( IsPullDown )
//		_V_PROP_( InitialValue )
//		bool FIsPullDown : 1;
//		bool FLastOutput : 1;

	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, IsPullUp() ? INPUT_PULLUP : ( IsPullDown() ? INPUT_PULLDOWN : INPUT ) );
		}

		inline void SystemStart()
		{
			bool ALastOutput = ( digitalRead( C_PIN_NUMBER ) == HIGH );
	    	T_OutputPin::SetPinValue( ALastOutput, false );
		}

		inline void SystemLoopBegin()
        {
    		bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
//			if( AValue == FLastOutput )
//				return;

//			FLastOutput = AValue;
	    	T_OutputPin::SetPinValue( AValue, true );
        }

	public:
		inline void UpdatePinOutputValue()
		{
			digitalWrite( C_PIN_NUMBER, HIGH );
		}

	public:
        inline void SetIsOutput( bool AValue ) {} // Placeholder
        inline void SetIsAnalog( bool AValue ) {} // Placeholder
        inline void SetIsOpenDrain( bool AValue ) {} // Placeholder

        void SetIsPullUp( bool AValue )
        {
            if( IsPullUp() == AValue )
                return;

            IsPullUp() = AValue;
            SystemInit();
        }

        void SetIsPullDown( bool AValue )
        {
            if( IsPullDown() == AValue )
                return;

            IsPullDown() = AValue;
            SystemInit();
        }

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

/*
	public:
		inline ArduinoDigitalOutputChannel( bool AIsPullUp, bool AIsPullDown )
		{
			IsPullUp() = AIsPullUp;
			IsPullDown() = AIsPullDown;
		}
*/
	};
//---------------------------------------------------------------------------
    template<
		typename T_DigitalOutputPin,
		int C_PIN_NUMBER
	> class ArduinoAnalogAsDigitalOutputChannel :
		public T_DigitalOutputPin
	{
	public:
		inline void SystemInit()
		{
			pinMode( C_PIN_NUMBER, INPUT );
		}

		inline void SystemStart()
		{
			bool ALastOutput = ( digitalRead( C_PIN_NUMBER ) == HIGH );
	    	T_DigitalOutputPin::SetPinValue( ALastOutput, false );
		}

		inline void SystemLoopBegin()
        {
    		bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
//			if( AValue == FLastOutput )
//				return;

//			FLastOutput = AValue;
	    	T_DigitalOutputPin::SetPinValue( AValue, true );
        }

	public:
		inline void SetMode( int AMode )
		{
#if defined( VISUINO_FREESOC2 ) || defined( VISUINO_STMDUINO )
			pinMode( C_PIN_NUMBER, (PIN_MODE)AMode );
#else
			pinMode( C_PIN_NUMBER, AMode );
#endif
		}

		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

		inline void DigitalWrite( bool AValue )
		{
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_InitialValue, 
		typename T_IsAnalog,
		typename T_IsCombinedInOut,
		typename T_IsOpenDrain,
		typename T_IsOutput,
		typename T_IsPullDown,
		typename T_IsPullUp, 
		typename T_IsRawInput,
		typename T_OutputPin, 
		int C_PIN_NUMBER
	> class ArduinoDigitalChannel :
		public T_InitialValue, 
		public T_IsAnalog,
		public T_IsCombinedInOut,
		public T_IsOpenDrain,
		public T_OutputPin,
		public T_IsPullDown,
		public T_IsPullUp,
		public T_IsRawInput,
		public T_IsOutput
    {
//	public:
//		OpenWire::SourcePin	OutputPin;

	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( IsAnalog )
		_V_PROP_( IsCombinedInOut )
		_V_PROP_( IsOpenDrain )
		_V_PROP_( IsPullUp )
		_V_PROP_( IsPullDown )
		_V_PROP_( IsRawInput )
		_V_PROP_( IsOutput )
		_V_PROP_( InitialValue )

	protected:
//        bool FIsOutput : 1;
//        bool FIsPullUp : 1;
//        bool FIsPullDown : 1;
//        bool FIsOpenDrain : 1;
//        bool FIsAnalog : 1;
//		bool FLastOutput : 1;
//		bool FRawInput : 1;
//		bool FCombinedInOut : 1;

	protected:
        void PinDirectionsInit()
        {
//			if( IsRawInput() )
//				return;

            if( IsOutput() )
			{
				pinMode( C_PIN_NUMBER, ( IsOpenDrain().GetValue() ) ? OUTPUT_OPEN_DRAIN : OUTPUT );
				if( ! IsAnalog().GetValue() )
					digitalWrite( C_PIN_NUMBER, InitialValue().GetValue() ? HIGH : LOW );
			}

            else
				pinMode( C_PIN_NUMBER, ( IsPullUp().GetValue() ) ? INPUT_PULLUP : ( IsPullDown() ? INPUT_PULLDOWN : INPUT ) );
        }

	public:
		inline void SystemInit()
		{
            PinDirectionsInit();
		}

		inline void SystemStart()
		{
//			if( IsRawInput() )
//				return;
            if( ! IsOutput().GetValue() )
				if( T_OutputPin::GetPinIsConnected() )
				{
					bool ALastOutput = ( digitalRead( C_PIN_NUMBER ) == HIGH );
	    			T_OutputPin::SetPinValue( ALastOutput, false );
				}

		}

		inline void SystemLoopBegin()
        {
            if( ! IsOutput().GetValue() )
            {
    			bool AValue = ( digitalRead( C_PIN_NUMBER ) == HIGH );
//				if( AValue == FLastOutput )
//					return;

//				FLastOutput = AValue;
//	Serial.println( AData.Value );
	    		T_OutputPin::SetPinValue( AValue, true );
            }
        }

	public:
		inline void UpdatePinOutputValue()
		{
			digitalWrite( C_PIN_NUMBER, InitialValue().GetValue() ? HIGH : LOW );
		}

	public:
		inline void AnalogInputPin_o_Receive( void *_Data )
        {
            if( IsOutput().GetValue() && IsAnalog().GetValue() )
            {
                float AValue = *(float*)_Data;
                analogWrite( C_PIN_NUMBER, ( AValue * PWMRANGE ) + 0.5 );
            }
        }

		inline void DigitalInputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool *)_Data;
			InitialValue() = AValue;
			if( IsRawInput() )
			{
				digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
//				*((int*)_Data) = C_PIN_NUMBER;
				return;
			}

            if( ( IsCombinedInOut().GetValue() || IsOutput().GetValue() ) && (! IsAnalog().GetValue() ))
            {
				if( AValue )
				  if( IsCombinedInOut() )
					  pinMode( C_PIN_NUMBER, OUTPUT );

                digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );

				if( ! AValue )
					if( IsCombinedInOut() )
						pinMode( C_PIN_NUMBER, IsPullUp() ? INPUT_PULLUP : ( IsPullDown() ? INPUT_PULLDOWN : INPUT ) );

            }
		}

	public:
        inline void SetIsOutput( bool AValue )
        {
            if( IsOutput().GetValue() == AValue )
                return;

            IsOutput() = AValue;
            PinDirectionsInit();
        }

        inline void SetIsPullUp( bool AValue )
        {
            if( IsPullUp().GetValue() == AValue )
                return;

            IsPullUp() = AValue;
            PinDirectionsInit();
        }

        inline void SetIsPullDown( bool AValue )
        {
            if( IsPullDown().GetValue() == AValue )
                return;

            IsPullDown() = AValue;
            PinDirectionsInit();
        }

        inline void SetIsAnalog( bool AValue )
        {
            if( IsAnalog().GetValue() == AValue )
                return;

            IsAnalog() = AValue;
            PinDirectionsInit();
        }

        inline void SetIsOpenDrain( bool AValue )
        {
            if( IsOpenDrain().GetValue() == AValue )
                return;

            IsOpenDrain() = AValue;
            PinDirectionsInit();
        }

/*
	public:
		ArduinoDigitalChannel( bool AIsOutput, bool AIsPullUp, bool AIsPullDown, bool AIsOpenDrain, bool AIsAnalog, bool ACombinedInOut, bool ARawInput ) // :
//            FIsOutput( AIsOutput ),
//            FIsPullUp( AIsPullUp ),
//			FIsPullDown( AIsPullDown ),
//			FIsOpenDrain( AIsOpenDrain ),
//            FIsAnalog( AIsAnalog ),
//			FRawInput( ARawInput ),
//			FCombinedInOut( ACombinedInOut )
		{
			IsPullUp() = AIsPullUp;
			IsOutput() = AIsOutput;
			IsPullDown() = AIsPullDown;
			IsOpenDrain() = AIsOpenDrain;
			IsAnalog() = AIsAnalog;
			IsRawInput() = ARawInput;
			IsCombinedInOut() = ACombinedInOut;
		}
*/
	};
//---------------------------------------------------------------------------
    template<int C_PIN_NUMBER> class ArduinoAnalogOutputChannel
    {
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
            float AValue = *(float*)_Data;
            analogWrite( C_PIN_NUMBER, ( AValue * PWMRANGE ) + 0.5 );
		}

	};
//---------------------------------------------------------------------------
	template<int PIN_NUMBER> class DirectOuputPin
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			digitalWrite( PIN_NUMBER, ( *(bool*)_Data ) ? HIGH : LOW );
		}

	public:
		inline DirectOuputPin( bool AInitialValue = false )
		{
			pinMode( PIN_NUMBER, OUTPUT );
			digitalWrite( PIN_NUMBER, ( AInitialValue ) ? HIGH : LOW );
		}
	};
//---------------------------------------------------------------------------
	template<
		int PIN_1, 
		int PIN_2,
		typename T_OutputPins_0,
		typename T_OutputPins_1
	> class DigtitalInputPins2 :
		public T_OutputPins_0,
		public T_OutputPins_1
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )

	public:
		inline void SystemInit()
		{
//			pinMode( PIN_1, INPUT );
//			pinMode( PIN_2, INPUT );
		}

		inline void SystemStart()
		{
			T_OutputPins_0::SetPinValue( digitalRead( PIN_1 ) == HIGH , false );
			T_OutputPins_1::SetPinValue( digitalRead( PIN_2 ) == HIGH , false );
		}

		inline void SystemLoopBegin()
		{
			T_OutputPins_0::SetPinValue( digitalRead( PIN_1 ) == HIGH , true );
			T_OutputPins_1::SetPinValue( digitalRead( PIN_2 ) == HIGH , true );
		}
	};
//---------------------------------------------------------------------------
	template<
		int PIN_1, 
		int PIN_2,
		typename T_OutputPins_0,
		typename T_OutputPins_1
	> class InvertedDigtitalInputPins2 :
		public T_OutputPins_0,
		public T_OutputPins_1
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )

	public:
		inline void SystemInit()
		{
//			pinMode( PIN_1, INPUT );
//			pinMode( PIN_2, INPUT );
		}

		inline void SystemStart()
		{
			T_OutputPins_0::SetPinValue( digitalRead( PIN_1 ) != HIGH , false );
			T_OutputPins_1::SetPinValue( digitalRead( PIN_2 ) != HIGH , false );
		}

		inline void SystemLoopBegin()
		{
			T_OutputPins_0::SetPinValue( digitalRead( PIN_1 ) != HIGH , true );
			T_OutputPins_1::SetPinValue( digitalRead( PIN_2 ) != HIGH , true );
		}
	};
//---------------------------------------------------------------------------
	template<
		int PIN_1, 
		int PIN_2,
		int PIN_3,
		int PIN_4,
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2,
		typename T_OutputPins_3
	> class InvertedDigtitalInputPins4 :
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_OutputPins_2,
		public T_OutputPins_3
	{
	public:
		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )
		_V_PIN_( OutputPins_3 )

	public:
		inline void SystemInit()
		{
//			pinMode( PIN_1, INPUT );
//			pinMode( PIN_2, INPUT );
//			pinMode( PIN_3, INPUT );
//			pinMode( PIN_4, INPUT );
		}

		inline void SystemStart()
		{
			T_OutputPins_0::SetPinValue( digitalRead( PIN_1 ) != HIGH , false );
			T_OutputPins_1::SetPinValue( digitalRead( PIN_2 ) != HIGH , false );
			T_OutputPins_2::SetPinValue( digitalRead( PIN_3 ) != HIGH , false );
			T_OutputPins_3::SetPinValue( digitalRead( PIN_4 ) != HIGH , false );
		}

		inline void SystemLoopBegin()
		{
			T_OutputPins_0::SetPinValue( digitalRead( PIN_1 ) != HIGH , true );
			T_OutputPins_1::SetPinValue( digitalRead( PIN_2 ) != HIGH , true );
			T_OutputPins_2::SetPinValue( digitalRead( PIN_3 ) != HIGH , true );
			T_OutputPins_3::SetPinValue( digitalRead( PIN_4 ) != HIGH , true );
		}
	};
//---------------------------------------------------------------------------
	template<int C_PIN_NUMBER>	class TPinAccess
	{
	public:
		inline bool DigitalRead()
		{
			return digitalRead( C_PIN_NUMBER );
		}

	};
//---------------------------------------------------------------------------
	template<
		int PIN_1,
		int PIN_2,
		typename T_OutputPins0,
		typename T_OutputPins1
	> class DigtitalInputChangeOnlyPins2 :
		public T_OutputPins0,
		public T_OutputPins1 //, public TPinAccess<PIN_1>, public TPinAccess<PIN_2>
	{
	public:
		typedef TPinAccess<PIN_1> T_PIN_TYPE_0;
		typedef TPinAccess<PIN_2> T_PIN_TYPE_1;

	public:
		static T_PIN_TYPE_0 PinObject0;
		static T_PIN_TYPE_1 PinObject1;

	public:
		_V_PIN_( OutputPins0 )
		_V_PIN_( OutputPins1 )

	public:
		inline void SystemInit()
		{
//			pinMode( PIN_1, INPUT );
//			pinMode( PIN_2, INPUT );
		}

		inline void SystemStart()
		{
			T_OutputPins0::SetPinValue( digitalRead( PIN_1 ) != LOW, false );
			T_OutputPins1::SetPinValue( digitalRead( PIN_2 ) != LOW, false );
		}

		inline void SystemLoopBegin()
		{
			T_OutputPins0::SetPinValue( digitalRead( PIN_1 ) != LOW, true );
			T_OutputPins1::SetPinValue( digitalRead( PIN_2 ) != LOW, true );
		}
	};
//---------------------------------------------------------------------------
	template<
		int PIN_1,
		int PIN_2,
		typename T_OutputPins0,
		typename T_OutputPins1
	> class InvertedDigtitalInputChangeOnlyPins2 :
		public T_OutputPins0, 
		public T_OutputPins1 //, public TPinAccess<PIN_1>, public TPinAccess<PIN_2>
	{
	public:
		typedef TPinAccess<PIN_1> T_PIN_TYPE_0;
		typedef TPinAccess<PIN_2> T_PIN_TYPE_1;

	public:
		static T_PIN_TYPE_0 PinObject0;
		static T_PIN_TYPE_1 PinObject1;

	public:
		_V_PIN_( OutputPins0 )
		_V_PIN_( OutputPins1 )

	public:
		inline void SystemInit()
		{
			pinMode( PIN_1, INPUT );
			pinMode( PIN_2, INPUT );
		}

		inline void SystemStart()
		{
			T_OutputPins0::SetPinValue( digitalRead( PIN_1 ) != HIGH, false );
			T_OutputPins1::SetPinValue( digitalRead( PIN_2 ) != HIGH, false );
		}

		inline void SystemLoopBegin()
		{
			T_OutputPins0::SetPinValue( digitalRead( PIN_1 ) != HIGH, true );
			T_OutputPins1::SetPinValue( digitalRead( PIN_2 ) != HIGH, true );
		}
	};
//---------------------------------------------------------------------------
//	template<int PIN_1, int PIN_2, typename T_OUT_PIN_0, typename T_OUT_PIN_1> InvertedDigtitalInputChangeOnlyPins2<PIN_1, PIN_2, T_OUT_PIN_0, T_OUT_PIN_1>::T_PIN_TYPE_0	InvertedDigtitalInputChangeOnlyPins2<PIN_1, PIN_2, T_OUT_PIN_0, T_OUT_PIN_1>::::PinAccess0;
//---------------------------------------------------------------------------
	template<int PIN_1, int PIN_2, typename T_OUT_PIN_0, typename T_OUT_PIN_1> class InvertedDigtitalInputPullUpChangeOnlyPins2 : public InvertedDigtitalInputChangeOnlyPins2<PIN_1, PIN_2, T_OUT_PIN_0, T_OUT_PIN_1>
	{
	public:
		inline void SystemInit()
		{
			pinMode( PIN_1, INPUT_PULLUP );
			pinMode( PIN_2, INPUT_PULLUP );
		}

	};
//---------------------------------------------------------------------------
	template<
		int PIN_1,
		int PIN_2,
		int PIN_3,
		typename T_OutputPins0,
		typename T_OutputPins1,
		typename T_OutputPins2
	> class InvertedDigtitalInputChangeOnlyPins3 :
		public T_OutputPins0,
		public T_OutputPins1,
		public T_OutputPins2 //, public TPinAccess<PIN_1>, public TPinAccess<PIN_2>
	{
	public:
		typedef TPinAccess<PIN_1> T_PIN_TYPE_0;
		typedef TPinAccess<PIN_2> T_PIN_TYPE_1;
		typedef TPinAccess<PIN_3> T_PIN_TYPE_2;

	public:
		static T_PIN_TYPE_0 PinObject0;
		static T_PIN_TYPE_1 PinObject1;
		static T_PIN_TYPE_2 PinObject2;

	public:
		_V_PIN_( OutputPins0 )
		_V_PIN_( OutputPins1 )
		_V_PIN_( OutputPins2 )

	public:
		inline void SystemInit()
		{
			pinMode( PIN_1, INPUT );
			pinMode( PIN_2, INPUT );
			pinMode( PIN_3, INPUT );
		}

		inline void SystemStart()
		{
			T_OutputPins0::SetPinValue( digitalRead( PIN_1 ) != HIGH, false );
			T_OutputPins1::SetPinValue( digitalRead( PIN_2 ) != HIGH, false );
			T_OutputPins2::SetPinValue( digitalRead( PIN_3 ) != HIGH, false );
		}

		inline void SystemLoopBegin()
		{
			T_OutputPins0::SetPinValue( digitalRead( PIN_1 ) != HIGH, true );
			T_OutputPins1::SetPinValue( digitalRead( PIN_2 ) != HIGH, true );
			T_OutputPins2::SetPinValue( digitalRead( PIN_3 ) != HIGH, true );
		}
	};
//---------------------------------------------------------------------------
	template<
		int PIN_1,
		int PIN_2,
		int PIN_3,
		typename T_OutputPins0,
		typename T_OutputPins1,
		typename T_OutputPins2
	> class InvertedDigtitalInputPullUpChangeOnlyPins3 :
		public InvertedDigtitalInputChangeOnlyPins3<
				PIN_1,
				PIN_2,
				PIN_3,
				T_OutputPins0,
				T_OutputPins1,
				T_OutputPins2
			>
	{
	public:
		inline void SystemInit()
		{
			pinMode( PIN_1, INPUT_PULLUP );
			pinMode( PIN_2, INPUT_PULLUP );
			pinMode( PIN_3, INPUT_PULLUP );
		}

	};
//---------------------------------------------------------------------------
	template<
		int IN_PIN,
		typename T_OutputPin
	> class AnalogInputPinSensor :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	protected:
		inline void ReadValue()
		{
			T_OutputPin::SetPinValue( analogRead( IN_PIN ) / float( VISUINO_ANALOG_IN_RANGE ) );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValue();
		}

	public:
		inline void SystemInit()
		{
//			pinMode( PIN, INPUT );
		}

		inline void SystemStart()
		{
			ReadValue();
		}

		inline void SystemLoopBegin()
		{
			ReadValue();
		}

	};
//---------------------------------------------------------------------------
	template<
		int PIN_1,
		int PIN_2,
		typename T_OutputPins0,
		typename T_OutputPins1
	> class AnalogInputPins2 :
		public T_OutputPins0,
		public T_OutputPins1
	{
	public:
		_V_PIN_( OutputPins0 )
		_V_PIN_( OutputPins1 )

	protected:
		inline void ReadValues()
		{
			T_OutputPins0::SetPinValue( analogRead( PIN_1 ) / float( VISUINO_ANALOG_IN_RANGE ) );
			T_OutputPins1::SetPinValue( analogRead( PIN_2 ) / float( VISUINO_ANALOG_IN_RANGE ) );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadValues();
		}

	public:
		inline void SystemInit()
		{
//			pinMode( PIN_1, INPUT );
//			pinMode( PIN_2, INPUT );
		}

		inline void SystemStart()
		{
			ReadValues();
		}

		inline void SystemLoopBegin()
		{
			ReadValues();
		}

	};
//---------------------------------------------------------------------------
/*
	template<int C_PIN_NUMBER> class AnalogInitialValuePin
	{
	public:
		float InitialValue = 0.5;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue = *(float*)_Data;
			SystemStart();
		}

	public:
		inline void SystemStart()
		{
			analogWrite( C_PIN_NUMBER, ( InitialValue * PWMRANGE ) + 0.5 );
		}
	};
*/
//---------------------------------------------------------------------------
#if defined( VISUINO_NRF52832 ) || defined( VISUINO_BBC_MICRO_BIT )
	#define SERIAL_TYPE Uart

#else // VISUINO_NRF52832
	#ifdef VISUINO_FREESOC2
		#define SERIAL_TYPE UARTClass

	#else // VISUINO_FREESOC2
		#ifdef VISUINO_FEATHER_M0
			#define SERIAL_TYPE Serial_

		#else // VISUINO_FEATHER_M0
			#ifdef VISUINO_FEMTO_USB
				#define SERIAL_TYPE Serial_
				#define Serial SerialUSB

			#else // VISUINO_FEMTO_USB
				#ifdef HAVE_CDCSERIAL
					#define SERIAL_TYPE Serial_
				#else // HAVE_CDCSERIAL
					#ifdef VISUINO_ARDUINO_101
						#define SERIAL_TYPE CDCSerialClass
					#else // VISUINO_ARDUINO_101
						#ifdef VISUINO_INTEL_EDISON
							#define SERIAL_TYPE TTYUARTClass
						#else // VISUINO_INTEL_EDISON
							#ifdef VISUINO_ARDUINO_NANO_EVERY
								#define SERIAL_TYPE UartClass
							#else // VISUINO_ARDUINO_NANO_EVERY
								#define SERIAL_TYPE HardwareSerial
							#endif // VISUINO_ARDUINO_NANO_EVERY
						#endif // VISUINO_INTEL_EDISON
					#endif // VISUINO_ARDUINO_101
				#endif // HAVE_CDCSERIAL
			#endif // VISUINO_FEMTO_USB
		#endif // VISUINO_FEATHER_M0
	#endif // VISUINO_FREESOC2
#endif // VISUINO_NRF52832
//---------------------------------------------------------------------------
	enum TArduinoSerialParity { spNone, spEven, slOdd, slMark, slSpace };
	typedef TArduinoSerialParity TArduinoStandardSerialParity;
//---------------------------------------------------------------------------
	template<typename T_INSTANCE, T_INSTANCE &C_SERIAL, typename T> class ArduinoSerialInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_SERIAL.Print( *(T*)_Data );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialInput<T_INSTANCE, C_SERIAL, bool>
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_SERIAL.Print( *(bool*)_Data ? "true" : "false" );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialInput<T_INSTANCE, C_SERIAL, char>
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( *(char*)_Data == '\n' )
				return;

			if( *(char*)_Data == '\r' )
				C_SERIAL.Print( "" );

			else
				C_SERIAL.PrintChar( *(char*)_Data );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialBinaryInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
			C_SERIAL.Write((uint8_t *) ADataBlock.Data, ADataBlock.Size );
//				C_SERIAL.GetStream().write( *(unsigned char*)_Data );
		}

	};
//---------------------------------------------------------------------------	
	template<typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoEnabledSerialBinaryInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::TDataBlock ADataBlock = *(Mitov::TDataBlock*)_Data;
			C_SERIAL.Write((uint8_t *) ADataBlock.Data, ADataBlock.Size );
//				C_SERIAL.GetStream().write( *(unsigned char*)_Data );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialStringInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_SERIAL.Print( (char*)_Data );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialClockInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_SERIAL.Print( "(Clock)" );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_INSTANCE, T_INSTANCE &C_SERIAL, typename T_OBJECT> class ArduinoSerialObjectInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_SERIAL.Print( ((T_OBJECT *)_Data)->ToString().c_str() );
		}

	};
//---------------------------------------------------------------------------
/*
	template<typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoDirectSerialInput : public OpenWire::Object
	{
		typedef OpenWire::Object	inherited;

	public:
		OpenWire::SinkPin	InputPin;

	public:
		virtual Stream &GetStream()
		{
			return C_SERIAL.GetStream();
		}

	protected:
		void DoReceive( void *_Data )
		{
//			if( C_SERIAL.Enabled )
//				C_SERIAL.write( *(unsigned char*)_Data );
		}

	public:
		ArduinoDirectSerialInput()
		{
			InputPin.SetCallback( this, (OpenWire::TOnPinReceive)&ArduinoDirectSerialInput::DoReceive );
		}
	};
*/
//---------------------------------------------------------------------------
	template<typename T_INSTANCE, T_INSTANCE &C_SERIAL> class ArduinoSerialOutput
	{
	public:
		OpenWire::SourcePin	OutputPin;

	public:
		inline void SystemLoopBegin()
		{
			if( ! C_SERIAL.Enabled() )
				return;

			uint8_t ABuffer[ 20 ];

			int i = 0;
			for( ; i < 20; ++i )
			{
				int AData = C_SERIAL.GetStream().read();
				if( AData < 0 )
					break;

				ABuffer[ i ] = AData;
			}

			if( i > 0 )
				OutputPin.SendValue( Mitov::TDataBlock( i, ABuffer ));

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T_AccessPointName,
		typename T_Enabled,
		typename T_NumberRetries,
		typename T_Password,
		typename T_RetryInterval,
		typename T_UserName
	> class GPRSAccessPoint :
		public T_AccessPointName,
		public T_Enabled,
		public T_NumberRetries,
		public T_Password,
		public T_RetryInterval,
		public T_UserName
	{
	public:
		_V_PROP_( Enabled )

		_V_PROP_( AccessPointName )
		_V_PROP_( UserName )
		_V_PROP_( Password )

		_V_PROP_( NumberRetries )
		_V_PROP_( RetryInterval )

	public:
		int32_t	FRetryCount = 0;
		unsigned long FLastTime = 0;

	public:
		inline void ResetRetry()
		{
			FRetryCount = 0;
		}

	public:
		void TryConnect( bool AFromStart, bool &ATryNextConnection, bool &AProccessed )
		{
			if( ! Enabled() )
				return;

			bool ACanRetry = true;
			if( ! AFromStart )
				if( ! CanRetry( ACanRetry ) )
				{
					ATryNextConnection |= ACanRetry;
					return;
				}

			ATryNextConnection |= ACanRetry;
			if( C_OWNER.TryConnectToPoint( AccessPointName(), UserName(), Password() ))
			{
				AProccessed = true;
				return;
			}

			FLastTime = micros();
			++ FRetryCount;
		}

	public:
		bool CanRetry( bool &ACanRetry )
		{
			ACanRetry = ( FRetryCount <= NumberRetries() );
//			Serial.print( "ACanRetry" ); Serial.println( ACanRetry );
			if( FRetryCount == 0 )
				return true;

			if( ! ACanRetry )
				return false;

			if(( micros() - FLastTime ) < RetryInterval() * 1000 )
				return false;

			return true;
		}

	};
//---------------------------------------------------------------------------
	class Convert_AnalogToComplexArray
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			TComplex	AComplex( *(float *)_Data );
			TArray<Mitov::TComplex> AComplexBuffer( 1, &AComplex );
			ACallback( &AComplexBuffer );
		}

	};
//---------------------------------------------------------------------------
	class Convert_BinaryBlockToChar
	{
	public:
		static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
//			Serial.println( "TEST" );
			Mitov::TDataBlock ABlock = *(Mitov::TDataBlock *)_Data;
			char *APtr = (char *)ABlock.Data;
			int ASize = ABlock.Size;
//			Serial.println( ASize );
			while( ASize-- )
			{
//  				Serial.println( *APtr );
				ACallback( APtr++ );
//				if( OutputPin.IsConnected() )
//					Serial.println( "TEST2" );
			}
		}

	};
//---------------------------------------------------------------------------
	class Convert_BinaryBlockToText
	{
	public:
		static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
//			Serial.println( "TEST" );
			Mitov::TDataBlock ABlock = *(Mitov::TDataBlock *)_Data;
			char *APtr = (char *)ABlock.Data;
			int ASize = ABlock.Size;
			Mitov::String AText;
			AText.reserve( ASize );
//			Serial.println( ASize );
			while( ASize-- )
				AText += *APtr++;

			ACallback( (char *)AText.c_str() );
		}

	};	
//---------------------------------------------------------------------------
	template<typename T_FLastValue> class Convert_BinaryToClock : public T_FLastValue
	{
	protected:
		_V_PROP_( FLastValue )

	public:
		inline void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			bool AValue = *(bool *)_Data;
			if( AValue == FLastValue() )
				return;

			FLastValue() = AValue;
			if( AValue )
				ACallback( nullptr );

		}

	public:
		Convert_BinaryToClock()
		{
			FLastValue() = false;
		}

	};
//---------------------------------------------------------------------------
	template<typename T_FROM, typename T_TO> class TypedConvert
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			T_TO AValue = T_TO( *(T_FROM *)_Data );
			ACallback( &AValue );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_FROM> class TypedConvert<T_FROM, Mitov::String>
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			Mitov::String AValue = Mitov::String( *(T_FROM *)_Data );
			ACallback( (void *)AValue.c_str() );
		}

	};
//---------------------------------------------------------------------------
	class Convert_DigitalToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			if(*(bool*)_Data )
				ACallback( (void *)"true" );

			else
				ACallback( (void *)"false" );

		}

	};
//---------------------------------------------------------------------------
	class Convert_DateTimeToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			TDateTime &ADate = *(TDateTime *)_Data;
			ACallback( (void *)ADate.ToString().c_str());
		}

	};
//---------------------------------------------------------------------------
	class Convert_ColorToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			TColor &AColor= *(TColor *)_Data;
			ACallback( (void *)AColor.ToString().c_str());
		}

	};
//---------------------------------------------------------------------------
	class Convert_RGBWColorToText
	{
	public:
		inline static void Convert( void *_Data, OpenWire::TPinCallback ACallback )
		{
			TRGBWColor &AColor= *(TRGBWColor *)_Data;
			ACallback( (void *)AColor.ToString().c_str());
		}

	};
//---------------------------------------------------------------------------
	typedef Convert_RGBWColorToText Convert_AlphaColorToText;
//---------------------------------------------------------------------------
	namespace Func
	{
		const char PROGMEM b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

		inline void a3_to_a4(unsigned char * a4, unsigned char * a3) 
		{
			a4[0] = (a3[0] & 0xfc) >> 2;
			a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
			a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
			a4[3] = (a3[2] & 0x3f);
		}

		inline void a4_to_a3(unsigned char * a3, unsigned char * a4) 
		{
			a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
			a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
			a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
		}

		inline unsigned char b64_lookup(char c) 
		{
			if(c >='A' && c <='Z') 
				return c - 'A';

			if(c >='a' && c <='z') 
				return c - 71;

			if(c >='0' && c <='9') 
				return c + 4;

			if(c == '+') 
				return 62;

			if(c == '/') 
				return 63;

			return -1;
		}

		int Base64Encode(char *output, const char *input, int inputLen) 
		{
			int i = 0, j = 0;
			int encLen = 0;
			unsigned char a3[3];
			unsigned char a4[4];

			while(inputLen--) 
			{
				a3[i++] = *(input++);
				if(i == 3) 
				{
					a3_to_a4(a4, a3);

					for(i = 0; i < 4; i++)
						output[encLen++] = pgm_read_byte(&b64_alphabet[a4[i]]);

					i = 0;
				}
			}

			if(i) 
			{
				for(j = i; j < 3; j++)
					a3[j] = '\0';

				a3_to_a4(a4, a3);

				for(j = 0; j < i + 1; j++)
					output[encLen++] = pgm_read_byte(&b64_alphabet[a4[j]]);

				while((i++ < 3))
					output[encLen++] = '=';
				
			}

			output[encLen] = '\0';
			return encLen;
		}

		int Base64Decode(char * output, const char * input, int inputLen) 
		{
			int i = 0, j = 0;
			int decLen = 0;
			unsigned char a3[3];
			unsigned char a4[4];


			while (inputLen--) 
			{
				if(*input == '=')
					break;

				a4[i++] = *(input++);
				if (i == 4) 
				{
					for (i = 0; i <4; i++)
						a4[i] = b64_lookup(a4[i]);

					a4_to_a3(a3,a4);

					for (i = 0; i < 3; i++)
						output[decLen++] = a3[i];
					
					i = 0;
				}
			}

			if (i) 
			{
				for (j = i; j < 4; j++)
					a4[j] = '\0';				

				for (j = 0; j <4; j++)
					a4[j] = b64_lookup(a4[j]);

				a4_to_a3(a3,a4);

				for (j = 0; j < i - 1; j++)
					output[decLen++] = a3[j];
				
			}

			output[decLen] = '\0';
			return decLen;
		}

		inline int Base64EncodeLength(int plainLen) 
		{
			int n = plainLen;
			return (n + 2 - ((n + 2) % 3)) / 3 * 4;
		}

		int Base64DecodeLength(char * input, int inputLen)
		{
			int i = 0;
			int numEq = 0;
			for(i = inputLen - 1; input[i] == '='; i--) 
				numEq++;

			return ((6 * inputLen) / 8) - numEq;
		}

		Mitov::String Base64Encode( Mitov::String input ) 
		{
			int ALength = Base64EncodeLength( input.length() );
			Mitov::String AResult;
			AResult.reserve( ALength );
			for( int i = 0; i < ALength; ++i )
				AResult += " ";

			Base64Encode( (char *)AResult.c_str(), input.c_str(), input.length() );

			return AResult;
		}

		Mitov::String ByteListToString( Mitov::SimpleList<uint8_t, uint16_t> &AValue )
		{
			Mitov::String Result;
			Result.reserve( AValue.size());
			for( uint32_t i = 0; i < AValue.size(); ++i )
				Result += (char)AValue[ i ];

			return Result;
		}

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
		inline uint16_t Color565(uint8_t r, uint8_t g, uint8_t b) 
		{
			return (uint16_t(r & 0xF8) << 8) | (uint16_t(g & 0xFC) << 3) | (b >> 3);
		}		
//---------------------------------------------------------------------------
		inline uint16_t Color565(uint32_t AColor ) 
		{
			uint8_t	r = AColor >> 16;
			uint8_t	g = AColor >> 8;
			uint8_t	b = AColor;
			return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
		}		
//---------------------------------------------------------------------------
		inline uint32_t ColorFrom565( uint16_t AColor ) 
		{
			return	0b000001110000001100000111 |
					(( uint32_t( AColor ) & 0xf800 ) << 8 ) |
					(( uint32_t( AColor ) & 0x07e0 ) << 5 ) |
					(( uint32_t( AColor ) & 0x001f ) << 3 );

		}		
//---------------------------------------------------------------------------
		void insert_sort(int array[], uint8_t size) 
		{
			uint8_t j;
			int save;
  
			for (int i = 1; i < size; i++) 
			{
				save = array[i];
				for (j = i; j >= 1 && save < array[j - 1]; j--)
					array[j] = array[j - 1];

				array[j] = save; 
			}
		}
//---------------------------------------------------------------------------
		inline uint8_t FromBcdToDec(const uint8_t bcd)
		{
			return (10 * ((bcd & 0xF0) >> 4) + (bcd & 0x0F));
		}
//---------------------------------------------------------------------------
		inline uint8_t FromDecToBcd(const uint8_t dec) 
		{
			const uint8_t tens = dec / 10;
			const uint8_t ones = dec % 10;
			return (tens << 4) | ones;
		}
//---------------------------------------------------------------------------
		uint8_t FromCharsToInt8( char *AValues, uint8_t ALength )
		{
			uint8_t AResult = 0;
			for( int i = 0; i < ALength; ++i )
				AResult = AResult * 10 + ( AValues[ i ] - '0' );

			return AResult;
		}
//---------------------------------------------------------------------------
		uint16_t FromCharsToInt16( char *AValues, uint8_t ALength )
		{
			uint16_t AResult = 0;
			for( int i = 0; i < ALength; ++i )
				AResult = AResult * 10 + ( AValues[ i ] - '0' );

			return AResult;
		}
//---------------------------------------------------------------------------
		inline float ConvertCtoF(float c) 
		{
			return c * 1.8 + 32;
		}
//---------------------------------------------------------------------------
		inline float ConvertFtoC(float f)
		{
			return (f - 32) * 0.55555;
		}
//---------------------------------------------------------------------------
		template<typename T>T MapRange( T x, T in_min, T in_max, T out_min, T out_max)
		{
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}
//---------------------------------------------------------------------------
		bool ExtractOptionallyQuotedCommaText( Mitov::String &AText, Mitov::String &AResult )
		{
			if( AText.length() == 0 )
				return false;

			bool	AStartsWithQuote;
//			int		AStartIndex;

			if( AText[ 0 ] == '"' )
			{
				AStartsWithQuote = true;
//				AStartIndex = 1;
			}

			else
			{
				AStartsWithQuote = false;
//				AStartIndex = 0;
			}

			if( ! AStartsWithQuote )
			{
				int APos = AText.indexOf( "," );
				if( APos < 0 )
				{
					AResult = AText;
					AText = "";
					return true;
				}

				AResult = AText.substring( 0, APos );
				AText.remove( 0, APos + 1 );
				return true;
			}

			AResult = "";
			int ASubStartsWithQuote = 0;
			uint32_t ALength = 0;
			while( ALength < AText.length() )
			{
				if( AText[ ASubStartsWithQuote ] == ',' )
					break;

				if( AText[ ASubStartsWithQuote ] != '"' )
					return false;

				++ASubStartsWithQuote;

	//			Serial.print( "ASubStartsWithQuote: " ); Serial.println( ASubStartsWithQuote );
				int APos = AText.indexOf( "\"", ASubStartsWithQuote );
				if( APos < 0 )
					return false;

	//			Serial.println( APos );
				AResult += AText.substring( ASubStartsWithQuote, APos );

				ASubStartsWithQuote = APos + 1;
				ALength = APos + 2;
			}

			AText.remove( 0, ALength );

			return true;
		}
//---------------------------------------------------------------------------
		bool ContainsTextAtPos( Mitov::String ALine, int APos, Mitov::String ASubText )
		{
			if( ALine.length() < APos + ASubText.length() )
				return false;

			for( int i = 0; i < int( ASubText.length()); ++i )
				if( ALine[ APos + i ] != ASubText[ i ] )
					return false;

			return true;
		}
//---------------------------------------------------------------------------
/*
		uint8_t Exponent(uint16_t value)
		{
			// Calculates the base 2 logarithm of a value
			uint8_t result = 0;
			while (((value >> result) & 1) != 1) 
				result++;
			return(result);
		}
*/
//---------------------------------------------------------------------------
		bool SetBitField( uint8_t *AValuesArray, int AIndex, bool AValue )
		{
			uint8_t AMask = 1 << ( AIndex & 0b111 ); // same as % 8
			uint8_t	AArrayIndex = AIndex / 8;
			if( AValue == (( AValuesArray[ AArrayIndex ] & AMask ) != 0 ))
				return false;

			if( AValue )
				AValuesArray[ AArrayIndex ] |= AMask;

			else
				AValuesArray[ AArrayIndex ] &= ~AMask;

			return true;
		}
//---------------------------------------------------------------------------
		inline uint32_t SwapRB( uint32_t AColor )
		{
			return ( ( AColor & 0xFF ) << 16 ) | ( AColor & 0x00FF00 ) | ( ( AColor >> 16 ) & 0xFF );
		}
	}
//---------------------------------------------------------------------------
	template <typename T> T ConstrainToList( T AValue, const T AList[], int ASize )
	{
		int AIndex = -1;
		for( int i = 0; i < ASize; ++i )
			if( AValue <= AList[ i ] )
			{
				AIndex = i;
				break;
			}

		if( AIndex == -1 )
			return AList[ ASize - 1 ];

		return AList[ AIndex ];
	}
//---------------------------------------------------------------------------
	class InterruptLock 
	{
	public:
		InterruptLock()
		{
#ifndef VISUINO_FREESOC2
			noInterrupts();
#endif
		}

		~InterruptLock() 
		{
#ifndef VISUINO_FREESOC2
			interrupts();
#endif
		}
	};
//---------------------------------------------------------------------------
	class StringEx : public Mitov::String
	{
		typedef	Mitov::String	inherited;

	public:
		using inherited::inherited;
		StringEx( const char *AStr, int ALength )
		{
			copy( AStr, ALength );
		}

	};
//---------------------------------------------------------------------------
/*
	template<
		typename T_BASE, 
		typename T_Enabled,
		typename T_OutputPin,
		typename T
	> class UnaryOperation : 
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
		inline void InputPin_o_Receive( void *_Data )
        {
            T_OutputPin::SetPinValue( T_BASE::CalculateOutput( *(T*)_Data ) );
        }

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_BASE, 
		typename T_Enabled,
		typename T_OutputPin,
		typename T
	> class UnaryOperation :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )

    public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

            T_OutputPin::SetPinValue( T_BASE::CalculateOutput( *(T*)_Data ) );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_IMPLEMENTATION_CLASS, 
		typename T_Enabled,
		typename T_OutputPin,
		typename T_TYPE,
		typename T_Value
	> class UnaryOperation_Param1 : 
		public T_Enabled,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
		_V_PROP_( Enabled )
        _V_PROP_( Value )

    public:
		inline void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
			    T_OutputPin::PinNotify( _Data );
                return;
            }

            T_OutputPin::SetPinValue( T_IMPLEMENTATION_CLASS::CalculateOutput( *(T_TYPE *)_Data, Value() ) );
        }
	};
//---------------------------------------------------------------------------
	template <typename T_BASE> class NullCallChain : public T_BASE
	{
	public:
		static inline bool ChainHasItems() { return false; }

	};
//---------------------------------------------------------------------------
	template <typename T_BASE> class EmbeddedCallChain : public T_BASE
	{
	public:
		static inline bool ChainHasItems() { return true; }

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class ClockPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void ClockPin()
		{
		}

		inline bool GetPinIsConnected()
		{
			return false;
		}

		inline ClockPin_NoImplementation & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class ClockPin_StandardPinImplementation : public OpenWire::SourcePin
	{
//	private:
//		OpenWire::SourcePin	_GenericOutputPin;

	public:
		inline void ClockPin()
		{
//			_GenericOutputPin.Notify( nullptr );
			Notify( nullptr );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline ClockPin_StandardPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class ClockPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
//	private:
//		OpenWire::SourcePin	_GenericOutputPin;

	public:
		inline void ClockPin()
		{
//			_GenericOutputPin.Notify( nullptr );
			T_BASE::Notify( nullptr );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline ClockPin_EmbeddedPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class GenericPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline bool GetPinIsConnected()
		{
			return false;
		}

		template<typename T> inline void SendPinValue( T AValue )
		{
		}

		template<typename T> inline void SetPinValue( T AValue, bool AChangeOnly = false )
		{
		}

		inline GenericPin_NoImplementation & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class GenericPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;
//	private:
//		OpenWire::SourcePin	_GenericOutputPin;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		template<typename T> inline void SendPinValue( T AValue )
		{
			PinNotify( &AValue );
		}

		template<typename T> inline void SetPinValue( T AValue, bool AChangeOnly = false )
		{
			PinNotify( &AValue );
		}

		inline GenericPin_EmbeddedPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class GenericPin_StandardPinImplementation : public OpenWire::SourcePin
	{
		typedef OpenWire::SourcePin inherited;
//	private:
//		OpenWire::SourcePin	_GenericOutputPin;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		template<typename T> inline void SendPinValue( T AValue )
		{
			PinNotify( &AValue );
		}

		template<typename T> inline void SetPinValue( T AValue, bool AChangeOnly = false )
		{
			PinNotify( &AValue );
		}

		inline GenericPin_StandardPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_DATA> class TypedPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( T_DATA AValue, bool AChangeOnly = false )
		{
		}

		inline bool GetPinIsConnected()
		{
			return false;
		}

		inline TypedPin_NoImplementation & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_DATA> class TypedPin_StandardPinImplementation : public OpenWire::SourcePin
	{
		typedef OpenWire::SourcePin inherited;

//	private:
//		OpenWire::SourcePin	_GenericOutputPin;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( T_DATA AValue, bool AChangeOnly = false )
		{
			SendValue<T_DATA>( AValue );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline TypedPin_StandardPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE, typename T_DATA> class TypedPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( T_DATA AValue, bool AChangeOnly = false )
		{
			inherited:: template SendValue<T_DATA>( AValue );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline TypedPin_EmbeddedPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_DATA> class TypedPin_NoImplementation_ChangeOnly : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( T_DATA AValue, bool AChangeOnly = false )
		{
		}

		inline bool GetPinIsConnected()
		{
			return false;
		}

		inline TypedPin_NoImplementation_ChangeOnly & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_DATA> class TypedPin_StandardPinImplementation_ChangeOnly : public OpenWire::SourcePin
	{
		typedef OpenWire::SourcePin inherited;

	public:
		T_DATA	Value;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		void SetValue( T_DATA AValue, bool AChangeOnly )
		{
			if( AChangeOnly )
				if( AValue == Value )
					return;

			Value = AValue;
			inherited::Notify( &Value );
		}

		inline void SetPinValue( T_DATA AValue, bool AChangeOnly = false )
		{
			SetValue( AValue, AChangeOnly );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline TypedPin_StandardPinImplementation_ChangeOnly & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE, typename T_DATA> class TypedPin_EmbeddedPinImplementation_ChangeOnly : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		T_DATA	Value;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		void SetValue( T_DATA AValue, bool AChangeOnly )
		{
			if( AChangeOnly )
				if( AValue == Value )
					return;

			Value = AValue;
			inherited::Notify( &Value );
		}

		inline void SetPinValue( T_DATA AValue, bool AChangeOnly = false )
		{
			SetValue( AValue, AChangeOnly );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline TypedPin_EmbeddedPinImplementation_ChangeOnly & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class TextPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( const char *AValue, bool AChangeOnly = false )
		{
		}

//		inline void SetPinIndirectValue( String AValue, bool AChangeOnly = false )
//		{
//		}

		inline void SetPinValue( String AValue, bool AChangeOnly = false )
		{
		}

		inline bool GetPinIsConnected()
		{
			return false;
		}

		inline TextPin_NoImplementation & GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class TextPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( const char *AValue, bool AChangeOnly = false )
		{
			inherited::SendValue( AValue );
		}

//		inline void SetPinIndirectValue( String AValue, bool AChangeOnly = false )
//		{
//			inherited::Notify( (char *)AValue.c_str() );
//		}

		inline void SetPinValue( String AValue, bool AChangeOnly = false )
		{
			inherited::Notify( (char *)AValue.c_str() );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline TextPin_EmbeddedPinImplementation & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class TextPin_StandardPinImplementation : public OpenWire::SourcePin
	{
//	private:
//		OpenWire::SourcePin	_GenericOutputPin;

		typedef OpenWire::SourcePin inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( const char *AValue, bool AChangeOnly = false )
		{
			Notify( (char *)AValue );
		}

//		inline void SetPinIndirectValue( String AValue, bool AChangeOnly = false )
//		{
//			Notify( (char *)AValue.c_str() );
//		}

		inline void SetPinValue( String AValue, bool AChangeOnly = false )
		{
			Notify( (char *)AValue.c_str() );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline TextPin_StandardPinImplementation & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class TextPin_NoImplementation_ChangeOnly : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( const char *AValue, bool AChangeOnly = false )
		{
		}

//		inline void SetPinIndirectValue( String AValue, bool AChangeOnly = false )
//		{
//		}

		inline void SetPinValue( String AValue, bool AChangeOnly = false )
		{
		}

		inline bool GetPinIsConnected()
		{
			return false;
		}

		inline TextPin_NoImplementation_ChangeOnly &GetInstance()
		{
			return *this;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class TextPin_EmbeddedPinImplementation_ChangeOnly : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		Mitov::String	Value;

	public:
		void SetValue( String &AValue, bool AChangeOnly )
		{
			if( AChangeOnly )
				if( AValue == Value )
					return;

			Value = AValue;
			inherited::Notify( (char *)Value.c_str() );
		}

	public:
		inline void PinNotify( void *_data )
		{
			SetValue( (const char *)_data, false );
		}

		inline void SetPinValue( const char *AValue, bool AChangeOnly = false )
		{
			SetValue( AValue, AChangeOnly );
		}

//		inline void SetPinIndirectValue( String AValue, bool AChangeOnly = false )
//		{
//			SetValue( AValue, AChangeOnly );
//		}

		inline void SetPinValue( String AValue, bool AChangeOnly = false )
		{
			SetValue( AValue, AChangeOnly );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline TextPin_EmbeddedPinImplementation_ChangeOnly & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class TextPin_StandardPinImplementation_ChangeOnly : public OpenWire::TypedSourcePin<Mitov::String>
	{
		typedef OpenWire::TypedSourcePin<Mitov::String> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( const char *AValue, bool AChangeOnly = false )
		{
			SetValue( AValue, AChangeOnly );
		}

//		inline void SetPinIndirectValue( String AValue, bool AChangeOnly = false )
//		{
//			SetValue( AValue, AChangeOnly );
//		}

		inline void SetPinValue( String AValue, bool AChangeOnly = false )
		{
			SetValue( AValue, AChangeOnly );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline TextPin_StandardPinImplementation_ChangeOnly & GetInstance()
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class DigitalPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
		}

		inline void SetPinValueHigh()
		{
		}

		inline void SetPinValueLow()
		{
		}

		inline bool GetPinValue()
		{
			return false;
		}

		inline bool GetPinIsConnected()
		{
			return false;
		}

		inline DigitalPin_NoImplementation & GetInstance()
		{
			return *this;
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class DigitalPin_StandardPinImplementation : public OpenWire::SourcePin
	{
		typedef OpenWire::SourcePin inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
			SendValue( AValue );
		}

		inline void SetPinValueHigh()
		{
			SendValue( true );
		}

		inline void SetPinValueLow()
		{
			SendValue( false );
		}

		inline bool GetPinValue()
		{
			return false;
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline DigitalPin_StandardPinImplementation & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class DigitalPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
			OpenWire::StreamPin<T_BASE>::SendValue( AValue );
		}

		inline void SetPinValueHigh()
		{
			OpenWire::StreamPin<T_BASE>::SendValue( true );
		}

		inline void SetPinValueLow()
		{
			OpenWire::StreamPin<T_BASE>::SendValue( false );
		}

		inline bool GetPinValue()
		{
			return false;
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline DigitalPin_EmbeddedPinImplementation & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE, typename T_Value> class DigitalPin_EmbeddedPinImplementation_ChangeOnly : public OpenWire::StreamPin<T_BASE>, public T_Value
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	protected:
		_V_PROP_( Value )

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
			if( AChangeOnly )
				if( AValue == Value() )
					return;

			Value() = AValue;
			OpenWire::StreamPin<T_BASE>::SendValue( AValue );
		}

		inline void SetPinValueHigh()
		{
			SetPinValue( true );
		}

		inline void SetPinValueLow()
		{
			SetPinValue( false );
		}

		inline bool GetPinValue()
		{
			return Value();
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline DigitalPin_EmbeddedPinImplementation_ChangeOnly & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class DigitalPin_StandardPinImplementation_ChangeOnly : public OpenWire::TypedSourcePin<bool>
	{
		typedef OpenWire::TypedSourcePin<bool> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
			SetValue( AValue, AChangeOnly );
		}

		inline void SetPinValueHigh()
		{
			SendValue( true );
		}

		inline void SetPinValueLow()
		{
			SendValue( false );
		}

		inline bool GetPinValue()
		{
			return false;
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline DigitalPin_StandardPinImplementation_ChangeOnly & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}

	};
//---------------------------------------------------------------------------
	template<typename T_PIN, T_PIN &C_PIN> class DigitalPin_DirectPinImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
			SetPinValue( *(bool *)_data, false );
		}

		inline void SetPinValueHigh()
		{
			C_PIN.DigitalWrite( true );
		}

		inline void SetPinValueLow()
		{
			C_PIN.DigitalWrite( false );
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
			C_PIN.DigitalWrite( AValue );
		}

		inline bool GetPinValue()
		{			
			return C_PIN.DigitalRead();
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline DigitalPin_DirectPinImplementation & GetInstance() 
		{
			return *this;
		}

	public:
		DigitalPin_DirectPinImplementation()
		{
			C_PIN.SetMode( OUTPUT );
		}
	};
//---------------------------------------------------------------------------
	template<uint8_t C_PIN_NUMBER> class DigitalPin_DirectBoardPinImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
			SetPinValue( *(bool *)_data, false );
		}

		inline void SetPinValueHigh()
		{
//			Serial.print( C_PIN_NUMBER ); Serial.println( " = True" );
			digitalWrite( C_PIN_NUMBER, HIGH );
		}

		inline void SetPinValueLow()
		{
//			Serial.print( C_PIN_NUMBER ); Serial.println( " = False" );
			digitalWrite( C_PIN_NUMBER, LOW );
		}

		inline void SetPinValue( bool AValue, bool AChangeOnly = false )
		{
//			Serial.print( C_PIN_NUMBER ); Serial.print( " = " ); Serial.println( AValue );
			digitalWrite( C_PIN_NUMBER, AValue ? HIGH : LOW );
		}

		inline bool GetPinValue()
		{			
			pinMode( C_PIN_NUMBER, INPUT );
			bool AValue = ( digitalRead( C_PIN_NUMBER ) != LOW );
			pinMode( C_PIN_NUMBER, OUTPUT );

			return AValue;
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline DigitalPin_DirectBoardPinImplementation & GetInstance()
		{
			return *this;
		}

	public:
		DigitalPin_DirectBoardPinImplementation()
		{
			pinMode( C_PIN_NUMBER, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class AnalogPin_NoImplementation : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
		}

		inline bool GetPinIsConnected()
		{
			return false;
		}

		inline AnalogPin_NoImplementation & GetInstance() 
		{
			return *this;
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class AnalogPin_EmbeddedPinImplementation : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			inherited::SendValue( AValue );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_EmbeddedPinImplementation & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE, typename T_BASE> class AnalogPin_EmbeddedPinImplementation_ChangeOnly : public OpenWire::StreamPin<T_BASE>
	{
		typedef OpenWire::StreamPin<T_BASE> inherited;

	protected:
		float	Value;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			if( AChangeOnly )
				if( AValue == Value )
					return;

			Value = AValue;
			inherited::SendValue( AValue );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_EmbeddedPinImplementation_ChangeOnly & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class AnalogPin_StandardPinImplementation : public OpenWire::SourcePin
	{
		typedef OpenWire::SourcePin inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			SendValue( AValue );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_StandardPinImplementation & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template<typename T_PIN, T_PIN &C_PIN> class AnalogPin_DirectPinImplementation
	{
	public:
		inline void PinNotify( void *_data )
		{
			SetPinValue( *(float *)_data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			C_PIN.AnalogWrite( AValue );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_DirectPinImplementation & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}
/*
	public:
		AnalogPin_DirectPinImplementation()
		{
			C_PIN.SetMode( OUTPUT );
		}
*/
	};
//---------------------------------------------------------------------------
	template<uint8_t C_PIN_NUMBER> class AnalogPin_DirectBoardPinImplementation
	{
	public:
		inline void PinNotify( void *_data )
		{
			SetPinValue( *(float *)_data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			analogWrite( C_PIN_NUMBER, ( AValue * PWMRANGE ) + 0.5 );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_DirectBoardPinImplementation & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}

	public:
		AnalogPin_DirectBoardPinImplementation()
		{
			pinMode( C_PIN_NUMBER, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class AnalogPin_NoImplementation_ChangeOnly : public OpenWire::Null_StreamPin
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
		}

		inline bool GetPinIsConnected()
		{
			return false;
		}

		inline AnalogPin_NoImplementation_ChangeOnly & GetInstance() 
		{
			return *this;
		}

	};
//---------------------------------------------------------------------------
	template <uint32_t C_UNIQUE> class AnalogPin_StandardPinImplementation_ChangeOnly : public OpenWire::TypedSourcePin<float>
	{
		typedef OpenWire::TypedSourcePin<float> inherited;

	public:
		inline void PinNotify( void *_data )
		{
			inherited::Notify( _data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			inherited::SetValue( AValue, AChangeOnly );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_StandardPinImplementation_ChangeOnly & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}
	};
//---------------------------------------------------------------------------
	template<typename T_PIN, T_PIN &C_PIN> class AnalogPin_DirectPinImplementation_ChangeOnly
	{
	public:
		inline void PinNotify( void *_data )
		{
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			C_PIN.AnalogWrite( AValue );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_DirectPinImplementation_ChangeOnly & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}

/*
	public:
		AnalogPin_DirectPinImplementation()
		{
			C_PIN.SetMode( OUTPUT );
		}
*/
	};
//---------------------------------------------------------------------------
	template<uint8_t C_PIN_NUMBER> class AnalogPin_DirectBoardPinImplementation_ChangeOnly
	{
	public:
		inline void PinNotify( void *_data )
		{
			SetPinValue( *(bool *)_data );
		}

		inline void SetPinValue( float AValue, bool AChangeOnly = false )
		{
			analogWrite( C_PIN_NUMBER, ( AValue * PWMRANGE ) + 0.5 );
		}

		inline bool GetPinIsConnected()
		{
			return true;
		}

		inline AnalogPin_DirectBoardPinImplementation_ChangeOnly & GetInstance() 
		{
			return *this; //_GenericOutputPin;
		}

	public:
		AnalogPin_DirectBoardPinImplementation_ChangeOnly()
		{
			pinMode( C_PIN_NUMBER, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
	class ResetPinImplementation
	{
	public:
		template<typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelay )
		{
			AInstance->SetResetValueLow();
			delay( ADelay );
			AInstance->SetResetValueHigh();
		}

		template<typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelayHigh, uint16_t ADelayLow )
		{
			AInstance->SetResetValueHigh();
			delayMicroseconds( ADelayHigh );
			AInstance->SetResetValueLow();
			delay( ADelayLow );
			AInstance->SetResetValueHigh();
		}

		template<typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelayHigh, uint16_t ADelayLow, uint16_t APostDelay )
		{
			AInstance->SetResetValueHigh();
			delayMicroseconds( ADelayHigh );
			AInstance->SetResetValueLow();
			delay( ADelayLow );
			AInstance->SetResetValueHigh();
			delay( APostDelay );
		}

/*
	public:
		template<typename T> static void ExecuteMicrosReset( T *AInstance, uint16_t ADelayHigh, uint16_t ADelayLow )
		{
			delayMicroseconds( ADelayHigh );
			AInstance->SetResetValueLow();
			delayMicroseconds( ADelayLow );
			AInstance->SetResetValueHigh();
		}
*/
	};
//---------------------------------------------------------------------------
	class NoResetImplementation
	{
	public:
		template<typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelay )
		{
		}

		template<typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelayHigh, uint16_t ADelayLow )
		{
		}

		template<typename T> inline static void ExecuteReset( T *AInstance, uint16_t ADelayHigh, uint16_t ADelayLow, uint16_t APostDelay )
		{
		}

	};
//---------------------------------------------------------------------------
	template<
		uint8_t C_RED_PIN,
		uint8_t C_GREEN_PIN,
		uint8_t C_BLUE_PIN,
		typename T_InitialValue
	> class ArduinoRGBLEDModule :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
			analogWrite( C_RED_PIN, int( InitialValue().GetValue().Red ) * PWMRANGE / 255 );
			analogWrite( C_GREEN_PIN, int( InitialValue().GetValue().Green ) * PWMRANGE / 255 );
			analogWrite( C_BLUE_PIN, int( InitialValue().GetValue().Blue ) * PWMRANGE / 255 );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
//			Serial.print( InitialValue().GetValue().ToString()); Serial.print( " " ); Serial.println( AColor.ToString() );
			if( AColor == InitialValue().GetValue() )
				return;

			InitialValue() = AColor;

//			Serial.print( InitialValue().GetValue().ToString()); Serial.print( " " ); Serial.println( AColor.ToString() );

			UpdateColor();
		}

	public:
		inline void SystemStart()
		{
			UpdateColor();
		}

	};
//---------------------------------------------------------------------------
	template<
		uint8_t C_RED_PIN,
		uint8_t C_GREEN_PIN,
		uint8_t C_BLUE_PIN,
		typename T_InitialValue
	> class ArduinoRGBLEDInvertedModule :
		public T_InitialValue
	{
	public:
		_V_PROP_( InitialValue )

	protected:
		void UpdateColor()
		{
//			Serial.println( InitialValue().GetValue().ToString());
			analogWrite( C_RED_PIN, int( 255 - InitialValue().GetValue().Red ) * PWMRANGE / 255 );
			analogWrite( C_GREEN_PIN, int( 255 - InitialValue().GetValue().Green ) * PWMRANGE / 255 );
			analogWrite( C_BLUE_PIN, int( 255 - InitialValue().GetValue().Blue ) * PWMRANGE / 255 );
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			TColor &AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetValue() )
				return;

//			Serial.print( InitialValue().GetValue().ToString()); Serial.print( " " ); Serial.println( AColor.ToString() );
			InitialValue() = AColor;
			UpdateColor();
		}

	public:
		inline void SystemStart()
		{
			UpdateColor();
		}

	};
//---------------------------------------------------------------------------
	class InvertedAnalog_Implementation_False
	{
	public:
		inline float Convert( float AValue )
		{
			return AValue;
		}
	};
//---------------------------------------------------------------------------
	class InvertedAnalog_Implementation_True
	{
	public:
		inline float Convert( float AValue )
		{
			return 1 - AValue;
		}
	};
//---------------------------------------------------------------------------
	class InvertedAnalog_Implementation_Dynamic_False
	{
	public:
		bool	Inverted = false;

	public:
		float Convert( float AValue )
		{
			if( Inverted )
				return 1 - AValue;

			return AValue;
		}
	};
//---------------------------------------------------------------------------
	class InvertedAnalog_Implementation_Dynamic_True
	{
	public:
		bool	Inverted = true;

	public:
		float Convert( float AValue )
		{
			if( Inverted )
				return 1 - AValue;

			return AValue;
		}
	};
//---------------------------------------------------------------------------
	class TElapsedMicros
	{
	protected:
		unsigned long FLastTime;

	public:
		inline void SystemStarted()
		{
			FLastTime = micros();
		}

		inline unsigned long GetNewElapsedTime()
		{
			unsigned long AOldTime = FLastTime;
			FLastTime = micros();
			return ( FLastTime - AOldTime );
		}
	};
//---------------------------------------------------------------------------
	class TElapsedMillis
	{
	protected:
		unsigned long FLastTime;

	public:
		inline void SystemStarted()
		{
			FLastTime = millis();
		}

		inline unsigned long GetNewElapsedTime()
		{
			unsigned long AOldTime = FLastTime;
			FLastTime = millis();
			return ( FLastTime - AOldTime );
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_AllPixels,
		typename T_Reversed
	> class MaximLedPixelsReversedProperty :
		public T_AllPixels,
		public T_Reversed
	{
	public:
		_V_PIN_( AllPixels )
		_V_PIN_( Reversed )

//	public:
//		bool FCurrentReversed : 1;

/*
	public:
		MaximLedPixelsReversedProperty() :
			Reversed( false ),
			AllPixels( false ),
			FCurrentReversed( false )
		{
		}
*/
	};
//---------------------------------------------------------------------------
	template<
		typename T_X,
		typename T_Y,
		typename T_Z
	> class TArduinoXYZOffsets :
		public T_X,
		public T_Y,
		public T_Z
	{
	public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Z )

	};
//---------------------------------------------------------------------------
	class TAnalogTransform_Normal
	{
	public:
		static inline float Transform( float AValue ) { return AValue; }

	};
//---------------------------------------------------------------------------
	class TAnalogTransform_Invert
	{
	public:
		static inline float Transform( float AValue ) { return 1 - AValue; }

	};
//---------------------------------------------------------------------------
	template <
		uint8_t C_PIN,
		uint16_t C_RANGE,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class DirectMotorDriverForwardChannel :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = constrain( *(float *)_Data, 0.0, 1.0 );
			if( InitialValue() == ASpeed )
				return;

			InitialValue() = ASpeed;
			UpdateOutputs();
		}

	public:
		inline void UpdateOutputs()
		{
			if( Enabled() )
//			{
//				Serial.print( C_PIN ); Serial.print( " - " ); Serial.println( ( InitialValue() * 200 ) + 0.5 );
				analogWrite( C_PIN, ( InitialValue() * C_RANGE ) + 0.5 );
//			}
//				analogWrite( C_PIN, ( InitialValue() * PWMRANGE ) + 0.5 );

			else
				analogWrite( C_PIN, 0 );

		}

	public:
		inline void SystemStart()
		{
			pinMode( C_PIN, OUTPUT );
			UpdateOutputs();
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros ) {} // Placeholder for *_Slopped compatibility
	};
//---------------------------------------------------------------------------
	template<
		uint8_t C_PIN,
		uint16_t C_RANGE,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_SlopeRun,
		typename T_SlopeStop
	> class DirectMotorDriverForwardChannel_Slopped :
		public T_Enabled,
		public T_InitialValue,
		public T_SlopeRun,
		public T_SlopeStop
	{
	public:
		_V_PROP_( InitialValue )
		_V_PROP_( Enabled )
		_V_PROP_( SlopeRun )
		_V_PROP_( SlopeStop )

	protected:
		float	FCurrentValue = 0.0f;
		unsigned long	FLastTime = 0;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = constrain( *(float *)_Data, 0.0, 1.0 );
			if( InitialValue() == ASpeed )
				return;

			InitialValue() = ASpeed;
//			UpdateOutputs();
		}

	public:
		inline void UpdateOutputs()
		{
//			analogWrite( C_PIN, ( FCurrentValue * PWMRANGE ) + 0.5 );
			analogWrite( C_PIN, ( FCurrentValue * C_RANGE ) + 0.5 );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = InitialValue();

			UpdateOutputs();
		}

		inline void SystemLoopBeginElapsed( unsigned long AElapsedMicros )
		{
			float ATargetValue = ( Enabled() ) ? InitialValue() : 0.5;
			if( FCurrentValue != ATargetValue )
			{
				float ASlope = SlopeRun();
				if( FCurrentValue > ATargetValue )
				{
					if( ATargetValue > 0.5 )
						ASlope = SlopeStop();
				}

				else
				{
					if( ATargetValue < 0.5 )
						ASlope = SlopeStop();
				}

				if( ASlope == 0 )
					FCurrentValue = ATargetValue;

				else
				{
					float ARamp = abs( AElapsedMicros * ASlope / 1000000 );
					if( FCurrentValue < ATargetValue )
					{
						FCurrentValue += ARamp;
						if( FCurrentValue > ATargetValue )
							FCurrentValue = ATargetValue;

					}
					else
					{
						FCurrentValue -= ARamp;
						if( FCurrentValue < ATargetValue )
							FCurrentValue = ATargetValue;

					}
				}

				UpdateOutputs();
			}
		}

	public:
		inline DirectMotorDriverForwardChannel_Slopped()
		{
			pinMode( C_PIN, OUTPUT );
		}
	};
//---------------------------------------------------------------------------
} // Mitov

namespace ComponentsHardware
{
	extern void SystemUpdateHardware();
} // ComponentsHardware
//---------------------------------------------------------------------------
#endif

