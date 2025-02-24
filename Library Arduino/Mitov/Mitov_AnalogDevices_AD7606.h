////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ANALOGDEVICES_AD7606_h
#define _MITOV_ANALOGDEVICES_AD7606_h

#include <Mitov.h>

namespace Mitov
{
	enum TArduinoAD7606Oversampling { aosNone, aos2, aos4, aos8, aos16, aos32, aos64 };
//---------------------------------------------------------------------------
	template <
		uint16_t C_BUSY_PIN,
		uint16_t C_CHIP_SELECT_PIN,
		uint16_t C_DATA_PIN_0,
		uint16_t C_DATA_PIN_1,
		uint16_t C_DATA_PIN_10,
		uint16_t C_DATA_PIN_11,
		uint16_t C_DATA_PIN_12,
		uint16_t C_DATA_PIN_13,
		uint16_t C_DATA_PIN_14,
		uint16_t C_DATA_PIN_15,
		uint16_t C_DATA_PIN_2,
		uint16_t C_DATA_PIN_3,
		uint16_t C_DATA_PIN_4,
		uint16_t C_DATA_PIN_5,
		uint16_t C_DATA_PIN_6,
		uint16_t C_DATA_PIN_7,
		uint16_t C_DATA_PIN_8,
		uint16_t C_DATA_PIN_9,
		uint16_t C_READ_PIN,
		uint16_t C_RESET_PIN,
		uint16_t C_START_PIN,
		typename T_Enabled,
		typename T_MostSignificantFirst,
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2,
		typename T_OutputPins_3,
		typename T_OutputPins_4,
		typename T_OutputPins_5,
		typename T_OutputPins_6,
		typename T_OutputPins_7,
		typename T_Oversampling,
		typename T_OversamplingOutputPins_0,
		typename T_OversamplingOutputPins_1,
		typename T_OversamplingOutputPins_2
	> class AD7606Parallel16bit :
		public T_Enabled,
		public T_MostSignificantFirst,
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_OutputPins_2,
		public T_OutputPins_3,
		public T_OutputPins_4,
		public T_OutputPins_5,
		public T_OutputPins_6,
		public T_OutputPins_7,
		public T_Oversampling,
		public T_OversamplingOutputPins_0,
		public T_OversamplingOutputPins_1,
		public T_OversamplingOutputPins_2
	{
	public:
//		OpenWire::SourcePin	OutputPins[ 8 ];
//		OpenWire::SourcePin	OversamplingOutputPins[ 3 ];

		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )
		_V_PIN_( OutputPins_3 )
		_V_PIN_( OutputPins_4 )
		_V_PIN_( OutputPins_5 )
		_V_PIN_( OutputPins_6 )
		_V_PIN_( OutputPins_7 )

		_V_PIN_( OversamplingOutputPins_0 )
		_V_PIN_( OversamplingOutputPins_1 )
		_V_PIN_( OversamplingOutputPins_2 )

	public:
		_V_PROP_( Enabled )

		_V_PROP_( Oversampling )

	protected:
//		uint16_t FPins[ 16 + 5 ];

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

//			Serial.println( "READ" );
			digitalWrite( C_START_PIN, LOW );

			unsigned long ACurrentTime = micros();

			digitalWrite( C_START_PIN, HIGH );

//			Serial.println( "WAIT" );
			while( micros() - ACurrentTime < 4 ); // Wait 4 uS
//			Serial.println( "WAIT1" );
			while( digitalRead( C_BUSY_PIN ) != LOW ); // Wait to finish reading
//			Serial.println( "WAIT2" );

			digitalWrite( C_CHIP_SELECT_PIN, LOW );

			for( int AChannel = 0; AChannel < 8; ++AChannel )
			{
				digitalWrite( C_READ_PIN, LOW );

				uint16_t AResult = 0;
//				for( int i = 0; i < 16; ++i )
//					if( digitalRead( FPins[ i ] ) == HIGH )
//						AResult |= ( 1 << i );

				if( digitalRead( C_DATA_PIN_0 ) == HIGH )
					AResult |= ( 1 << 0 );
					
				if( digitalRead( C_DATA_PIN_1 ) == HIGH )
					AResult |= ( 1 << 1 );
					
				if( digitalRead( C_DATA_PIN_2 ) == HIGH )
					AResult |= ( 1 << 2 );
					
				if( digitalRead( C_DATA_PIN_3 ) == HIGH )
					AResult |= ( 1 << 3 );
					
				if( digitalRead( C_DATA_PIN_4 ) == HIGH )
					AResult |= ( 1 << 4 );
					
				if( digitalRead( C_DATA_PIN_5 ) == HIGH )
					AResult |= ( 1 << 5 );
					
				if( digitalRead( C_DATA_PIN_6 ) == HIGH )
					AResult |= ( 1 << 6 );
					
				if( digitalRead( C_DATA_PIN_7 ) == HIGH )
					AResult |= ( 1 << 7 );

				if( digitalRead( C_DATA_PIN_8 ) == HIGH )
					AResult |= ( 1 << 8 );

				if( digitalRead( C_DATA_PIN_9 ) == HIGH )
					AResult |= ( 1 << 9 );

				if( digitalRead( C_DATA_PIN_10 ) == HIGH )
					AResult |= ( 1 << 10 );

				if( digitalRead( C_DATA_PIN_11 ) == HIGH )
					AResult |= ( 1 << 11 );

				if( digitalRead( C_DATA_PIN_12 ) == HIGH )
					AResult |= ( 1 << 12 );

				if( digitalRead( C_DATA_PIN_13 ) == HIGH )
					AResult |= ( 1 << 13 );

				if( digitalRead( C_DATA_PIN_14 ) == HIGH )
					AResult |= ( 1 << 14 );

				if( digitalRead( C_DATA_PIN_15 ) == HIGH )
					AResult |= ( 1 << 15 );

				digitalWrite( C_READ_PIN, HIGH );

//				if( ! AChannel )
//					Serial.println( AResult, HEX );

				float AValue = float( int16_t( AResult )) / 0x7FFF;
//				OutputPins[ AChannel ].Notify( &AValue );
				switch( AChannel )
				{
					case 0: T_OutputPins_0::SetPinValue( AValue ); break;
					case 1: T_OutputPins_1::SetPinValue( AValue ); break;
					case 2: T_OutputPins_2::SetPinValue( AValue ); break;
					case 3: T_OutputPins_3::SetPinValue( AValue ); break;
					case 4: T_OutputPins_4::SetPinValue( AValue ); break;
					case 5: T_OutputPins_5::SetPinValue( AValue ); break;
					case 6: T_OutputPins_6::SetPinValue( AValue ); break;
					case 7: T_OutputPins_7::SetPinValue( AValue ); break;
				}
			}

			digitalWrite( C_CHIP_SELECT_PIN, HIGH );
		}

		void ResetInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "RESET" );
			digitalWrite( C_RESET_PIN, HIGH );
			delay( 1 );
			digitalWrite( C_RESET_PIN, LOW );
		}

	public:
		inline void SystemStart()
		{
//			for( int i = 0; i < 16; ++ i )
//				pinMode( FPins[ i ], INPUT );

			pinMode( C_DATA_PIN_0, INPUT );
			pinMode( C_DATA_PIN_1, INPUT );
			pinMode( C_DATA_PIN_2, INPUT );
			pinMode( C_DATA_PIN_3, INPUT );
			pinMode( C_DATA_PIN_4, INPUT );
			pinMode( C_DATA_PIN_5, INPUT );
			pinMode( C_DATA_PIN_6, INPUT );
			pinMode( C_DATA_PIN_7, INPUT );
			pinMode( C_DATA_PIN_8, INPUT );
			pinMode( C_DATA_PIN_9, INPUT );
			pinMode( C_DATA_PIN_10, INPUT );
			pinMode( C_DATA_PIN_11, INPUT );
			pinMode( C_DATA_PIN_12, INPUT );
			pinMode( C_DATA_PIN_13, INPUT );
			pinMode( C_DATA_PIN_14, INPUT );
			pinMode( C_DATA_PIN_15, INPUT );

//			for( int i = 16; i < 16 + 4; ++ i )
//				pinMode( FPins[ i ], OUTPUT );

			pinMode( C_CHIP_SELECT_PIN, OUTPUT );
			pinMode( C_READ_PIN, OUTPUT );
			pinMode( C_RESET_PIN, OUTPUT );
			pinMode( C_START_PIN, OUTPUT );

			digitalWrite( C_CHIP_SELECT_PIN, HIGH );
			digitalWrite( C_READ_PIN, HIGH );
			digitalWrite( C_START_PIN, HIGH );
			digitalWrite( C_RESET_PIN, LOW );

			pinMode( C_BUSY_PIN, INPUT );
			
//			for( int i = 0; i < 3; ++i )
//				OversamplingOutputPins[ i ].SendValue<bool>( Oversampling & ( 1 << i ) != 0 );
			
			T_OversamplingOutputPins_0::SetPinValue( ( Oversampling().GetValue() & ( 1 )) != 0 );
			T_OversamplingOutputPins_1::SetPinValue( ( Oversampling().GetValue() & ( 1 << 1 )) != 0 );
			T_OversamplingOutputPins_2::SetPinValue( ( Oversampling().GetValue() & ( 1 << 2 )) != 0 );

			ResetInputPin_o_Receive( nullptr );
		}

		inline void SystemLoopBegin()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
	template <
		uint16_t C_BUSY_PIN,
		uint16_t C_CHIP_SELECT_PIN,
		uint16_t C_DATA_PIN_0,
		uint16_t C_DATA_PIN_1,
		uint16_t C_DATA_PIN_2,
		uint16_t C_DATA_PIN_3,
		uint16_t C_DATA_PIN_4,
		uint16_t C_DATA_PIN_5,
		uint16_t C_DATA_PIN_6,
		uint16_t C_DATA_PIN_7,
		uint16_t C_READ_PIN,
		uint16_t C_RESET_PIN,
		uint16_t C_START_PIN,
		typename T_Enabled,
		typename T_MostSignificantFirst,
		typename T_OutputPins_0,
		typename T_OutputPins_1,
		typename T_OutputPins_2,
		typename T_OutputPins_3,
		typename T_OutputPins_4,
		typename T_OutputPins_5,
		typename T_OutputPins_6,
		typename T_OutputPins_7,
		typename T_Oversampling,
		typename T_OversamplingOutputPins_0,
		typename T_OversamplingOutputPins_1,
		typename T_OversamplingOutputPins_2
	> class AD7606Parallel8bit :
		public T_Enabled,
		public T_MostSignificantFirst,
		public T_OutputPins_0,
		public T_OutputPins_1,
		public T_OutputPins_2,
		public T_OutputPins_3,
		public T_OutputPins_4,
		public T_OutputPins_5,
		public T_OutputPins_6,
		public T_OutputPins_7,
		public T_Oversampling,
		public T_OversamplingOutputPins_0,
		public T_OversamplingOutputPins_1,
		public T_OversamplingOutputPins_2
	{
//	protected:
//		static const uint8_t RESET_PIN = 8;
//		static const uint8_t CHIP_SELECT_PIN = 9;
//		static const uint8_t READ_PIN = 10;
//		static const uint8_t START_PIN = 11;
//		static const uint8_t BUSY_PIN = 12;

	public:
//		OpenWire::SourcePin	OutputPins[ 8 ];
//		OpenWire::SourcePin	OversamplingOutputPins[ 3 ];

		_V_PIN_( OutputPins_0 )
		_V_PIN_( OutputPins_1 )
		_V_PIN_( OutputPins_2 )
		_V_PIN_( OutputPins_3 )
		_V_PIN_( OutputPins_4 )
		_V_PIN_( OutputPins_5 )
		_V_PIN_( OutputPins_6 )
		_V_PIN_( OutputPins_7 )

		_V_PIN_( OversamplingOutputPins_0 )
		_V_PIN_( OversamplingOutputPins_1 )
		_V_PIN_( OversamplingOutputPins_2 )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( MostSignificantFirst )

		_V_PROP_( Oversampling )

//	protected:
//		uint16_t FPins[ 8 + 5 ];

	protected:
		uint16_t WriteMostSignificant()
		{
			uint16_t AResult = 0;

			if( digitalRead( C_DATA_PIN_0 ) == HIGH )
				AResult |= ( 1 << ( 8 + 0 ));
					
			if( digitalRead( C_DATA_PIN_1 ) == HIGH )
				AResult |= ( 1 << ( 8 + 1 ));
					
			if( digitalRead( C_DATA_PIN_2 ) == HIGH )
				AResult |= ( 1 << ( 8 + 2 ));
					
			if( digitalRead( C_DATA_PIN_3 ) == HIGH )
				AResult |= ( 1 << ( 8 + 3 ));
					
			if( digitalRead( C_DATA_PIN_4 ) == HIGH )
				AResult |= ( 1 << ( 8 + 4 ));
					
			if( digitalRead( C_DATA_PIN_5 ) == HIGH )
				AResult |= ( 1 << ( 8 + 5 ));
					
			if( digitalRead( C_DATA_PIN_6 ) == HIGH )
				AResult |= ( 1 << ( 8 + 6 ));
					
			if( digitalRead( C_DATA_PIN_7 ) == HIGH )
				AResult |= ( 1 << ( 8 + 7 ));
					
			return AResult;
		}

		uint16_t WriteLeastSignificant()
		{
			uint16_t AResult = 0;

			if( digitalRead( C_DATA_PIN_0 ) == HIGH )
				AResult |= ( 1 << 0 );
					
			if( digitalRead( C_DATA_PIN_1 ) == HIGH )
				AResult |= ( 1 << 1 );
					
			if( digitalRead( C_DATA_PIN_2 ) == HIGH )
				AResult |= ( 1 << 2 );
					
			if( digitalRead( C_DATA_PIN_3 ) == HIGH )
				AResult |= ( 1 << 3 );
					
			if( digitalRead( C_DATA_PIN_4 ) == HIGH )
				AResult |= ( 1 << 4 );
					
			if( digitalRead( C_DATA_PIN_5 ) == HIGH )
				AResult |= ( 1 << 5 );
					
			if( digitalRead( C_DATA_PIN_6 ) == HIGH )
				AResult |= ( 1 << 6 );
					
			if( digitalRead( C_DATA_PIN_7 ) == HIGH )
				AResult |= ( 1 << 7 );

			return AResult;
		}

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
				return;

//			Serial.println( "READ" );
			digitalWrite( C_START_PIN, LOW );

			unsigned long ACurrentTime = micros();

			digitalWrite( C_START_PIN, HIGH );

//			Serial.println( "WAIT" );
			while( micros() - ACurrentTime < 4 ); // Wait 4 uS
//			Serial.println( "WAIT1" );
			while( digitalRead( C_BUSY_PIN ) != LOW ); // Wait to finish reading
//			Serial.println( "WAIT2" );

			digitalWrite( C_CHIP_SELECT_PIN, LOW );

			for( int AChannel = 0; AChannel < 8; ++AChannel )
			{
				uint16_t AResult = 0;

				digitalWrite( C_READ_PIN, LOW );

				if( MostSignificantFirst() )
				{
					AResult |= WriteMostSignificant();
//					for( int i = 0; i < 8; ++i )
//						if( digitalRead( FPins[ i ] ) == HIGH )
//							AResult |= ( 1 << ( 8 + i ));
				}

				else
				{
					AResult |= WriteLeastSignificant();
//					for( int i = 0; i < 8; ++i )
//						if( digitalRead( FPins[ i ] ) == HIGH )
//							AResult |= ( 1 << i );
				}

				digitalWrite( C_READ_PIN, HIGH );

//				delay( 100 );

				digitalWrite( C_READ_PIN, LOW );

				if( MostSignificantFirst() )
				{
					AResult |= WriteLeastSignificant();
//					for( int i = 0; i < 8; ++i )
//						if( digitalRead( FPins[ i ] ) == HIGH )
//							AResult |= ( 1 << i );
				}

				else
				{
					AResult |= WriteMostSignificant();
//					for( int i = 0; i < 8; ++i )
//						if( digitalRead( FPins[ i ] ) == HIGH )
//							AResult |= ( 1 << ( 8 + i ));
				}

				digitalWrite( C_READ_PIN, HIGH );

//				if( ! AChannel )
//					Serial.println( AResult, HEX );

				float AValue = float( int16_t( AResult )) / 0x7FFF;
//				OutputPins[ AChannel ].Notify( &AValue );
				switch( AChannel )
				{
					case 0: T_OutputPins_0::SetPinValue( AValue ); break;
					case 1: T_OutputPins_1::SetPinValue( AValue ); break;
					case 2: T_OutputPins_2::SetPinValue( AValue ); break;
					case 3: T_OutputPins_3::SetPinValue( AValue ); break;
					case 4: T_OutputPins_4::SetPinValue( AValue ); break;
					case 5: T_OutputPins_5::SetPinValue( AValue ); break;
					case 6: T_OutputPins_6::SetPinValue( AValue ); break;
					case 7: T_OutputPins_7::SetPinValue( AValue ); break;
				}
			}

			digitalWrite( C_CHIP_SELECT_PIN, HIGH );
		}

		void ResetInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "RESET" );
			digitalWrite( C_RESET_PIN, HIGH );
			delay( 1 );
			digitalWrite( C_RESET_PIN, LOW );
		}

	public:
		inline void SystemStart()
		{
//			for( int i = 0; i < 8; ++ i )
//				pinMode( FPins[ i ], INPUT );

			pinMode( C_DATA_PIN_0, INPUT );
			pinMode( C_DATA_PIN_1, INPUT );
			pinMode( C_DATA_PIN_2, INPUT );
			pinMode( C_DATA_PIN_3, INPUT );
			pinMode( C_DATA_PIN_4, INPUT );
			pinMode( C_DATA_PIN_5, INPUT );
			pinMode( C_DATA_PIN_6, INPUT );
			pinMode( C_DATA_PIN_7, INPUT );

//			for( int i = 8; i < 8 + 4; ++ i )
//				pinMode( FPins[ i ], OUTPUT );

			pinMode( C_CHIP_SELECT_PIN, OUTPUT );
			pinMode( C_READ_PIN, OUTPUT );
			pinMode( C_RESET_PIN, OUTPUT );
			pinMode( C_START_PIN, OUTPUT );

			digitalWrite( C_CHIP_SELECT_PIN, HIGH );
			digitalWrite( C_READ_PIN, HIGH );
			digitalWrite( C_START_PIN, HIGH );
			digitalWrite( C_RESET_PIN, LOW );

			pinMode( C_BUSY_PIN, INPUT );
			
//			for( int i = 0; i < 3; ++i )
//				OversamplingOutputPins[ i ].SendValue<bool>( Oversampling & ( 1 << i ) != 0 );
			
			T_OversamplingOutputPins_0::SetPinValue( ( Oversampling().GetValue() & ( 1 )) != 0 );
			T_OversamplingOutputPins_1::SetPinValue( ( Oversampling().GetValue() & ( 1 << 1 )) != 0 );
			T_OversamplingOutputPins_2::SetPinValue( ( Oversampling().GetValue() & ( 1 << 2 )) != 0 );

			ResetInputPin_o_Receive( nullptr );
		}

		inline void SystemLoopBegin()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	};
//---------------------------------------------------------------------------
}

#endif
