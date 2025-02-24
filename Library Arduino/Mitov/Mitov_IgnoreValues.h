////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_IGNORE_VALUES_h
#define _MITOV_IGNORE_VALUES_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif


namespace Mitov
{
	template<
		typename T,
		typename T_Enabled,
		typename T_OutputPin,
		typename T_PassEqualValues,
		typename T_PassValuesBellow,
		typename T_Value
	> class IgnoreValues :
		public T_Enabled,
		public T_OutputPin,
		public T_PassEqualValues,
		public T_PassValuesBellow,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Value )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( PassValuesBellow )
		_V_PROP_( PassEqualValues )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			T AValue = *(T*)_Data;
			if( PassValuesBellow() )
			{
				if( AValue < Value() )
					T_OutputPin::PinNotify( _Data );
			}

			else
			{
				if( AValue > Value() )
					T_OutputPin::PinNotify( _Data );
			}

			if( PassEqualValues() )
				if( AValue == Value() )
					T_OutputPin::PinNotify( _Data );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_PassEqualValues,
		typename T_PassValuesBellow,
		typename T_Value
	> class IgnoreValuesText :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_PassEqualValues,
		public T_PassValuesBellow,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Value )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( PassValuesBellow )
		_V_PROP_( PassEqualValues )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			Mitov::String AValue = Mitov::String( (char*)_Data );
			Mitov::String ACurrentValue = Value();
			if( IgnoreCase() )
			{
				ACurrentValue.toUpperCase();
				AValue.toUpperCase();
			}

			if( PassValuesBellow() )
			{
				if( AValue < ACurrentValue )
					T_OutputPin::PinNotify( _Data );
			}

			else
			{
				if( AValue > ACurrentValue )
					T_OutputPin::PinNotify( _Data );
			}

			if( PassEqualValues() )
				if( AValue == ACurrentValue )
					T_OutputPin::PinNotify( _Data );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_PassEqualValues,
		typename T_PassValuesBellow,
		typename T_Value
	> class IgnoreValuesChar :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_PassEqualValues,
		public T_PassValuesBellow,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Value )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( PassValuesBellow )
		_V_PROP_( PassEqualValues )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			char AValue = *(char*)_Data;
			char ACurrentValue = Value();
			if( IgnoreCase() )
			{
				ACurrentValue = toupper( ACurrentValue );
				AValue = toupper( AValue );
			}

			if( PassValuesBellow() )
			{
				if( AValue < ACurrentValue )
					T_OutputPin::PinNotify( _Data );
			}

			else
			{
				if( AValue > ACurrentValue )
					T_OutputPin::PinNotify( _Data );
			}

			if( PassEqualValues() )
				if( AValue == ACurrentValue )
					T_OutputPin::PinNotify( _Data );
		}

	};
//---------------------------------------------------------------------------
/*
	template<> void IgnoreValues<char>::InputPin_o_Receive( void *_Data )
	{
		if( ! Enabled )
		{
			OutputPin.Notify( _Data );
			return;
		}

		char AValue = *(char*)_Data;
		char ACurrentValue = Value;
		if( IgnoreCase )
		{
			ACurrentValue = toupper( ACurrentValue );
			AValue = toupper( AValue );
		}

		if( PassValuesBellow )
		{
			if( AValue < ACurrentValue )
				OutputPin.Notify( _Data );
		}

		else
		{
			if( AValue > ACurrentValue )
				OutputPin.Notify( _Data );
		}

		if( PassEqualValues )
			if( AValue == ACurrentValue )
				OutputPin.Notify( _Data );

	}
//---------------------------------------------------------------------------
	template<> void IgnoreValues<Mitov::String>::InputPin_o_Receive( void *_Data )
	{
		if( ! Enabled )
		{
			OutputPin.Notify( _Data );
			return;
		}

		Mitov::String AValue = Mitov::String( (char*)_Data );
		Mitov::String ACurrentValue = Value;
		if( IgnoreCase )
		{
			ACurrentValue.toUpperCase();
			AValue.toUpperCase();
		}

		if( PassValuesBellow )
		{
			if( AValue < ACurrentValue )
				OutputPin.Notify( _Data );
		}

		else
		{
			if( AValue > ACurrentValue )
				OutputPin.Notify( _Data );
		}

		if( PassEqualValues )
			if( AValue == ACurrentValue )
				OutputPin.Notify( _Data );

	}
*/
//---------------------------------------------------------------------------
//	template<typename T> class IgnoreRanges;
//---------------------------------------------------------------------------
	template<
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T,
		typename T_Min,
		typename T_Max,
		typename T_PassEqualMax,
		typename T_PassEqualMin
	> class IgnoreRangeElement :
		public T_Min,
		public T_Max,
		public T_PassEqualMax,
		public T_PassEqualMin
	{
	public:
		_V_PROP_( Min )
		_V_PROP_( Max )
		_V_PROP_( PassEqualMax )
		_V_PROP_( PassEqualMin )
//		bool	IgnoreCase : 1;

	public:
		void Exclude( T &AValue, bool &AExclude )
		{
			if( AExclude )
				return;

			if( PassEqualMin() )
				if( AValue <= Min() )
					return;

			if( AValue < Min() )
				return;

			if( PassEqualMax() )
				if( AValue >= Max() )
					return;

			if( AValue > Max() )
				return;

			AExclude = true;
		}

//	public:
//		IgnoreRangeElement( IgnoreRanges<T> &AOwner );

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER,
		T_OWNER &C_OWNER,
		typename T,
		typename T_Min,
		typename T_Max,
		typename T_IgnoreCase,
		typename T_PassEqualMax,
		typename T_PassEqualMin
	> class IgnoreRangeElementChar :
		public T_Min,
		public T_Max,
		public T_IgnoreCase,
		public T_PassEqualMax,
		public T_PassEqualMin
	{
	public:
		_V_PROP_( Min )
		_V_PROP_( Max )
		_V_PROP_( PassEqualMax )
		_V_PROP_( PassEqualMin )
		_V_PROP_( IgnoreCase )

	public:
		void Exclude( T &AValue, bool &AExclude )
		{
			if( AExclude )
				return;

			if( IgnoreCase() )
				AValue = toupper( AValue );

			if( PassEqualMin() )
				if( AValue <= Min() )
					return;

			if( AValue < Min() )
				return;

			if( PassEqualMax() )
				if( AValue >= Max() )
					return;

			if( AValue > Max() )
				return;

			AExclude = true;
		}

//	public:
//		IgnoreRangeElement( IgnoreRanges<T> &AOwner );

	};
//---------------------------------------------------------------------------
/*
	template<> bool IgnoreRangeElement<char>::Exclude( char &AValue )
	{
		if( IgnoreCase )
			AValue = toupper( AValue );

		if( PassEqualMin )
			if( AValue <= Min )
				return false;

		if( AValue < Min )
			return false;

		if( PassEqualMax )
			if( AValue >= Max )
				return false;

		if( AValue > Max )
			return false;

		return true;
	}
*/
//---------------------------------------------------------------------------
	template<
		typename T,
		typename T_Elements_Exclude,
		typename T_Enabled,
		typename T_IgnoredOutputPin,
		typename T_OutputPin
	> class IgnoreRanges :
		public T_Elements_Exclude,
		public T_Enabled,
		public T_IgnoredOutputPin,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )
		_V_PIN_( IgnoredOutputPin )

    public:
        _V_PROP_( Enabled )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			if( Enabled() )
			{
				T AValue = *(T*)_Data;
				bool AExclude = false;
				T_Elements_Exclude::ChainCall( AValue, AExclude );
				if( AExclude )
				{
					T_IgnoredOutputPin::SetPinValue( true );
					return;
				}
			}

			T_IgnoredOutputPin::SetPinValue( false );
			T_OutputPin::PinNotify( _Data );
		}

	};
//---------------------------------------------------------------------------
/*
	template<typename T> IgnoreRangeElement<T>::IgnoreRangeElement( IgnoreRanges<T> &AOwner ) :
		PassEqualMax( false ),
		PassEqualMin( false ),
		IgnoreCase( false )
	{
		AOwner.FElements.push_back( this );
	}
*/
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
