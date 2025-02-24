////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_REMEMBER_h
#define _MITOV_REMEMBER_h

#include <Mitov.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_DATA,
		typename T_Enabled,
		typename T_FLastData,
//		typename T_FForceRemember,
		typename T_InitialValue,
		typename T_OutputPin,
		typename T_RecallInputPin_o_IsConnected,
		typename T_RememberInputPin_o_IsConnected,
		typename T_STORE
	> class Remember :
		public T_Enabled,
		public T_FLastData,
//		public T_FForceRemember,
		public T_InitialValue,
		public T_OutputPin,
		public T_RecallInputPin_o_IsConnected,
		public T_RememberInputPin_o_IsConnected
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( InitialValue )

    public:
		_V_PROP_( RecallInputPin_o_IsConnected )
		_V_PROP_( RememberInputPin_o_IsConnected )

	protected:
//		bool FPopulated : 1;
//		_V_PROP_( FForceRemember )
//		T_STORE FSavedData;
		_V_PROP_( FLastData )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			T_STORE AValue = ExtractValue<T_STORE>( _Data );
			FLastData() = AValue;
//			FPopulated = true;

			if( ! RememberInputPin_o_IsConnected().GetValue() )
//			{
				InitialValue() = FLastData().GetValue();
//#ifdef _MITOV_REMEMBER_RECALL_PIN_
//				if( ! RecallInputPin_o_IsConnected() )
//#endif // _MITOV_REMEMBER_RECALL_PIN_
//					T_OutputPin::SetPinValue( InitialValue() );
//			}

//			FForceRemember() = false;
		}

		void RememberInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
			{
/*
				if( ! FPopulated )
				{
					FForceRemember() = true;
					return;
				}
*/
				InitialValue() = FLastData().GetValue();
//#ifdef _MITOV_REMEMBER_RECALL_PIN_
				if( ! RecallInputPin_o_IsConnected().GetValue() )
//#endif // _MITOV_REMEMBER_RECALL_PIN_
					T_OutputPin::SetPinValue( InitialValue().GetValue() );
			}
		}

//#ifdef _MITOV_REMEMBER_RECALL_PIN_
		void RecallInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue().GetValue() );
		}
//#endif // _MITOV_REMEMBER_RECALL_PIN_

	public:
		inline Remember()
//			Enabled( true ),
//			RecallInputPin_o_IsConnected( false ),
//			FPopulated( false ),
//			FForceRemember( false ),
//			FSavedData( AData )
		{
//			FForceRemember() = false;
			FLastData() = InitialValue().GetValue();
		}
	};
//---------------------------------------------------------------------------
/*
	template<> void Remember<Mitov::String, char>::InputPin_o_Receive( void *_Data )
	{
		Mitov::String AValue = (char *)_Data;
		FLastData = AValue;
		FPopulated = true;

		if( FForceRemember )
		{
			FSavedData = FLastData;
#ifdef _MITOV_REMEMBER_RECALL_PIN_
			if( ! RecallInputPin_o_IsConnected )
#endif // _MITOV_REMEMBER_RECALL_PIN_
				OutputPin.SendValue( FSavedData );
		}

		FForceRemember = false;
	}
*/
//---------------------------------------------------------------------------
}

#endif
