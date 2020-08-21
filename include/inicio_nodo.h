#ifndef _INICIO_
#define _INICIO_

int ESTE_NODO = -1;

#define NUM_MAXIMO_DE_FALLAS 10   // Número maximo de fallas al querer enviar un mensaje
                                  // Si se supera este valor, la red RF se reinicia.


#include "serial_printf.h"

//--------------------------------------------------------------------------
// DEBUG
//--------------------------------------------------------------------------
#if DEBUG == 0
  #define DEBUG0
#elif DEBUG == 1
  #define DEBUG0
  #define DEBUG1
#elif DEBUG == 2
  #define DEBUG0
  #define DEBUG1
  #define DEBUG2
#elif DEBUG == 3
  #define DEBUG3
#endif

#define PUERTO_DEBUG    Serial
#define BAUDRATE_DEBUG  9600

#ifdef DEBUG0
      #define INICIA_DEBUG    PUERTO_DEBUG.begin( BAUDRATE_DEBUG );  
      //printf_begin();
      //#define DEBUG(...)      serial_printf( PUERTO_DEBUG , __VA_ARGS__ );
#else
      #define INICIA_DEBUG
      //#define DEBUG(...)
#endif

//--------------------------------------------------------------------------
// WATCHDOG TIMER
//--------------------------------------------------------------------------
#include <avr/wdt.h>

//#define WDTR wdt_reset();

#if WATCHDOG == true
  #define WDTR wdt_reset();
  #define WDT_PERIODO WDTO_8S
    // WDTO_15MS,    WDTO_30MS,    WDTO_60MS,    WDTO_120MS,    WDTO_250MS,    WDTO_500MS,    WDTO_1S
    // WDTO_2S,    WDTO_4S,    WDTO_8S
#else
  #define WDTR
#endif

#include "LED.h"
#include "codigos_pumas.h"
#include <TimerOne.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <EEPROM.h>

//--------------------------------------------------------------------------
// VARIABLES EEPROM
//--------------------------------------------------------------------------
int eeprom_este_nodo   = 5;
int eeprom_address     = 0;
int eeprom_address_2   = 10;


//--------------------------------------------------------------------------
// VARIABLES GLOBAES
//--------------------------------------------------------------------------

volatile int            segundos              = -1    ,
                        minutos               = -1    ,
                        horas                 = -1    ,
                        dias                  = -1    ;
volatile unsigned long  previousMillis_ENVIO  = 0     ,
                        previousMillis_BLINK  = 0     ,
                        currentMillis         = 0     ;
volatile unsigned long  intervalo_ENVIO       = 3000  ,         //15 * 1000 ,  //  en ms
                        intervalo_BLINK       = 5000  ;         //  en ms
volatile bool           FLAG_ENVIO            = false ,
                        FLAG_BLINK            = false ,
                        FLAG_DIA              = false ,
                        FLAG_VIBRACION        = false ,         // DIA = true NOCHE = false
                        FLAG_PIR_IZQUIERDO    = false ,
                        FLAG_PIR_DERECHO      = false ,
                        FLAG_BATERIA_BAJA     = false ,
                        FLAG_NMAX_INTENTOS_DE_CONEXION  = false ;
volatile unsigned int   vBAT                  = 0     ,
                        vIN                   = 0     ,
                        vIN_umbral            = 500   ,
                        PIR1                  = LOW   ,
                        PIR2                  = LOW   ,
                        contador              = 0     ;
unsigned int            MODO                  = M_APAGADO,
                        MODO_ANTERIOR         = M_APAGADO;
bool                    estado_conectado      = false ;
unsigned long           ultimo_contacto       = 0     ,
                        ultimo_envio_del_master = 0   ;

RF24NetworkHeader       header;

String                  DBG_dia         = "" ;
String                  DBG_aux         = "" ;
String                  entrada_serial  = "" ;

RF24        radio(pinCE,pinCSN);      // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio

Mensaje mensaje_a_enviar    = {0,0,0,0,0,0,0,0};
Mensaje mensaje_recibido    = {0,0,0,0,0,0,0,0};

void ISR_RF24(void);
void ISR_vibracion(void);
void reloj(void);

void inicio(void){
 
  #if RESET_NODO
    EEPROM.write(eeprom_address, ESTE_NODO_);
  #endif
  
  ESTE_NODO = EEPROM.read(eeprom_address);

  #if WATCHDOG == true

    wdt_disable();

    wdt_enable(WDT_PERIODO);  // WDTO_15MS,    WDTO_30MS,    WDTO_60MS,    WDTO_120MS,    WDTO_250MS,    WDTO_500MS,    WDTO_1S
                              // WDTO_2S,    WDTO_4S,    WDTO_8S
    WDTR

  #endif

  INICIA_DEBUG
 
  Serial.print(" NODO ");
  Serial.println(ESTE_NODO);
  
  SPI.begin();
  radio.begin();
  network.begin(CANAL,ESTE_NODO);
  
  pinMode(pinIRQ , INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinIRQ), ISR_RF24     , RISING );//FALLING) ;

  pinMode(pinVIB , INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), ISR_vibracion, CHANGE) ;

  Timer1.initialize(1000000)    ;  // 1 segundo
  Timer1.attachInterrupt(reloj) ;

  pinMode(pinPIR1, INPUT_PULLUP);
  pinMode(pinPIR2, INPUT_PULLUP);

}


void ISR_RF24(void){
 //  DEBUG("\n\r. interrupt ISR_RF24 %i.\n\r", numero++);
  noInterrupts();                               // DES-habilita las interrupciones
  EIFR = (0 << INTF0);                          // use before attachInterrupt(0,isr,xxxx) to clear interrupt 0 flag
  EIFR = (0 << INTF1);                          // use before attachInterrupt(1,isr,xxxx) to clear interrupt 1 flag
  interrupts();                                 // habilita las interrupciones*/
}




void ISR_vibracion(void){
  
  noInterrupts();                              // DES-habilita las interrupciones
  
  FLAG_VIBRACION = true;

  EIFR = (1 << INTF0);                          // use before attachInterrupt(0,isr,xxxx) to clear interrupt 0 flag
  EIFR = (1 << INTF1);                          // use before attachInterrupt(1,isr,xxxx) to clear interrupt 1 flag
  
  interrupts();                                 // habilita las interrupciones
  
}



void reloj(void) {

  segundos = (segundos + 1) % 60; // tiempo en segundos

  minutos   = ( segundos == 0)                                      ? (minutos + 1) % 60  :   minutos   ;
  horas     = ((segundos == 0) && (minutos == 0))                   ? (horas   + 1) % 24  :   horas     ;
  dias      = ((segundos == 0) && (minutos == 0) && (horas == 0) )  ?  dias    + 1        :   dias      ;

  currentMillis =millis();

  if( (currentMillis - previousMillis_ENVIO ) >= intervalo_ENVIO ) {
    FLAG_ENVIO = true;
    previousMillis_ENVIO = currentMillis; 
  }

  if( (currentMillis - previousMillis_BLINK ) >= intervalo_BLINK ) {
    FLAG_BLINK = true;
    previousMillis_BLINK  = currentMillis; 
  }
     
}


bool envio_rf(struct Mensaje *mensaje){
  
  static unsigned int numero_de_fallas = 0;

  //RF24NetworkHeader header(mensaje->nodo);
  header.to_node = mensaje->nodo;
  header.type = mensaje->tipo;
  mensaje->contador++         ;

  bool ok = network.write(header, mensaje, sizeof(*mensaje));

  if (ok){
    
    Serial.print(F(" OK >>"));
    ultimo_contacto   = millis(); // actualiza el tiempo del ultimo contacto.
    estado_conectado  =  true;
    numero_de_fallas = 0;   // resetea el numero de fallas
  }
  else {
    
    Serial.print(F(" NOK >>"));
    
    numero_de_fallas++;
    
    #ifdef DEBUG1
      Serial.print(F(" {falla # "));
      Serial.print(numero_de_fallas);
      Serial.print(F("} "));
    #endif

    if (numero_de_fallas == NUM_MAXIMO_DE_FALLAS){

      FLAG_NMAX_INTENTOS_DE_CONEXION = true;
     
      numero_de_fallas = 0;
    }
  }
{ // muestra el mensaje que intenta enviar
      Serial.print(F(" Tipo ("));
      Serial.print((char)mensaje->tipo);
      Serial.print(F(") Numero ("));
      Serial.print(mensaje->contador);
      Serial.print(F(") Origen ("));
      Serial.print(ESTE_NODO,HEX);
      Serial.print(F(") Destino ("));
      Serial.print(mensaje->nodo,HEX);
      Serial.print(F(") vBat ("));
      Serial.print(mensaje->vbat);
      Serial.print(F(") vIn ("));
      Serial.print(mensaje->vin);
      Serial.print(F(") modo ("));
      Serial.print(codigo[mensaje->modo]);
      Serial.print(F(") P1 ("));
      Serial.print(mensaje->parametro1);
      Serial.print(F(") P2 ("));
      Serial.print(mensaje->parametro2);
      Serial.print(F(")\n\r"));  
    }
  return ok;  // devuelve el resultado del write
}
 

void modo_alarma_eco(void){

  static int ciclo = -1 ;
  Serial.print(F(">> Modo Alarma ECO <<\n"));
  
  ciclo++;
  ciclo= ciclo%5;  // para que ciclo quede confinado entre 0 y 4

  if (ciclo  == 0 ) LED_TAD01();
  if (ciclo  == 1 ) LED_TAD02();
  if (ciclo  == 2 ) LED_TAD03();
  if (ciclo  == 3 ) LED_TAD04();
  if (ciclo  == 4 ) LED_TAD05();

  if (millis()-ultimo_envio_del_master > 30 *1000)
  {
    MODO = MODO_ANTERIOR;
    Serial.print(F("Saliendo de Modo Alarma"));
    ciclo = 0;
  }

}




void modo_desconectado(void){
  
  Serial.print(F("modo desconectado #"));
  Serial.print (contador);
  contador++;

  if (!FLAG_DIA){ // si es de noche
  
    Serial.print(F(" NOCHE "));
    
    LED_TAD01();
    WDTR
    delay(500);
   /* 
    LED_TAD02();
    WDTR
    delay(500);
    
    LED_TAD03();
    WDTR
    delay(500);
    
    LED_TAD04();
    WDTR
    delay(500);
    
    LED_TAD05();
    WDTR
   */
  }
  else{   // si es de dia no hace nada
    Serial.print(F(" DIA "));
  }

}


void modo_random (int _repeticiones_, int _velocidad_,int _espera_){
  
  long _velocidad_map=map(_velocidad_,100,0,100,1000);

  if (!FLAG_DIA){
  
    for (int i = _repeticiones_; i >= 0; i--) {
      LED_random();
      delay(_velocidad_map);
      WDTR

      LED_apagado();
      delay(50);
      WDTR
    }
  }
  delay(_espera_*1000); // en segundos
  
}

void modo_test_distancia (void){

  mensaje_a_enviar.modo = M_TEST_DISTANCIA;
  mensaje_a_enviar.nodo = NODO_MASTER;
  mensaje_a_enviar.tipo = T_PING;
  mensaje_a_enviar.contador++;

  if (envio_rf(&mensaje_a_enviar)){
    LED_blink(AZUL);
    delay(200);
  
    LED_blink(AZUL);
    delay(500);
  }
  else {
    LED_blink(ROJO);
    delay(200);
  
    LED_blink(ROJO);
    delay(500);
  }
}

void modo_eeprom(){
  EEPROM.write(eeprom_address, mensaje_recibido.parametro1);
  //EEPROM.put(eeprom_address_2,parametros);
  Serial.print(F("\n\rrenombrando nodo >>"));
  Serial.print(mensaje_recibido.parametro1);
  Serial.print(F("\n\r"));
  ESTE_NODO = mensaje_recibido.parametro1;

  network.begin(CANAL,ESTE_NODO);

  MODO = MODO_ANTERIOR;
}

void modo_reset(){
  EEPROM.write(eeprom_address, 5);
}


void modo_test (void){

  mensaje_a_enviar.modo = M_TEST;
  mensaje_a_enviar.nodo = NODO_MASTER;
  mensaje_a_enviar.tipo = T_TEST;
  mensaje_a_enviar.contador++;
  
  switch (mensaje_recibido.parametro1){
  
   case 0 :
      mensaje_a_enviar.parametro1 = 0;
      mensaje_a_enviar.parametro2 = mensaje_recibido. parametro2;
      Serial.println(F("\n\rTest 0")); // solo contesta lo mismo que se le envió para testear la comunicación
      envio_rf(&mensaje_a_enviar);
    break;
   
   case 1 :
      mensaje_a_enviar.parametro1 = 1;
      mensaje_a_enviar.parametro2 = mensaje_recibido. parametro2;
      envio_rf(&mensaje_a_enviar);
      FLAG_VIBRACION = true ;   //para simular vibracion del nodo
      Serial.println(F("\n\rTest 1"));
    break;
   
   case 2 :
      mensaje_a_enviar.parametro1 = 2;
      mensaje_a_enviar.parametro2 = mensaje_recibido. parametro2;
      envio_rf(&mensaje_a_enviar);
      FLAG_PIR_DERECHO = true ;   //para simular la accion del PIR derecho
      Serial.println(F("\n\rTest 2"));
    break;
   
   case 3 :
      mensaje_a_enviar.parametro1 = 3;
      mensaje_a_enviar.parametro2 = mensaje_recibido. parametro2;
      envio_rf(&mensaje_a_enviar);
      FLAG_PIR_IZQUIERDO = true ;   //para simular la accion del PIR derecho
      Serial.println(F("\n\rTest 3"));
    break;
   
   case 4 :
      mensaje_a_enviar.parametro1 = 4;
      mensaje_a_enviar.parametro2 = mensaje_recibido. parametro2;
      envio_rf(&mensaje_a_enviar);
      LED_blink(AZUL);
      Serial.println(F("\n\rTest 4"));
    break;
   
   default:
     Serial.println(F("\n\rTest no reconocido"));
   break;

  }

}




#endif
