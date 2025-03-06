#include <FastLED.h>

#define LED_PIN     5
#define NUM_LEDS    14
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// Esse exemplo mostra várias maneiras de configurar e usar “paletas” de cores
// com o FastLED.
//
// Essas paletas compactas fornecem uma maneira fácil de recolorir sua
// animação em tempo real, de forma rápida, fácil e com pouca sobrecarga.
//
// O USO das paletas é MUITO mais simples na prática do que na teoria, portanto, primeiro
// execute esse sketch e observe as luzes bonitas enquanto você lê o código.
// o código.  Embora esse esboço tenha oito (ou mais) esquemas de cores diferentes,
// o sketch inteiro compila em cerca de 6,5K no AVR.
//
// O FastLED fornece algumas paletas de cores pré-configuradas e torna
// extremamente fácil criar seus próprios esquemas de cores com paletas.
//
// Algumas notas sobre a “teoria e prática” mais abstrata das
// As paletas compactas do FastLED estão no final deste arquivo.


CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
    delay( 3000 ); // atraso de segurança na inicialização
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


void loop()
{
    ChangePalettePeriodically();
    
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

// Há várias paletas de cores diferentes demonstradas aqui.
//
// O FastLED fornece várias paletas “predefinidas”: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p e PartyColors_p.
//
// Além disso, você pode definir manualmente suas próprias paletas de cores ou pode escrever
// código que cria paletas de cores em tempo real.  Todas são mostradas aqui.


void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    if( lastSecond != secondHand) {
        lastSecond = secondHand;
        if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
    }
}

// Essa função preenche a paleta com cores totalmente aleatórias.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// Essa função configura uma paleta de listras pretas e brancas,
// usando código.  Como a paleta é efetivamente uma matriz de
// dezesseis cores CRGB, as várias funções fill_* podem ser usadas
// para configurá-las.
void SetupBlackAndWhiteStripedPalette()
{
    // “apagou” todas as 16 entradas da paleta...
    fill_solid( currentPalette, 16, CRGB::Black);
    // e definir cada quarto como branco.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
}

// Essa função configura uma paleta de listras roxas e verdes.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}

// Este exemplo mostra como configurar uma paleta de cores estática
// estática que é armazenada no PROGMEM (flash), que quase sempre é mais
// mais abundante do que a RAM.  Uma paleta PROGMEM estática como essa
// ocupa 64 bytes de flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'branco' é muito brilhante em comparação com vermelho e azul
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};

// Observações adicionais sobre as paletas compactas FastLED:
//
// Normalmente, em computação gráfica, a paleta (ou “tabela de pesquisa de cores”)
// tem 256 entradas, cada uma contendo uma cor RGB de 24 bits específica.  Você pode então
// indexar na paleta de cores usando um valor simples de 8 bits (um byte).
// Uma paleta de cores com 256 entradas ocupa 768 bytes de RAM, o que no Arduino
// possivelmente “muitos” bytes.
//
// O FastLED oferece paletas tradicionais de 256 elementos, para configurações que
// podem arcar com o custo de 768 bytes de RAM.
//
// Entretanto, o FastLED também oferece uma alternativa compacta.  O FastLED oferece
// paletas que armazenam 16 entradas distintas, mas podem ser acessadas COMO SE
// na verdade, elas têm 256 entradas; isso é feito interpolando
// entre as 16 entradas explícitas para criar quinze entradas de paleta intermediárias
// intermediárias entre cada par.
//
// Por exemplo, se você definir as duas primeiras entradas explícitas de uma paleta compacta 
// paleta compacta para Green (0,255,0) e Blue (0,0,255) e, em seguida, recuperou 
// as primeiras dezesseis entradas da paleta virtual (de 256), você obteria
// Verde, seguido por um gradiente suave de verde para azul e, em seguida, Azul.
