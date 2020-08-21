
#ifndef _LED_
#define _LED_

#include "Arduino.h"
#include "FastLED.h"


CRGB leds[NUM_LEDS];

#define ROJO      0
#define AZUL      160
#define VERDE     96
#define AMARILLO  46


/*FUNCIONES RECONOCIDAS*/

void LED_inicia(void);
void LED_apagado(void);
void LED_encendido(int color, int saturacion);
void LED_esperando(unsigned int color, unsigned int saturacion, unsigned int intensidad);
void LED_voltaje(int voltaje);
void LED_numero(int numero);
void LED_random(void);
void LED_arranque(void);
void LED_blink (void);
void LED_TAD01(void);			// estos son esquemas de encendido y apagado que pidió la gente de TAD
void LED_TAD02(void);			// estos son esquemas de encendido y apagado que pidió la gente de TAD
void LED_TAD03(void);			// estos son esquemas de encendido y apagado que pidió la gente de TAD
void LED_TAD04(void);			// estos son esquemas de encendido y apagado que pidió la gente de TAD
void LED_TAD05(void);			// estos son esquemas de encendido y apagado que pidió la gente de TAD


void LED_random(void){

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(random(1,213) , 255, random(1,255));
  }
  FastLED.show();
}



/*FUNCIIONEs*/

void LED_inicia(void) {

  FastLED.addLeds<NEOPIXEL, pinLED>(leds, NUM_LEDS);
}

void LED_arranque(void){

	for (int j = 0;  j < NUM_LEDS  ; j++)
	{
		LED_apagado();

    leds[j    % NUM_LEDS] 		= CHSV(   0 , 255 , 255 * 0.1); //CRGB::White;CHSV( 160, 255, 255);
    leds[(j + 1) % NUM_LEDS] 	= CHSV(   0 , 255 , 255 * 0.3); //CRGB::White;CHSV( 160, 255, 255);
    leds[(j + 2) % NUM_LEDS] 	= CHSV(   0 , 255 , 255 * 1.0); //CRGB::White;CHSV( 160, 255, 255);
    
    leds[(j+4)    % NUM_LEDS] 	= CHSV( 160 , 255 , 255 * 0.1); //CRGB::White;CHSV( 160, 255, 255);
    leds[(j+4 + 1) % NUM_LEDS] 	= CHSV( 160 , 255 , 255 * 0.3); //CRGB::White;CHSV( 160, 255, 255);
    leds[(j+4 + 2) % NUM_LEDS] 	= CHSV( 160 , 255 , 255 * 1.0); //CRGB::White;CHSV( 160, 255, 255);

    FastLED.show();

    delay(150);
	}

}

void LED_blink (unsigned int COLOR){
  	LED_apagado();
    leds[0] = CHSV(COLOR  , 255, 255);
    FastLED.show();
    delay(200);
    LED_apagado();

}


void LED_numero (int numero)
{

  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::Black;

  if (numero == 1) leds[0] = CHSV(0  , 255, 255);

  if (numero == 2) {
    leds[0] = CHSV(0  , 255, 255);
    leds[4] = CHSV(0  , 255, 255);
  }
  if (numero == 3) {
    leds[0] = CHSV(0  , 255, 255);
    leds[3] = CHSV(0  , 255, 255);
    leds[5] = CHSV(0  , 255, 255);
  }
  if (numero == 4) {
    leds[0] = CHSV(0  , 255, 255);
    leds[2] = CHSV(0  , 255, 255);
    leds[4] = CHSV(0  , 255, 255);
    leds[6] = CHSV(0  , 255, 255);
  }
  if (numero == 5) {
    leds[0] = CHSV(0  , 255, 255);
    leds[2] = CHSV(0  , 255, 255);
    leds[3] = CHSV(0  , 255, 255);
    leds[5] = CHSV(0  , 255, 255);
    leds[6] = CHSV(0  , 255, 255);
  }
  if (numero == 6) {
    for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(0  , 255, 255);
    leds[0] = CRGB::Black;
    leds[4] = CRGB::Black;

  }
  if (numero == 7) {
    for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(0  , 255, 255);
    leds[0] = CRGB::Black;
  }

  if (numero == 8) {
    for (int i = 0; i < NUM_LEDS; i++) leds[i] = CHSV(0  , 255, 255);

  }
  FastLED.show();

}

void LED_voltaje(int voltaje) {

  for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::Black;

  if (voltaje > 12.5) leds[0] = CHSV(0  , 255, 255); // (1/8) 12,5%
  if (voltaje > 25  ) leds[1] = CHSV(0  , 255, 255); // (2/8) 25,0%
  if (voltaje > 37.5) leds[2] = CHSV(0  , 255, 255); // (3/8) 37,5%
  if (voltaje > 50  ) leds[3] = CHSV(0  , 255, 255); // (4/8) 50,0%
  if (voltaje > 62.5) leds[4] = CHSV(42 , 255, 255); // (5/8) 62,5%
  if (voltaje > 75  ) leds[5] = CHSV(85 , 255, 255); // (6/8) 75,0%
  if (voltaje > 87.5) leds[6] = CHSV(85 , 255, 255); // (7/8) 87,5%
  if (voltaje > 90  ) leds[7] = CHSV(160, 255, 255); // (8/8) 100.0%
  FastLED.show();

}



void LED_apagado(void)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}



void LED_esperando(unsigned int color, unsigned int saturacion, unsigned int intensidad)
{
Serial.print(F("esperando"));
  for (int j = 0;  j < NUM_LEDS  ; j++)
  {
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i] = CRGB::Black;
    }

    leds[j    % NUM_LEDS] = CHSV( color , saturacion , intensidad * 0.1); //CRGB::White;CHSV( 160, 255, 255);
    leds[(j + 1) % NUM_LEDS] = CHSV( color , saturacion , intensidad * 0.3); //CRGB::White;CHSV( 160, 255, 255);
    leds[(j + 2) % NUM_LEDS] = CHSV( color , saturacion , intensidad    ); //CRGB::White;CHSV( 160, 255, 255);

    FastLED.show();

    delay(100);
  }

}


void LED_encendido(int color, int saturacion)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(color, saturacion, 255); //CRGB::White;CHSV( 160, 255, 255);
    //  0   : RED
    //  42  : YELLOW
    //  85  : GREEN
    //  128 : AQUA
    //  160 : BLUE
  }
  FastLED.show();
}



void LED_TAD01(void) {
  int tiempo = 1000;
  //  0   : RED
  //  42  : YELLOW
  //  85  : GREEN
  //  128 : AQUA
  //  160 : BLUE

  Serial.print(F("\n\r[LED_TAD01]\n\r"));

  LED_encendido(0, 0);
  delay(tiempo);
  WDTR

  LED_apagado();
  delay(tiempo);
  WDTR

  LED_encendido(160, 255);
  delay(tiempo);
  WDTR

  LED_apagado();
  delay(tiempo);
  WDTR

  LED_encendido(85, 255);
  delay(tiempo);
  WDTR

  LED_apagado();
  delay(tiempo);
  WDTR

}

void LED_TAD01_V(int _velocidad_,int _espera_) {
  long _velocidad_map=map(_velocidad_,100,0,100,1000);
  //  0   : RED
  //  42  : YELLOW
  //  85  : GREEN
  //  128 : AQUA
  //  160 : BLUE

  Serial.print(F("\n\r[LED_TAD01]\n\r"));

  LED_encendido(0, 0);
  delay(_velocidad_map);
  WDTR

  LED_apagado();
  delay(_velocidad_map);
  WDTR

  LED_encendido(160, 255);
  delay(_velocidad_map);
  WDTR

  LED_apagado();
  delay(_velocidad_map);
  WDTR

  LED_encendido(85, 255);
  delay(_velocidad_map);
  WDTR

  LED_apagado();
  delay(_velocidad_map);
  WDTR

  delay(_espera_*1000); // en segundos

}


void LED_TAD02(void) {
  int tiempo = 1000;
  int i = 0;
  int destellos = 10;
  //  0   : RED
  //  42  : YELLOW
  //  85  : GREEN
  //  128 : AQUA
  //  160 : BLUE

  Serial.print(F("\n\r[LED_TAD02]\n\r"));

  for (i = 0 ; i <= destellos; i++) {
    LED_encendido(0, 0);
    WDTR
    delay(tiempo / 2 / destellos);
    LED_apagado();
    WDTR
    delay(tiempo / 2 / destellos);
  }

  delay(tiempo);

  for (i = 0 ; i <= destellos; i++) {
    LED_encendido(160, 255);
    delay(tiempo / 2 / destellos);
    WDTR
    LED_apagado();
    delay(tiempo / 2 / destellos);
    WDTR
  }

  delay(tiempo);

  for (i = 0 ; i <= destellos; i++) {
    LED_encendido(85, 255);
    delay(tiempo / 2 / destellos);
    WDTR
    LED_apagado();
    delay(tiempo / 2 / destellos);
    WDTR
  }
  delay(tiempo);

}


void LED_TAD02_V(int _velocidad_,int _espera_)  {

  long _velocidad_map_=map(_velocidad_,0,100,10,100);
  long _espera_map_=map(_espera_,0,100,500,5000);

  int tiempo = 1000 * _velocidad_map_ /100;
  int i = 0;
  int destellos = 10;
  //  0   : RED
  //  42  : YELLOW
  //  85  : GREEN
  //  128 : AQUA
  //  160 : BLUE

  Serial.print(F("\n\r[LED_TAD02]\n\r"));

  for (i = 0 ; i <= destellos; i++) {
    LED_encendido(0, 0);
    WDTR
    delay(tiempo / 2 / destellos);
    LED_apagado();
    WDTR
    delay(tiempo / 2 / destellos);
  }

  delay( _espera_map_);

  for (i = 0 ; i <= destellos; i++) {
    LED_encendido(160, 255);
    delay(tiempo / 2 / destellos);
    WDTR
    LED_apagado();
    delay(tiempo / 2 / destellos);
    WDTR
  }

  delay( _espera_map_);

  for (i = 0 ; i <= destellos; i++) {
    LED_encendido(85, 255);
    delay(tiempo / 2 / destellos);
    WDTR
    LED_apagado();
    delay(tiempo / 2 / destellos);
    WDTR
  }
  delay( _espera_map_);

  delay( _espera_map_);
}


void LED_TAD03(void) {
  int tiempo_on = 2000;
  int tiempo_off = 3000;
  //  0   : RED
  //  42  : YELLOW
  //  85  : GREEN
  //  128 : AQUA
  //  160 : BLUE

  Serial.print(F("\n\r[LED_TAD03]\n\r"));

  LED_encendido(85, 0);
  delay(tiempo_on);
  WDTR

  LED_apagado();
  delay(tiempo_off);
  WDTR

  LED_encendido(85, 255);
  delay(tiempo_on);
  WDTR

  LED_apagado();
  delay(tiempo_off);
  WDTR

}

void LED_TAD03_V(int _velocidad_,int _espera_)  {

  long _velocidad_map_  =map(_velocidad_,0,100,2000,300);
  long _espera_map_     =map(_espera_,0,100,100,3000);
  
  int tiempo_on = _velocidad_map_;
  int tiempo_off = _espera_map_;

  //int tiempo_on = 2000;
  //int tiempo_off = 3000;
  //  0   : RED
  //  42  : YELLOW
  //  85  : GREEN
  //  128 : AQUA
  //  160 : BLUE

  Serial.print(F("\n\r[LED_TAD03]\n\r"));

  LED_encendido(85, 0);
  delay(tiempo_on);
  WDTR

  LED_apagado();
  delay(tiempo_off);
  WDTR

  LED_encendido(85, 255);
  delay(tiempo_on);
  WDTR

  LED_apagado();
  delay(tiempo_off);
  WDTR

}


void LED_TAD04(void) {
  int tiempo_on = 2000;
  int tiempo_off = 3000;
  //  0   : RED
  //  42  : YELLOW
  //  85  : GREEN
  //  128 : AQUA
  //  160 : BLUE
  Serial.print(F("\n\r[LED_TAD04]\n\r"));
  

  LED_encendido(85, 0);
  delay(tiempo_on);
  WDTR

  LED_apagado();
  delay(tiempo_off);
  WDTR

  LED_encendido(85, 255);
  delay(tiempo_on);
  WDTR

  LED_apagado();
  delay(tiempo_off);
  WDTR

}


void LED_TAD04_V(int _velocidad_,int _espera_)  {
  long _velocidad_map_  =map(_velocidad_,0,100,2000,300);
  long _espera_map_     =map(_espera_,0,100,100,3000);
  

  int tiempo_on = _velocidad_map_;
  int tiempo_off = _espera_map_;

  //int tiempo_on = 2000;
  //int tiempo_off = 3000;
  //  0   : RED
  //  42  : YELLOW
  //  85  : GREEN
  //  128 : AQUA
  //  160 : BLUE
  Serial.print(F("\n\r[LED_TAD04]\n\r"));
  

  LED_encendido(85, 0);
  delay(tiempo_on);
  WDTR

  LED_apagado();
  delay(tiempo_off);
  WDTR

  LED_encendido(85, 255);
  delay(tiempo_on);
  WDTR

  LED_apagado();
  delay(tiempo_off);
  WDTR

}

void LED_TAD05_V(int _velocidad_,int _espera_) 
{

  long _velocidad_map_  =map(_velocidad_,0,100,2000,300);
  long _espera_map_     =map(_espera_,0,100,500,5000);
  
  int tiempo_on = _velocidad_map_ ;
  int tiempo_off = _espera_map_;

  // int tiempo_on = 2000;
  //int tiempo_off = 5000;

  Serial.print(F("\n\r[LED_TAD05]\n\r"));
  WDTR
  // secuencia blanca .....................
  LED_apagado();
  delay(tiempo_off);
  WDTR

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV( 0 , 0 , 255    );
    FastLED.show();
    delay(tiempo_on / NUM_LEDS);
    WDTR
  }

  LED_apagado();
  delay(tiempo_off);
  WDTR
  
  LED_encendido(85, 0);
  delay(tiempo_on);
  WDTR
   
  // secuencia azul .....................
  LED_apagado();
  delay(tiempo_off);
  WDTR
  
   for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV( 160 , 255 , 255    );
    FastLED.show();
    delay(tiempo_on / NUM_LEDS);
    WDTR
  }
  LED_apagado();
  delay(tiempo_off);
  WDTR
  
  LED_encendido(160, 255);
  delay(tiempo_on);
  WDTR

// secuencia rojo .....................
  LED_apagado();
  delay(tiempo_off);
  WDTR
  
   for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV( 0 , 255 , 255    );
    FastLED.show();
    delay(tiempo_on / NUM_LEDS);
    WDTR
  }
  LED_apagado();
  delay(tiempo_off);
  WDTR
  
  LED_encendido(0, 255);
  delay(tiempo_on);
  WDTR

// secuencia verde .....................
LED_apagado();
  delay(tiempo_off);
  WDTR
  
   for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV( 85 , 255 , 255    );
    FastLED.show();
    delay(tiempo_on / NUM_LEDS);
    WDTR
  }
  
  delay(tiempo_off);
  WDTR
  
  LED_encendido(85, 255);
  delay(tiempo_on);
  WDTR

}


void LED_TAD05(void)
{
  int tiempo_on = 2000;
  int tiempo_off = 5000;
  Serial.print(F("\n\r[LED_TAD05]\n\r"));
  WDTR
  // secuencia blanca .....................
  LED_apagado();
  delay(tiempo_off);
  WDTR

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV( 0 , 0 , 255    );
    FastLED.show();
    delay(tiempo_on / NUM_LEDS);
    WDTR
  }

  LED_apagado();
  delay(tiempo_off);
  WDTR
  
  LED_encendido(85, 0);
  delay(tiempo_on);
  WDTR
   
  // secuencia azul .....................
  LED_apagado();
  delay(tiempo_off);
  WDTR
  
   for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV( 160 , 255 , 255    );
    FastLED.show();
    delay(tiempo_on / NUM_LEDS);
    WDTR
  }
  LED_apagado();
  delay(tiempo_off);
  WDTR
  
  LED_encendido(160, 255);
  delay(tiempo_on);
  WDTR

// secuencia rojo .....................
  LED_apagado();
  delay(tiempo_off);
  WDTR
  
   for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV( 0 , 255 , 255    );
    FastLED.show();
    delay(tiempo_on / NUM_LEDS);
    WDTR
  }
  LED_apagado();
  delay(tiempo_off);
  WDTR
  
  LED_encendido(0, 255);
  delay(tiempo_on);
  WDTR

// secuencia verde .....................
LED_apagado();
  delay(tiempo_off);
  WDTR
  
   for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV( 85 , 255 , 255    );
    FastLED.show();
    delay(tiempo_on / NUM_LEDS);
    WDTR
  }
  
  delay(tiempo_off);
  WDTR
  
  LED_encendido(85, 255);
  delay(tiempo_on);
  WDTR

}


#endif
