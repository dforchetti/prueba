#ifndef _INICIO_
#define _INICIO_

#include "stdio.h "
#include "codigos_pumas.h"  // carpeta pumas misdocumentos/Arduino
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <TimerOne.h>
//#include <LED.h>

#include "U8glib.h"
// configuracion para display I2c
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 

Mensaje mensaje_enviar, mensaje_recibido, mensaje_bt;

RF24NetworkHeader header;        // If so, grab it and print it out

int    indice          = 0, 
       intNodoDestino  = 5;
bool   Flag_Nodo       = false;    //indice 
String SNodoDestino    = "";

unsigned int  MODO_GENERAL_NODOS = M_APAGADO;


void envio_rf(struct Mensaje *mensaje);
void envio_rf_multicast(struct Mensaje *mensaje, int level);
void print_mensaje(Mensaje *mensaje);
void doEncodeA(void);
void doEncodeB(void);
void draw(void);
bool actualizaLCD(void);



//--------------------------------------------------------------------------
// WATCHDOG TIMER
//--------------------------------------------------------------------------
#include <avr/wdt.h>

//#define WDTR wdt_reset();
#define WATCHDOG false

#if WATCHDOG == true
  #define WDTR wdt_reset();
  #define WDT_PERIODO WDTO_8S
    // WDTO_15MS,    WDTO_30MS,    WDTO_60MS,    WDTO_120MS,    WDTO_250MS,    WDTO_500MS,    WDTO_1S
    // WDTO_2S,    WDTO_4S,    WDTO_8S
#else
  #define WDTR
#endif



//--------------------------------------------------------------------------
// VARIABLES GOLBALES
//--------------------------------------------------------------------------

volatile int            segundos              = -1    ,
                        minutos               = -1    ,
                        horas                 = -1    ,
                        dias                  = -1    ;
volatile unsigned long  previousMillis_ENVIO  = 0     ,
                        previousMillis_BLINK  = 0     ,
                        currentMillis         = 0     ,
                        segundos_totales      = 0     ;
volatile unsigned long  intervalo_ENVIO       = 3000  ,         //15 * 1000 ,  //  en ms
                        intervalo_BLINK       = 5000  ;         //  en ms
volatile bool           FLAG_ENVIO            = false ,
                        FLAG_BLINK            = false ,
                        FLAG_ALARMA           = false ;
volatile unsigned int   contador              = 0     ;
unsigned int            MODO                  = M_APAGADO;

bool FLAG_DATO_NUEVO =false;

int CONTADOR=0;


String  ENTRADA  ,
        MSG_aux  , 
        MSG      ;



void reloj(void);

//#define BT_

#ifdef BT_
      #include <SoftwareSerial.h>
      
      SoftwareSerial mySerial(pinBT_RX,pinBT_TX); // RX, TX van cruzadas con las del HC-06/06
      #define PUERTO_BT    mySerial
      #define BAUDRATE_BT  9600
      
      
      #define INICIA_BT    PUERTO_BT.begin( BAUDRATE_BT );  \
                           pinMode(pinEN    , OUTPUT);      \
                           pinMode(pinVCC   , OUTPUT);      \
                           pinMode(pinSTATE , INPUT );      \
                           digitalWrite(pinEN   , LOW);     \
                           digitalWrite(pinSTATE, LOW);     \
                           digitalWrite(pinVCC, HIGH);    //Enciende el modulo
                           
      #define BT(...)      mySerial.print(__VA_ARGS__ );
#else
      #define INICIA_BT
      #define BT(...)
#endif


#define NMAX 10

RF24 radio(pinCE,pinCSN);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long now = millis();              // If it's time to send a message, send it!

unsigned int numero_de_fallas=10;

unsigned long MAC=0;

// variables necesarias para el MENU grafico
#define outputA 3
#define outputB 2
#define boton A1
const unsigned int timeThreshold = 5;
unsigned long timeCounter = 0;
volatile int ISRCounter = 0;
bool IsCW = true;
const int maxSteps = 255;
int     cont_menu      = 0   ,
        cont2     = 0   ,
        delta_menu     = 15  ,
        offset_menu    = 6   ,
        NOpciones = 3   ,
        indice_menu    = 0   ,
        foco      = 0   ;



unsigned int N_items  = 4 ,
             item     = 0,
             modo_1   = 0;

        

                
int campo1[] = {1,2,3,4,5,6};

int campo2[] = {10,20,30,40,50,60};


String  MSG_menu[10]={ "TAD01 ",
                "TAD02 ",
                "TAD03 ",
                "TAD04 ",
                "TAD05 ",
                "RAND  ",
                "FFFF"};
char strBuf[30];





void inicio(void)
{
  
  INICIA_BT
  Serial.begin(9600);
  Serial.println(F("NODO00 (MASTER)"));
  
  SPI.begin();
  radio.begin();
  network.begin(CANAL,ESTE_NODO);

  Timer1.initialize(1000000)    ;  // 1 segundo
  Timer1.attachInterrupt(reloj) ;

  //#ifdef PROGRAMACIÓN
      // genera MAC Aleatoria
      randomSeed(analogRead(0));
      MAC = random(2147483647);
  //#endif

    pinMode (outputA,INPUT);
    pinMode (outputB,INPUT);
    pinMode (boton, INPUT_PULLUP);
  //  aLastState = digitalRead(outputA);   //Leemos el valor incial
  
  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);
  for (NOpciones=0 ; MSG_menu[NOpciones].indexOf("FFFF")==-1 ; NOpciones++);
  
  //u8g.setRot180();
  
  NOpciones =6;

  indice_menu=NOpciones-1;
  Serial.print("numero de opciones :");
  Serial.println(NOpciones);
   Serial.print("indice_menu :");
  Serial.println(indice_menu);


  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
   attachInterrupt(digitalPinToInterrupt(outputA), doEncodeA, CHANGE);
   attachInterrupt(digitalPinToInterrupt(outputB), doEncodeB, CHANGE);

}

void reloj(void) {

  segundos_totales++;

  segundos = (segundos + 1) % 60; // tiempo en segundos

  minutos   = ( segundos == 0)                                      ? (minutos + 1) % 60  :   minutos   ;
  horas     = ((segundos == 0) && (minutos == 0))                   ? (horas   + 1) % 24  :   horas     ;
  dias      = ((segundos == 0) && (minutos == 0) && (horas == 0) )  ?  dias    + 1        :   dias      ;


  currentMillis =millis();

  if((currentMillis - previousMillis_ENVIO )>= intervalo_ENVIO) {
    FLAG_ENVIO = true;
    previousMillis_ENVIO = currentMillis; 
    }

    if((currentMillis - previousMillis_BLINK )>= intervalo_BLINK) {
    FLAG_BLINK = true;
    previousMillis_BLINK  = currentMillis; 
    }
     
 }


void envio_rf(struct Mensaje *mensaje){

  //RF24NetworkHeader header(mensaje->nodo);
  header.to_node = mensaje->nodo;
  header.type = mensaje->tipo;
  mensaje->contador++         ;
/*
  Serial.print(F("\n\renvio al Nodo ("));
  Serial.print(mensaje->nodo);
  Serial.print(F(") Modo("));
  Serial.print(mensaje->modo);
  Serial.print(F(") P1("));
  Serial.print(mensaje->parametro1);
  Serial.print(F(") P2("));
  Serial.print(mensaje->parametro2);
  Serial.println(F(")"));
*/
  print_mensaje(mensaje);
  
  header.type = mensaje->tipo;

  bool ok = network.write(header, mensaje, sizeof(*mensaje));

  if (ok) {

    Serial.println(F(" OK "));

    numero_de_fallas = 0;
  }
  else {

    Serial.println(F(" NOK "));

    numero_de_fallas++;

    Serial.println(numero_de_fallas);

    if (numero_de_fallas == NMAX) {
      radio.begin();
      network.begin( CANAL,  ESTE_NODO);
      Serial.println(F("\n\rreiniciando."));
      numero_de_fallas = 0;
    }
  }
}


void envio_rf_multicast(struct Mensaje *mensaje, int level){

  RF24NetworkHeader header(mensaje->nodo);

  header.type = mensaje->tipo;

  bool ok = network.multicast(header, mensaje, sizeof(*mensaje),level);
  

  if (ok) {

    Serial.println(F("ok"));

    numero_de_fallas = 0;
  }
  else {

     numero_de_fallas++;

    Serial.print(F("\n\rfalla :"));
    Serial.println(numero_de_fallas);

    if (numero_de_fallas == NMAX) {
      radio.begin();
      network.begin( CANAL,  ESTE_NODO);
      Serial.println(F("\n\rreiniciando"));
      numero_de_fallas = 0;
    }
  }
}

void print_mensaje(Mensaje *mensaje){
  { // muestra el mensaje que intenta enviar
      Serial.print(F(" [salida] "));
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
}


void doEncodeA(void)
{
   if (millis() > timeCounter + timeThreshold)
   {
      if (digitalRead(outputA) == digitalRead(outputB))
      {
         IsCW = true;
         if (ISRCounter + 1 <= maxSteps) ISRCounter++;
      }
      else
      {
         IsCW = false;
         if (ISRCounter - 1 > 0) ISRCounter--;
      }
      timeCounter = millis();
   }
}
 
void doEncodeB(void)
{
   if (millis() > timeCounter + timeThreshold)
   {
      if (digitalRead(outputA) != digitalRead(outputB))
      {
         IsCW = true;
         if (ISRCounter + 1 <= maxSteps) ISRCounter++;
      }
      else
      {
         IsCW = false;
         if (ISRCounter - 1 > 0) ISRCounter--;
      }
      timeCounter = millis();
   }
}

void draw(void) {
  int i=0;

  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);

  i=(indice_menu-1)%NOpciones;
  
  if(i==-1) i=NOpciones-1;

  sprintf(strBuf,"%s %02d%%  %02d%%",MSG_menu[i].c_str(),campo1[i],campo2[i]);
  u8g.drawStr( 2,cont_menu + offset_menu            , strBuf);
  
  i=(indice_menu)%NOpciones;
  if(i==-1) i=NOpciones-1;
  sprintf(strBuf,"%s %02d%%  %02d%%",MSG_menu[i].c_str(),campo1[i],campo2[i]);
  //strBuf="hola";
  u8g.drawStr( 2,cont_menu + offset_menu + delta_menu    , strBuf);
  
  i=(indice_menu+1)%NOpciones;
  if(i==-1) i=NOpciones-1;
  sprintf(strBuf,"%s %02d%%  %02d%%",MSG_menu[i].c_str(),campo1[i],campo2[i]);
  u8g.drawStr( 2,cont_menu + offset_menu + 2*delta_menu  , strBuf);

  
  //u8g.drawRFrame(0,8,128,delta+2,2);  // toda la linea
  //u8g.drawRFrame(0,8,46,delta+2,2);   // primer campo
 // u8g.drawRFrame(46,8,42,delta+2,2);    // Segundo campo
  //  u8g.drawRFrame(87,8,40,delta+2,2);    // tercer campo
  if(foco==0) u8g.drawRFrame(0 ,8,46,delta_menu +2,2);   // primer campo
  if(foco==1) u8g.drawRFrame(46,8,42,delta_menu +2,2);    // Segundo campo
  if(foco==2) u8g.drawRFrame(87,8,40,delta_menu +2,2);    // tercer campo
  
}


bool actualizaLCD(void){
  static int ISRCounter_anterior  = false;
  bool       enviar_mensaje       = false;

  if ( ISRCounter_anterior != ISRCounter ){

    ISRCounter_anterior=ISRCounter;
    enviar_mensaje=true;

    if (foco==0) indice_menu = (ISRCounter/4)%(NOpciones);

    if (foco==1) {
      if (ISRCounter>=99)  ISRCounter=99;
      if (ISRCounter<=0)    ISRCounter=0;
      campo1[indice_menu] = ISRCounter;
    }

    if (foco==2) {
      if (ISRCounter>=99)  ISRCounter=99;
      if (ISRCounter<=0)    ISRCounter=0;
      campo2[indice_menu] = ISRCounter;
    }
  }
      
  //Serial.println(indice_menu);   
  bool B = digitalRead(boton);


  if ( !B ){
    //Serial.println("Boton pulsado: Contador a 0");
    enviar_mensaje=true;

    foco=(foco+1)%3;
    if (foco==0) ISRCounter = indice_menu*4;
    if (foco==1) ISRCounter = campo1[indice_menu];
    if (foco==2) ISRCounter = campo2[indice_menu];
    delay(200);
  }

  u8g.firstPage();  

  do {
    draw();
  } while( u8g.nextPage() );

  return enviar_mensaje;
}


#endif
