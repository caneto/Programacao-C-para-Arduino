////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_TEXT_h
#define _MITOV_TEXT_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
//		typename T_FStateActive,
		typename T_OutputPin,
		typename T_Value
	> class TextValue : 
//		public T_FStateActive,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		inline void SetValue( Mitov::String AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}

		void ExternalSetValue( Mitov::String AValue, bool AFromState )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			T_OutputPin::SetPinValue( AValue );
		}
/*
		inline void SetValue( Mitov::String AValue )
		{
			ExternalSetValue( AValue );
		}
*/
	public:
		inline void SystemStart()
		{
			T_OutputPin::SetPinValue( Value() );
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( Value() );
		}

	};
//---------------------------------------------------------------------------
/*
	class BindableTextValue : public TextValue
	{
		typedef TextValue inherited;

	protected:
		Mitov::String OldValue = inherited::Value;

	public:
		inline void SystemInit()
		{
			OldValue = inherited::Value;
		}

		inline void SystemLoopBegin()
		{
			if( inherited::Value == OldValue )
				return;

			OldValue = inherited::Value;
			inherited::OutputPin.Notify( (void *)OldValue.c_str() );
		}

	public:
		BindableTextValue( char *AValue ) :
			inherited( AValue ),
			OldValue( AValue )
		{
		}

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Value
	> class ArduinoSetTextValueElement :
		public T_Value
	{
	public:
		_V_PROP_( Value )

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_OWNER.ExternalSetValue( Value(), false );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FInputValue,
		typename T_Value
	> class ArduinoStateSetTextValueElement :
		public T_FInputValue,
		public T_Value
	{
	public:
		_V_PROP_( Value )

	protected:
		_V_PROP_( FInputValue )

	public:
		void TryActive( bool &AResult )
		{
			if( FInputValue() )
			{
				C_OWNER.ExternalSetValue( Value(), true );
				AResult = true;
			}
		}

	public:
		void InputPin_o_Receive( void *_Data )
		{
			if( FInputValue() == *(bool *)_Data )
				return;

			FInputValue() = *(bool *)_Data;
			if( FInputValue() )
				C_OWNER.ExternalSetValue( Value(), true );

			else
				C_OWNER.ResetValue();
		}

	public:
		ArduinoStateSetTextValueElement()
		{
			FInputValue() = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		typename T_Elements_TryActive,
		typename T_FStateActive,
		typename T_OutputPin,
		typename T_Value
	> class TextStateValue :
		public T_FStateActive,
		public T_OutputPin,
		public T_Value
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Value )

	public:
		_V_PROP_( FStateActive )

	protected:
		Mitov::String FCurrentValue;

	public:
		void SetValue( Mitov::String AValue )
		{
			if( Value().GetValue() == AValue )
				return;

			Value() = AValue;
			if( FStateActive() )
				return;

			T_OutputPin::SetPinValue( AValue );
		}

		void ExternalSetValue( Mitov::String AValue, bool AFromState )
		{
			if( FCurrentValue == AValue )
				return;

			FCurrentValue = AValue;
			if( AFromState )
				FStateActive() = true;

			T_OutputPin::SetPinValue( FCurrentValue );
		}

		void ResetValue()
		{
//			for( int i = FElements.size(); i -- ; )
//				if( FElements[ i ]->TryActive( this ) )
//					return;

			bool AResult = false;
			T_Elements_TryActive::ChainCall( AResult );
			if( AResult )
				return;

			FStateActive() = false;
			FCurrentValue = Value();
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline void SystemStart()
		{
			FCurrentValue = Value();
			T_OutputPin::SetPinValue( Value() );
//			if( ! ClockInputPin_o_IsConnected )
//				OutputPin.Notify( (void *)FCurrentValue.c_str() );

		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			T_OutputPin::SetPinValue( FCurrentValue );
		}

	public:
		inline TextStateValue()
		{
			FStateActive() = false;
		}
	};
//---------------------------------------------------------------------------
/*
	class BindableTextStateValue : public TextStateValue
	{
		typedef TextValue inherited;

	protected:
		Mitov::String OldValue;

	public:
		inline void SystemInit()
		{
			inherited::SystemInit();
			OldValue = inherited::Value;
		}

		inline void SystemLoopBegin()
		{
			if( inherited::Value == OldValue )
				return;

			OldValue = inherited::Value;
			ResetValue();
//			inherited::OutputPin.Notify( (void *)OldValue.c_str() );
		}

	public:
		using inherited::inherited;

	};
*/
//---------------------------------------------------------------------------
	template<
		typename T_INSTANCE, typename T_STRING, 
		uint16_t C_COUNT,
		typename T_Elements_GetText,
		typename T_FModified,
		typename T_OutputPin
	> class BasicTypedFormattedText :
		public T_FModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( FModified )

	protected:
		struct TStringItem
		{
			T_STRING	Text;
			int8_t		Index;
		};

	protected:
		Mitov::SimpleObjectDeleteList<TStringItem *>	FReadyElements;

	public:
		void AddReadyElement( T_STRING ATextItem, int16_t AIndex )
		{
//			Serial.print( "AddElement: " ); Serial.print( ATextItem ); Serial.println( AIndex );

			TStringItem	*AItem = new TStringItem;
			AItem->Text = ATextItem;
//			Serial.print( "AddElement: " ); Serial.println( AItem->Text );
			if( AIndex < C_COUNT )
				AItem->Index = AIndex;

			else
				AItem->Index = -1;

			FReadyElements.push_back( AItem );

//			Serial.println( FReadyElements[ FReadyElements.size() - 1 ]->Text );
//			Serial.println( "DEBUG>>" );
//			for( Mitov::SimpleList<TStringItem *>::iterator Iter = FReadyElements.begin(); Iter != FReadyElements.end(); ++Iter )
//				Serial.println( ( *Iter )->Text );

//			Serial.println( "<<DEBUG" );
		}

	protected:
		void ProcessSendOutput()
		{
//			Serial.println( "ProcessSendOutput" );
			Mitov::String AText;
			for( int i = 0; i < FReadyElements.size(); ++i )
			{
				AText += FReadyElements[ i ]->Text;
				if( T_Elements_GetText::ChainHasItems() )
					if( FReadyElements[ i ]->Index >= 0 )
					{
						Mitov::String ASubText;
						T_Elements_GetText::ChainCall( FReadyElements[ i ]->Index, ASubText );
						AText += ASubText;
					}

			}

//			Serial.println( AText );
			T_OutputPin::SetPinValue( AText.c_str() );
			FModified() = false;
		}

	public:
		inline void SystemLoopEnd()
		{
			if( FModified() )
				ProcessSendOutput();

		}

		inline void SystemStart() // Placeholder for inherited classes
		{
		}

	public:
		inline void ClockInputPin_o_Receive( void *_Data )
		{
			ProcessSendOutput();
		}

	public:
		BasicTypedFormattedText()
		{
			FModified() = false;
		}
	};
//---------------------------------------------------------------------------
	template<
		uint16_t C_COUNT,
		typename T_Elements_GetText,
		typename T_FModified,
		typename T_OutputPin,
		typename T_Text
	> class FormattedText_Fixed : 
		public Mitov::BasicTypedFormattedText<
				FormattedText_Fixed<
						C_COUNT,
						T_Elements_GetText,
						T_FModified,
						T_OutputPin,
						T_Text
					>, 
				char *, C_COUNT, T_Elements_GetText, T_FModified, T_OutputPin
			>
	{
		typedef Mitov::BasicTypedFormattedText<
				FormattedText_Fixed<
						C_COUNT,
						T_Elements_GetText,
						T_FModified,
						T_OutputPin,
						T_Text
					>, 
				char *, C_COUNT, T_Elements_GetText, T_FModified, T_OutputPin
		> inherited;

	public:
		void AddNullElement( const char *ATextItem )
		{
			typename inherited::TStringItem	*AItem = new typename inherited::TStringItem;

			AItem->Text = (char *)ATextItem;
			AItem->Index = -1;

			inherited::FReadyElements.push_back( AItem );
		}

	public:
		inline void SystemStart()
		{
			inherited::ProcessSendOutput();
		}

	};
//---------------------------------------------------------------------------
	template<
		uint16_t C_COUNT,
		typename T_Elements_GetText,
		typename T_FModified,
		typename T_OutputPin,
		typename T_Text
	> class FormattedText : 
		public Mitov::BasicTypedFormattedText<
				FormattedText<
						C_COUNT,
						T_Elements_GetText,
						T_FModified,
						T_OutputPin,
						T_Text
					>, 
				Mitov::String, C_COUNT, T_Elements_GetText, T_FModified, T_OutputPin
			>,
		public T_Text
//		public Mitov::BasicTypedFormattedText<FormattedText<C_COUNT>, Mitov::String, C_COUNT>
	{
//		typedef Mitov::BasicTypedFormattedText<FormattedText<C_COUNT>, Mitov::String, C_COUNT> inherited;
		typedef Mitov::BasicTypedFormattedText<
				FormattedText<
						C_COUNT,
						T_Elements_GetText,
						T_FModified,
						T_OutputPin,
						T_Text
					>, 
				Mitov::String, C_COUNT, T_Elements_GetText, T_FModified, T_OutputPin
		> inherited;

	public:
		_V_PROP_( Text )

	public:
		void InitElements()
		{
			inherited::FReadyElements.clear();
			Mitov::String	ATextItem;
			Mitov::String	AIndexText;
			bool	AInEscape = false;

//			Serial.println( "INIT" );
//			Serial.println( Text );
//			delay( 1000 );

			for( unsigned int i = 0; i < Text().GetValue().length(); ++ i )
			{
				char AChar = Text().GetValue()[ i ];
				if( AInEscape )
				{
					if( AChar >= '0' && AChar <= '9' )
						AIndexText += AChar;

					else
					{
						if( AChar == '%' )
						{
							if( AIndexText.length() == 0 )
							{
								ATextItem += '%';
								AInEscape = false;
							}

							else
							{
								inherited::AddReadyElement( ATextItem, AIndexText.toInt() );
								ATextItem = "";
								AIndexText = "";
							}

						}

						else
						{
							if( AIndexText.length() == 0 )
								ATextItem += '%';

							else
							{
								inherited::AddReadyElement( ATextItem, AIndexText.toInt() );
								ATextItem = "";
							}

							ATextItem += AChar;
						}

						if( AChar != '%' )
							AInEscape = false;
					}
				}

				else
				{
					if( AChar == '%' )
					{
						AInEscape = true;
						AIndexText = "";
					}

					else
						ATextItem += AChar;

				}

			}

			if( AInEscape )
				inherited::AddReadyElement( ATextItem, AIndexText.toInt() );

			else if( ATextItem.length() )
			{
				typename inherited::TStringItem	*AItem = new typename inherited::TStringItem;

				AItem->Text = ATextItem;
				AItem->Index = -1;

				inherited::FReadyElements.push_back( AItem );
			}

//			Serial.println( "DEBUG>>" );
//			for( Mitov::SimpleList<TStringItem *>::iterator Iter = FReadyElements.begin(); Iter != FReadyElements.end(); ++Iter )
//				Serial.println( ( *Iter )->Text );

//			Serial.println( "<<DEBUG" );
		}

	public:
		inline void SystemStart()
		{
			InitElements();
			inherited::ProcessSendOutput();
		}

	};
//---------------------------------------------------------------------------
	template<typename T_LCD, T_LCD &C_LCD, typename T> class TextFormatElementInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( Mitov::String( *(T*)_Data ));
		}
	};
//---------------------------------------------------------------------------
	template<typename T_LCD, T_LCD &C_LCD> class TextFormatElementStringInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( (char*)_Data );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_LCD, T_LCD &C_LCD> class TextFormatElementClockInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( "(Clock)" );
		}

	};
//---------------------------------------------------------------------------
	template<typename T_LCD, T_LCD &C_LCD, typename T_OBJECT> class TextFormatElementObjectInput
	{
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			C_LCD.SetValue( ((T_OBJECT *)_Data)->ToString() );
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Length
	> class FormattedTextElementText :
		public T_FillCharacter,
		public T_InitialValue,
		public T_Length
	{
	public:
		_V_PROP_( FillCharacter )
		_V_PROP_( InitialValue )
		_V_PROP_( Length )

	public:
		void SetValue( Mitov::String AValue )
		{
//			Serial.println( AValue );
			C_OWNER.FModified() = true;
//			FModified = true;
			InitialValue() = AValue;
		}

	public:
		inline void GetText( Mitov::String &AResult )
		{
			AResult = InitialValue();
		}

	public:
		inline void SystemStart()
		{
		}

	};
//---------------------------------------------------------------------------
	template<typename T> class FormattedTextLengthElementTyped : public T
	{
		typedef T inherited;

	public:
		inline void GetText( Mitov::String &AResult )
		{
			inherited::GetText( AResult );
			for( int i = AResult.length(); i < inherited::Length(); ++i )
				AResult = inherited::FillCharacter().GetValue() + AResult;

		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Base,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Length
	> class FormattedTextElementInteger :
		public T_Base,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Length
	{
	public:
		_V_PROP_( Base )
		_V_PROP_( FillCharacter )
		_V_PROP_( InitialValue )
		_V_PROP_( Length )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			char AText[ 16 ];
			ltoa( InitialValue(), AText, Base() );
			AResult = AText;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			int32_t AValue = *(int32_t*)_Data;
			if( AValue == InitialValue() )
				return;

			InitialValue() = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void SystemStart() // Placeholder for inherited classes
		{
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Base,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Length
	> class FormattedTextElementUnsigned :
		public T_Base,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Length
	{
	public:
		_V_PROP_( Base )
		_V_PROP_( FillCharacter )
		_V_PROP_( InitialValue )
		_V_PROP_( Length )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			char AText[ 16 ];
			itoa( InitialValue(), AText, Base() );
			AResult = AText;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			uint32_t AValue = *(uint32_t*)_Data;
			if( AValue == InitialValue() )
				return;

			InitialValue() = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void SystemStart() // Placeholder for inherited classes
		{
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Length,
		typename T_MinWidth,
		typename T_Precision
	> class FormattedTextElementAnalog :
		public T_FillCharacter,
		public T_InitialValue,
		public T_Length,
		public T_MinWidth,
		public T_Precision
	{
	public:
		_V_PROP_( FillCharacter )
		_V_PROP_( InitialValue )
		_V_PROP_( Length )
		_V_PROP_( MinWidth )
		_V_PROP_( Precision )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			char AText[ 16 ];
			dtostrf( InitialValue(),  MinWidth(), Precision(), AText );
//			Serial.println( AText );

			AResult = AText;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			float AValue = *(float*)_Data;
			if( AValue == InitialValue() )
				return;

			InitialValue() = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void SystemStart() // Placeholder for inherited classes
		{
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_FalseValue,
		typename T_FillCharacter,
		typename T_InitialValue,
		typename T_Length,
		typename T_TrueValue
	> class FormattedTextElementDigital :
		public T_FalseValue,
		public T_FillCharacter,
		public T_InitialValue,
		public T_Length,
		public T_TrueValue
	{
	public:
		_V_PROP_( FillCharacter )
		_V_PROP_( Length )
		_V_PROP_( TrueValue )
		_V_PROP_( FalseValue )
		_V_PROP_( InitialValue )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( InitialValue() )
				AResult = TrueValue();

			else
				AResult = FalseValue();

		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			bool AValue = *(bool*)_Data;
			if( AValue == InitialValue() )
				return;

			InitialValue() = AValue;
			C_OWNER.FModified() = true;
		}

	public:
		inline void SystemStart() // Placeholder for inherited classes
		{
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_OutputPin
	> class TextLength :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			int32_t AValue = strlen( (char *)_Data );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_Text
	> class FindTextPosition :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( Text )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			Mitov::String AText = Text();
			Mitov::String ATextValue = Mitov::String( (char *)_Data );

			if( IgnoreCase() )
			{
				AText.toUpperCase();
				ATextValue.toUpperCase();
			}

			int32_t AValue = ATextValue.indexOf( AText );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_Text
	> class FindLastTextPosition :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( Text )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			Mitov::String AText = Text();
			Mitov::String ATextValue = Mitov::String( (char *)_Data );

			if( IgnoreCase() )
			{
				AText.toUpperCase();
				ATextValue.toUpperCase();
			}

			int32_t AValue = ATextValue.lastIndexOf( AText );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_Text
	> class ContainsText :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( Text )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			Mitov::String AText = Text();
			Mitov::String ATextValue = Mitov::String( (char *)_Data );

			if( IgnoreCase() )
			{
				AText.toUpperCase();
				ATextValue.toUpperCase();
			}

			bool AValue = ( ATextValue.indexOf( AText ) >= 0 );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_Text
	> class TextStartsWith :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( Text )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			Mitov::String AText = Text();
			Mitov::String ATextValue = Mitov::String( (char *)_Data );

			if( IgnoreCase() )
			{
				AText.toUpperCase();
				ATextValue.toUpperCase();
			}

			bool AValue = ATextValue.startsWith( AText );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_Text
	> class TextEndsWith :
		public T_Enabled,
		public T_IgnoreCase,
		public T_OutputPin,
		public T_Text
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( IgnoreCase )
		_V_PROP_( Text )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
                return;

			Mitov::String AText = Text();
			Mitov::String ATextValue = Mitov::String( (char *)_Data );

			if( IgnoreCase() )
			{
				AText.toUpperCase();
				ATextValue.toUpperCase();
			}

			bool AValue = ATextValue.endsWith( AText );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_FromValue,
//		typename T_IgnoreCase,
		typename T_OutputPin,
		typename T_ToValue
	> class ReplaceText :
		public T_Enabled,
		public T_FromValue,
//		public T_IgnoreCase,
		public T_OutputPin,
		public T_ToValue
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( FromValue )
		_V_PROP_( ToValue )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = (char *)_Data;

			AValue.replace( FromValue(), ToValue() );

			T_OutputPin::SetPinValue( AValue, false );
        }
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin,
		typename T_Position
	> class SubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin,
		public T_Position
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )
		_V_PROP_( Position )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data ).substring( Position(), Position() + Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin
	> class LeftSubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data ).substring( 0, Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin
	> class RightSubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue = AValue.substring( AValue.length() - Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin,
		typename T_Position
	> class DeleteSubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin,
		public T_Position
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )
		_V_PROP_( Position )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.remove( Position(), Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin
	> class DeleteLeftSubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.remove( 0, Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_Length,
		typename T_OutputPin
	> class DeleteRightSubText :
		public T_Enabled,
		public T_Length,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )
		_V_PROP_( Length )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
			{
				T_OutputPin::PinNotify( _Data );
                return;
			}

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.remove( AValue.length() - Length() );

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_OutputPin
	> class ToUpperCase :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
				T_OutputPin::PinNotify( _Data );
                return;
            }

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.toUpperCase();

			T_OutputPin::SetPinValue( AValue, false );
        }
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_OutputPin
	> class ToLowerCase :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
				T_OutputPin::PinNotify( _Data );
                return;
            }

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.toLowerCase();

			T_OutputPin::SetPinValue( AValue, false );
        }
	};
//---------------------------------------------------------------------------
	template<
		typename T_Enabled,
		typename T_OutputPin
	> class TrimText :
		public T_Enabled,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

    public:
        _V_PROP_( Enabled )

	public:
		void InputPin_o_Receive( void *_Data )
        {
            if( ! Enabled() )
            {
				T_OutputPin::PinNotify( _Data );
                return;
            }

			Mitov::String AValue = Mitov::String( (char *)_Data );
			AValue.trim();

			T_OutputPin::SetPinValue( AValue, false );
        }

	};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
}

#ifdef __TEST_FOR_DEBUG_PRINTS__
#undef Serial
#endif

#endif
