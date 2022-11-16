/*===========================================================================*\
 * FILE: name: FW_ADC.cpp
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 05/16/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   Archico que contiene las funciones para leer las variables ADC del Rellenador
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
#include "FW_ADC.h"
#include "HW_IO.h"
#include "FW_Wifi.h"
#include "FW_MQTT.h"
#include "FW_Debug.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
//Filtro Media Movil como Pasa Bajos
//An=a*M+(1-a)*An
//alpha 1: Sin filtro
//alpha 0: Filtrado totalmente
//alpha clasico 0.05
#define ALPHA 0.5
#define VOLREFADC 1            //Ya que es el ESP12F, el ADC va directo y es a 1VDC
#define ANRESOLUTION 1024.00
#define ANRATIOFACTOR 11.00    //Resistors Ration Factor For VCC voltage Cto Div 47K/4K7
                               //5VDC/0.4545 "(Div47K/4K7)" = 11.001

/*============================================================================*\
 * Local Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Local Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Object Definitions
\*============================================================================*/

/*============================================================================*\
 * Local Object Definitions
\*============================================================================*/

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/

/*============================================================================*\
 * Local Inline Function Definitions and Function-Like Macros
\*============================================================================*/

/*============================================================================*\
 * Local Preprocessor Declaration
\*============================================================================*/

/*******************************************************************************
 * FUNCTION: Fun_Leer_ESP8266ADC
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
 * Fun para leer el ADC del ESP8266
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Leer_ESP8266ADC()
{
  static float adc_filtrado_vcc;
  float adc_raw_value;
  float vol_vcc;
  
  adc_raw_value = analogRead(GPAVFTE);
  adc_filtrado_vcc = ((ALPHA * adc_raw_value) + ((1 - ALPHA) * adc_filtrado_vcc));
  vol_vcc = (((adc_filtrado_vcc * VOLREFADC) / ANRESOLUTION) * ANRATIOFACTOR);
  
  #ifdef IDEBUGSERIAL
  Serial.print("Cuentas ADC Raw Value: "); 
  Serial.println(adc_raw_value);
  Serial.print("Cuentas ADC Voltaje filtrado Value: "); 
  Serial.println(adc_filtrado_vcc);
  Serial.print("Voltaje VCC: "); 
  Serial.print(vol_vcc);
  Serial.println(" V"); 
  #endif
  
  if(WIFI_UP_MQTT_UP == Connection_Status)
  {
    clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_VFTE)), String(vol_vcc).c_str());
  }
}