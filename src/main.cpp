#define IDEBUGSERIAL
/*===========================================================================*\
 * FILE: name: Ctrl_Acuario_Rellenador
 *===========================================================================
 * Copyright 2020 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 01/Nov/2020
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the Ctrl_Acuario_Rellenador program.
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
#include <Arduino.h>

/*============================================================================*\
 * Program Header Files
\*============================================================================*/
#include "FW_ADC.h"
#include "FW_Botones.h"
#include "FW_Debug.h"
#include "FW_HandlrRndr.h"
#include "FW_MQTT.h"
#include "FW_NivAcuario.h"
#include "FW_NivRndr.h"
#include "FW_Wifi.h"
#include "HW_IO.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
#ifdef IDEBUGSERIAL
#define SERIALBAUDR 9600
#endif

#define HEARTBEATVAL 0.25   //Como se manda cada 15 segundos el HB es un cuarto de minuto
#define TIMER_LEER_HB 15000 //Quiero que el HB se mande cada 15 segundos

/*============================================================================*\
 * Local Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Local Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Local Variable Definitions
\*============================================================================*/
float Heart_Beat = 0.0;

/*============================================================================*\
 * Local Preprocessor Declaration
\*============================================================================*/

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/
void Fun_Heart_Beat(void);

/*============================================================================*\
 * Local Inline Function Definitions and Function-Like Macros
\*============================================================================*/

/*============================================================================*\
 * Function Definitions
\*============================================================================*/

/*******************************************************************************
 * FUNCTION: setup
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
 * Setup de Arduino del programa
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void setup()
{
  delay(250);

  #ifdef IDEBUGSERIAL
  Serial.begin(SERIALBAUDR); //Inicia el serial para comunicarse con la tarjeta de relevadores
  while (!Serial)
  {
    // wait serial port initialization
  }
  #endif

  pinMode(GPOLALRM, OUTPUT);
  pinMode(GPORELAY, OUTPUT);
  pinMode(GPINIVAG, INPUT);
  
  pinMode(GPOLEDOR, OUTPUT);
  pinMode(GPIBTNGR, INPUT);

  APAGAR_LED_ALRM();

  Fun_Iniciar_Conf_Botones();

  Fun_WifiConnect();
  Fun_HTTP_Web_Server_Config();
  
  MQTTPUBHBEAT();
  MQTTPUBACTDESFUNRDR_ST(MQTT_STATE_OFF);
  MQTTPUBALMRDROVRTME_ST(MQTT_STATE_OFF);
  MQTTPUBALMRDRNIVRDR_ST(MQTT_STATE_OFF);

  Alarmas_Rdr.ARDR_Byte = 0;
}

/*******************************************************************************
 * FUNCTION: loop
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
 * Loop infinito de Arduino
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void loop()
{
  httpServer.handleClient();

  Fun_Checa_MQTT_Con();

  if(MsjMQTTRcb == true)
  {
    Fun_Analizar_MsjRc();
    if(Mqtt_PL_Rcd_Bfr.isEmpty())
    {
      MsjMQTTRcb = false;
    }
  }

  SENSOR_NIV_AGUA.read();
  BTN_GENERAL.read();

  /*if(SENSOR_NIV_AGUA.pressedFor(TIMER_ALRMOVRTMETME))
  {
    Fun_Alarma_Over_Time();
    SENSOR_NIV_AGUA.begin();
  }*/

  Fun_Heart_Beat();
  Fun_Rellenar_Acuario();
  Fun_Checar_Nivel_Acuario();
  Fun_Checar_Nivel_Rellenador();
  Fun_Checar_Alarmas();
}

/*******************************************************************************
 * FUNCTION: Fun_Heart_Beat
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
 * Set the Heart Beat of the Circuit
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Heart_Beat()
{
  static long previousmillishb;
  static unsigned long currentmillishb;

  currentmillishb = millis();
  if(currentmillishb - previousmillishb > TIMER_LEER_HB)
  {
    previousmillishb = currentmillishb;
    
    Heart_Beat = Heart_Beat + HEARTBEATVAL;
    MQTTPUBHBEAT();

    Fun_Leer_ESP8266ADC();
  }
}