/*===========================================================================*\
 * FILE: name: FW_HandlrRndr.cpp
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 05/17/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   Archico que maneja el algoritmo de Relleno
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
#include "FW_HandlrRndr.h"
#include "FW_NivRndr.h"
#include "FW_Botones.h"
#include "FW_MQTT.h"
#include "FW_Debug.h"
#include "FW_MQTT.h"
#include "HW_IO.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
#define TIMER_ALM_LED 5000   //Quiero que cada 5 segundos se vea si no hay alarmas y se debe apagar el LED

/*============================================================================*\
 * Local Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Local Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Object Definitions
\*============================================================================*/
Alarmas_Rdr_T Alarmas_Rdr;
Control_Rdr_T Control_Rdr;

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

/*******************************************************************************
 * FUNCTION: Fun_Rellenar_Acuario
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
 * Fun para rellenar el acuario
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Rellenar_Acuario()
{
  if(!Alarmas_Rdr.bits.General)
  {
    if(!Control_Rdr.bits.SCtrl_Rellenador_Activado)
    {
      APAGAR_BOMBA();
    }
    else  //Esta Activo el Servicio para validar si hay que Rellenar
    {
      if(Control_Rdr.bits.Rellenador_A_Nivel)
      {
        if(Control_Rdr.bits.Acuario_A_Nivel)
        {
          APAGAR_BOMBA();
        }
        else  //Acuario no a nivel
        {
          if(!Control_Rdr.bits.Rellenando)  //No esta la bomba prendida, prendela
          {
            PRENDER_BOMBA();
            #ifdef IDEBUGSERIAL
            Serial.println("Rellenando");
            #endif
          }
          else  //La bomba ya esta prendida
          {
            if(Alarmas_Rdr.bits.OverTime_Rellenando)  //Checa si hay alarma de sobre tiempo de rellenado
            {
              APAGAR_BOMBA();   //Si, si, apaga la bomba
            }
          }
        }
      }
      else  //Rellenador Empty
      {
        APAGAR_BOMBA();
      }
    }
  }
  else  //Hay Alarmas
  {
    APAGAR_BOMBA();
    if(!Control_Rdr.bits.SCtrl_Rellenador_Activado)   //Si desactivamos la funcio de rellenar
    {
      Alarmas_Rdr.bits.OverTime_Rellenando = false;   //Borra alarma de sobre tiempo, porque ya checamos fisicamente que pasa
      MQTTPUBALMRDROVRTME_ST(MQTT_STATE_OFF);
    }
  }
}

/*******************************************************************************
 * FUNCTION: Fun_Alarma_Over_Time
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
 * Fun que genera una alarma si paso el tiempo de rellenado y aun no llega
 * la señal de que se relleno el acuario
 *
 * PRECONDITIONS:
 * Es llamada por el main al detectar que ya paso el tiempo de seguridad para 
 * rellenar y no se ha detectado el sensor a nive de Acuario OK
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Alarma_Over_Time()
{
  Alarmas_Rdr.bits.OverTime_Rellenando = true;
  Alarmas_Rdr.bits.General = true;
  PRENDER_LED_ALRM();
  SENSOR_NIV_AGUA.begin();

  #ifdef IDEBUGSERIAL
  Serial.println("Alarma sobre tiempo de Rellenado");
  #endif

  MQTTPUBALMRDROVRTME_ST(MQTT_STATE_ON);
}

/*******************************************************************************
 * FUNCTION: Fun_Checar_Alarmas
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
 * Maneja las alarmas que se generen
 *
 * PRECONDITIONS:
 * Es llamada del main constantemente
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Checar_Alarmas()
{
  static long previousmillisalm;
  static unsigned long currentmillisalm;

  currentmillisalm = millis();
  if(currentmillisalm - previousmillisalm > TIMER_ALM_LED)
  {
    previousmillisalm = currentmillisalm;
    if(Alarmas_Rdr.bits.General)
    {
      if((!Alarmas_Rdr.bits.OverTime_Rellenando) && (!Alarmas_Rdr.bits.Rellenador_Empty_b))
      {
        Alarmas_Rdr.bits.General = false;
        APAGAR_LED_ALRM();
      }
    }
  }
}