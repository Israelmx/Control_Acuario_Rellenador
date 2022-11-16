#ifndef HW_IO_H /* { */
#define HW_IO_H
/*===========================================================================*\
 * FILE: name: HW_IO.h
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
 *   Archico que contiene los headers para las I/Os del programa
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
 * Hardware Pins Declaration
\*============================================================================*/
#define GPOHCSR04TRIG 16
#define GPIHCSR04ECHO 14

#define GPOLALRM 0
#define GPORELAY 12
#define GPINIVAG 13

#define GPIBTNGR 5   //Boton para uso general activar/desactivar la funcion rellenador, forzar rellenar, etc
#define GPOLEDOR 4   //LED para validar el Edo de la Fun Rndr act/des

#define GPAVFTE A0

/*============================================================================*\
 * Exported Preprocessor #define Constants
\*============================================================================*/

/*============================================================================*\
 * Exported Function Prototypes
 * ----------------------------------------------------------------------------
 * The header information for each of the following function prototypes is 
 * located in the source file
\*============================================================================*/

/*============================================================================*\
 * Exported In-line Function Definitions (#define function-like macros)
\*============================================================================*/
#define PRENDER_GPO(CH_OUTPUT) digitalWrite(CH_OUTPUT, HIGH)
#define APAGARF_GPO(CH_OUTPUT) digitalWrite(CH_OUTPUT, LOW)

#define PRENDER_LED_ALRM() digitalWrite(GPOLALRM, LOW)
#define APAGAR_LED_ALRM() digitalWrite(GPOLALRM, HIGH)

#define APAGAR_BOMBA() if(digitalRead(GPORELAY)){              \
                        digitalWrite(GPORELAY, LOW);          \
                        Control_Rdr.bits.Rellenando = false;  \
                        MQTTPUBRELLENANDO_ST(MQTT_STATE_OFF); \
                      }
#define PRENDER_BOMBA() if(!digitalRead(GPORELAY)){            \
                         digitalWrite(GPORELAY, HIGH);        \
                         Control_Rdr.bits.Rellenando = true;  \
                         MQTTPUBRELLENANDO_ST(MQTT_STATE_ON); \
                       }

/*============================================================================*\
 * File Revision History (top to bottom: first revision to last revision)
 *==============================================================================
 *
 * Date         userid    (Description on following lines: SCR #, etc.)
 * -----------  --------
 * 11-FEB-2021  Israel Gomez
 * - Created initial file.
 *
\*============================================================================*/
#endif /* } HW_IO_H */
