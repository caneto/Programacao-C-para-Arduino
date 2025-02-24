////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_GRAPHICS_MONOCHROME_h
#define _MITOV_GRAPHICS_MONOCHROME_h

#include <Mitov.h>
#include <Mitov_Graphics.h>

namespace Mitov
{
//---------------------------------------------------------------------------
//#ifdef _GRAPHICS_DRAW_BLACK_WHITE_BITMAP_

	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Centered,
		typename T_Color,
		typename T_DATA,
		typename T_FillColor,
		uint32_t C_HEIGHT, 
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsMonochromeElementDrawBitmap :
		public T_Centered,
		public T_Color,
		public T_FillColor,
		public T_Size,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Color )
		_V_PROP_( FillColor )
		_V_PROP_( Size )
		_V_PROP_( Centered )
		_V_PROP_( X )
		_V_PROP_( Y )

	public:
		inline void Render()
		{
			if(( FillColor() == tmcNone ) && ( Color() == tmcNone ))
				return;

			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			if( Centered() )
			{
				AParentX -= C_WIDTH / 2;
				AParentY -= C_HEIGHT / 2;
			}

			T_OWNER::drawBWBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_WIDTH, C_HEIGHT, Color(), FillColor(), Size() );
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
		typename T_Threshold,
		typename T_X, 
		typename T_Y
	> class GraphicsMonochromeElementDrawGraphics : 
		public GraphicsDelegate<
			GraphicsMonochromeElementDrawGraphics<
				T_OWNER,
				T_CARD,
				C_OWNER,
				C_CARD,
				T_Threshold,
				T_X,
				T_Y
			>, T_OWNER, C_OWNER>,

		public T_Threshold,
		public T_X,
		public T_Y
	{
		typedef GraphicsMonochromeElementDrawGraphics<
				T_OWNER,
				T_CARD,
				C_OWNER,
				C_CARD, 
				T_Threshold,
				T_X, 
				T_Y
			> self;

    public:
		_V_PROP_( X )
		_V_PROP_( Y )
		_V_PROP_( Threshold )

	public:
		void Render()
		{
			TGraphicsPos AParentX, AParentY;
			C_OWNER.GetPosition( AParentX, AParentY );

			C_CARD.template RenderGraphics<self>( this, AParentX + X(), AParentY + Y() );

//			FOwner.GetGraphics()->drawRepeatColorBitmap( AParentX + X, AParentY + Y, _Data, Count, Width, Height, Size );
		}

	public:
		inline TUniColor MakeColorColor( uint8_t r, uint8_t g, uint8_t b )
		{
			float AValue = 0.21 * ( float( r ) / 255 ) + 0.72 * ( float( g ) / 255 ) + 0.07 * ( float( b ) / 255 );
//					Serial.println( AValue );
				
			//pushColor(tft.Color565(r,g,b));
			if( AValue > Threshold() )
				return tmcWhite;

			return tmcBlack;
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
		typename T_Centered,
		typename T_Color,
		typename T_DATA,
		typename T_FillColor,
		uint32_t C_HEIGHT, 
		typename T_Size,
		uint32_t C_WIDTH,
		typename T_X,
		typename T_Y
	> class GraphicsElementDrawTransparencyBitmap :
		public T_Centered,
		public T_Color,
		public T_FillColor,
		public T_Size,
		public T_X,
		public T_Y
	{
    public:
		_V_PROP_( Color )
		_V_PROP_( FillColor )
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

			T_OWNER::drawTransparencyBitmap( C_OWNER, AParentX + X(), AParentY + Y(), T_DATA::GetValue(), C_WIDTH, C_HEIGHT, Size() );
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
