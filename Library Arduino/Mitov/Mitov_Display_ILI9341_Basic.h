////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Mitov_Graphics_Color.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
namespace ILI9341Extended
{
	const uint8_t INIT_DELAY = 0xFF;

	const uint8_t MADCTL		= 0x36;
	const uint8_t MADCTL_MY  = 0x80;
	const uint8_t MADCTL_MX  = 0x40;
	const uint8_t MADCTL_MV  = 0x20;
	const uint8_t MADCTL_ML  = 0x10;
	const uint8_t MADCTL_RGB = 0x00;
	const uint8_t MADCTL_BGR = 0x08;
	const uint8_t MADCTL_MH  = 0x04;

	const uint8_t COLADDRSET = 0x2A;
	const uint8_t PAGEADDRSET = 0x2B;

	const uint8_t SLPOUT = 0x11;
	const uint8_t DISPON = 0x29;
	const uint8_t PWCTR1 = 0xC0;
	const uint8_t PWCTR2 = 0xC1;
	const uint8_t VMCTR1 = 0xC5;
	const uint8_t VMCTR2 = 0xC7;
	const uint8_t PIXFMT = 0x3A;
	const uint8_t FRMCTR1 = 0xB1;
	const uint8_t DFUNCTR = 0xB6;
	const uint8_t GAMMASET = 0x26;
	const uint8_t GMCTRP1 = 0xE0;
	const uint8_t GMCTRN1 = 0xE1;
}

	template<
		typename T_BackgroundColor,
		typename T_Elements_Render,
        typename T_Font,
		typename T_IMPLEMENTATION,
		bool C_SUPPORTS_PIXEL_READ,
		typename T_Orientation,
		uint8_t C_MADCTL, uint16_t WIDTH, uint16_t HEIGHT,
		typename T_Text
	> class DisplayILI9341_Basic : 
		public T_IMPLEMENTATION,
		public GraphicsImpl<
			DisplayILI9341_Basic<
					T_BackgroundColor,
					T_Elements_Render,
                    T_Font,
					T_IMPLEMENTATION,
					C_SUPPORTS_PIXEL_READ,
					T_Orientation,
					C_MADCTL, WIDTH, HEIGHT,
					T_Text
				>, 
                T_BackgroundColor,
                T_Font,
                T_Text
            >,
		public T_Orientation
	{
		typedef DisplayILI9341_Basic<
				T_BackgroundColor,
				T_Elements_Render,
                T_Font,
				T_IMPLEMENTATION,
				C_SUPPORTS_PIXEL_READ,
				T_Orientation,
				C_MADCTL, WIDTH, HEIGHT,
				T_Text
			> T_SELF;

		typedef GraphicsImpl<
                T_SELF,
                T_BackgroundColor,
                T_Font,
                T_Text
            > inherited;

	public:
		_V_PROP_( Orientation )

	public:
		static inline void Elements_CallChain()
		{
			T_Elements_Render::ChainCall();
		}

	public:
		inline int16_t width(void) const
		{
			return WIDTH;
//			return T_IMPLEMENTATION::width( Orientation, DisplayILI9341Const::WIDTH, DisplayILI9341Const::HEIGHT );
		}

		inline int16_t height(void) const
		{
			return HEIGHT;
//			return T_IMPLEMENTATION::height( Orientation, DisplayILI9341Const::WIDTH, DisplayILI9341Const::HEIGHT );
		}

	protected:
		void SendCommandSequence( const uint8_t *APointer, int ASize )
		{
			register int i = 0;
			while( i < ASize )
			{
				uint8_t a = pgm_read_byte(& APointer[i++]);
				uint8_t ACount = pgm_read_byte(& APointer[i++]);
				if( a == ILI9341Extended::INIT_DELAY ) 
					delay( ACount );

				else
				{
					cmd( a );
					for( int j = 0; j < ACount; ++j )
						w_data( pgm_read_byte(& APointer[i++] ));

				}
			}
		}

	public:
		inline void SystemInit()
		{
			T_IMPLEMENTATION::InitPins();
		}

		inline void SystemStart()
		{
			UpdateOrientation();

			ClearScreen( *this, inherited::BackgroundColor() );

//			inherited::setTextSize( Text().Size() );
//			inherited::setTextColor( Text().Color(), Text().BackgroundColor() );
//			inherited::setTextWrap( Text().Wrap() );

			inherited::RenderElements();
//			for( int i = 0; i < FElements.size(); ++ i )
//				FElements[ i ]->Render( false );

		}

		void UpdateOrientation()
		{
			cmd( ILI9341Extended::MADCTL);

			w_data( C_MADCTL );

			setAddrWindow( 0, 0, width() - 1, height() - 1 ); // CS_IDLE happens here
		}

		void setAddrWindow(int x1, int y1, int x2, int y2)
		{
			T_IMPLEMENTATION::setAddrWindow( x1, y1, x2, y2 );
		}

		void writeRegister32(uint8_t r, uint32_t d) 
		{
			T_IMPLEMENTATION::writeRegister32( r, d );
		}

	public:
//#ifdef _GRAPHICS_DRAW_ALPHA_COLOR_BITMAP_

		static void drawAlphaColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint32_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawAlphaColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			// check rotation, move pixel around if necessary

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint16_t AColor = Func::Color565( pgm_read_dword( bitmap++ ));
					AImplementation.w_data( AColor >> 8 );
					AImplementation.w_data( AColor );
				}

		}

//#endif // _GRAPHICS_DRAW_ALPHA_COLOR_BITMAP_

//#ifdef _GRAPHICS_DRAW_COLOR_BITMAP_
		static void drawColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawColorBitmap( AImplementation, x, y, bitmap, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t ARed = pgm_read_byte( bitmap ++ );
					uint32_t AGreen = pgm_read_byte( bitmap ++ );
					uint32_t ABlue = pgm_read_byte( bitmap ++ );
					uint16_t AColor = Func::Color565( ( AGreen << 8 ) | ( ARed << 16 ) | ABlue );

					AImplementation.w_data( AColor >> 8 );
					AImplementation.w_data( AColor );
				}

		}

//#endif // _GRAPHICS_DRAW_COLOR_BITMAP_

//#ifdef _GRAPHICS_DRAW_GRAYSCALE_BITMAP_

		static void drawGrayscaleBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawGrayscaleBitmap( AImplementation, x, y, bitmap, w, h, AAlpha, size );
				return;
			}

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t AGray = pgm_read_byte( bitmap ++ );
					uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

					AImplementation.w_data( AColor >> 8 );
					AImplementation.w_data( AColor );
				}

		}

//#endif // _GRAPHICS_DRAW_GRAYSCALE_BITMAP_

//#ifdef _GRAPHICS_DRAW_GRAYSCALE_ALPHA_BITMAP_

		static void drawAlphaGrayscaleBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint16_t *_bitmap, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawAlphaGrayscaleBitmap( AImplementation, x, y, _bitmap, w, h, size );
				return;
			}

			const uint8_t *bitmap = (const uint8_t *)_bitmap;

			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint32_t AGray = pgm_read_byte( bitmap );
					uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );
					bitmap += 2;

					AImplementation.w_data( AColor >> 8 );
					AImplementation.w_data( AColor );
				}

		}

//#endif // _GRAPHICS_DRAW_GRAYSCALE_ALPHA_BITMAP_

//#ifdef _GRAPHICS_DRAW_BLACK_WHITE_BITMAP_

		static void drawBWBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, TUniColor color, TUniColor bg, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawBWBitmap( AImplementation, x, y, bitmap, w, h, color, bg, size );
				return;
			}

			int16_t byteWidth = (w + 7) / 8;

//			color = Func::Color565( color );
//			bg = Func::Color565( bg );
  
			AImplementation.setAddrWindow(x,y,x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );

			for( int16_t j=0; j<h; j++)
				for( int16_t i=0; i<w; ++i )
				{
					uint16_t AColor;
					if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> ( i & 0b111 )))
						AColor = 0xFFFF;

					else 
      					AColor = 0;

					AImplementation.w_data( AColor >> 8 );
					AImplementation.w_data( AColor );
				}

		}

//#endif // _GRAPHICS_DRAW_BLACK_WHITE_BITMAP_

//#ifdef _GRAPHICS_DRAW_REPEAT_ALPHA_COLOR_BITMAP_

		static void drawRepeatAlphaColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawRepeatAlphaColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( pgm_read_dword( bitmap ));
				uint8_t AHi = AColor >> 8;
				uint8_t ALo = AColor;
				bitmap += 4;
				for( int16_t j = 0; j <= ACount; ++ j )
				{
					AImplementation.w_data( AHi );
					AImplementation.w_data( ALo );
				}
			}
		}

//#endif // _GRAPHICS_DRAW_REPEAT_ALPHA_COLOR_BITMAP_

//#ifdef _GRAPHICS_DRAW_REPEAT_COLOR_BITMAP_

		static void drawRepeatColorBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawRepeatColorBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t ARed = pgm_read_byte( bitmap ++ );
				uint32_t AGreen = pgm_read_byte( bitmap ++ );
				uint32_t ABlue = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( ARed << 16 ) | ( AGreen << 8 ) | ABlue );

				uint8_t AHi = AColor >> 8;
				uint8_t ALo = AColor;
				for( int16_t j = 0; j <= ACount; ++ j )
				{
					AImplementation.w_data( AHi );
					AImplementation.w_data( ALo );
				}
			}
		}

//#endif // _GRAPHICS_DRAW_REPEAT_COLOR_BITMAP_

//#ifdef _GRAPHICS_DRAW_GRAYSCALE_REPEAT_BITMAP_

		static void drawRepeatGrayscaleBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t AAlpha, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawRepeatGrayscaleBitmap( AImplementation, x, y, bitmap, AListCount, w, h, AAlpha, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap ++ );

				uint32_t AGray = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

				uint8_t AHi = AColor >> 8;
				uint8_t ALo = AColor;
				for( int16_t j = 0; j <= ACount; ++ j )
				{
					AImplementation.w_data( AHi );
					AImplementation.w_data( ALo );
				}
			}
		}

//#endif // _GRAPHICS_DRAW_GRAYSCALE_REPEAT_BITMAP_

//#ifdef _GRAPHICS_DRAW_GRAYSCALE_REPEAT_ALPHA_BITMAP_

		static void drawAlphaGrayscaleRepeatBitmap( T_SELF &AImplementation, int16_t x, int16_t y, const uint8_t *bitmap, int16_t AListCount, int16_t w, int16_t h, uint8_t size )
		{
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();

			if( ( size > 1 ) || ( x < 0 ) || ( y < 0 ) || ( x + w > AWidth ) || ( x + w > AHeight ))
			{
				inherited::drawAlphaGrayscaleRepeatBitmap( AImplementation, x, y, bitmap, AListCount, w, h, size );
				return;
			}

			AImplementation.setAddrWindow(x,y, x + w - 1, y + h - 1);

			AImplementation.cmd( 0x2C );

			for( int16_t i = 0; i < AListCount; ++i )
			{
				uint8_t ACount = pgm_read_byte( bitmap );

				bitmap += 2;

				uint32_t AGray = pgm_read_byte( bitmap ++ );
				uint16_t AColor = Func::Color565( ( AGray << 8 ) | ( AGray << 16 ) | AGray );

				uint8_t AHi = AColor >> 8;
				uint8_t ALo = AColor;
				for( int16_t j = 0; j <= ACount; ++ j )
				{
					AImplementation.w_data( AHi );
					AImplementation.w_data( ALo );
				}
			}
		}

//#endif // _GRAPHICS_DRAW_GRAYSCALE_REPEAT_ALPHA_BITMAP_

		static void drawGrayscalePixel( T_SELF &AImplementation, int16_t x, int16_t y, TUniColor color )
		{
			uint16_t AGray = ( color & 0xFF );
			AImplementation.drawPixel( x, y, color << 16 | ( AGray << 8 ) | AGray );
		}

		void drawPixel( int16_t x, int16_t y, TUniColor color )
		{
			uint16_t AHeight = height();
			uint16_t AWidth = width();

			if((x < 0) ||(x >= AWidth) || (y < 0) || (y >= AHeight)) 
				return;

			uint8_t AAlpha = color >> 24;
			if( AAlpha == 0 )
				return;

			if( ! C_SUPPORTS_PIXEL_READ )
				color |= 0xFF000000;

			T_IMPLEMENTATION::drawPixel( x, y, color, AAlpha );
		}

		static void fillGrayscaleRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color)
		{
//			Serial.print( x ); Serial.print( " " ); Serial.print( y ); Serial.print( " " ); Serial.print( w ); Serial.print( " " ); Serial.print( h ); Serial.print( " " ); Serial.println( uint32_t( color ));

			uint16_t AGray = ( color & 0xFF );
			fillRect( AImplementation, x, y, w, h, color << 16 | ( AGray << 8 ) | AGray );
		}

//#if ( defined(__ILI9341_SCROLL__) && defined(__PARALLEL_COMPONENT_DISPLAY_ILI9341__))

		static inline void Scroll( T_SELF &AImplementation, TGraphicsPos X, TGraphicsPos Y, TUniColor color )
		{
			if( C_SUPPORTS_PIXEL_READ )
				T_IMPLEMENTATION::template InternalScroll<T_SELF>( AImplementation, AImplementation.width(), AImplementation.height(), X, Y, color );
		}

//#endif

		static void drawFastHLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, TUniColor color )
		{
			// Rudimentary clipping
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();
			if((x >= AWidth ) || (y >= AHeight )) 
				return;

			uint8_t AAlpha = color >> 24;
			if( C_SUPPORTS_PIXEL_READ && AAlpha < 255 )
			{
				inherited::drawFastHLine( AImplementation, x, y, w, color );
				return;
			}

			if((x+w-1) >= AWidth )
				w = AWidth - x;

			color = Func::Color565( color );
			AImplementation.setAddrWindow(x, y, x+w-1, y);

			uint8_t hi = color >> 8, lo = color;

			AImplementation.cmd( 0x2C );

			while (w--) 
			{
				AImplementation.w_data(hi);
				AImplementation.w_data(lo);
			}
		}

		static void drawFastVLine( T_SELF &AImplementation, int16_t x, int16_t y, int16_t h, TUniColor color )
		{
			// Rudimentary clipping
			uint16_t AHeight = AImplementation.height();
			uint16_t AWidth = AImplementation.width();
			if((x >= AWidth) || (y >= AHeight)) 
				return;

			uint8_t AAlpha = color >> 24;
			if( AAlpha == 0 )
				return;

			if( C_SUPPORTS_PIXEL_READ && AAlpha < 255 )
			{
				inherited::drawFastVLine( AImplementation, x, y, h, color );
				return;
			}

			if((y+h-1) >= AHeight) 
				h = AHeight-y;

			color = Func::Color565( color );
			AImplementation.setAddrWindow(x, y, x, y+h-1);

			uint8_t hi = color >> 8, lo = color;

			AImplementation.cmd( 0x2C );

			while (h--) 
			{
				AImplementation.w_data(hi);
				AImplementation.w_data(lo);
			}
    
		}

		static void fillRect( T_SELF &AImplementation, int16_t x, int16_t y, int16_t w, int16_t h, TUniColor color )
		{
			// rudimentary clipping (drawChar w/big text requires this)
			int16_t AWidth = AImplementation.width();
			int16_t AHeight = AImplementation.height();

			if((x >= AWidth) || (y >= AHeight))
				return;

			uint8_t AAlpha = color >> 24;
			if( AAlpha == 0 )
				return;

			if( C_SUPPORTS_PIXEL_READ && AAlpha < 255 )
			{
				inherited::fillRect( AImplementation, x, y, w, h, color );
				return;
			}

			AImplementation.fillRectImplementation( AWidth, AHeight, x, y, w, h, color );
		}

		static inline void ClearScreen( T_SELF &AImplementation, TUniColor color )
		{
			AImplementation.fillRectImplementation( AImplementation.width(), AImplementation.height(), 0, 0, AImplementation.width(), AImplementation.height(), 0xFF000000 | color );
		}

		TUniColor GetPixelColor( int16_t x, int16_t y )
		{
			if( ! C_SUPPORTS_PIXEL_READ )
				return 0x0000;

			if((x < 0) ||(x >= width()) || (y < 0) || (y >= height())) 
				return 0x0000;

			return T_IMPLEMENTATION::GetPixelColor( x, y );
		}

	protected:
		void cmd( uint8_t cmd )
		{
			T_IMPLEMENTATION::cmd( cmd );
		}

		void  w_data( uint8_t data )
		{
			T_IMPLEMENTATION::w_data( data );
		}

	public:
		template<typename T_CONFIG> inline static size_t write( T_SELF &AImplementation, T_CONFIG &AConfig, uint8_t AValue )
		{
			return inherited::write( AImplementation, AConfig, AValue );
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
//		template<typename T> size_t print( T AValue )
		template<typename T_CONFIG, typename T> size_t print( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_SELF> APrint( this, (typename GraphicsPrint<T_SELF>::TCallback) &T_SELF::write );
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &inherited::template write<T_CONFIG> );
			return APrint.print( AValue );
		}

//		template<typename T> size_t println( T AValue )
		template<typename T_CONFIG, typename T> size_t println( T_CONFIG *AConfig, T AValue )
		{
//			GraphicsPrint<T_SELF> APrint( this, (typename GraphicsPrint<T_SELF>::TCallback) &T_SELF::write );
			GraphicsPrint<T_SELF, T_CONFIG> APrint( this, AConfig, (typename GraphicsPrint<T_SELF, T_CONFIG>::TCallback) &inherited::template write<T_CONFIG> );
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
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
/*
	template<
		typename T_BackgroundColor,
		typename T_Brightness,
		uint8_t C_BRIGHNESS_PIN,
		typename T_Elements_Render,
        typename T_Font,
		typename T_IMPLEMENTATION,
		typename T_Orientation,
		uint8_t C_MADCTL, uint16_t WIDTH, uint16_t HEIGHT,
		typename T_Text
	> class DisplayILI9341_Brightness : 
		public DisplayILI9341_Basic <
				T_BackgroundColor,
				T_Elements_Render,
				T_Font,
				T_IMPLEMENTATION,
				T_Orientation,
				C_MADCTL, WIDTH, HEIGHT,
				T_Text
			>,
	    public T_Brightness
	{
		typedef DisplayILI9341_Basic <
				T_BackgroundColor,
				T_Elements_Render,
				T_Font,
				T_IMPLEMENTATION,
				T_Orientation,
				C_MADCTL, WIDTH, HEIGHT,
				T_Text
			> inherited;

	public:
		_V_PROP_( Brightness )

	public:
		inline void UpdateBrightness()
		{
			analogWrite( C_BRIGHNESS_PIN, Brightness() * 255 + 0.5 );
		}

	public:
		inline void SystemStart()
		{
			inherited::SystemStart();
			pinMode( C_BRIGHNESS_PIN, OUTPUT );
			UpdateBrightness();
		}

	};
//---------------------------------------------------------------------------
*/
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif


