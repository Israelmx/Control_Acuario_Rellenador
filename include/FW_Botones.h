#ifndef FW_BOTONES_H /* { */
#define FW_BOTONES_H
/*===========================================================================*\
 * FILE: name: FW_Botones.h
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
 *   Archico que contiene los headers de los botones del circuito
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
#include <EasyButton.h>

/*============================================================================*\
 * Other Header Files
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor #define Constants
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Exported Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Object Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Function Prototypes
 * ----------------------------------------------------------------------------
 * The header information for each of the following function prototypes is 
 * located in the source file
\*============================================================================*/
extern void Fun_Iniciar_Conf_Botones(void);

/*============================================================================*\
 * Exported In-line Function Definitions (#define function-like macros)
\*============================================================================*/

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/
extern EasyButton SENSOR_NIV_AGUA;
extern EasyButton BTN_GENERAL;

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
#endif /* } FW_BOTONES_H */