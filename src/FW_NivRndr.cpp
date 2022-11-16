/*===========================================================================*\
 * FILE: name: FW_NivRndr.cpp
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
 *   Archico que maneja el Nivel de Agua del Rellenador
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
#include "FW_NivRndr.h"
#include "FW_Debug.h"
#include "FW_MQTT.h"
#include "HW_IO.h"
#include "FW_HandlrRndr.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
#define NIVELOKRELLENADOR 11
#define NIVELBAJORELLENADOR 33
#define NIVELBAJOREPORCIENTO 1
#define NIVELOKREPORCIENTO 100

#define TIMER_LEER_NIV_RDR 20000

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
 * Exported Preprocessor Declaration
\*============================================================================*/
HCSR04 hc_acuario(GPOHCSR04TRIG, GPIHCSR04ECHO); //initialisation class HCSR04 (trig pin , echo pin)

/*******************************************************************************
 * FUNCTION: Fun_Checar_Nivel_Rellenador
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
 * Fun que verifica el nivel de agua del rellenador
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Checar_Nivel_Rellenador()
{
  int nivel_rellenador;
  static long previousmillisnivrdr;
  static unsigned long currentmillisnivrdr;
  static bool estado_anterior_ok = false;
  static bool estado_anterior_bajo = false;

  currentmillisnivrdr = millis();
  if(currentmillisnivrdr - previousmillisnivrdr > TIMER_LEER_NIV_RDR)
  {
    previousmillisnivrdr = currentmillisnivrdr;
    
    nivel_rellenador = hc_acuario.dist();
    #ifdef IDEBUGSERIAL
    Serial.println(nivel_rellenador);
    #endif
    
    if(nivel_rellenador < NIVELBAJORELLENADOR)
    {
      if(!estado_anterior_ok)
      {
        Control_Rdr.bits.Rellenador_A_Nivel = true;
        Alarmas_Rdr.bits.Rellenador_Empty_b = false;
        //MQTTPUBNIVRELLENADOR_ST(MQTT_STATE_ON);
        MQTTPUBALMRDRNIVRDR_ST(MQTT_STATE_OFF);
        #ifdef IDEBUGSERIAL
        Serial.println("Nivel del Rellandor OK");
        #endif
        estado_anterior_ok = true;        
        estado_anterior_bajo = false;
      }
    }
    else
    {
      if(!estado_anterior_bajo)
      {
        Control_Rdr.bits.Rellenador_A_Nivel = false;
        Alarmas_Rdr.bits.Rellenador_Empty_b = true;
        Alarmas_Rdr.bits.General = true;
        PRENDER_LED_ALRM();
        //MQTTPUBNIVRELLENADOR_ST(MQTT_STATE_OFF);
        MQTTPUBALMRDRNIVRDR_ST(MQTT_STATE_ON);
        #ifdef IDEBUGSERIAL
        Serial.println("Nivel del Rellandor Bajo");
        #endif
        estado_anterior_bajo = true;
        estado_anterior_ok = false;
      }
    }

    int niv_rdr_porciento = map(nivel_rellenador, NIVELBAJORELLENADOR, NIVELOKRELLENADOR, NIVELBAJOREPORCIENTO, NIVELOKREPORCIENTO);
    MQTTPUBNIVRELLENADOR(niv_rdr_porciento);
  }
}