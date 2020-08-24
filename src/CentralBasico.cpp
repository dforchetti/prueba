#include <Arduino.h>

int ESTE_NODO = 0;
#define N_NODOS_RED 20

#define ESTE_NODO 00
#define CANAL     90

#include "configura.h"
#include "inicio_master.h"
#include "LED.h"
 

void decide_entrada_serie_BT(void);
void decide_entrada_rf(void);
int modo(String *modo);


int MODO_MASTER = M_VIVO;
unsigned long t_ultimo_mensaje = 0;


void setup(void){
  inicio();
}


String ENTRADA1 = "hola mundo";
int cont=0;

void loop(void) {
  

 network.update();                  // Check the network regularly
 
 //------------------------------------------------------------------------------------------
 //  Recibe Mensajes RF
 //------------------------------------------------------------------------------------------
  
  if(network.available()) {     // Is there anything ready for us?

    //decide_entrada_rf();
   
  }

 //------------------------------------------------------------------------------------------
 //  Recibe Mensajes RF
 //------------------------------------------------------------------------------------------
 
  if (Serial.available()){ 

    ENTRADA = Serial.readString();
    ENTRADA.trim();               // quita espacios antes y despues del texto
    ENTRADA.toUpperCase();        // pasa too a mayúsculas
    Serial.print(F("\n\r Serial 1<<"));
    
    Serial.println(ENTRADA.c_str() );
   // decide_entrada_serie_BT();
  }


  if (FLAG_ENVIO & (MODO_MASTER == M_ALARMA_ECO)){
    
    FLAG_ENVIO = false ;

    if ((t_ultimo_mensaje - millis()) > 20 * 1000) {  // despues de 20 segundos de no recibior señal de alarma cambia el modo del nodo
      MODO_MASTER = M_VIVO;
    }
    else
    {
      Serial.print(F("ALARMA_ECO \r\n"));
      BT("ALARMA_ECO\r\n")
    
      mensaje_bt.contador++;
      mensaje_bt.tipo       = T_ALARMA;
      mensaje_bt.modo       = M_ALARMA_ECO;
      mensaje_bt.nodo       = ESTE_NODO;
      mensaje_bt.parametro1 = 0;
      mensaje_bt.parametro2 = 0;
    
 
      envio_rf_multicast(&mensaje_bt, 1);
      envio_rf_multicast(&mensaje_bt, 2);
      envio_rf_multicast(&mensaje_bt, 3);
      envio_rf_multicast(&mensaje_bt, 4);
      
      LED_esperando(ROJO, 255, 255);
    }
    
    
    
  }

  Serial.print(F("."));
  if((cont++)%20==0) Serial.println("");

  if ( actualizaLCD() ){  // si ha cambiado algo en la entrada LCD + encode rotativo
    
    if(indice_menu == 0 ) mensaje_enviar.modo = M_TAD01;
    if(indice_menu == 1 ) mensaje_enviar.modo = M_TAD02;
    if(indice_menu == 2 ) mensaje_enviar.modo = M_TAD03;
    if(indice_menu == 3 ) mensaje_enviar.modo = M_TAD04;
    if(indice_menu == 4 ) mensaje_enviar.modo = M_TAD05;
    if(indice_menu == 5 ) mensaje_enviar.modo = M_RANDOM;
    mensaje_enviar.parametro1 = campo1[indice_menu]       ;   // color
    mensaje_enviar.parametro2 = campo2[indice_menu]       ;   // intensidad
    envio_rf_multicast(&mensaje_enviar, 1) ;
 
    Serial.println("enviando multicast");
    
  };

  delay(100);


}
// FIN LOOP principal


void decide_entrada_rf (void) {

    network.read(header, &mensaje_recibido, sizeof(mensaje_recibido));

      { // muestra el mensaje leido
        Serial.print("\n\rTipo(");
        Serial.print((char)header.type);
        Serial.print(") Numero(");
        Serial.print(mensaje_recibido.contador);
        Serial.print(") Origen(");
        Serial.print(header.from_node);
        Serial.print(") vbat(");
        Serial.print(mensaje_recibido.vbat);
        Serial.print(") vin(");
        Serial.print(mensaje_recibido.vin);
        Serial.print(") modo (");
        Serial.print(codigo[mensaje_recibido.modo]);
        Serial.print(") P1 (");
        Serial.print(mensaje_recibido.parametro1);
        Serial.print(") P2 (");
        Serial.print(mensaje_recibido.parametro2);
        //Serial.print(")\033[0m\n\r");
        Serial.print(")\n\r");
     }

    if(mensaje_recibido.tipo == T_CONEXION){
      Serial.print(F("\n\r<<Solicitud de conexión>> Nodo ["));//por ahora nada
      Serial.print(header.from_node);
      Serial.print(F("]"));

      mensaje_enviar.nodo = header.from_node;
      mensaje_enviar.tipo = T_LATIDO; // para que no vuelva a pedir conexión
      mensaje_enviar.modo = MODO_GENERAL_NODOS; // es el modo al que van a parar los nodos al desconectarse
      mensaje_enviar.parametro1 = ROJO ;
      mensaje_enviar.parametro2 = 120;
      envio_rf(&mensaje_enviar);
      WDTR
    }

    if(mensaje_recibido.tipo == T_ALARMA){
      
      MODO_MASTER = M_ALARMA_ECO;

      t_ultimo_mensaje = millis();

    }      
}

void decide_entrada_serie_BT (void){
   
    int i=0;

    //-------------------------------------------------------------------------
    // NODO
    //
    // nodo -m apagado -n 1 -n 5 ... etc 
    //-------------------------------------------------------------------------
    if ( ENTRADA.indexOf("NODO") != -1) { // los espacios también importan
      
      String SubString  ="";
      int     aux       = 0;
      
      mensaje_enviar.modo       = M_DUMMY   ;   
      mensaje_enviar.tipo       = T_ENVIO   ;
      mensaje_enviar.parametro1 = ROJO      ;   // color
      mensaje_enviar.parametro2 = 255       ;   // intensidad
      
      aux = ENTRADA.lastIndexOf("-M");
      if (aux !=-1){
        SubString=ENTRADA.substring( aux , aux + 20 ); // Extrae los 20 caracteres posteriores a -M
        aux = modo(&SubString);
        if( aux!=-1 ) mensaje_enviar.modo       = aux    ;

      } 

      aux = ENTRADA.indexOf("-P1");
      if (aux !=-1){
        aux = aux + 4;
        SubString=ENTRADA.substring( aux , ENTRADA.indexOf(" ",aux) ); // Extrae los 20 caracteres posteriores a -M
        SubString.trim();
        mensaje_enviar.parametro1 = SubString.toInt();
        //Serial.print(F("P1 : "));
        //Serial.println(mensaje_enviar.parametro1);
      } 

      aux = ENTRADA.indexOf("-P2");
      if (aux !=-1){
        aux = aux + 4 ;
        SubString=ENTRADA.substring( aux , ENTRADA.indexOf(" ",aux) ); // Extrae los 20 caracteres posteriores a -M
        SubString.trim();
        mensaje_enviar.parametro2 = SubString.toInt();
        //Serial.print(F("P2 : "));
       // Serial.println(mensaje_enviar.parametro2);
      } 

      
      
      //nodo -m apagado -p1 123 -p2 234 -n 1 -n 3 -n 5
      aux = ENTRADA.indexOf("-N");
      while (aux !=-1){
        aux = aux + 3 ;
        SubString=ENTRADA.substring( aux , ENTRADA.indexOf(" ",aux ) ); // Extrae los 20 caracteres posteriores a -M
        SubString.trim();
        int indice= SubString.toInt();
        //Serial.print(F("(N "));
        //Serial.print(indice);
        //Serial.print(F(") "));
        aux = ENTRADA.indexOf( "-N", aux ); //  busca el siguiente -N
        mensaje_enviar.nodo = indice; /// ojo hay que controlar que sea numero valida
        envio_rf(&mensaje_enviar);
      }
    } 
    //-------------------------------------------------------------------------
    // MULTICAST
    //
    // multicast -m apagado -n1 -n5 ... -todos etc 
    //-------------------------------------------------------------------------
    else if ( ENTRADA.indexOf("MULTICAST") != -1) { // los espacios también importan
      
      String SubString  ="";
      int     aux       = 0;
      
      mensaje_enviar.modo       = M_DUMMY   ;   
      mensaje_enviar.parametro1 = ROJO       ;   // color
      mensaje_enviar.parametro2 = 255        ;   // intensidad
      
      aux = ENTRADA.lastIndexOf("-M");
      if (aux !=-1){
        SubString=ENTRADA.substring( aux , aux + 20 ); // Extrae los 20 caracteres posteriores a -M
        aux = modo(&SubString);
        if( aux!=-1 ) mensaje_enviar.modo       = aux    ;

      } 

      aux = ENTRADA.indexOf("-P1");
      if (aux !=-1){
        SubString=ENTRADA.substring( aux+3 , ENTRADA.indexOf(" ",aux + 4) ); // Extrae los 20 caracteres posteriores a -M
        SubString.trim();
        mensaje_enviar.parametro1 = SubString.toInt();
        Serial.print(F("P1 : "));
        Serial.println(mensaje_enviar.parametro1);
      } 

      aux = ENTRADA.indexOf("-P2");
      if (aux !=-1){
        SubString=ENTRADA.substring( aux+3 , ENTRADA.indexOf(" ",aux +4 ) ); // Extrae los 20 caracteres posteriores a -M
        SubString.trim();
        mensaje_enviar.parametro2 = SubString.toInt();
        Serial.print(F("P2 : "));
        Serial.println(mensaje_enviar.parametro2);
      } 
      
      if (ENTRADA.indexOf("-N1")!= -1) envio_rf_multicast(&mensaje_enviar, 1)  ;   // envia un mensaje a todos los nodos del nivel 1
      if (ENTRADA.indexOf("-N2")!= -1) envio_rf_multicast(&mensaje_enviar, 2)  ;   // envia un mensaje a todos los nodos del nivel 1
      if (ENTRADA.indexOf("-N3")!= -1) envio_rf_multicast(&mensaje_enviar, 3)  ;   // envia un mensaje a todos los nodos del nivel 1
      if (ENTRADA.indexOf("-N4")!= -1) envio_rf_multicast(&mensaje_enviar, 4)  ;   // envia un mensaje a todos los nodos del nivel 1
      if (ENTRADA.indexOf("-N5")!= -1) envio_rf_multicast(&mensaje_enviar, 5)  ;   // envia un mensaje a todos los nodos del nivel 1
      if (ENTRADA.indexOf("-TODOS")!= -1)  for (int i=1 ; i<=5 ; i++) envio_rf_multicast(&mensaje_enviar, i)  ;
      
    }

    //-------------------------------------------------------------------------
    // CAMBIA el MODO GENERAL de los NODOS
    // Sintaxis: modo general xxxxx
    //-------------------------------------------------------------------------
    else if (( ENTRADA.indexOf("MODO" ) != -1)&& \
             ( ENTRADA.indexOf("GENERAL") != -1)) {
      Serial.print(F("cambiando el modo general >> "));
      
      for(i = 0;codigo[i]!="FFFF";i++){
        if (ENTRADA.indexOf(codigo[i])!=-1) {
          MODO = i;
          Serial.println(codigo[i].c_str());
          break;
        }
      }

      //  En caso de no encotrar codigo valido muestra los disponibles
      if (codigo[i]=="FFFF") {
        Serial.print(F("\n\r[error] >> no coinicide nigún codigo : "));
        for(i = 0;codigo[i]!="FFFF";i++){
        Serial.println(codigo[i]);
        //}Serial.print(F(" , "));
        }
      }
    }
   
    //-------------------------------------------------------------------------
    // DESCONEXION DE BT
    //-------------------------------------------------------------------------
    else if ( ENTRADA.indexOf("+DISC:SUCCESS") != -1) { // desconexión del BT
      Serial.println(F("\n\rBT Desconectado"));
    }
    
    //-------------------------------------------------------------------------
    // TESTBT
    //
    //  sirve para enviar cualquier codigo por el BT
    //-------------------------------------------------------------------------
    else if ( ENTRADA.indexOf("TESTBT") != -1) {    // TEST es uncodigo para pasar transparente el MSG recibido
      if (ENTRADA.indexOf(":") != -1){
       
        MSG=ENTRADA.substring(ENTRADA.indexOf(":")+1);
        //TEST:ALARMA_MOV;002;003;004;005;006;002;003;004
        MSG.toUpperCase();
        MSG.trim();
        BT(MSG.c_str())
        Serial.print(F("TESTBT>> "));
        Serial.println(MSG.c_str());
      }
    }
    //-------------------------------------------------------------------------
    // TEST DISTANCIA
    //  sirve para ver si hay conexión a la distancia de RF
    //-------------------------------------------------------------------------
    else if ((ENTRADA.indexOf("TEST") != -1)&&(ENTRADA.indexOf("DISTANCIA") != -1)){
      Serial.println(F("\n\rTEST DISTANCIA"));
      mensaje_enviar.modo = M_ESPERA;
      mensaje_enviar.nodo = intNodoDestino;// OTRO_NODO;
      mensaje_enviar.tipo = T_TEST;    //cambio de modo
      mensaje_enviar.vbat = 0;
      mensaje_enviar.parametro1 = 0;    // color
      mensaje_enviar.parametro2 = 0;  // intensidad
      envio_rf_multicast(&mensaje_enviar,1) ;
    }
    else if (ENTRADA.indexOf("ALARMA") != -1){
      // cambia el modo del nodo central como si hubiese recibido una alarma
      MODO_MASTER = M_ALARMA_ECO;
      t_ultimo_mensaje = millis();
      FLAG_ENVIO = true;
    }
    else 
    {
      Serial.print(F("no se eoncotraron coincidencias"));
    }
 
}

int modo(String *modo){   // devuelve el índice del modo o -1 si no encuentra ningun modo coincidente
 for( int i = 0 ; codigo[i]!="FFFF" ; i++ ) {
   if ( modo->indexOf(codigo[i])!=-1)  {
      Serial.print("modo : ");
      Serial.print(codigo[i]);
      return i;
    }
   }
   Serial.print("no se encontró modo compatible");
  return -1;
}