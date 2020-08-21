#ifndef _CODIGOS_
#define _CODIGOS_

#define ROJO      0
#define AZUL      160
#define VERDE     96
#define AMARILLO  46

// definicion de los códigos para modo

#define M_ESPERA  		    0
#define M_VIVO    		    1
#define M_APAGADO 		    2
#define M_RANDOM 			    3
#define M_CONECTANDO		  4
#define M_DESCONECTADO	  5
#define M_TAD01			      6
#define M_TAD02			      7
#define M_TAD03			      8
#define M_TAD04			      9
#define M_TAD05			      10
#define M_PING            11
#define M_ALARMA_VIB      12
#define M_ALARMA_MOV      13
#define M_ALARMA_ECO      14
#define M_CONSULTA        15
#define M_RESPUESTA       16
#define M_TEST            17
#define M_TEST_DISTANCIA  18
#define M_EEPROM          19
#define M_RESET           20
#define M_DUMMY           21   
#define M_TIEMPO          22   

int MODO_CENTRAL = M_VIVO;

// cada vez que se agregue un codigo nuevo, debe agregarse el texto en el String codigos[]

String codigo[24]={ "ESPERA"      , //  0
                    "VIVO"        , //  1
                    "APAGADO"     , //  2
                    "RANDOM"      , //  3
                    "CONECTANDO"  , //  4
                    "DESCONECTADO", //  5
                    "TAD01"       , //  6
                    "TAD02"       , //  7
                    "TAD03"       , //  8
                    "TAD04"       , //  9
                    "TAD05"       , //  10
                    "PING"        , //  11
                    "ALARMA_VIB"  , //  12
                    "ALARMA_MOV"  , //  13
                    "ALARMA_ECO"  , //  14
                    "CONSULTA"    , //  15
                    "RESPUESTA"   , //  16     
                    "TEST"        , //  17
                    "TESTDISTANCIA",//  18
                    "EEPROM"      , //  19
                    "RESET"       , //  20
                    "DUMMY"       , //  21
                    "TIEMPO"      , //  22  // sirve para cargar la hora en todos los nodos
                    "FFFF"          //  last msg
                    };

// definicion del tipo de mensaje
// mensaje_xxxxxx.tipo

#define T_LATIDO      'L'             // mensaje para indicar la sobrevida de un nodo
#define T_CONSULTA    'C'             // mensaje que requiere una respuesta
#define T_RESPUESTA   'D'             // mensaje en respuesta de alguna consulta
#define T_PING        'P'             // mensaje para medir el tiempo de comunicacion
#define T_CONEXION	  'X'             // mensaje de solicitud de conexión
#define T_ENVIO   	  'E'             // mensajes 
#define T_ESPERA   	  'S'             // mensajes que no necesitan respuesta
#define T_ALARMA      'A'             // Alarma por vibración del modulo
#define T_TEST    	  'T'             // mensajes de Testeo de la RED
#define T_RESET    	  'R'             // mensajes de Testeo de la RED


struct Mensaje {                    //  Estructura del mensaje
  unsigned int  nodo;               //  Dirección del nodo de destino donde va o viene el mensaje
  unsigned int  tipo;               //  Tipo de mensaje
  unsigned long contador;           //  contador del numero de mensaje
  unsigned long vbat;               //  voltaje de la propia batería
  unsigned long vin;                //  voltaje de la propia batería
  unsigned int  modo;               //  
  int           parametro1;         //  parametro1 adicional
  int           parametro2;         //  parametro2 adicional
};

struct Parametros{                  // Parámetros de funcionamiento
int numero_nodo = 0                           ;
int umbral_voltaje_dia_noche = 0              ;
int vBat_umbral = 500                         ;
volatile unsigned long intervalo_ENVIO = 3000 ;
volatile unsigned long intervalo_BLINK = 5000 ;
unsigned long tiempo_de_reconexion = 60000;     // un minuto de tiempo maximo desconectado
} parametros;



#endif
