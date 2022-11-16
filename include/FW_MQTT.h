#ifndef FW_MQTT_H /* { */
#define FW_MQTT_H
/*===========================================================================*\
 * FILE: name: FW_MQTT.h
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: m/d/y 03/08/2021
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
#include <MQTTClient.h>
#include <CircularBuffer.h>

/*============================================================================*\
 * Other Header Files
\*============================================================================*/
#include "FW_Wifi.h"

/*============================================================================*\
 * Exported Preprocessor #define Constants
\*============================================================================*/
#define CIRCULARBUFFERSIZE 5

/*============================================================================*\
 * Exported Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Exported Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Object Declarations
\*============================================================================*/
//MQTT Topics
extern const char* MQTT_TOPIC;
//MQTT Pub Topics
extern const char* MQTT_PUBTOPIC_NIVAGUAREDR;
extern const char* MQTT_PUBTOPIC_NIVAGUACUARIO;
extern const char* MQTT_PUBTOPIC_ALARMARDR;
extern const char* MQTT_PUBTOPIC_ALMRDR_NIVRDR;
extern const char* MQTT_PUBTOPIC_ALMRDR_OVRTME;
extern const char* MQTT_PUBTOPIC_VFTE;
extern const char* MQTT_PUBTOPIC_HBEAT;
//MQTT Sub Topic
extern const char* MQTT_SUBTOPICGRAL;
extern const char* MQTT_SUBTOPIC;
//MQTT SSubs Topics
extern const char* MQTT_SSUBTOPIC_ACTDESFUNRDR;
extern const char* MQTT_SSUBTOPIC_RELLENAR;
//MQTT Pub state Topic
extern const char* MQTT_STATE;

extern const char HOST_MQTT_DEVICE_ID[];

extern const char MQTT_STATE_ON[];
extern const char MQTT_STATE_OFF[];
extern const char MQTT_STATE_NVACBIEN[];
extern const char MQTT_STATE_NVACBAJO[];

extern const char MQTT_IPSERVER[];
extern const char MQTT_PORT[];

extern bool MsjMQTTRcb;

/*============================================================================*\
 * Exported Function Prototypes
 * ----------------------------------------------------------------------------
 * The header information for each of the following function prototypes is 
 * located in the source file
\*============================================================================*/
extern void Fun_MessageReceived(String &topic, String &payload);
extern void Fun_Analizar_MsjRc(void);

/*============================================================================*\
 * Exported In-line Function Definitions (#define function-like macros)
\*============================================================================*/
#define MQTTPUBNIVRELLENADOR(NIVELRDR_EN_PORCIENTO) clientmqttc.publish(String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_NIVAGUAREDR), String(NIVELRDR_EN_PORCIENTO).c_str())
#define MQTTPUBNIVRELLENADOR_ST(NIVELRDR_ST) clientmqttc.publish(String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_NIVAGUAREDR) + String(MQTT_STATE), NIVELRDR_ST)
#define MQTTPUBNIVACUARIO(NIVELACUARIO_ST) clientmqttc.publish(String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_NIVAGUACUARIO) + String(MQTT_STATE), NIVELACUARIO_ST)
#define MQTTPUBRELLENANDO_ST(RDR_ST) clientmqttc.publish(String(MQTT_TOPIC) + String(MQTT_SSUBTOPIC_RELLENAR) + String(MQTT_STATE), RDR_ST)
#define MQTTPUBACTDESFUNRDR_ST(ACTDESFUNRDR_ST) clientmqttc.publish(String(MQTT_TOPIC) + String(MQTT_SSUBTOPIC_ACTDESFUNRDR) + String(MQTT_STATE), ACTDESFUNRDR_ST)
#define MQTTPUBALMRDROVRTME_ST(ALMRDROVRTME_ST) clientmqttc.publish(String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_ALARMARDR) + String(MQTT_PUBTOPIC_ALMRDR_OVRTME), ALMRDROVRTME_ST);
#define MQTTPUBALMRDRNIVRDR_ST(ALMRDRNIVRDR_ST)clientmqttc.publish(String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_ALARMARDR) + String(MQTT_PUBTOPIC_ALMRDR_NIVRDR), ALMRDRNIVRDR_ST);
#define MQTTPUBHBEAT() clientmqttc.publish((String(MQTT_TOPIC) + String(MQTT_PUBTOPIC_HBEAT)), String(Heart_Beat).c_str())

/*============================================================================*\
 * Exported Preprocessor Declaration
\*============================================================================*/
extern CircularBuffer<String, CIRCULARBUFFERSIZE> Mqtt_PL_Rcd_Bfr;

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
#endif /* } FW_MQTT_H */