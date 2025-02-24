////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_GRAPHICS_RGBW_h
#define _MITOV_GRAPHICS_RGBW_h

#include <Mitov.h>
#include <Mitov_Graphics_Color.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
//#ifdef _GRAPHICS_DRAW_ALPHA_RGBW_COLOR_BITMAP_

	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Centered,
		typename T_DATA,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsRGBWElementDrawAlphaBitmap :
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawAlphaRGBWColorBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_WIDTH, C_HEIGHT, Size() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};

//#endif // _GRAPHICS_DRAW_ALPHA_RGBW_COLOR_BITMAP_
//---------------------------------------------------------------------------	
//#ifdef _GRAPHICS_DRAW_REPEAT_ALPHA_RGBW_COLOR_BITMAP_

	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_COUNT,
		typename T_Centered,
		typename T_DATA,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsRGBWRepeatElementDrawAlphaBitmap :
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawRepeatAlphaRGBWColorBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_COUNT, C_WIDTH, C_HEIGHT, Size() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};

//#endif // _GRAPHICS_DRAW_REPEAT_ALPHA_RGBW_COLOR_BITMAP_
//---------------------------------------------------------------------------	
//#ifdef _GRAPHICS_DRAW_RGBW_COLOR_BITMAP_

	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Centered,
		typename T_DATA,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsRGBWElementDrawBitmap :
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawRGBWColorBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_WIDTH, C_HEIGHT, Size() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};

//#endif // _GRAPHICS_DRAW_RGBW_COLOR_BITMAP_
//---------------------------------------------------------------------------	
//#ifdef _GRAPHICS_DRAW_REPEAT_RGBW_COLOR_BITMAP_

	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		uint32_t C_COUNT,
		typename T_Centered,
		typename T_DATA,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsRGBWRepeatElementDrawBitmap :
		public T_Size,
		public T_Centered,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawRepeatRGBWColorBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_COUNT, C_WIDTH, C_HEIGHT, Size() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};

//#endif // _GRAPHICS_DRAW_REPEAT_RGBW_COLOR_BITMAP_
//---------------------------------------------------------------------------	
	template<
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_BackgroundColor,
		typename T_BlackColor,
		typename T_Elements_Render,
        typename T_Font,
		typename T_Text,
		typename T_WhiteColor,
		typename T_X,
		typename T_Y
	> class GraphicsElementRGBWMonohromeDrawScene : 
		public GraphicsIntf<T_OWNER>, 
		public Graphics<
			GraphicsElementRGBWMonohromeDrawScene<
					T_OWNER, C_OWNER,
					T_BackgroundColor,
					T_BlackColor,
					T_Elements_Render,
					T_Font,
					T_Text,
					T_WhiteColor,
					T_X,
					T_Y
				>
			>,
		public T_WhiteColor,
		public T_BlackColor,
		public T_BackgroundColor,
		public T_X,
		public T_Y
	{
		typedef GraphicsIntf<T_OWNER> inherited;
		typedef GraphicsElementRGBWMonohromeDrawScene<
				T_OWNER, C_OWNER,
				T_BackgroundColor,
				T_BlackColor,
				T_Elements_Render,
				T_Font,
				T_Text,
				T_WhiteColor,
				T_X,
				T_Y
			>	T_SELF;

		typedef Graphics<T_SELF>	graphics;

    public:
		_V_PROP_( WhiteColor )
		_V_PROP_( BlackColor )
		_V_PROP_( BackgroundColor )
		_V_PROP_( Text )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::ChainCall();
		}

	public:
		template<typename T_SELF_DELEGATOR> static inline uint8_t DrawChar( T_SELF_DELEGATOR &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
			return T_Font::DrawChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );
		}

	public:
		inline void GetPosition( TGraphicsPos &AX, TGraphicsPos &AY )
		{ 
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			AX = AParentX + X();
			AY = AParentY + Y();
		}

	public:
		inline T_BackgroundColor	&GetBackgroundColor()
		{
			return BackgroundColor();
		}

		inline int16_t width(void) const
		{
			return C_OWNER.width() - T_X::GetValue();
		}

		inline int16_t height(void) const
		{
			return C_OWNER.height() - T_X::GetValue();
		}

		void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
			if( color == tmcNone )
				return;

			switch( color )
			{
				case tmcWhite:	color =	WhiteColor(); break;
				case tmcBlack:	color =	BlackColor(); break;
				case tmcInvert:	color =	( C_OWNER.GetPixelColor( x, y ) > 0xFF00000000 ) ? WhiteColor() : BlackColor(); break;
			}

//			color = WhiteColor; //0xFF0000ff00;
//			color = 0xFFFFFF0000;
			//color = 0xFF000000FF;
			C_OWNER.drawPixel( x, y, color );
		}

		inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			return ( C_OWNER.GetPixelColor( x, y ) > 0xFF00000000 ) ? tmcWhite : tmcBlack;
		}

	public:
		inline void Render()
		{
			T_Elements_Render::ChainCall();
//			inherited::RenderElements();
//			for( int i = 0; i < GraphicsIntf::FElements.size(); ++ i )
//				GraphicsIntf::FElements[ i ]->Render( AForce );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

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
		typename T_BackgroundColor,
		typename T_Elements_Render,
        typename T_Font,
		typename T_Text,
		typename T_X,
		typename T_Y
	> class GraphicsElementRGBWGrayscaleDrawScene : 
		public GraphicsIntf<T_OWNER>,
		public Graphics<
			GraphicsElementRGBWGrayscaleDrawScene<
					T_OWNER, C_OWNER,
					T_BackgroundColor,
					T_Elements_Render,
					T_Font,
					T_Text,
					T_X,
					T_Y
				>
			>,
		public T_BackgroundColor,
		public T_Text,
		public T_X,
		public T_Y
	{
		typedef GraphicsIntf<T_OWNER> inherited;
		typedef GraphicsElementRGBWGrayscaleDrawScene<
				T_OWNER, C_OWNER,
				T_BackgroundColor,
				T_Elements_Render,
				T_Font,
				T_Text,
				T_X,
				T_Y
			>	T_SELF;

		typedef Graphics<T_SELF>	graphics;

    public:
		_V_PROP_( BackgroundColor )
		_V_PROP_( Text )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::ChainCall();
		}

	public:
		template<typename T_SELF_DELEGATOR> static inline uint8_t DrawChar( T_SELF_DELEGATOR &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
			return T_Font::DrawChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );
		}

	public:
		inline void GetPosition( TGraphicsPos &AX, TGraphicsPos &AY )
		{ 
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			AX = AParentX + X();
			AY = AParentY + Y();
		}

	public:
		inline T_BackgroundColor &GetBackgroundColor(void)
		{
			return BackgroundColor();
		}

		inline int16_t width(void) const
		{
			return C_OWNER.width() - T_X::GetValue();
		}

		inline int16_t height(void) const
		{
			return C_OWNER.height() - T_Y::GetValue();
		}

		static void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
			if( !( color & 0xFF00 ))
				return;

			uint16_t AGray = color & 0xFF;
			color = 0xFF00000000 | ( color << 16 ) | ( AGray << 8 ) | AGray;

			C_OWNER.drawPixel( x, y, color );
		}

		static TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			uint32_t	AColor = C_OWNER.GetPixelColor( x, y );
			uint8_t ARed = AColor >> 16;
			uint8_t AGreen = AColor >> 8;
			uint8_t ABlue = AColor >> 8;
			uint16_t AGray = 0.2989 * ARed + 0.5870 * AGreen + 0.1140 * ABlue + 0.5;
			AGray |= ( AColor >> 16 ) & 0xFF00;
			return AGray;
		}

/*
	protected:
		inline void RenderElements()
		{
			C_OWNER.fillRect( C_OWNER, 0, 0, width(), height(), BackgroundColor );
			inherited::RenderElements();
		}
*/
	public:
		inline void Render()
		{
			T_Elements_Render::ChainCall();
//			inherited::RenderElements();
//			for( int i = 0; i < GraphicsIntf::FElements.size(); ++ i )
//				GraphicsIntf::FElements[ i ]->Render( AForce );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

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

	public:
		using graphics::graphics;
//		GraphicsElementRGBWGrayscaleDrawScene( const unsigned char *AFont ) :
//			Graphics<GraphicsElementRGBWGrayscaleDrawScene<T_OWNER,C_OWNER>>( AFont )
//		{
//		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_BackgroundColor,
		typename T_Elements_Render,
        typename T_Font,
		typename T_Text,
		typename T_X,
		typename T_Y
	> class GraphicsElementRGBWColorDrawScene : 
		public GraphicsIntf<T_OWNER>, 
		public Graphics<
				GraphicsElementRGBWColorDrawScene<
					T_OWNER,C_OWNER,
					T_BackgroundColor,
					T_Elements_Render,
					T_Font,
					T_Text,
					T_X,
					T_Y
				>
			>,
		public T_BackgroundColor,
		public T_Text,
		public T_X,
		public T_Y
	{
		typedef GraphicsIntf<T_OWNER> inherited;
		typedef GraphicsElementRGBWColorDrawScene<
				T_OWNER,C_OWNER,
				T_BackgroundColor,
				T_Elements_Render,
				T_Font,
				T_Text,
				T_X,
				T_Y
			>	T_SELF;

		typedef Graphics<T_SELF>	graphics;

    public:
		_V_PROP_( BackgroundColor )
		_V_PROP_( Text )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::ChainCall();
		}

	public:
		template<typename T_SELF_DELEGATOR> static inline uint8_t DrawChar( T_SELF_DELEGATOR &AImplementation, int16_t x, int16_t y, unsigned char c, TUniColor color, TUniColor bg, uint8_t size, bool AUseCp437 )
		{
			return T_Font::DrawChar( AImplementation, x, y, c, color, bg, size, AUseCp437 );
		}

	public:
		inline void GetPosition( TGraphicsPos &AX, TGraphicsPos &AY )
		{ 
			C_OWNER.GetPosition( AX, AY );

			AX += X();
			AY += Y();
		}

	public:
		inline T_BackgroundColor &GetBackgroundColor()
		{
			return BackgroundColor();
		}

		inline int16_t width(void) const
		{
			return C_OWNER.width() - T_X::GetValue();
		}

		inline int16_t height(void) const
		{
			return C_OWNER.height() - T_Y::GetValue();
		}

		static void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
			if( !( color & 0xFF000000 ))
				return;

			color = (( color & 0xFF000000 ) << 8 ) | ( color & 0xFFFFFF );

//			uint16_t AGray = color & 0xFF;
//			color = ( color << 16 ) | ( AGray << 8 ) | AGray;

			C_OWNER.drawPixel( x, y, color );
		}

		static TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			TUniColor	AColor = C_OWNER.GetPixelColor( x, y );

			AColor = (( AColor & 0xFF00000000 ) >> 8 ) | ( AColor & 0xFFFFFF );

			return AColor;
		}

	public:
		inline void Render()
		{
			T_Elements_Render::ChainCall();
//			inherited::RenderElements();
//			for( int i = 0; i < GraphicsIntf::FElements.size(); ++ i )
//				GraphicsIntf::FElements[ i ]->Render( AForce );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

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

	public:
		using graphics::graphics;

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER,
		typename T_CARD,
		T_OWNER &C_OWNER,
		T_CARD &C_CARD, 
		typename T_X, 
		typename T_Y
	> class GraphicsRGBWElementDrawGraphics : 
		public GraphicsDelegate<
			GraphicsRGBWElementDrawGraphics<
				T_OWNER,
				T_CARD,
				C_OWNER,
				C_CARD, 
				T_X, 
				T_Y
			>, T_OWNER, C_OWNER>,
		public T_X,
		public T_Y
	{
		typedef GraphicsRGBWElementDrawGraphics<
				T_OWNER,
				T_CARD,
				C_OWNER,
				C_CARD, 
				T_X, 
				T_Y
			> T_SELF;

    public:
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		void Render()
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			C_CARD.template RenderGraphics<T_SELF>( this, AParentX + X(), AParentY + Y() );

//			FOwner.GetGraphics()->drawRepeatColorBitmap( AParentX + X, AParentY + Y, _Data, Count, Width, Height, Size );
		}

	public:
		inline TUniColor MakeColorColor( uint8_t r, uint8_t g, uint8_t b )
		{
			return 0xFF00000000 | ( uint32_t( r ) << 16 ) | ( uint32_t( g ) << 8 ) | b;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
//---------------------------------------------------------------------------	
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
