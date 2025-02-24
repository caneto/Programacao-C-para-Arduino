////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_ARRAY_OPERATIONS_h
#define _MITOV_ARRAY_OPERATIONS_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T
	> class ArduinoSetValueArrayElement
	{
	public:
		T *_Values;
		uint32_t _Count;

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.ExternalSetValue( _Values, _Count );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T
	> class ValueArraySource : 
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		T *_Values;
		uint32_t _Count;

	protected:
		void SetValue()
		{
			if( ! _Count )
				return;

			TArray<T> ABuffer( _Count, _Values );
			T_OutputPin::SetPinValue( ABuffer );
//			OutputPin.Notify( &ABuffer );
		}

	public:
		void ExternalSetValue( T *AValues, uint32_t ACount )
		{
			TArray<T> AValue( ACount, AValues );
			TArray<T> Value( _Count, _Values );
			if( Value == AValue )
				return;

			_Values = AValues;
			_Count = ACount;
			T_OutputPin::SetPinValue( AValue );
		}

	public:
		inline void SystemStart()
		{
			SetValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SetValue();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER, 
		typename T_FInputValue,
		typename T
	> class ArduinoStateSetArrayValueElement :
		public T_FInputValue
	{
	protected:
		_V_PROP_( FInputValue )

	public:
		T *_Values;
		uint32_t _Count;

	public:
		inline void TryActive( bool &AResult )
		{
			if( AResult )
				return;

			if( FInputValue() )
			{
//					Serial.println( "TryActive" );
//					Serial.println( Value );
				C_OWNER.ExternalSetValue( _Values, _Count );
				AResult = true;
			}

		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FInputValue() == *(bool *)_Data )
				return;

//			Serial.println( FInputValue );

			FInputValue() = *(bool *)_Data;
//			if( FInputValue )
//				FOwner.ExternalSetValue( nullptr, Value );

//			else
			C_OWNER.ResetValue();
		}

	public:
		ArduinoStateSetArrayValueElement()
		{
			FInputValue() = false;
//			C_OWNER.RegisterElement( this );
		}

	};
//---------------------------------------------------------------------------    
	template<
		typename T_Elements_TryActive,
		typename T_OutputPin,
		typename T
	> class ValueArrayStateSource : // public ValueArraySource<T, C_ELEMENTS_COUNT, T_OUTPUT_PIN>
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		T *_Values;
		uint32_t _Count;

	protected:
		void SetValue()
		{
			if( ! _Count )
				return;

			TArray<T> ABuffer( _Count, _Values );
			T_OutputPin::SetPinValue( ABuffer );
//			OutputPin.Notify( &ABuffer );
		}

	protected:
		TValueArray<T> FCurrentValue;

	public:
		void ExternalSetValue( T *AValues, uint32_t ACount )
		{
			TArray<T> AValue( ACount, AValues );
			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::ChainCall( AResult );
			if( AResult )
				return;

			FCurrentValue.Assign( _Values, _Count );
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue.Assign( _Values, _Count );
			SetValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_OutputPin
	> class ComplexValueArraySource :
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		float *_Values;
		uint32_t _Count;

	public:
		void ExternalSetValue( float *AValues, uint32_t ACount )
		{
			TArray<Mitov::TComplex> AValue( ACount, (Mitov::TComplex *)AValues );
			TArray<Mitov::TComplex> Value( _Count, (Mitov::TComplex *)_Values );
			if( Value == AValue )
				return;

			_Values = AValues;
			_Count = ACount;
			T_OutputPin::SetPinValue( AValue );
		}

	protected:
		void SetValue()
		{
			if( ! _Count )
				return;

			TArray<Mitov::TComplex> ABuffer( _Count, (Mitov::TComplex *)_Values );
			T_OutputPin::SetPinValue( ABuffer );
//			OutputPin.Notify( &ABuffer );
		}

	public:
		inline void SystemStart()
		{
			SetValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			SetValue();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_OutputPin
	> class ValueComplexArrayStateSource : // public ComplexValueArraySource<C_ELEMENTS_COUNT, T_OUTPUT_PIN>
		public T_OutputPin
	{
//		typedef Mitov::ComplexValueArraySource<C_ELEMENTS_COUNT, T_OUTPUT_PIN> inherited;

	public:
		_V_PIN_( OutputPin )

	public:
		float *_Values;
		uint32_t _Count;

	protected:
		TValueArray<Mitov::TComplex> FCurrentValue;

	public:
		void SetValue()
		{
			if( ! _Count )
				return;

			TArray<Mitov::TComplex> ABuffer( _Count, (Mitov::TComplex *)_Values );
			T_OutputPin::SetPinValue( ABuffer );
//			OutputPin.Notify( &ABuffer );
		}

		void ExternalSetValue( float *AValues, uint32_t ACount )
		{
			TArray<Mitov::TComplex> AValue( ACount, (Mitov::TComplex *)AValues );
			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
			bool AResult = false;
			T_Elements_TryActive::ChainCall( AResult );
			if( AResult )
				return;

			FCurrentValue.Assign( (Mitov::TComplex *)_Values, _Count );
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue.Assign( (Mitov::TComplex *)_Values, _Count );
			SetValue();
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Count,
		typename T_Index,
		typename T_OutputPin,
		typename T
	> class ArrayDeleteItems : 
		public T_Count,
		public T_Index,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin ) 

	public:
		_V_PROP_( Index )
		_V_PROP_( Count )

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TArray<T> *AData = (TArray<T>*)_Data;
			if( Count() == 0 || ( Index() > AData->Size ))
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			uint8_t ACount = MitovMin<int>( Count, int( AData->Size ) - int( Index ) );
			if( ACount == AData->Size )
				return;

			uint8_t AOutSize = AData->Size - ACount;
			T *AOutData = new T[ AOutSize ];
			for( int i = 0; i < Index(); ++i )
				AOutData[ i ] = AData->Data[ i ];

			for( int i = 0; i < AOutSize; ++i )
				AOutData[ Index + i ] = AData->Data[ Index + ACount + i ];

			TArray<T> ABuffer( AOutSize, AOutData );
			T_OutputPin::SetPinValue( ABuffer );
			delete [] AOutData;
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Index,
		typename T_OutputPin,
		typename T
	> class ArrayInsertItems :
		public T_Index,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin ) 

	public:
		_V_PROP_( Index )

	public:
		T *_Values;
		uint32_t _Count;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TArray<T> *AData = (TArray<T>*)_Data;

			if( _Count == 0 || ( Index() > AData->Size ))
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			uint8_t AOutSize = AData->Size + _Count;
			T *AOutData = new T[ AOutSize ];

			for( int i = 0; i < Index(); ++i )
				AOutData[ i ] = AData->Data[ i ];

			for( int i = 0; i < _Count; ++i )
				AOutData[ i + Index() ] = _Values[ i ];

			for( int i = Index; i < AData->Size; ++i )
				AOutData[ i + _Count ] = AData->Data[ i ];

			TArray<T> ABuffer( AOutSize, AOutData );
			T_OutputPin::SetPinValue( ABuffer );
			delete [] AOutData;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Index,
		typename T_OutputPin,
		typename T
	> class ArrayReplaceItemsAtIndex :
		public T_Index,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin ) 

	public:
		_V_PROP_( Index )

	public:
		T *_Values;
		uint32_t _Count;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			TArray<T> *AData = (TArray<T>*)_Data;

			uint8_t AOutSize = AData->Size;
			if( _Count == 0 || ( Index() > AOutSize ))
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			uint8_t ACount = MitovMin<int>( _Count, int( AOutSize ) - int( Index() ) );
			if( ACount == AData->Size )
				return;

			T *AOutData = new T[ AOutSize ];

			for( int i = 0; i < Index(); ++i )
				AOutData[ i ] = AData->Data[ i ];

			for( int i = 0; i < ACount; ++i )
				AOutData[ i + Index() ] = _Values[ i ];

			for( int i = Index() + ACount; i < AOutSize; ++i )
				AOutData[ i ] = AData->Data[ i ];

//			for( int i = 0; i < Index; ++i )
//				AOutData[ i ] = AData->Data[ i ];

			TArray<T> ABuffer( AOutSize, AOutData );
			T_OutputPin::SetPinValue( ABuffer );
			delete [] AOutData;

//			OutputPin.Notify( &ABuffer );
//			uint8_t AOutSize = AData->Size;

/*
			if( _Count == 0 || ( Index > AData->Size ))
			{
				OutputPin.Notify( _Data );
				return;
			}

			uint8_t AOutSize = AData->Size + _Count;
			T *AOutData = new T[ AOutSize ];

			for( int i = 0; i < Index; ++i )
				AOutData[ i ] = AData->Data[ i ];

			for( int i = 0; i < _Count; ++i )
				AOutData[ i + Index ] = _Values[ i ];

			for( int i = Index; i < AData->Size; ++i )
				AOutData[ i + _Count ] = AData->Data[ i ];

			TArray<T> ABuffer( AOutSize, AOutData );
			OutputPin.Notify( &ABuffer );
			delete [] AOutData;
*/
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_OutputPin,
		typename T
	> class ArrayReplace : 
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin ) 

	public:
		T *_FromValues;
		uint32_t _FromCount;

		T *_ToValues;
		uint32_t _ToCount;

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( _FromCount == 0 )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			TArray<T> &AData = *(TArray<T>*)_Data;
			TValueArray<T> AOutData;
			if( _FromCount > AData.Size )
			{
				T_OutputPin::PinNotify( _Data );
				return;
			}

			int AEndIndex = AData.Size;

//			Serial.println( AData.ToString() );

			for( int i = AData.Size - _FromCount + 1; i-- > 0; )
			{
//				for( int j = 0; j < _FromCount; ++j )
				bool AFound = true;
				for( int j = _FromCount; j--; )
				{
//					Serial.print( i ); Serial.print( " + " ); Serial.print( j ); Serial.print( ": " ); Serial.print( AData.Data[ i + j ] ); Serial.print( " " ); Serial.println( _FromValues[ j ] );
					if( AData.Data[ i + j ] != _FromValues[ j ] )
					{
						AFound = false;
						break;
					}
				}

				if( AFound )
				{
//					Serial.println( "Test" );
					uint32_t ASubSize = AEndIndex - i - _FromCount;
					AOutData.ResizeFront( AOutData.Size + ASubSize + _ToCount );

//					Serial.println( ASubSize );
//					delay( 1000 );
					AOutData.Copy( AData.Data + i + _FromCount, ASubSize, _ToCount );
//					Serial.println( AOutData.ToString() );
					AOutData.Copy( _ToValues, _ToCount, 0 );
//					Serial.println( AOutData.ToString() );
					AEndIndex = i;
					i -= _FromCount - 1;
				}
			}

			if( AEndIndex )
			{
				AOutData.ResizeFront( AOutData.Size + AEndIndex );
				AOutData.Copy( AData.Data, AEndIndex, 0 );
			}

			T_OutputPin::SetPinValue( AOutData );
		}
	};
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
