////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_GRAPHICS_GRAYSCALE_h
#define _MITOV_GRAPHICS_GRAYSCALE_h

#include <Mitov.h>
#include <Mitov_Graphics.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
        typename T_OWNER,
        typename T_BackgroundColor,
        typename T_Font,
        typename T_Text
    > class GraphicsGrayscale :
        public GraphicsImpl<
                T_OWNER,
                T_BackgroundColor,
                T_Font,
                T_Text
            >
	{
		typedef GraphicsImpl<
                T_OWNER,
                T_BackgroundColor,
                T_Font,
                T_Text
            > inherited;

	public:
		static inline void Scroll( T_OWNER &AImplementation, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
			color |= 0xFF00;
			inherited::Scroll( AImplementation, X, Y, color );
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
//#ifdef _GRAPHICS_DRAW_GRAYSCALE_ALPHA_BITMAP_

	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Centered,
		typename T_DATA,
		uint32_t C_HEIGHT, 
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsGrayscaleElementDrawAlphaBitmap :
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

			T_OWNER::drawAlphaGrayscaleBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_WIDTH, C_HEIGHT, Size() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};

//#endif // _GRAPHICS_DRAW_GRAYSCALE_ALPHA_BITMAP_
//---------------------------------------------------------------------------	
//#ifdef _GRAPHICS_DRAW_GRAYSCALE_REPEAT_ALPHA_BITMAP_

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
	> class GraphicsGrayscaleRepeatElementDrawAlphaBitmap :
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

			T_OWNER::drawAlphaGrayscaleRepeatBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_COUNT, C_WIDTH, C_HEIGHT, Size() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};

//#endif // _GRAPHICS_DRAW_GRAYSCALE_REPEAT_ALPHA_BITMAP_
//---------------------------------------------------------------------------	
//#ifdef _GRAPHICS_DRAW_GRAYSCALE_BITMAP_

	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Centered,
		typename T_DATA,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsGrayscaleElementDrawBitmap :
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

			T_OWNER::drawGrayscaleBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_WIDTH, C_HEIGHT, 255, Size() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};

//#endif // _GRAPHICS_DRAW_GRAYSCALE_BITMAP_
//---------------------------------------------------------------------------
//#ifdef _GRAPHICS_DRAW_GRAYSCALE_REPEAT_BITMAP_

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
	> class GraphicsGrayscaleRepeatElementDrawBitmap :
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

			T_OWNER::drawRepeatGrayscaleBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_COUNT, C_WIDTH, C_HEIGHT, 255, Size() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};

//#endif // _GRAPHICS_DRAW_GRAYSCALE_REPEAT_BITMAP_
//---------------------------------------------------------------------------
//#ifdef _GRAPHICS_DRAW_BLACK_WHITE_BITMAP_

	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Centered,
		typename T_DATA,
		uint32_t C_HEIGHT,
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsGrayscaleMonochromeElementDrawBitmap :
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

			T_OWNER::drawBWBitmap( C_OWNER, AParentX + X, AParentY + Y, T_DATA::GetValue(), C_WIDTH, C_HEIGHT, 0xFFFF, 0xFF00, Size() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};

//#endif // _GRAPHICS_DRAW_BLACK_WHITE_BITMAP_
//---------------------------------------------------------------------------
	template<
		typename T_OWNER,
		typename T_CARD,
		T_OWNER &C_OWNER,
		T_CARD &C_CARD, 
		typename T_X, 
		typename T_Y
	> class GraphicsGrayscaleElementDrawGraphics : 
		public GraphicsDelegate<
			GraphicsGrayscaleElementDrawGraphics<
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
		typedef GraphicsGrayscaleElementDrawGraphics<
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
		}

	public:
		inline TUniColor MakeColorColor( uint8_t r, uint8_t g, uint8_t b )
		{
			return 0xFF00 | uint8_t( 0.21 * float( r ) + 0.72 * float( g ) + 0.07 * float( b ) + 0.5 );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			Render();
		}

	};
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
	> class GraphicsElementGrayscaleMonohromeDrawScene : 
		public GraphicsIntf<T_OWNER>,
		public Graphics<
			GraphicsElementGrayscaleMonohromeDrawScene<
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
		typedef GraphicsElementGrayscaleMonohromeDrawScene<
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

		void drawPixel(int16_t x, int16_t y, TUniColor color)
		{
			if( color == tmcNone )
				return;

			switch( color )
			{
				case tmcWhite:	color =	WhiteColor(); break;
				case tmcBlack:	color =	BlackColor(); break;
				case tmcInvert:	color =	( C_OWNER.GetPixelColor( x, y ) > 0xFF00 ) ? WhiteColor() : BlackColor(); break;
			}

			C_OWNER.drawPixel( x, y, color );
		}

		inline TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			return ( C_OWNER.GetPixelColor( x, y ) > 0xFF00 ) ? tmcWhite : tmcBlack;
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

	};
//---------------------------------------------------------------------------
}

#endif
