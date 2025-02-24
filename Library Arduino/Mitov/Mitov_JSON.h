////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_JSON_h
#define _MITOV_JSON_h

#include <Mitov.h>

#ifdef __TEST_FOR_DEBUG_PRINTS__
#define Serial UNGUARDED DEBUG PRINT!!!
#endif

namespace Mitov
{
	class JSONSplitIntf
	{
	protected:
		static bool ParseHex( Mitov::String &AText, int &AIndex, Mitov::String &AName )
		{
			++AIndex;
//			Mitov::String AResult;
			int AStartIndex = AIndex;

			if( ! IsHexChar( AText[ AIndex ] ))
				return false;

//			while( IsHexChar( AText[ AIndex ] ))
//				++ AIndex;
//				AResult += AText[ AIndex ++ ];

			const char *ACharText = AText.c_str() + AStartIndex;
			char *AEnd;

			int AValue = strtol( ACharText, &AEnd, 16 );

			AIndex += ( AEnd - ACharText );

			AName += char( AValue );
		}

		static bool IsHexChar( char AChar )
		{
			if( AChar >= '0' && AChar <= '9' )
				return true;

			if( AChar >= 'a' && AChar <= 'f' )
				return true;

			if( AChar >= 'A' && AChar <= 'F' )
				return true;

			return false;
		}

	public:
		static bool ExtractString( Mitov::String &AText, Mitov::String &AName, Mitov::String &AElementValue )
		{
//			Serial.println( "::::" );
//			Serial.println( AElementValue );
			if( AText[ 0 ] != '"' )
				return false;

			bool AInEscape = false;			

			AElementValue += '"';
			for( int i = 1; i < AText.length(); ++ i )
			{
				char AChar = AText[ i ];
				AElementValue += AChar;
//				Serial.println( AChar );
				if( AInEscape )
				{
					switch( AChar )
					{
						case '"' :
						case '\\' :
						case '/' :
							AName += AChar;
							break;

						case 'b' :	AName += char( 8 );		break;
						case 'f' :	AName += char( 12 );	break;
						case 'n' :	AName += char( 10 );	break;
						case 'r' :	AName += char( 13 );	break;
						case 't' :	AName += char( 9 );	break;
						case 'u' :	
							if( ! ParseHex( AText, i, AName ))
								return false;

							break;

						default:
							return false;
					}
				}

				else
				{
					if( AChar == '"' )
					{
//						Serial.println( "---------" );
//						Serial.println( AElementValue );
						AText.remove( 0, i + 1 );
//						Serial.println( AText );
						return true;
					}

					if( AChar == '\\' )
						AInEscape = true;

					else
						AName += AChar;
				}
			}

			return false;
		}

		static bool ExtractObject( Mitov::String &AText, Mitov::String &AElementText )
		{
//			Serial.println( "OBJECT" );
			AElementText += "{";

			AText.remove( 0, 1 );
			AText.trim();
			while( AText.length() )
			{
				if( AText.startsWith( "}" ) )
				{
					AElementText += "}";
					AText.remove( 0, 1 );
					AText.trim();
					break;
				}

				Mitov::String AValue;
				if( ! ExtractString( AText, AValue, AElementText ))
					return false;

				AText.trim();
				if( ! AText.startsWith( ":" ) )
					return false;

				AElementText += ":";
				AText.remove( 0, 1 );
				AText.trim();

				if( ! ExtractElement( AText, AElementText ))
					return false;

				if( AText.startsWith( "," ) )
				{
					AElementText += ",";
					AText.remove( 0, 1 );
					AText.trim();
				}
			}

			return true;
		}

		static bool ExtractArray( Mitov::String &AText, Mitov::String &AElementText )
		{
			AElementText += "[";

			AText.remove( 0, 1 );
			AText.trim();
			while( AText.length() )
			{
				if( AText.startsWith( "]" ) )
				{
					AElementText += "]";
					AText.remove( 0, 1 );
					AText.trim();
					break;
				}

				if( ! ExtractElement( AText, AElementText ))
					return false;

//				Serial.println( AElementText );

				if( AText.startsWith( "," ) )
				{
					AElementText += ",";
					AText.remove( 0, 1 );
					AText.trim();
				}
			}

			return true;
		}

		static bool IsNumberChar( char AChar )
		{
			static const char numberChars[] = { '-', '+', '.', 'e', 'E' };

			if( ( AChar >= '0' ) && ( AChar <= '9' ) )
				return true;

			for( int i = 0; i < MITOV_ARRAY_SIZE( numberChars ); ++ i )
				if( AChar == numberChars[ i ] )
					return true;

			return false;
		}

		static bool ExtractNumber( Mitov::String &AText, Mitov::String &AValue )
		{
			for( int i = 0; i < AText.length(); ++ i )
			{
				if( ! IsNumberChar( AText[ i ] ))
				{
					if( i == 0 )
						return false;

					AValue += AText.substring( 0, i );

//					Serial.println( AValue );

					AText.remove( 0, i );
					return true;
				}
			}

			return true;
		}

		static bool ExtractElement( Mitov::String &AText, Mitov::String &AElementText )
		{
//			Serial.println( "ExtractElement" );
//			Serial.println( AText );

			Mitov::String AValue;
			if( AText.startsWith( "\"" ))
				return ExtractString( AText, AValue, AElementText );

			if( AText.startsWith( "true" ))
			{
				AElementText += "true";
				AText.remove( 0, 4 );
				return true;
			}

			if( AText.startsWith( "false" ))
			{
				AElementText += "false";
				AText.remove( 0, 5 );
				return true;
			}

			if( AText.startsWith( "null" ))
			{
				AElementText += "null";
//				Serial.println( "NULL" );
				AText.remove( 0, 4 );
				return true;
			}

			if( AText.startsWith( "{" ))
				return ExtractObject( AText, AElementText );

			if( AText.startsWith( "[" ))
				return ExtractArray( AText, AElementText );

			return ExtractNumber( AText, AElementText );
		}

	public:
		template<typename T_INSTANCE> static bool ProcessObject( T_INSTANCE *AInstance, Mitov::String &AText )			
		{
			AText.trim();

	//			Serial.println( AText );

			if( ! AText.startsWith( "{" ))
				return false;

			AText.remove( 0, 1 );
			AText.trim();

			if( AText.startsWith( "}" ))
			{
				AText.remove( 0, 1 );
				AText.trim();
				return true;
			}

			for(;;)
			{

	//				Serial.println( AText );

				Mitov::String AElementText;
				Mitov::String AName;
				if( ! ExtractString( AText, AName, AElementText ) )
					return false;

	//			Serial.println( AName );
	//			Serial.println( AText );

				AText.trim();
				if( ! AText.startsWith( ":" ))
					return false;

				AText.remove( 0, 1 );
				AText.trim();

	//			Serial.println( AText );

				bool AParsed = AInstance->ExtractElement( AName.c_str(), AText );
/*
				bool AParsed = false;
				for( int i = 0; i < Elements.size(); ++i )
					if( AName == ((SplitJSONBasicObjectElement *) Elements[ i ] )->FName )
					{
	//					Serial.println( "MATCH" );
	//					Serial.println( AName );
	//					Serial.println( ((SplitJSONBasicObjectElement *) Elements[ i ] )->FName );
						if( ! Elements[ i ]->Extract( AText ))
							return false;

	//					Serial.println( "SUCCESS" );

						AParsed = true;
						break;
					}
*/
	//			Serial.println( AParsed );

				Mitov::String AElementText1;
				if( ! AParsed )
					if( ! ExtractElement( AText, AElementText1 ))
						return false;

	//			Serial.println( "SUCCESS1" );
	//			Serial.println( AText );
				AText.trim();
				if( AText.startsWith( "}" ))
				{
					AText.remove( 0, 1 );
					AText.trim();
					break;
				}

	//			Serial.println( AText );

				AText.trim();
				if( !AText.startsWith( "," ))
					break;

				AText.remove( 0, 1 );
				AText.trim();

	//			Serial.println( "SUCCESS2" );
	//			Serial.println( AText );
			}

			return true;
		}

		template<typename T_INSTANCE> static bool ProcessArray( T_INSTANCE *AInstance, Mitov::String &AText )
		{
			AText.trim();

	//		Serial.println( "->> ProcessArray" );

	//			Serial.println( AText );

			if( ! AText.startsWith( "[" ))
				return false;

			AText.remove( 0, 1 );
			AText.trim();

			if( AText.startsWith( "]" ))
			{
				AText.remove( 0, 1 );
				AText.trim();
				return true;
			}

//			int AElementIndex = 0;
			bool AAfterFirst = false;

			if( ! AInstance->ExtractElements( AText, AAfterFirst ))
				return false;

			AText.trim();
			if( AText.startsWith( "]" ))
			{
				AText.remove( 0, 1 );
				AText.trim();
				return true;
			}

			if( AAfterFirst )
			{
				AText.trim();
				if( ! AText.startsWith( "," ))
					return false;

				AText.remove( 0, 1 );
				AText.trim();
			}

			while( AText.length() )
			{

//					Serial.println( AText );

//				if( AElementIndex < Elements.size() )
//				{
//					if( ! Elements[ AElementIndex ++ ]->Extract( AText ))
//						return false;

//				}

//				if( ! AInstance->ExtractElements( AElementIndex, AText ))
//					return false;

				Mitov::String AElementText;
				if( ! ExtractElement( AText, AElementText ))
					return false;

				AText.trim();
				if( AText.startsWith( "]" ))
				{
					AText.remove( 0, 1 );
					AText.trim();
					return true;
				}

//				Serial.println( AText );

				AText.trim();
				if( ! AText.startsWith( "," ))
					return false;

				AText.remove( 0, 1 );
				AText.trim();

//				Serial.println( AText );
			}

			return false;
		}

		static bool SkipComma( Mitov::String & AText )
		{
			AText.trim();
			if( ! AText.startsWith( "," ))
				return false;

			AText.remove( 0, 1 );
			AText.trim();
			return true;
		}
	};
//---------------------------------------------------------------------------
	template <
		char C_BRACKET_OPEN,
		char C_BRACKET_CLOSE,
		typename T_Elements_GetText,
		typename T_FModified,
		typename T_OnlyModified,
		typename T_OutputPin
	> class MakeJSON :
		public T_FModified,
		public T_OnlyModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( OnlyModified )

	public:
		_V_PROP_( FModified ) // = false;

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "++++++" );
//			int AElementCount = Elements.size();
			if( OnlyModified() )
				if( ! FModified() )
					return;
/*
			{
				bool AModified = false;
				for( int i = 0; i < AElementCount; ++i )
					if( Elements[ i ]->GetIsModified() )
					{
						AModified = true;
						break;
					}

				if( ! AModified )
					return;
			}
*/
/*
			else
			{
				for( int i = 0; i < AElementCount; ++i )
					if( ! Elements[ i ]->GetIsPopulated() )
						return;

			}
*/
			FModified() = false;
			Mitov::String AResult; // Needs to be empty string at start!

			T_Elements_GetText::ChainCall( AResult );

			AResult = C_BRACKET_OPEN + AResult + C_BRACKET_CLOSE;

			T_OutputPin::SetPinValue( AResult );
		}

	public:
		inline void SetModified()
		{
			FModified() = true;
		}

	public:
		inline void SystemLoopBegin()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	public:
		inline MakeJSON()
		{
			FModified() = false;
		}

	};
//---------------------------------------------------------------------------
/*
	template <
		typename T_Elements_GetText,
		typename T_FModified,
		typename T_OnlyModified,
		typename T_OutputPin
	> class MakeJSONArray :
		public T_FModified,
		public T_OnlyModified,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( OnlyModified )

	public:
		_V_PROP_( FModified ) // = false;

	public:
		void ClockInputPin_o_Receive( void *_Data )
		{
//			Serial.println( "++++++" );
//			int AElementCount = Elements.size();
			if( OnlyModified() )
				if( ! FModified() )
					return;

			FModified() = false;
			Mitov::String AResult; // Needs to be empty string at start!

			T_Elements_GetText::ChainCall( AResult );

			AResult = "[" + AResult + "]";

			T_OutputPin::SetPinValue( AResult );
		}

	public:
		inline void SetModified()
		{
			FModified() = true;
		}

	public:
		inline void SystemLoopBegin()
		{
			ClockInputPin_o_Receive( nullptr );
		}

	public:
		inline MakeJSONArray()
		{
			FModified() = false;
		}

	};
*/
//---------------------------------------------------------------------------
	template <
		typename T_Elements_ExtractElement,
		typename T_Elements_SendValue,
		typename T_ErrorOutputPin
	> class SplitJSON :
		public T_ErrorOutputPin
	{
	public:
		_V_PIN_( ErrorOutputPin )

	public:
		inline bool ExtractElement( const char *AName, Mitov::String &AText )
		{
			bool AResult = false;
			T_Elements_ExtractElement::ChainCall( AName, AText, AResult );
			return AResult;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::String AText = (char *)_Data;

			if( ! JSONSplitIntf::ProcessObject( this, AText ))
			{
				T_ErrorOutputPin::ClockPin();
				return;
			}

			if( AText.length() )
			{
				T_ErrorOutputPin::ClockPin();
				return;
			}

			T_Elements_SendValue::ChainCall();
//			for( int i = 0; i < Elements.size(); ++i )
//				Elements[ i ]->SendDefault();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_ExtractElements,
		typename T_Elements_SendValue,
		typename T_ErrorOutputPin
	> class SplitJSONArray :
		public T_ErrorOutputPin
	{
	public:
		_V_PIN_( ErrorOutputPin )

	public:
		inline bool ExtractElements( Mitov::String &AText, bool &AAfterFirst )
		{
			bool AFailed = false;
			T_Elements_ExtractElements::ChainCall( AText, AAfterFirst, AFailed );
			return ! AFailed;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			Mitov::String AText = (char *)_Data;

//			Serial.println( AText );

			if( ! JSONSplitIntf::ProcessArray( this, AText ))
			{
				T_ErrorOutputPin::ClockPin();
				return;
			}

			if( AText.length() )
			{
				T_ErrorOutputPin::ClockPin();
				return;
			}

			T_Elements_SendValue::ChainCall();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name
	> class MakeJSONCustomElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Name )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( AResult != "" )
				AResult += ",";

			AResult += Mitov::String( "\"" ) + Name().GetValue() + "\":" + InitialValue().GetValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = (char *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name
	> class MakeJSONTextElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Name )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( AResult != "" )
				AResult += ",";

			AResult += Mitov::String( "\"" ) + Name().GetValue() + "\":\"" + InitialValue().GetValue() + "\"";
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = (char *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name
	> class MakeJSONDigitalElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Name )

	protected:
		inline const char *GetValueText()
		{
			if( InitialValue() )
				return "true";

			return "false";
		}

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			AResult += Mitov::String( "\"" ) + Name().GetValue() + "\":" + GetValueText();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(bool *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name
	> class MakeJSONIntegerElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Name )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			char AText[ 16 ];
			ltoa( InitialValue(), AText, 10 );

			AResult += Mitov::String( "\"" ) + Name().GetValue() + "\":" + AText;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(int32_t *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name
	> class MakeJSONUnsignedElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( Name )

	protected:
		Mitov::String GetValueText()
		{
			char AText[ 16 ];
			ltoa( InitialValue, AText, 10 );
			return AText;
		}

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			char AText[ 16 ];
			utoa( InitialValue(), AText, 10 );

			AResult += Mitov::String( "\"" ) + Name().GetValue() + "\":" + AText;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(uint32_t *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_MinWidth,
		typename T_Name,
		typename T_Precision
	> class MakeJSONAnalogElement :
		public T_Enabled,
		public T_InitialValue,
		public T_MinWidth,
		public T_Name,
		public T_Precision
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( MinWidth )
		_V_PROP_( Name )
		_V_PROP_( Precision )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			char AText[ 16 ];
			dtostrf( InitialValue(),  MinWidth(), Precision(), AText );

			AResult += Mitov::String( "\"" ) + Name().GetValue() + "\":" + AText;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(float *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_Name
	> class MakeJSONNullElement :
		public T_Enabled,
		public T_Name
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			AResult += Mitov::String( "\"" ) + Name().GetValue() + "\":null";
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Elements_GetText,
		typename T_Enabled,
		typename T_Name
	> class MakeJSONObjectElement :
		public T_Enabled,
		public T_Name
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			Mitov::String ASubResult; // Needs to be empty string at start!

			T_Elements_GetText::ChainCall( ASubResult );

			AResult += Mitov::String( "\"" ) + Name().GetValue() + "\":{" + ASubResult + "}";
		}

	public:
		inline void SetModified()
		{
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Elements_GetText,
		typename T_Enabled,
		typename T_Name
	> class MakeJSONArrayElement :
		public T_Enabled,
		public T_Name
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			Mitov::String ASubResult; // Needs to be empty string at start!

			T_Elements_GetText::ChainCall( ASubResult );

			AResult += Mitov::String( "\"" ) + Name().GetValue() + "\":[" + ASubResult + "]";
		}

	public:
		inline void SetModified()
		{
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue
	> class MakeJSONTextArrayElement :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			AResult += Mitov::String( "\"" ) + InitialValue().GetValue() + "\"";
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = (char *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue
	> class MakeJSONDigitalArrayElement :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			if( InitialValue() )
				AResult += "true";

			else
				AResult += "false";

		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(bool *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue
	> class MakeJSONIntegerArrayElement :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			char AText[ 16 ];
			ltoa( InitialValue(), AText, 10 );

			AResult += AText;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(int32_t *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue
	> class MakeJSONUnsignedArrayElement :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			char AText[ 16 ];
			utoa( InitialValue(), AText, 10 );

			AResult += AText;
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(uint32_t *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_MinWidth,
		typename T_Precision
	> class MakeJSONAnalogArrayElement :
		public T_Enabled,
		public T_InitialValue,
		public T_MinWidth,
		public T_Precision
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )
		_V_PROP_( MinWidth )
		_V_PROP_( Precision )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			char AText[ 16 ];
			dtostrf( InitialValue(), MinWidth(), Precision(), AText );

			AResult += AText;
		}
	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = *(float *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled
	> class MakeJSONNullArrayElement :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",null";

			else
				AResult += "null";

		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Enabled,
		typename T_InitialValue
	> class MakeJSONCustomArrayElement :
		public T_Enabled,
		public T_InitialValue
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			AResult += InitialValue().GetValue();
		}

	public:
		inline void InputPin_o_Receive( void *_Data )
		{
			InitialValue() = (char *)_Data;
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Elements_GetText,
		typename T_Enabled
	> class MakeJSONObjectArrayElement :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			Mitov::String ASubResult; // Needs to be empty string at start!

			T_Elements_GetText::ChainCall( ASubResult );

			AResult += "{" + ASubResult + "}";
		}

	public:
		inline void SetModified()
		{
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_OWNER, T_OWNER &C_OWNER,
		typename T_Elements_GetText,
		typename T_Enabled
	> class MakeJSONArrayArrayElement :
		public T_Enabled
	{
	public:
		_V_PROP_( Enabled )

	public:
		inline void GetText( Mitov::String &AResult )
		{
			if( ! Enabled() )
				return;

			if( AResult != "" )
				AResult += ",";

			Mitov::String ASubResult; // Needs to be empty string at start!

			T_Elements_GetText::ChainCall( ASubResult );

			AResult += "[" + ASubResult + "]";
		}

	public:
		inline void SetModified()
		{
			C_OWNER.SetModified();
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name,
		typename T_OutputPin
	> class SplitJSONTextElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElement( const char * AName, Mitov::String & AText, bool & AResult )
		{
			if( AResult )
				return;

			if( ! Enabled() )
				return;

			if( Name().GetValue() != AName )
				return;

			Mitov::String ATextValue;
			Mitov::String AValue;
			if( ! JSONSplitIntf::ExtractString( AText, AValue, ATextValue ))
				return;

//			Serial.println( AValue );

			InitialValue() = AValue;
			AResult = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name,
		typename T_OutputPin
	> class SplitJSONAnyElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElement( const char * AName, Mitov::String & AText, bool & AResult )
		{
//			Serial.println( "SplitJSONAnyElement" );
			if( AResult )
				return;

//			Serial.println( "SplitJSONAnyElement 1" );
			if( ! Enabled() )
				return;

			if( Name().GetValue() != AName )
				return;

//			Serial.println( "ExtractElement" );
//			Serial.println( AText );
			Mitov::String AValue;
			if( ! JSONSplitIntf::ExtractElement( AText, AValue ) )
				return;

//			Serial.println( "**********" );
//			Serial.println( AValue );

			InitialValue() = AValue;
			AResult = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name,
		typename T_OutputPin
	> class SplitJSONDigitalElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElement( const char * AName, Mitov::String & AText, bool & AResult )
		{
			if( AResult )
				return;

			if( ! Enabled() )
				return;

			if( Name().GetValue() != AName )
				return;

//			Serial.println( AText );

			if( AText.startsWith( "true" ))
			{
				AText.remove( 0, 4 );
//				Serial.println( "TRUE" );
				InitialValue() = true;
				AResult = true;
				return;
			}

			if( AText.startsWith( "false" ))
			{
				AText.remove( 0, 5 );
//				Serial.println( "FALSE" );
				InitialValue() = false;
				AResult = true;
				return;
			}

		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name,
		typename T_OutputPin
	> class SplitJSONIntegerElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElement( const char * AName, Mitov::String & AText, bool & AResult )
		{
			if( AResult )
				return;

			if( ! Enabled() )
				return;

			if( Name().GetValue() != AName )
				return;

			Mitov::String AValue;
			if( ! JSONSplitIntf::ExtractNumber( AText, AValue ))
				return;

//			Serial.println( "INTEGER" );
//			Serial.println( AValue );
			InitialValue() = atol( AValue.c_str() );
			AResult = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name,
		typename T_OutputPin
	> class SplitJSONUnsignedElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElement( const char * AName, Mitov::String & AText, bool & AResult )
		{
			if( AResult )
				return;

			if( ! Enabled() )
				return;

			if( Name().GetValue() != AName )
				return;

			Mitov::String AValue;
			if( ! JSONSplitIntf::ExtractNumber( AText, AValue ))
				return;

//			Serial.println( "UNSIGNED" );
//			Serial.println( AValue );
			InitialValue() = atol( AValue.c_str() );
			AResult = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name,
		typename T_OutputPin
	> class SplitJSONAnalogElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElement( const char * AName, Mitov::String & AText, bool & AResult )
		{
			if( AResult )
				return;

			if( ! Enabled() )
				return;

			if( Name().GetValue() != AName )
				return;

			Mitov::String AValue;
			if( ! JSONSplitIntf::ExtractNumber( AText, AValue ))
				return;

//			Serial.println( "ANALOG" );
//			Serial.println( AValue );
			InitialValue() = strtod( AValue.c_str(), nullptr );
			AResult = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_ExtractElement,
		typename T_Elements_SendValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name,
		typename T_OutputPin
	> class SplitJSONObjectElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )
		_V_PROP_( InitialValue )

	public:
		_V_PIN_( OutputPin )

	public:
		inline void ExtractElement( const char * AName, Mitov::String & AText, bool & AResult )
		{
			if( AResult )
				return;

			if( ! Enabled() )
				return;

			if( Name().GetValue() != AName )
				return;

			AResult = JSONSplitIntf::ProcessObject( this, AText );
		}

	public:
		inline bool ExtractElement( const char *AName, Mitov::String &AText )
		{
			bool AResult = false;
			T_Elements_ExtractElement::ChainCall( AName, AText, AResult );
			return AResult;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_Elements_SendValue::ChainCall();

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_ExtractElement,
		typename T_Elements_SendValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name,
		typename T_OutputPin
	> class SplitJSONObjectElement_Output :
		public T_Enabled,
		public T_InitialValue,
		public T_Name,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )
		_V_PROP_( InitialValue )

	public:
		_V_PIN_( OutputPin )

	public:
		inline void ExtractElement( const char * AName, Mitov::String & AText, bool & AResult )
		{
			if( AResult )
				return;

			if( ! Enabled() )
				return;

			if( Name().GetValue() != AName )
				return;

			InitialValue() = AText;
			AResult = JSONSplitIntf::ProcessObject( this, AText );
			InitialValue() = InitialValue().GetValue().substring( 0, InitialValue().GetValue().length() - AText.length() );
		}

	public:
		inline bool ExtractElement( const char *AName, Mitov::String &AText )
		{
			bool AResult = false;
			T_Elements_ExtractElement::ChainCall( AName, AText, AResult );
			return AResult;
		}

		inline void SendValue()
		{
			if( Enabled() )
			{
				T_OutputPin::SetPinValue( InitialValue() );
				T_Elements_SendValue::ChainCall();
			}

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_ExtractElements,
		typename T_Elements_SendValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name,
		typename T_OutputPin
	> class SplitJSONArrayElement :
		public T_Enabled,
		public T_InitialValue,
		public T_Name,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )
		_V_PROP_( InitialValue )

	public:
		_V_PIN_( OutputPin )

	public:
		inline void ExtractElement( const char * AName, Mitov::String & AText, bool & AResult )
		{
			if( AResult )
				return;

			if( ! Enabled() )
				return;

			if( Name().GetValue() != AName )
				return;

			AResult = JSONSplitIntf::ProcessArray( this, AText );
		}

	public:
		inline bool ExtractElements( Mitov::String &AText, bool &AAfterFirst )
		{
			bool AFailed = false;
			T_Elements_ExtractElements::ChainCall( AText, AAfterFirst, AFailed );
			return ! AFailed;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_Elements_SendValue::ChainCall();

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_ExtractElements,
		typename T_Elements_SendValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_Name,
		typename T_OutputPin
	> class SplitJSONArrayElement_Output :
		public T_Enabled,
		public T_InitialValue,
		public T_Name,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( Name )
		_V_PROP_( InitialValue )

	public:
		_V_PIN_( OutputPin )

	public:
		inline void ExtractElement( const char * AName, Mitov::String & AText, bool & AResult )
		{
			if( AResult )
				return;

			if( ! Enabled() )
				return;

			if( Name().GetValue() != AName )
				return;

			InitialValue() = AText;
			AResult = JSONSplitIntf::ProcessArray( this, AText );
			InitialValue() = InitialValue().GetValue().substring( 0, InitialValue().GetValue().length() - AText.length() );
		}

	public:
		inline bool ExtractElements( Mitov::String &AText, bool &AAfterFirst )
		{
			bool AFailed = false;
			T_Elements_ExtractElements::ChainCall( AText, AAfterFirst, AFailed );
			return ! AFailed;
		}

		inline void SendValue()
		{
			if( Enabled() )
			{
				T_OutputPin::SetPinValue( InitialValue() );
				T_Elements_SendValue::ChainCall();
			}

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class SplitJSONTextArrayElement :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElements( Mitov::String & AText, bool & AAfterFirst, bool & AFailed )
		{
			if( AFailed )
				return;

			if( ! Enabled() )
				return;

			AText.trim();
			if( AText.startsWith( "]" ))
				return;

			if( AAfterFirst )
				if( ! JSONSplitIntf::SkipComma( AText ) )
				{
					AFailed = true;
					return;
				}

//			Serial.println( AText );

			Mitov::String AValue;
			Mitov::String ATextValue;
			if( ! JSONSplitIntf::ExtractString( AText, AValue, ATextValue ))
			{
				AFailed = true;
				return;
			}

//			Serial.println( AValue );

			InitialValue() = AValue;
			AAfterFirst = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class SplitJSONDigitalArrayElement :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElements( Mitov::String & AText, bool & AAfterFirst, bool & AFailed )
		{
			if( AFailed )
				return;

			if( ! Enabled() )
				return;

			AText.trim();
			if( AText.startsWith( "]" ))
				return;

			if( AAfterFirst )
				if( ! JSONSplitIntf::SkipComma( AText ) )
				{
					AFailed = true;
					return;
				}

//			Serial.println( AText );

			if( AText.startsWith( "true" ))
			{
				AText.remove( 0, 4 );
//				Serial.println( "TRUE" );
				InitialValue() = true;
				AAfterFirst = true;
				return;
			}

			if( AText.startsWith( "false" ))
			{
				AText.remove( 0, 5 );
//				Serial.println( "FALSE" );
				InitialValue() = false;
				AAfterFirst = true;
				return;
			}

			AFailed = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class SplitJSONIntegerArrayElement :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElements( Mitov::String & AText, bool & AAfterFirst, bool & AFailed )
		{
			if( AFailed )
				return;

			if( ! Enabled() )
				return;

			AText.trim();
			if( AText.startsWith( "]" ))
				return;

			if( AAfterFirst )
				if( ! JSONSplitIntf::SkipComma( AText ) )
				{
					AFailed = true;
					return;
				}

			Mitov::String AValue;
			if( ! JSONSplitIntf::ExtractNumber( AText, AValue ))
			{
				AFailed = true;
				return;
			}

//			Serial.println( "INTEGER" );
//			Serial.println( AValue );
			InitialValue() = atol( AValue.c_str() );
			AAfterFirst = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class SplitJSONUnsignedArrayElement :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElements( Mitov::String & AText, bool & AAfterFirst, bool & AFailed )
		{
			if( AFailed )
				return;

			if( ! Enabled() )
				return;

			AText.trim();
			if( AText.startsWith( "]" ))
				return;

			if( AAfterFirst )
				if( ! JSONSplitIntf::SkipComma( AText ) )
				{
					AFailed = true;
					return;
				}

			Mitov::String AValue;
			if( ! JSONSplitIntf::ExtractNumber( AText, AValue ))
			{
				AFailed = true;
				return;
			}

//			Serial.println( "UNSIGNED" );
//			Serial.println( AValue );
			InitialValue() = atol( AValue.c_str() );
			AAfterFirst = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class SplitJSONAnalogArrayElement :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElements( Mitov::String & AText, bool & AAfterFirst, bool & AFailed )
		{
			if( AFailed )
				return;

			if( ! Enabled() )
				return;

			AText.trim();
			if( AText.startsWith( "]" ))
				return;

			if( AAfterFirst )
				if( ! JSONSplitIntf::SkipComma( AText ) )
				{
					AFailed = true;
					return;
				}

			Mitov::String AValue;
			if( ! JSONSplitIntf::ExtractNumber( AText, AValue ))
			{
				AFailed = true;
				return;
			}

//			Serial.println( "ANALOG" );
//			Serial.println( AValue );
			InitialValue() = strtod( AValue.c_str(), nullptr );
			AAfterFirst = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

		}
	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_ExtractElement,
		typename T_Elements_SendValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class SplitJSONObjectArrayElement :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		_V_PIN_( OutputPin )

	public:
		inline bool ExtractElement( const char *AName, Mitov::String &AText )
		{
			bool AResult = false;
			T_Elements_ExtractElement::ChainCall( AName, AText, AResult );
			return AResult;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_Elements_SendValue::ChainCall();

		}

	public:
		inline void ExtractElements( Mitov::String & AText, bool & AAfterFirst, bool & AFailed )
		{
			if( AFailed )
				return;

			if( ! Enabled() )
				return;

			AText.trim();
			if( AText.startsWith( "]" ))
				return;

			if( AAfterFirst )
				if( ! JSONSplitIntf::SkipComma( AText ) )
				{
					AFailed = true;
					return;
				}

			if( ! JSONSplitIntf::ProcessObject( this, AText ))
			{
				AFailed = true;
				return;
			}

			AAfterFirst = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_ExtractElement,
		typename T_Elements_SendValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class SplitJSONObjectArrayElement_Output :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		_V_PIN_( OutputPin )

	public:
		inline bool ExtractElement( const char *AName, Mitov::String &AText )
		{
			bool AResult = false;
			T_Elements_ExtractElement::ChainCall( AName, AText, AResult );
			return AResult;
		}

		inline void SendValue()
		{
			if( Enabled() )
			{
				T_OutputPin::SetPinValue( InitialValue() );
				T_Elements_SendValue::ChainCall();
			}

		}

	public:
		inline void ExtractElements( Mitov::String & AText, bool & AAfterFirst, bool & AFailed )
		{
			if( AFailed )
				return;

			if( ! Enabled() )
				return;

			AText.trim();
			if( AText.startsWith( "]" ))
				return;

			if( AAfterFirst )
				if( ! JSONSplitIntf::SkipComma( AText ) )
				{
					AFailed = true;
					return;
				}

			InitialValue() = AText;
			if( ! JSONSplitIntf::ProcessObject( this, AText ))
			{
				InitialValue() = "";
				AFailed = true;
				return;
			}

			InitialValue() = InitialValue().GetValue().substring( 0, InitialValue().GetValue().length() - AText.length() );
			AAfterFirst = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_ExtractElements,
		typename T_Elements_SendValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class SplitJSONArrayArrayElement :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		_V_PIN_( OutputPin )

	public:
		inline bool ExtractElements( Mitov::String &AText, bool &AAfterFirst )
		{
			bool AFailed = false;
			T_Elements_ExtractElements::ChainCall( AText, AAfterFirst, AFailed );
			return ! AFailed;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_Elements_SendValue::ChainCall();

		}

	public:
		inline void ExtractElements( Mitov::String & AText, bool & AAfterFirst, bool & AFailed )
		{
			if( AFailed )
				return;

			if( ! Enabled() )
				return;

			AText.trim();
			if( AText.startsWith( "]" ))
				return;

			if( AAfterFirst )
				if( ! JSONSplitIntf::SkipComma( AText ) )
				{
					AFailed = true;
					return;
				}

			if( ! JSONSplitIntf::ProcessArray( this, AText ) )
			{
				AFailed = true;
				return;
			}

			AAfterFirst = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Elements_ExtractElements,
		typename T_Elements_SendValue,
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class SplitJSONArrayArrayElement_Output :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		_V_PIN_( OutputPin )

	public:
		inline bool ExtractElements( Mitov::String &AText, bool &AAfterFirst )
		{
			bool AFailed = false;
			T_Elements_ExtractElements::ChainCall( AText, AAfterFirst, AFailed );
			return ! AFailed;
		}

		inline void SendValue()
		{
			if( Enabled() )
			{
				T_OutputPin::SetPinValue( InitialValue() );
				T_Elements_SendValue::ChainCall();
			}

		}

	public:
		inline void ExtractElements( Mitov::String & AText, bool & AAfterFirst, bool & AFailed )
		{
			if( AFailed )
				return;

			if( ! Enabled() )
				return;

			AText.trim();
			if( AText.startsWith( "]" ))
				return;

			if( AAfterFirst )
				if( ! JSONSplitIntf::SkipComma( AText ) )
				{
					AFailed = true;
					return;
				}

			InitialValue() = AText;
			if( ! JSONSplitIntf::ProcessArray( this, AText ) )
			{
				InitialValue() = "";
				AFailed = true;
				return;
			}

			InitialValue() = InitialValue().GetValue().substring( 0, InitialValue().GetValue().length() - AText.length() );
			AAfterFirst = true;
		}

	};
//---------------------------------------------------------------------------
	template <
		typename T_Enabled,
		typename T_InitialValue,
		typename T_OutputPin
	> class SplitJSONAnyArrayElement :
		public T_Enabled,
		public T_InitialValue,
		public T_OutputPin
	{
	public:
		_V_PIN_( OutputPin )

	public:
		_V_PROP_( Enabled )
		_V_PROP_( InitialValue )

	public:
		inline void ExtractElements( Mitov::String & AText, bool & AAfterFirst, bool & AFailed )
		{
			if( AFailed )
				return;

			if( ! Enabled() )
				return;

			AText.trim();
			if( AText.startsWith( "]" ))
				return;

			if( AAfterFirst )
				if( ! JSONSplitIntf::SkipComma( AText ) )
				{
					AFailed = true;
					return;
				}

//			Serial.println( AText );

			Mitov::String AValue;
			if( ! JSONSplitIntf::ExtractElement( AText, AValue ) )
			{
				AFailed = true;
				return;
			}

//			Serial.println( AValue );

			InitialValue() = AValue;
			AAfterFirst = true;
		}

		inline void SendValue()
		{
			if( Enabled() )
				T_OutputPin::SetPinValue( InitialValue() );

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