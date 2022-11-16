/*===========================================================================*\
 * FILE: name: FW_Botones.cpp
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
 *   Archico que maneja las funciones de los botones del circuito
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
#include "FW_Botones.h"
#include "FW_Debug.h"
#include "HW_IO.h"
#include "FW_HandlrRndr.h"
#include "FW_MQTT.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
#define DBNCTIMENIVAG 35
#define PULLUPNIVAG 1
#define INVERTLOGICNIVAG 1

#define DBNC_TIME_BTNGRL 35
#define PULL_UP_BTNGRL 0
#define INVERT_LOGIC_BTNGRL 1
#define NUM_OF_PRS_BTNGRL_ACT_FUN_RNDR 2
#define SQNCE_TIMEOUT_ACT_FUN_RNDR 1500

#define TIMER_BTN_RELLENAR 2000 //Quiero que si pasan 2 segundos se active la bomba para rellenar

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
void Fun_ActDes_Rndr_Por_Btn(void);
void Fun_Rellenar_Por_Btn(void);

/*============================================================================*\
 * Local Inline Function Definitions and Function-Like Macros
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/
EasyButton SENSOR_NIV_AGUA(GPINIVAG, DBNCTIMENIVAG, PULLUPNIVAG, INVERTLOGICNIVAG);
EasyButton BTN_GENERAL(GPIBTNGR, DBNC_TIME_BTNGRL, PULL_UP_BTNGRL, INVERT_LOGIC_BTNGRL);

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
void Fun_Iniciar_Conf_Botones(void)
{
  SENSOR_NIV_AGUA.begin();
  SENSOR_NIV_AGUA.onPressedFor(TIMER_ALRMOVRTMETME, Fun_Alarma_Over_Time);

  BTN_GENERAL.begin();
  BTN_GENERAL.onSequence(NUM_OF_PRS_BTNGRL_ACT_FUN_RNDR, SQNCE_TIMEOUT_ACT_FUN_RNDR, Fun_ActDes_Rndr_Por_Btn);
  BTN_GENERAL.onPressedFor(TIMER_BTN_RELLENAR, Fun_Rellenar_Por_Btn);
}

/*******************************************************************************
 * FUNCTION: Fun_ActDes_Rndr_Por_Btn
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
 * Fun que activa o desactiva la funcion rellenador
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_ActDes_Rndr_Por_Btn(void)
{
  if(Control_Rdr.bits.SCtrl_Rellenador_Activado)
  {
    Control_Rdr.bits.SCtrl_Rellenador_Activado = false;
    MQTTPUBACTDESFUNRDR_ST(MQTT_STATE_OFF);
    APAGARF_GPO(GPOLEDOR);
  }
  else
  {
    Control_Rdr.bits.SCtrl_Rellenador_Activado = true;
    MQTTPUBACTDESFUNRDR_ST(MQTT_STATE_ON);
    PRENDER_GPO(GPOLEDOR);
  }
}

/*******************************************************************************
 * FUNCTION: Fun_Rellenar_Por_Btn
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
 * Fun que rellena o apaga rellenar por boton
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_Rellenar_Por_Btn(void)
{
  
}