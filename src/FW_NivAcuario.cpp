/*===========================================================================*\
 * FILE: name: FW_NivAcuario.cpp
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
 *   Archico que maneja el Nivel de Agua del Acuario
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
#include "FW_NivAcuario.h"
#include "FW_Debug.h"
#include "FW_MQTT.h"
#include "HW_IO.h"
#include "FW_HandlrRndr.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
#define TIMER_NIVAGDBNCE 5000 //Quiero que si pasan 5 segundos en nivel bajo de agua active el rellenador

#define NIVELOKACUARIO 1
#define NIVELBAJOACUARIO 0

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
#define LEERNIVACUARIO() digitalRead(GPINIVAG)

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/

/*******************************************************************************
 * FUNCTION: Fun_Nivel_Acuario_Bajo
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
 * Fun donde entra si el sensor de nivel acuario es bajo
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Checar_Nivel_Acuario()
{
  static bool previostate;
  static bool estadoactual;
  static long previousmillisbtnnivac;
  static unsigned long currentmillisbtnnivac;
  static bool mismo_estado_anterior = false;  //Valida el estado anterior del nivel para no repetir el envio de mqtt

  estadoactual = LEERNIVACUARIO();

  if(previostate == estadoactual)
  {
    currentmillisbtnnivac = millis();
    if(currentmillisbtnnivac - previousmillisbtnnivac > TIMER_NIVAGDBNCE)
    {
      previousmillisbtnnivac = currentmillisbtnnivac;
      if(!mismo_estado_anterior)
      {
        if(NIVELOKACUARIO == estadoactual)
        {
          Control_Rdr.bits.Acuario_A_Nivel = NIVELOKACUARIO;
          MQTTPUBNIVACUARIO(MQTT_STATE_NVACBIEN);
          #ifdef IDEBUGSERIAL
          Serial.println("Nivel del Acuario OK");
          #endif
        }
        else
        {
          Control_Rdr.bits.Acuario_A_Nivel = NIVELBAJOACUARIO;
          MQTTPUBNIVACUARIO(MQTT_STATE_NVACBAJO);
          #ifdef IDEBUGSERIAL
          Serial.println("Nivel del Acuario Bajo");
          #endif
        }
        mismo_estado_anterior = true;
      }
      else
      {
        // Do nothing
      }
    }
  }
  else
  {
    previostate = estadoactual;
    previousmillisbtnnivac = millis();
    mismo_estado_anterior = false;
  }
}