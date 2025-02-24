////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_NEO_PIXEL_h
#define _MITOV_NEO_PIXEL_h

#include <Mitov.h>
#include <Mitov_Graphics_RGBW.h>

#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
  #include <NeoPixelBrightnessBus.h>
#else
  #include <MitovEmbedded_Adafruit_NeoPixel/MitovEmbedded_Adafruit_NeoPixel.h>
#endif

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_IMPLEMENTATION, 
		T_IMPLEMENTATION &C_IMPLEMENTATION,
		typename T_FModified
	> class NeoPixelsBasic :
		public T_FModified
	{
	public:
		_V_PROP_( FModified )

	public:
		void SetPixelColor( int AIndex, TRGBWColor AColor )
		{
//			Serial.print( "SetPixelColor: " );
//			Serial.print( AIndex );
//			Serial.print( " " );
//			Serial.print( AColor.ToString() );

#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			RgbColor ARgbColor( AColor.Red, AColor.Green, AColor.Blue );
			C_IMPLEMENTATION.SetPixelColor( AIndex, ARgbColor );
#else
			C_IMPLEMENTATION.setPixelColor( AIndex, AColor.Red, AColor.Green, AColor.Blue, AColor.White );
#endif
//			C_IMPLEMENTATION.show();
//			delay( 200 );
			FModified() = true;
		}

		TRGBWColor GetPixelColor( int AIndex )
		{
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			RgbColor ARgbColor = C_IMPLEMENTATION.GetPixelColor( AIndex );
			return TRGBWColor( ARgbColor.R, ARgbColor.G, ARgbColor.B, 0 );
#else
			return TRGBWColor( C_IMPLEMENTATION.getPixelColor( AIndex ), true );
#endif
		}

/*
		inline void SystemLoopBegin()
		{
			for( int i = 0; i < PixelGroups.size(); ++i )
				PixelGroups[ i ]->PixelsClock();

		}
*/
		inline void SystemLoopEnd()
		{
			ClockInputPin_o_Receive( nullptr );
		}

		inline void SystemInit()
		{
//			FPixel.setPin( C_PIN_NUMBER );

#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			C_IMPLEMENTATION.Begin();
#else
			C_IMPLEMENTATION.begin();
#endif
		}

	protected:
		void IntSetBrightness( float AValue )
		{
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
			C_IMPLEMENTATION.SetBrightness( AValue * 255 );
#else
			C_IMPLEMENTATION.setBrightness( AValue * 255 );
#endif
			FModified() = true;
		}
	public:
		void ClockInputPin_o_Receive(void *)
		{
			if( FModified() )
			{
//				Serial.println( "TEST11" );
//				Serial.println( "TEST12" );
#ifdef __VISUINO_USE_NEO_PIXELS_BUS_LIBRARY__
				C_IMPLEMENTATION.Show();
#else
				C_IMPLEMENTATION.show();
#endif
				FModified() = false;
			}
		}

	public:
		inline NeoPixelsBasic()
		{
			FModified() = true;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_IMPLEMENTATION, 
		T_IMPLEMENTATION &C_IMPLEMENTATION,
		typename T_Brightness,
		typename T_FModified
	> class NeoPixels : public NeoPixelsBasic <
				T_IMPLEMENTATION, 
				C_IMPLEMENTATION,
				T_FModified
			>,
		public T_Brightness
	{
		typedef NeoPixelsBasic <
				T_IMPLEMENTATION, 
				C_IMPLEMENTATION,
				T_FModified
		> inherited;

	public:
		_V_PROP_( Brightness )

	public:
		void SetBrightness( float AValue )
		{
			if( Brightness() == AValue )
				return;

			Brightness() = AValue;
			inherited::IntSetBrightness( AValue );
		}

	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
//			FPixel.setPin( C_PIN_NUMBER );

			inherited::IntSetBrightness( Brightness() );
		}

	};
/*
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_InitialColor,
		uint16_t C_START_PIXEL
	> class NeoPixelsBasicInitialColorGroup :
		public T_InitialColor
	{
	public:
		_V_PROP_( InitialColor )

	public:
		inline void SystemStart()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_InitialColor,
		uint16_t C_START_PIXEL
	> class NeoPixelsBasicColorGroup :
		public T_InitialColor
	{
	public:
		_V_PROP_( InitialColor )

	public:
		inline void SystemStart()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor );
		}

	public:
		void ApplyColorsAll()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor() );
		}

	protected:
		void ApplyColors() {}

		void IntSetColor( TRGBWColor AValue )
		{
			if( InitialColor == AValue )
				return;

			InitialColor() = AValue;
			ApplyColors(); 
//			FOwner.FModified = true;
		}

	public:
		inline void ColorInputPin_o_Receive( void *_Data )
		{
			IntSetColor( *(TRGBWColor *)_Data );
		}

	public:
		inline void SystemStart()
		{
			inherited::SystemStart();
			IntSetColor( InitialColor() );
		}

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_InitialColor,
		uint16_t C_START_PIXEL
	> class NeoPixelsGroup :
		public T_InitialColor
	{
	public:
		_V_PROP_( InitialColor )

	protected:
		TRGBWColor	FValues[ C_COUNT_PIXELS ] = { 0 };

	public:
		inline void SystemStart()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, TRGBWColor( InitialColor().GetValue()) );
		}

	public:
		void InputPins_o_Receive( int AIndex, void *_Data )
		{
			TRGBWColor AValue = *(TRGBWColor *)_Data;
			if( FValues[ AIndex ] == AValue )
				return;

			FValues[ AIndex ] = AValue;
			C_OWNER.SetPixelColor( AIndex, AValue );
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_InitialColor,
		uint16_t C_REPEAT_COUNT,
		uint16_t C_SUB_PIXEL_COUNT,
		uint16_t C_START_PIXEL
	> class NeoPixelsRepeatGroup : 
		public T_InitialColor
	{
	public:
		_V_PROP_( InitialColor )

	public:
		inline void SystemStart()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	public:
		void SetPixelColor( int AIndex, TRGBWColor AColor )
		{
			for( int i = 0; i < C_REPEAT_COUNT; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + AIndex + i * C_SUB_PIXEL_COUNT, AColor );
		}

		TRGBWColor GetPixelColor( int AIndex )
		{
			return C_OWNER.GetPixelColor( C_START_PIXEL + AIndex );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_InitialColor,
		uint16_t C_START_PIXEL
	> class NeoPixelsSingleColorGroup : 
		public T_InitialColor
	{
	public:
		_V_PROP_( InitialColor )

	public:
		void ApplyColorsAll()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	protected:
		void IntSetColor( TRGBWColor AValue )
		{
			if( InitialColor() == AValue )
				return;

			InitialColor() = AValue;
			ApplyColorsAll(); 
//			FOwner.FModified = true;
		}

	public:
		inline void ColorInputPin_o_Receive( void *_Data )
		{
			IntSetColor( *(TRGBWColor *)_Data );
		}

	public:
		inline void SystemStart()
		{
			ApplyColorsAll();
		}

	};
//---------------------------------------------------------------------------
/*
	class NeoPixelsReversedProperty
	{
	public:
		bool Reversed : 1;
		bool AllPixels : 1;

	public:
		NeoPixelsReversedProperty() :
			Reversed( false ),
			AllPixels( false )
		{
		}
	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_ColorOutputPin,
		typename T_InitialColor,
		typename T_Reversed,
		uint16_t C_START_PIXEL,
		typename T_StepInputPin_o_IsConnected
	> class NeoPixelsRunningColorGroup : 
		public T_ColorOutputPin,
		public T_InitialColor,
		public T_Reversed,
		public T_StepInputPin_o_IsConnected
	{
	public:
		_V_PIN_( ColorOutputPin )

	public:
		_V_PROP_( InitialColor )
		_V_PROP_( Reversed )

	public:
		_V_PROP_( StepInputPin_o_IsConnected )

	public:
		inline void SystemStart()
		{
			ApplyColorsAll();
		}

	public:
		inline void ApplyColorsAll()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	protected:
		void ApplyColors() {}

		void IntSetColor( TRGBWColor AValue )
		{
			if( InitialColor() == AValue )
				return;

			InitialColor() = AValue;
			ApplyColors(); 
//			FOwner.FModified = true;
		}

	public:
		inline void ColorInputPin_o_Receive( void *_Data )
		{
			IntSetColor( *(TRGBWColor *)_Data );
		}

	public:
		inline void UpdateReversePixels()
		{
			if( Reversed().AllPixels() )
				ReversePixels();

		}

	public:
		void ReversePixels()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS / 2; ++i )
			{
				TRGBWColor AOldColor1 = C_OWNER.GetPixelColor( C_START_PIXEL + ( C_COUNT_PIXELS - i - 1 ));
				TRGBWColor AOldColor2 = C_OWNER.GetPixelColor( C_START_PIXEL + i );

				C_OWNER.SetPixelColor( C_START_PIXEL + i, AOldColor1 );
				C_OWNER.SetPixelColor( C_START_PIXEL + ( C_COUNT_PIXELS - i - 1 ), AOldColor2 );
			}
		}

	protected:
		void AnimatePixels()
		{
			if( Reversed().Reversed() )
			{
				TRGBWColor AOldColor = C_OWNER.GetPixelColor( C_START_PIXEL );
				T_ColorOutputPin::SetPinValue( AOldColor );
				for( uint32_t i = 0; i < C_COUNT_PIXELS - 1; ++i )
				{
					AOldColor = C_OWNER.GetPixelColor( C_START_PIXEL + i + 1 );
					C_OWNER.SetPixelColor( C_START_PIXEL + i, AOldColor );
				}

				C_OWNER.SetPixelColor( C_START_PIXEL + C_COUNT_PIXELS - 1, InitialColor().GetValue() );
			}

			else
			{
				TRGBWColor AOldColor = C_OWNER.GetPixelColor( C_START_PIXEL + C_COUNT_PIXELS - 1 );
				T_ColorOutputPin::SetPinValue( AOldColor );
				for( int i = C_COUNT_PIXELS - 1; i--; )
				{
					AOldColor = C_OWNER.GetPixelColor( C_START_PIXEL + i );
					C_OWNER.SetPixelColor( C_START_PIXEL + i + 1, AOldColor );
				}

				C_OWNER.SetPixelColor( C_START_PIXEL, InitialColor().GetValue() );
			}
//			FOwner.FModified = true;
		}

	public:
		inline void StepInputPin_o_Receive( void *_Data )
		{
			AnimatePixels();
		}

	public:
		inline void SystemLoopBegin()
		{
			if( StepInputPin_o_IsConnected() )
				return;

			AnimatePixels();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint16_t C_COUNT_PIXELS,
		typename T_FModified,
		typename T_InitialColor,
		typename T_InitialIndex,
		uint16_t C_START_PIXEL
	> class NeoPixelsColorPixelGroup : 
		public T_FModified,
		public T_InitialColor,
		public T_InitialIndex
	{
	public:
		_V_PROP_( InitialIndex )

	protected:
		uint32_t	FIndex;
		_V_PROP_( FModified )

	public:
		_V_PROP_( InitialColor )

	public:
		inline void SystemStart()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	public:
		inline void ApplyColorsAll()
		{
			for( uint32_t i = 0; i < C_COUNT_PIXELS; ++i )
				C_OWNER.SetPixelColor( C_START_PIXEL + i, InitialColor().GetValue() );
		}

	protected:
		void IntSetColor( TRGBWColor AValue )
		{
			if( InitialColor() == AValue )
				return;

			InitialColor() = AValue;
			ApplyColors(); 
//			FOwner.FModified = true;
		}

	public:
		inline void ColorInputPin_o_Receive( void *_Data )
		{
			IntSetColor( *(TRGBWColor *)_Data );
		}

	public:
		inline void IndexInputPin_o_Receive( void *_Data )
		{
			uint32_t AIndex = *(uint32_t *)_Data;
			if( AIndex >= C_COUNT_PIXELS )
				AIndex = C_COUNT_PIXELS - 1;

			if( FIndex == AIndex )
				return;

			FIndex = AIndex;
			FModified() = true;
		}

	protected:
		void ApplyColors()
		{
			FModified() = true;
		}

	public:
		inline void SystemLoopBegin()
		{
			if( FModified() )
			{
				C_OWNER.SetPixelColor( C_START_PIXEL + InitialIndex(), InitialColor().GetValue() );
//				FOwner.FModified = true;
				FModified() = false;
			}
		}

	public:
		inline NeoPixelsColorPixelGroup()
		{
			FModified() = true;
		}

	};
//---------------------------------------------------------------------------
	class TRGBWColorTransfrom
	{
	public:
		static const TUniColor BLACK = 0xFF00000000;

	public:
		inline static uint8_t	ExtractGray( TUniColor AColor )
		{
			return ( AColor & 0xFF );
		}

		inline static uint8_t	ExtractAlpha( TUniColor AColor )
		{
			return ( AColor >> 32 );
		}

		inline static TUniColor	ExtractColor( TUniColor AColor )
		{
			return ( AColor & 0xFFFFFFFF );
		}

		inline static uint32_t	MakeGray( TUniColor AColor )
		{
			uint32_t AGray = ( AColor & 0xFF );
			return ( 0xFF00000000 | ( AGray << 16 ) | ( AGray << 16 ) | ( AGray << 8 ) | AGray );
		}

		inline static TUniColor	MakeColor( TUniColor AColor )
		{
			uint64_t AAlpha = ( AColor & 0xFF000000 );
			return ( AAlpha << 8 ) | ( AColor & 0xFFFFFF );
		}

		inline static TUniColor	BlendColor( TUniColor AOldColor, TUniColor AColor, float AAlpha )
		{
			float ACoefficient = AAlpha / 255.0f;

			uint8_t AWhite = (( AOldColor >> 24 ) & 0xFF ) * ( 1 - ACoefficient ) + (( AColor >> 24 ) & 0xFF ) * ACoefficient + 0.5;
			uint8_t ARed = (( AOldColor >> 16 ) & 0xFF ) * ( 1 - ACoefficient ) + (( AColor >> 16 ) & 0xFF ) * ACoefficient + 0.5;
			uint8_t AGreen = (( AOldColor >> 8 ) & 0xFF ) * ( 1 - ACoefficient ) + (( AColor >> 8 ) & 0xFF ) * ACoefficient + 0.5;
			uint8_t ABlue = ( AOldColor & 0xFF ) * ( 1 - ACoefficient ) + ( AColor & 0xFF ) * ACoefficient + 0.5;

			return ( ( uint32_t( AWhite ) << 24 ) | ( uint32_t( ARed ) << 16 ) | ( uint32_t( AGreen ) << 8 ) | ABlue );
		}

		inline static TUniColor	TransformBackgroundColor( TUniColor AColor )
		{
			return AColor;
		}
	};
//---------------------------------------------------------------------------
	class TRGBColorTransfrom
	{
	public:
		static const TUniColor BLACK = 0xFF000000;

	public:
		inline static uint8_t	ExtractGray( TUniColor AColor )
		{
			return ( AColor & 0xFF );
		}

		inline static uint8_t	ExtractAlpha( TUniColor AColor )
		{
			return ( AColor >> 24 );
		}

		inline static TUniColor	ExtractColor( TUniColor AColor )
		{
			return ( AColor & 0xFFFFFF );
		}

		inline static uint32_t	MakeGray( TUniColor AColor )
		{
			uint16_t AGray = ( AColor & 0xFF );
			return ( 0xFF000000 | ( AColor << 16 ) | ( AGray << 8 ) | AGray );
		}

		inline static TUniColor	MakeColor( TUniColor AColor )
		{
			return AColor;
//			uint64_t AAlpha = ( AColor & 0xFF000000 );
//			return ( AAlpha << 8 ) | ( AColor & 0xFFFFFF );
		}

		inline static TUniColor	BlendColor( TUniColor AOldColor, TUniColor AColor, float AAlpha )
		{
			float ACoefficient = AAlpha / 255.0f;

			uint8_t ARed = (( AOldColor >> 16 ) & 0xFF ) * ( 1 - ACoefficient ) + (( AColor >> 16 ) & 0xFF ) * ACoefficient + 0.5;
			uint8_t AGreen = (( AOldColor >> 8 ) & 0xFF ) * ( 1 - ACoefficient ) + (( AColor >> 8 ) & 0xFF ) * ACoefficient + 0.5;
			uint8_t ABlue = ( AOldColor & 0xFF ) * ( 1 - ACoefficient ) + ( AColor & 0xFF ) * ACoefficient + 0.5;

			return ( ( uint32_t( ARed ) << 16 ) | ( uint32_t( AGreen ) << 8 ) | ABlue );
		}

		inline static TUniColor	TransformBackgroundColor( TUniColor AColor )
		{
			return (( AColor & 0xFF ) << 16 ) | ( AColor & 0x00FF00 ) | (( AColor >> 16 ) & 0xFF );
		}
	};
//---------------------------------------------------------------------------
	class TSequentialMatrixMap
	{
	public:
		inline static uint32_t	Remap( uint32_t AIndex )
		{
			return AIndex;
		}

	};
//---------------------------------------------------------------------------
//#if 0
	template<
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_BackgroundColor,
		uint16_t C_COUNT_PIXELS, 
		typename T_Elements_Render,
		typename T_Font,
		int HEIGHT, 
		typename T_MATRIX_MAP, 
		typename T_Orientation,
		uint16_t C_START_PIXEL,
		typename T_COLOR_TRANSFORM, 
		typename T_Text,
		int WIDTH
	> class NeoPixels2DGraphicsGroup : 
		public GraphicsImpl<
			NeoPixels2DGraphicsGroup<
						T_OWNER, C_OWNER, 
						T_BackgroundColor,
						C_COUNT_PIXELS, 
						T_Elements_Render,
						T_Font,
						HEIGHT,
						T_MATRIX_MAP, 
						T_Orientation,
						C_START_PIXEL,
						T_COLOR_TRANSFORM, 
						T_Text,
						WIDTH
					>, 
				T_BackgroundColor,
				T_Font,
				T_Text
			>,
		public T_Orientation
//		public T_Text
	{
		typedef NeoPixels2DGraphicsGroup<
				T_OWNER, C_OWNER, 
				T_BackgroundColor,
				C_COUNT_PIXELS, 
				T_Elements_Render,
				T_Font,
				HEIGHT, 
				T_MATRIX_MAP, 
				T_Orientation,
				C_START_PIXEL,
				T_COLOR_TRANSFORM, 
				T_Text,
				WIDTH
			>	T_SELF;

		typedef GraphicsImpl<
				T_SELF,
				T_BackgroundColor,
				T_Font,
				T_Text
			>	graphics;

	public:
		_V_PROP_( Orientation )

	public:
		_V_PROP_( Text )

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::ChainCall();
		}

	public:
		int16_t width(void) const
		{
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return WIDTH;

			return HEIGHT;
		}

		int16_t height(void) const
		{
			if( T_Orientation::GetValue() == goUp || T_Orientation::GetValue() == goDown )
				return HEIGHT;

			return WIDTH;
		}

	public:
		inline T_OWNER &GetOwner() { return C_OWNER; }

	public:
		inline void SetBrightness( float AValue ) // Used for Owner Delegation
		{
			C_OWNER.SetBrightness( AValue );
		}

	public:
		inline void RefreshInputPin_o_Receive( void *_Data ) // Used for Owner Delegation
		{
			C_OWNER.ClockInputPin_o_Receive( _Data );
		}

	public:
		static inline void drawGrayscalePixel( T_SELF &AImplementation, int16_t x, int16_t y, TUniColor color)
		{
			AImplementation.drawPixel( x, y, T_COLOR_TRANSFORM::MakeGray( color ));
		}

		static inline void fillGrayscaleRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
			fillGrayscaleRect( AImplementation, x, y, w, h, T_COLOR_TRANSFORM::MakeGray( color ));
		}

		static inline void drawColorPixel( T_SELF &AImplementation, int16_t x, int16_t y, TUniColor color )
		{
			AImplementation.drawPixel( x, y, T_COLOR_TRANSFORM::MakeColor( color ) );
		}

		static inline void fillColorRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
//			Serial.println( "TEST88" );
			fillGrayscaleRect( AImplementation, x, y, w, h, T_COLOR_TRANSFORM::MakeColor( color ) );
		}

		void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
//			Serial.println( "GRAPHICS" );
			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return;

//			Serial.println( uint32_t( color ), HEX );
//			Serial.print( "drawPixel: " );
//			color = 0xFFFFFF0000;
			uint8_t AAlpha = T_COLOR_TRANSFORM::ExtractAlpha( color );
			if( AAlpha == 0 )
				return;

//			Serial.print( x ); Serial.print( ":" ); Serial.print( y ); Serial.print( " = " ); Serial.print( uint32_t( AAlpha ), HEX ); Serial.print( " " ); Serial.println( uint32_t( color ), HEX ); 

			switch ( Orientation() ) 
			{
				case goRight:
					swap(x, y);
					x = WIDTH - x - 1;
					break;

				case goDown:
					x = WIDTH - x - 1;
					y = HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = HEIGHT - y - 1;
					break;
			}  

			color = T_COLOR_TRANSFORM::ExtractColor( color );

//			Serial.print( "Color: " ); Serial.println( uint32_t( color ), HEX ); 

			uint32_t APixelIndex = C_START_PIXEL + T_MATRIX_MAP::Remap( x + y * WIDTH );

			if( AAlpha < 255 )
			{
				TUniColor AOldColor = C_OWNER.GetPixelColor( APixelIndex );
				color = T_COLOR_TRANSFORM::BlendColor( AOldColor, color, AAlpha );
			}

//			color = 0xFFFF0000;
//			Serial.print( C_START_PIXEL + x + y * WIDTH ); Serial.print( " = " ); Serial.println( uint32_t( color ));
			C_OWNER.SetPixelColor( APixelIndex, color );
//			FBuffer[ x + y * WIDTH ] = color;
//			FModified = true;
		}

		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if ((x < 0) || (x >= width() ) || (y < 0) || (y >= height() ))
				return T_COLOR_TRANSFORM::BLACK;

			switch ( Orientation() ) 
			{
				case goRight:
					swap(x, y);
					x = WIDTH - x - 1;
					break;

				case goDown:
					x = WIDTH - x - 1;
					y = HEIGHT - y - 1;
					break;

				case goLeft:
					swap(x, y);
					y = HEIGHT - y - 1;
					break;
			}  

			return TUniColor( C_OWNER.GetPixelColor( C_START_PIXEL + T_MATRIX_MAP::Remap( x + y * WIDTH ) )) | T_COLOR_TRANSFORM::BLACK;
//			return TUniColor( FBuffer[ x + y * WIDTH ] ) | 0xFF00000000;
		}

/*
	protected:
		inline void RenderElements()
		{
			graphics::RenderElements();
		}
*/
	public:
		inline void SystemStart()
		{
//			graphics::setTextSize( Text().Size() );
//			graphics::setTextColor( Text().Color(), Text().BackgroundColor() );
//			graphics::setTextWrap( Text().Wrap() );

			graphics::ClearScreen( *this, T_COLOR_TRANSFORM::TransformBackgroundColor( graphics::BackgroundColor() ) | T_COLOR_TRANSFORM::BLACK );
//			graphics::fillRect( *this, 0, 0, width(), height(), graphics::BackgroundColor | T_COLOR_TRANSFORM::BLACK );
//			Serial.println( "SystemStart" );
			graphics::RenderElements();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->Render( false );
		}

/*
		virtual void PixelsClock()
		{
			if( FModified )
			{
				for( int i = 0; i < WIDTH * HEIGHT; ++i )
				{
//					Serial.print( i ); Serial.print( " = " ); Serial.println( FBuffer[ i ], HEX ); 
					C_OWNER.SetPixelColor( C_START_PIXEL + i, FBuffer[ i ] );
				}

				FModified = false;
			}
		}
*/
	public:
		template<typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
			return graphics::write( AImplementation, AConfig, AValue );
		}

		template<typename T> void PrintChar( T AValue )
		{
			write( *this, *this, AValue );
		}

	public:
		template<typename T> void Print( T AValue )
		{
			println( AValue );
		}

	public:
		template<typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &graphics::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

		template<typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &graphics::template write<T_CONFIG> );
			return APrint.println( AValue );
		}

		template<typename T> inline size_t print( T AValue )
		{
			return print<T_SELF, T>( this, AValue );
		}

		template<typename T> inline size_t println( T AValue )
		{
			return println<T_SELF, T>( this, AValue );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER, 
		uint8_t C_INDEX,
		typename T_Enabled,
		typename T_InitialValue
	> class NeoPixelSingleLED :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void SystemStart()
		{
			if( Enabled() )
				C_OWNER.SetPixelColor( C_INDEX, TRGBWColor( InitialValue().GetValue() ));

			else
				C_OWNER.SetPixelColor( C_INDEX, TRGBWColor( 0 ));

//			C_OWNER.SetChannelValue( C_INDEX, 1 - InitialValue );
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TColor AColor = *(TColor *)_Data;
			if( AColor == InitialValue().GetValue() )
				return;

			InitialValue() = AColor;
			SystemStart();
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
