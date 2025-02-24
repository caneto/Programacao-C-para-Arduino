////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_GPIO_PCF857X_h
#define _MITOV_GPIO_PCF857X_h

#include <Mitov.h>
#include <SPI.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_I2C, T_I2C &C_I2C,
		uint8_t C_ADDRESS
	> class GPIO_PCF8574
	{
	protected:
		uint8_t	FDirectionBits = 0xFF;
		uint8_t	FReadBits = 0;

//	public:
//		void ( *Channels_CallChain )( uint32_t AIndex, bool AValue ) = nullptr;		

	protected:
		void UpdateAll()
		{
			C_I2C.beginTransmission( C_ADDRESS );
			C_I2C.write( FDirectionBits );
			C_I2C.endTransmission();
		}

	public:
		void PerformRead()
		{
			if( ! FDirectionBits )
				return;

			C_I2C.requestFrom( C_ADDRESS, uint8_t( 1 ) );
			FReadBits = C_I2C.read();

//			Serial.println( FReadBits, BIN );

//			if( Channels_CallChain )
//				Channels_CallChain

//			for( int i = 0; i < C_CHANNELS_COUNT; ++i )
//				FChannels[ i ]->UpdateOutput( GetBitValue( i ));

		}

	public:
		bool GetBitValue( uint8_t AIndex )
		{
			return( ( FReadBits & ( ((uint8_t)1 ) << AIndex )) != 0 );
		}

	public:
		void SetChannelValue( int AIndex, bool AValue )
		{
			if( AValue )
				FDirectionBits |= ( (uint8_t)1 ) << AIndex;

			else
				FDirectionBits &= ~( ((uint8_t)1 ) << AIndex );
		}

	public:
		inline void SystemStart()
		{
			UpdateAll();

//			for( int i = 0; i < C_CHANNELS_COUNT; ++i )
//				FChannels[ i ]->SendOutput();
		}

		inline void SystemLoopBegin()
		{
//			if( ! ReadInputPin.IsConnected() )
				PerformRead();
		}

		inline void SystemLoopUpdateHardware()
		{
			UpdateAll();
//			PerformRead();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_I2C, T_I2C &C_I2C,
		uint8_t C_ADDRESS
	> class GPIO_PCF8575
	{
	protected:
		uint16_t	FDirectionBits = 0xFFFF;
		uint16_t	FReadBits = 0;

	protected:
		void UpdateAll()
		{
			C_I2C.beginTransmission( C_ADDRESS );
			C_I2C.write( uint8_t( FDirectionBits ));
			C_I2C.write( uint8_t( FDirectionBits >> 8 ));
			C_I2C.endTransmission();
		}

	public:
		void PerformRead()
		{
			if( ! FDirectionBits )
				return;

			C_I2C.requestFrom( C_ADDRESS, (uint8_t)2 );
			FReadBits = C_I2C.read();
			FReadBits |= uint16_t( C_I2C.read() ) << 8;

//			Serial.println( FDirectionBits, BIN );
//			Serial.println( FReadBits, BIN );

//			for( int i = 0; i < C_CHANNELS_COUNT; ++i )
//				FChannels[ i ]->UpdateOutput( GetBitValue( i ));

		}

	public:
		bool GetBitValue( uint8_t AIndex )
		{
			return( ( FReadBits & ( ((uint16_t)1 ) << AIndex )) != 0 );
		}

	public:
		void SetChannelValue( int AIndex, bool AValue )
		{
			if( AValue )
				FDirectionBits |= ( (uint16_t)1 ) << AIndex;

			else
				FDirectionBits &= ~( ((uint16_t)1 ) << AIndex );
		}

	public:
		inline void SystemStart()
		{
			UpdateAll();

//			for( int i = 0; i < C_CHANNELS_COUNT; ++i )
//				FChannels[ i ]->SendOutput();
		}

		inline void SystemLoopBegin()
		{
//			if( ! ReadInputPin.IsConnected() )
				PerformRead();
		}

		inline void SystemLoopUpdateHardware()
		{
			UpdateAll();
//			PerformRead();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint8_t C_INDEX,
		typename T_InitialValue,
		typename T_IsCombinedInOut,
		typename T_IsOutput,
		typename T_OutputPin
	> class GPIO_PCF857X_Channel :
		public T_InitialValue,
		public T_IsCombinedInOut,
		public T_IsOutput,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( InitialValue )
        _V_PROP_( IsOutput )

	public:
        void PinDirectionsInit()
        {
			if( IsOutput() )
				C_OWNER.SetChannelValue( C_INDEX, InitialValue() );

			else
				C_OWNER.SetChannelValue( C_INDEX, true );

        }

	public:
		inline void SetMode( int AMode ) // Placeholder for direct pin control!
		{
//			pinMode( FPinNumber, AMode );
		}

		bool DigitalRead()
		{
//			Serial.println( "TEST1" );
			C_OWNER.PerformRead();
//			Serial.println( FInValue );
			return C_OWNER.GetBitValue( C_INDEX );
		}

		inline void DigitalWrite( bool AValue )
		{
			InitialValue() = AValue;
			C_OWNER.SetChannelValue( C_INDEX, AValue );
		}

	public:
		inline void InputPin_o_Receive( void * _Data )
		{
			DigitalWrite( *(bool *)_Data );
		}

	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( InitialValue() );
		}

		inline void SystemLoopBegin()
		{
			bool AValue = C_OWNER.GetBitValue( C_INDEX );
			T_OutputPin::SetPinValue( AValue );
		}

	public:
//		GPIO_PCF857X_Channel( bool AInitialValue, bool AIsOutput, bool AIsCombinedInOut ) :
//			IsOutput( AIsOutput ),
//			FValue( AInitialValue ),
//			FCombinedInOut( AIsCombinedInOut )
		inline GPIO_PCF857X_Channel()
		{
			PinDirectionsInit();
		}

	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif