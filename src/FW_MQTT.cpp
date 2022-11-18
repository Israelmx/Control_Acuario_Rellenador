/*===========================================================================*\
 * FILE: name: FW_MQTT.cpp
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 11/02/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the MQTT features for Control_Acuario_Rellenador program.
 *
 * ABBREVIATIONS:
 *   None.
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *
 *   Requirements Document(s):
 *
 *   Applicable Standards (in order of precedence: highest first):
 *   C Coding Standards SW REF 264.15D SW
 *
 * DEVIATIONS FROM STANDARDS:
 *
\*===========================================================================*/
/*============================================================================*\
 * Standard Header Files
\*============================================================================*/

/*============================================================================*\
 * Other Header Files
\*============================================================================*/
#include "FW_MQTT.h"
#include "FW_Wifi.h"
#include "HW_IO.h"
#include "FW_Debug.h"
#include "FW_HandlrRndr.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
//#define STARTINDEXSUBTOPIC 38   // Topic: RicXav/Sala/Acuario/Rellenador/RcbCmmn 38 chars
//#define STARTINDEXSUBTOPIC 39   // Topic: IsraXav/Sala/Acuario/Rellenador/RcbCmmn 39 chars
//#define STARTINDEXSUBTOPIC 40   // Topic: IsraXav/Sala/Acuario/Rellenador2/RcbCmmn 40 chars
//#define STARTINDEXSUBTOPIC 43   // Topic: IsraXav/Estudio/Gambario/Rellenador/RcbCmmn 43 chars
//TODO: Cambiar segun el largo del topico
#define STARTINDEXSUBTOPIC 49   // Topic: IsraXav/Dormitorio/Acuario_Pas/Rellenador/RcbCmmn 90 chars

/*============================================================================*\
 * Local Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Local Type Declarations
\*============================================================================*/
typedef enum El_SsubTopic_Es_Tag
{
  SSUBTOPIC_ACTDESFUNRDR,   //Activar / Desactivar Funcion Rellenador
  SSUBTOPIC_RELLENAR,
  SSUBTOPIC_NO_DEFINIDO
}
El_SsubTopic_Es_T;

/*============================================================================*\
 * Exported Object Definitions
\*============================================================================*/
//MQTT Topics
//const char* MQTT_TOPIC                  = "RicXav/Sala/Acuario/Rellenador";
//const char* MQTT_TOPIC                  = "IsraXav/Sala/Acuario/Rellenador";
//const char* MQTT_TOPIC                  = "IsraXav/Estudio/Gambario/Rellenador";
//TODO: Cambiar el nombre del topico segun el rellenador que sea
const char* MQTT_TOPIC                  = "IsraXav/Dormitorio/Acuario_Pas/Rellenador";
//MQTT Pub Topics
const char* MQTT_PUBTOPIC_NIVAGUAREDR   = "/NivAguaRellenador";
const char* MQTT_PUBTOPIC_NIVAGUACUARIO = "/NivAguaAcuario";
const char* MQTT_PUBTOPIC_ALARMARDR     = "/AlarmaRdr";
const char* MQTT_PUBTOPIC_ALMRDR_NIVRDR = "/AlarmaNivRdr";
const char* MQTT_PUBTOPIC_ALMRDR_OVRTME = "/AlarmaOvrTme";
const char* MQTT_PUBTOPIC_VFTE          = "/VFuente";
const char* MQTT_PUBTOPIC_HBEAT         = "/RellenadorHeartBeat";
//MQTT Sub Topic
const char* MQTT_SUBTOPICGRAL           = "/RcbCmmn/#"; //Topico a suscribirse
const char* MQTT_SUBTOPIC               = "/RcbCmmn";   //Topico a suscribirse
//MQTT SSubs Topics
const char* MQTT_SSUBTOPIC_ACTDESFUNRDR = "/ActDesFunRdr";
const char* MQTT_SSUBTOPIC_RELLENAR     = "/Rellenando";
//MQTT Pub state Topic
const char* MQTT_STATE                  = "/state";     //Agrega en el topico la palabra state

//const char HOST_MQTT_DEVICE_ID[]= "ESP8266_V201_ACRDR";   //ID del dispositivo Acuario Rellenador
//const char HOST_MQTT_DEVICE_ID[]= "ESP8266_V201_ACRDR_RicXav";   //ID del dispositivo Acuario Rellenador
//const char HOST_MQTT_DEVICE_ID[]= "ESP8266_V201_GAMRDPAS";   //ID del dispositivo Acuario Rellenador
//TODO: Cambiar el nombre del dispositivo de MQTT
const char HOST_MQTT_DEVICE_ID[]= "ESP8266_V202_ACRDRPAS";   //ID del dispositivo Acuario Rellenador

const char MQTT_STATE_ON[] = "ON";
const char MQTT_STATE_OFF[] = "OFF";
const char MQTT_STATE_NVACBIEN[] = "BIEN";
const char MQTT_STATE_NVACBAJO[] = "BAJO";

//const char MQTT_IPSERVER[] = "192.168.1.11";      //RicXav
//const char MQTT_IPSERVER[] = "192.168.17.75";     //IsraXav DockerLinux
const char MQTT_IPSERVER[] = "192.168.17.77";     //IsraXav
const char MQTT_PORT[] = "1883";

bool MsjMQTTRcb = false;

/*============================================================================*\
 * Local Object Definitions
\*============================================================================*/
El_SsubTopic_Es_T El_SsubTopic_Es;

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

/*============================================================================*\
 * Local Inline Function Definitions and Function-Like Macros
\*============================================================================*/

/*============================================================================*\
 * Local Preprocessor Declaration
\*============================================================================*/
CircularBuffer<String, CIRCULARBUFFERSIZE> SSubTopic_Bfr;

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/
CircularBuffer<String, CIRCULARBUFFERSIZE> Mqtt_PL_Rcd_Bfr;

/*******************************************************************************
 * FUNCTION: Fun_messageReceived
 *******************************************************************************
 * RETURN VALUE:
 *    None.
 *     
 * PARAMETERS:
 *    None.
 *     
 * EXTERNAL REFERENCES:
 *    None.
 *     
 * DEVIATION FROM STANDARDS:
 *    None.
 *
 * -----------------------------------------------------------------------------
 * ABSTRACT:
 * -----------------------------------------------------------------------------
 * Fun que maneja los mensajes de MQTT entrantes
 * 
 * PRECONDITIONS:
 * None.
 * 
 * POSTCONDITIONS:
 * None.      
 *
\******************************************************************************/
void Fun_MessageReceived(String &topic, String &payload)
{
  String ssubtopic;

  ssubtopic = topic;
  ssubtopic = ssubtopic.substring(STARTINDEXSUBTOPIC);  //De todo el topico solo tomo el subtopico

  #ifdef IDEBUGSERIAL
  Serial.print("Mensaje Recibido en el Topico Suscrito: ");
  Serial.print(ssubtopic);
  Serial.print(": ");
  Serial.println(payload);
  #endif

  MsjMQTTRcb = true;

  if(ssubtopic == MQTT_SSUBTOPIC_ACTDESFUNRDR)
  {
    El_SsubTopic_Es = SSUBTOPIC_ACTDESFUNRDR;
  }
  else if(ssubtopic == MQTT_SSUBTOPIC_RELLENAR)
  {
    El_SsubTopic_Es = SSUBTOPIC_RELLENAR;
  }
  else
  {
    El_SsubTopic_Es = SSUBTOPIC_NO_DEFINIDO;
    MsjMQTTRcb = false;
    #ifdef IDEBUGSERIAL
    Serial.println("No se ha definido el topico");
    #endif
  }

  if(MsjMQTTRcb)
  {
    Mqtt_PL_Rcd_Bfr.push(payload);  //Agrego el mensaje recibido de MQTT al buffer
    SSubTopic_Bfr.push(ssubtopic);  //Agrego el Sub Topico recibido de MQTT al buffer
  }
}

/*******************************************************************************
 * FUNCTION: Fun_Analizar_MsjRc
 *******************************************************************************
 * RETURN VALUE:
 *    None.
 *
 * PARAMETERS:
 *    None.
 *
 * EXTERNAL REFERENCES:
 *    None.
 *
 * DEVIATION FROM STANDARDS:
 *    None.
 *
 * -----------------------------------------------------------------------------
 * ABSTRACT:
 * -----------------------------------------------------------------------------
 * Analiza el mensaje recibido al topico suscrito
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Analizar_MsjRc()
{
  bool msjvalido = true;
  String mqtt_pl_rcd = Mqtt_PL_Rcd_Bfr.shift(); //Extraigo del Buffer por medio de FIFO el mensaje que toca
  String sstopic = SSubTopic_Bfr.shift();       //Extraigo del Buffer por medio de FIFO el Sub Topico que toca
  
  #ifdef IDEBUGSERIAL
  Serial.println("Analizando Topico y Mensaje Recibido: ");
  #endif
  
  switch (El_SsubTopic_Es)
  {
    case SSUBTOPIC_ACTDESFUNRDR:
    {
      if(mqtt_pl_rcd == MQTT_STATE_ON)
      {
        Control_Rdr.bits.SCtrl_Rellenador_Activado = true;
        PRENDER_GPO(GPOLEDOR);
      }
      else if(mqtt_pl_rcd == MQTT_STATE_OFF)
      {
        Control_Rdr.bits.SCtrl_Rellenador_Activado = false;
        APAGARF_GPO(GPOLEDOR);
      }
      else
      {
        msjvalido = false;
      }
      break;
    }

    case SSUBTOPIC_RELLENAR:
    {
      if(mqtt_pl_rcd == MQTT_STATE_ON)
      {
        PRENDER_BOMBA();
      }
      else if(mqtt_pl_rcd == MQTT_STATE_OFF)
      {
        APAGAR_BOMBA();
      }
      else
      {
        msjvalido = false;
      }
      break;
    }

    case SSUBTOPIC_NO_DEFINIDO:
    {
      #ifdef IDEBUGSERIAL
      Serial.println("No se ha definido el topico");
      #endif
      msjvalido = false;
      break;
    }

    default:
    {
      msjvalido = false;
      break;
    }
  }
  
  if(msjvalido)
  {
    clientmqttc.publish((String(MQTT_TOPIC) + sstopic + String(MQTT_STATE)), mqtt_pl_rcd.c_str());
  }
  else
  {
    #ifdef IDEBUGSERIAL
    Serial.println("Mensaje no valido en un SSusTopico Valido");
    #endif
  }
}