////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_SPEED_TO_SPEED_AND_DIRECTION_h
#define _MITOV_SPEED_TO_SPEED_AND_DIRECTION_h

#include <Mitov.h>

namespace Mitov
{
	template <
		typename T_DirectionOutputPin,
		typename T_Enabled,
		typename T_InverseForward,
		typename T_InverseReverse,
		typename T_SpeedOutputPin
	> class SpeedToSpeedAndDirection :
		public T_DirectionOutputPin,
		public T_Enabled,
		public T_InverseForward,
		public T_InverseReverse,
		public T_SpeedOutputPin
	{
	public:
		_V_PIN_( DirectionOutputPin )
		_V_PIN_( SpeedOutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InverseForward )
		_V_PROP_( InverseReverse )

	public:
		void	SetEnabled( bool AValue ) 
		{
			if( Enabled() == AValue )
				return;

			Enabled() = AValue;
			UpdateOutputs();
		}

		void	SetInverseForward( bool AValue ) 
		{
			if( InverseForward() == AValue )
				return;

			InverseForward() = AValue;
			UpdateOutputs();
		}

		void	SetInverseReverse( bool AValue ) 
		{
			if( InverseReverse() == AValue )
				return;

			InverseReverse() = AValue;
			UpdateOutputs();
		}

	protected:
		float	FCurrentSpeed = 0.5f;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float ASpeed = constrain( *(float *)_Data, 0.0, 1.0 );
			if( FCurrentSpeed == ASpeed )
				return;

			FCurrentSpeed = ASpeed;
			UpdateOutputs();
		}

	protected:
		void	UpdateOutputs()
		{
			if( Enabled() )
			{
				float AOutSpeed = fabs( FCurrentSpeed - 0.5 ) * 2;
				bool ADirection = FCurrentSpeed > 0.5;

				if( ADirection )
				{
					if( InverseForward() )
						AOutSpeed = 1 - AOutSpeed;
				}

				else
				{
					if( InverseReverse() )
						AOutSpeed = 1 - AOutSpeed;
				}

				T_SpeedOutputPin::SetPinValue( AOutSpeed );
				T_DirectionOutputPin::SetPinValue( ADirection );
			}

			else
			{
				T_SpeedOutputPin::SetPinValue( 0 );
				T_DirectionOutputPin::SetPinValue( false );
			}
		}

	public:
		inline void SystemStart()
		{
			UpdateOutputs();
		}

	};
}

#endif
