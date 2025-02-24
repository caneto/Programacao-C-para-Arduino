////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_PS2_CONTROLLER_h
#define _MITOV_PS2_CONTROLLER_h

#include <Mitov.h>
#include <Mitov_PS2_Controller_Basic.h>
#include <PS2X_lib.h>

namespace Mitov
{
/*
	class PS2ControllerDirect
	{
	public:
		inline void SmallVibrateMotorInputPin_o_Receive( void *_Data )
		{
			FSmallMotor = *(bool *)_Data;
		}

		inline void LargeVibrateMotorInputPin_o_Receive( void *_Data )
		{
			FLargeMotor = (int8_t)( constrain( *(float *)_Data, 0, 1 ) * 255 + 0.5 );
		}

	protected:
		inline void ReadController()
		{
			Fps2x.read_gamepad( FSmallMotor, FLargeMotor );
		}

	protected:
		PS2X Fps2x;

	public:
		inline bool	ReadDigital( unsigned int AIndex )
		{
			return Fps2x.Button( AIndex );
		}

		inline float	ReadAnalog( unsigned int AIndex )
		{
			return ((float)Fps2x.Analog( AIndex )) / 255;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadData();
		}

	protected:
		void ReadData()
		{
			if( ! Enabled() )
				return;

			ReadController();

			for( int i = 0; i < Sensors.size(); ++i )
				Sensors[ i ]->Process( this );
		}

	public:
		inline void SystemStart()
		{
			ReadController();

			for( int i = 0; i < Sensors.size(); ++i )
				Sensors[ i ]->StartProcess( this );

		}

	public:
		inline PS2ControllerDirect( int ADataPinNumber, int ACommandPinNumber, int AAttentionPinNumber, int AClockPinNumber )
		{
			Fps2x.config_gamepad( AClockPinNumber, ACommandPinNumber, AAttentionPinNumber, ADataPinNumber, true, true );
		}

	};
*/
//---------------------------------------------------------------------------
/*
	class PS2ControllerImplementation : public PS2ControllerDirect
	{
		typedef PS2ControllerDirect inherited;

	protected:
		bool	FSmallMotor = false;
		int8_t	FLargeMotor = 0;

	public:
		inline void SmallVibrateMotorInputPin_o_Receive( void *_Data )
		{
			FSmallMotor = *(bool *)_Data;
		}

		inline void LargeVibrateMotorInputPin_o_Receive( void *_Data )
		{
			FLargeMotor = (int8_t)( constrain( *(float *)_Data, 0, 1 ) * 255 + 0.5 );
		}

	protected:
		inline void ReadController()
		{
			Fps2x.read_gamepad( FSmallMotor, FLargeMotor );
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
	typedef PS2BasicController<PS2ControllerImplementation> PS2Controller;
//---------------------------------------------------------------------------
	class PS2GuitarImplementation : public PS2ControllerDirect
	{
		typedef PS2ControllerDirect inherited;

	protected:
		inline void ReadController()
		{
			Fps2x.read_gamepad( false, 0 );
		}

	public:
		using inherited::inherited;

	};
//---------------------------------------------------------------------------
	typedef PS2BasicController<PS2GuitarImplementation> PS2Guitar;
//---------------------------------------------------------------------------
*/
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_FSmallMotor,
		typename T_Sensors_Process
	> class PS2Controller :
		public T_Enabled,
		public T_FSmallMotor
	{
	public:
		_V_PROP_( Enabled )

	protected:
		_V_PROP_( FSmallMotor )
		int8_t	FLargeMotor = 0;

	protected:
		PS2X Fps2x;

	public:
		inline void SmallVibrateMotorInputPin_o_Receive( void *_Data )
		{
			FSmallMotor() = *(bool *)_Data;
		}

		inline void LargeVibrateMotorInputPin_o_Receive( void *_Data )
		{
			FLargeMotor = (int8_t)( constrain( *(float *)_Data, 0, 1 ) * 255 + 0.5 );
		}

	protected:
		inline void ReadController()
		{
			Fps2x.read_gamepad( FSmallMotor(), FLargeMotor );
		}

	public:
		inline bool	ReadDigital( unsigned int AIndex )
		{
			return Fps2x.Button( AIndex );
		}

		inline float	ReadAnalog( unsigned int AIndex )
		{
			return ((float)Fps2x.Analog( AIndex )) / 255;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadData( false );
		}

	protected:
		void ReadData( bool AChangeOnly )
		{
			if( ! Enabled() )
				return;

			ReadController();
			T_Sensors_Process::ChainCall( AChangeOnly );
		}

	public:
		inline void SystemStart()
		{
			ReadData( false );
		}

		inline void SystemLoopBegin()
		{
			ReadData( true );
		}

	public:
		inline PS2Controller( int ADataPinNumber, int ACommandPinNumber, int AAttentionPinNumber, int AClockPinNumber )
		{
			Fps2x.config_gamepad( AClockPinNumber, ACommandPinNumber, AAttentionPinNumber, ADataPinNumber, true, true );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_Sensors_Process
	> class PS2Guitar :
		public T_Enabled	
	{
	public:
		_V_PROP_( Enabled )

	protected:
		PS2X Fps2x;

	protected:
		inline void ReadController()
		{
			Fps2x.read_gamepad();
		}

	public:
		inline bool	ReadDigital( unsigned int AIndex )
		{
			return Fps2x.Button( AIndex );
		}

		inline float ReadAnalog( unsigned int AIndex )
		{
			return ((float)Fps2x.Analog( AIndex )) / 255;
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ReadData( false );
		}

	protected:
		void ReadData( bool AChangeOnly )
		{
			if( ! Enabled() )
				return;

			ReadController();
			T_Sensors_Process::ChainCall( AChangeOnly );
		}

	public:
		inline void SystemStart()
		{
			ReadData( false );
		}

		inline void SystemLoopBegin()
		{
			ReadData( true );
		}

	public:
		inline PS2Guitar( int ADataPinNumber, int ACommandPinNumber, int AAttentionPinNumber, int AClockPinNumber )
		{
			Fps2x.config_gamepad( AClockPinNumber, ACommandPinNumber, AAttentionPinNumber, ADataPinNumber, true, true );
		}

	};
//---------------------------------------------------------------------------
}

#endif
