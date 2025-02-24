////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_SNAPSHOT_h
#define _MITOV_SNAPSHOT_h

#include <Mitov.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template <
		typename T_DATA,
		typename T_Enabled,
		typename T_OutputPin
	> class Snapshot :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		T_DATA FData;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
//			FData = *(T_DATA*)_Data;
			FData = ::Mitov::template ExtractValue<T_DATA>( _Data );
		}

		void SnapshotInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				TakeSnapshot();
		}

	protected:
		inline void TakeSnapshot()
		{
			T_OutputPin::SetPinValue( FData );
		}

	public:
		Snapshot( T_DATA AData ) 
		{
			FData = AData;
		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_FData,
		typename T_OutputPin
	> class DigitalSnapshot :
		public T_Enabled,
		public T_FData,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	protected:
		_V_PROP_( FData )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			FData() = *(bool*)_Data;
		}

		inline void SnapshotInputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
				TakeSnapshot();
		}

	protected:
		inline void TakeSnapshot()
		{
			T_OutputPin::SetPinValue( FData().GetValue() );
		}

	public:
		inline DigitalSnapshot()
		{
			FData() = false;
		}
	};
//---------------------------------------------------------------------------
}

#endif
