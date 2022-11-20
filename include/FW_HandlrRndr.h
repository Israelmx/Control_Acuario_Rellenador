#ifndef FW_HANDLRRNDR_H /* { */
#define FW_HANDLRRNDR_H
/*===========================================================================*\
 * FILE: name: FW_HandlrRndr.h
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
 *   Archico que contiene los headers del algortimo de Relleno
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
#include "Arduino.h"

/*============================================================================*\
 * Other Header Files
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor #define Constants
\*============================================================================*/
//TODO: Cambiar el tiempo que se desee esperar para alarmar overtime rellenando
#define TIMER_ALRMOVRTMETME 60000 //Quiero que si pasan 60 segundos (1min) rellenando se genere alarma

/*============================================================================*\
 * Exported Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Exported Type Declarations
\*============================================================================*/
typedef struct Alarmas_Rdr_bits_Tag
{
  bool Rellenador_Empty_b:1;
  bool OverTime_Rellenando:1;
  bool General:1;
  bool:5;
}
Alarmas_Rdr_bits_T;

typedef union Alarmas_Rdr_Tag
{
      byte ARDR_Byte;
      Alarmas_Rdr_bits_T bits;
}
Alarmas_Rdr_T;

typedef struct Control_Rdr_bits_Tag
{
  bool SCtrl_Rellenador_Activado:1;   //Señal de Control para activar la fun de rellenar
  bool Rellenando:1;
  bool Rellenador_A_Nivel:1;
  bool Acuario_A_Nivel:1;
  bool SCtrl_Rellenar_Btn:1;          //Señal de Control rellenar por boton
  bool:3;
}
Control_Rdr_bits_T;

typedef union Control_Rdr_Tag
{
      byte Control_Byte;
      Control_Rdr_bits_T bits;
}
Control_Rdr_T;

/*============================================================================*\
 * Exported Object Declarations
\*============================================================================*/
extern Alarmas_Rdr_T Alarmas_Rdr;
extern Control_Rdr_T Control_Rdr;

/*============================================================================*\
 * Exported Function Prototypes
 * ----------------------------------------------------------------------------
 * The header information for each of the following function prototypes is 
 * located in the source file
\*============================================================================*/
extern void Fun_Checar_Alarmas(void);
extern void Fun_Rellenar_Acuario(void);
extern void Fun_Alarma_Over_Time(void);

/*============================================================================*\
 * Exported In-line Function Definitions (#define function-like macros)
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/

/*============================================================================*\
 * File Revision History (top to bottom: first revision to last revision)
 *==============================================================================
 *
 * Date         userid    (Description on following lines: SCR #, etc.)
 * -----------  --------
 * 16-MAY-2021  Israel Gomez
 * - Created initial file.
 *
\*============================================================================*/
#endif /* } FW_HANDLRRNDR_H */