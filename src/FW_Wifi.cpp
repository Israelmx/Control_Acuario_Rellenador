/*===========================================================================*\
 * FILE: name: FW_Wifi.cpp
 *===========================================================================
 * Copyright 2021 IXGL Electronics., All Rights Reserved.
 * IXGL Electronics
 *---------------------------------------------------------------------------
 * version: 1
 * derived_by: Israel Gomez
 * date_modified: 03/08/2021
 *----------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains the Wifi features for Control_Acuario_Rellenador program.
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
#include "FW_Wifi.h"
#include "FW_MQTT.h"
#include "FW_Debug.h"

/*============================================================================*\
 * Local Preprocessor #define Constants
\*============================================================================*/
#define TIMER_VERIRICAR_WIFI_MQTT 30000 //Quiero si perdi cnx al Wifi y al servidor de MQTT intente reconectar cada 30 segundos

/*============================================================================*\
 * Local Preprocessor #define MACROS
\*============================================================================*/

/*============================================================================*\
 * Local Type Declarations
\*============================================================================*/

/*============================================================================*\
 * Exported Object Definitions
\*============================================================================*/
Connection_Status_T Connection_Status = WIFI_DOWN_MQTT_DOWN;   // Connection status for WiFi and MQTT:

/*============================================================================*\
 * Local Object Definitions
\*============================================================================*/
// WiFi network info.
//const char* SSID = "Gonar2";
//const char* PASSWORD = "1977123456";
const char* SSID = "EatonDemoMX24";
const char* PASSWORD = "eaton54321";


const char UPDATE_USERNAME[] = "Israel";
const char UPDATE_PASSWORD[] = "1234";
const char UPDATE_PATH[] = "/firmware";

/*============================================================================*\
 * Local Function Prototypes
\*============================================================================*/
void Fun_Connection_Status(void);

/*============================================================================*\
 * Local Inline Function Definitions and Function-Like Macros
\*============================================================================*/

/*============================================================================*\
 * Expoerted Preprocessor Declaration
\*============================================================================*/
WiFiClient clientnetwf;
MQTTClient clientmqttc;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

/*******************************************************************************
 * FUNCTION: Fun_Checa_MQTT_Con
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
 * Fun que verifica el estado de cnx del servidor MQTT
 * 
 * PRECONDITIONS:
 * None.
 * 
 * POSTCONDITIONS:
 * None.      
 *
\******************************************************************************/
void Fun_Checa_MQTT_Con()
{
  static long previousmillis_wifi_mqtt_chk;
  static unsigned long currentmillis_wifi_mqtt_chk;

  clientmqttc.loop();
  delay(15);  // <- fixes some issues with WiFi stability

  if(!clientmqttc.connected()) 
  {
    currentmillis_wifi_mqtt_chk = millis();
    if(currentmillis_wifi_mqtt_chk - previousmillis_wifi_mqtt_chk > TIMER_VERIRICAR_WIFI_MQTT)
    {
      previousmillis_wifi_mqtt_chk = currentmillis_wifi_mqtt_chk;
      Connection_Status = WIFI_DOWN_MQTT_DOWN;
      Fun_Connection_Status();
    }
  }
  
}

/*******************************************************************************
 * FUNCTION: Fun_Connection_Status
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
 * Fun que verifica el estado de cnx de Wifi y MQTT
 * 
 * PRECONDITIONS:
 * None.
 * 
 * POSTCONDITIONS:
 * None.      
 *
\******************************************************************************/
void Fun_Connection_Status()
{
  int wifi_status;
  int mqtt_status;

  wifi_status = WiFi.status();
  mqtt_status = clientmqttc.connected();

  #ifdef IDEBUGSERIAL
  Serial.println("Verificando estado de cnx con Wifi y el Servidor MQTT");
  Serial.print("Wifi Status: ");
  Serial.println(wifi_status);
  Serial.print("MQTT Status: ");
  Serial.println(mqtt_status);
  #endif
  
  if (wifi_status != WL_CONNECTED)
  {
    #ifdef IDEBUGSERIAL
    Serial.println("WiFi and MQTT down: reconectar primero al WiFi");
    #endif
    WiFi.reconnect();
  }
  else if(Connection_Status != WIFI_UP_MQTT_UP)
  {
    #ifdef IDEBUGSERIAL
    Serial.println("WiFi UP, MQTT down: reconectar al servidor MQTT");
    #endif

    if(clientmqttc.connect(HOST_MQTT_DEVICE_ID))
    {
      #ifdef IDEBUGSERIAL
      Serial.println("Reconectado al Servidor MQTT");
      #endif
      if(clientmqttc.subscribe((String(MQTT_TOPIC) + String(MQTT_SUBTOPICGRAL)).c_str()))
      {
        #ifdef IDEBUGSERIAL
        Serial.println("ReSuscrito al Topico");
        #endif
        Connection_Status = WIFI_UP_MQTT_UP;
      }
      else
      {
        #ifdef IDEBUGSERIAL
        Serial.println("NO Suscrito al Topico: ");
        #endif
      }
    }
    else
    {
      #ifdef IDEBUGSERIAL
      Serial.println("No se ha podido reconectar al Servidor MQTT");
      #endif
    }
  }
  else
  {
    //do nothing
  }
}

/*******************************************************************************
 * FUNCTION: Fun_WifiConnect
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
 * Fun que conecta por primera vez al Wifi y al servidor MQTT
 * 
 * PRECONDITIONS:
 * None.
 * 
 * POSTCONDITIONS:
 * None.      
 *
\******************************************************************************/
void Fun_WifiConnect()
{
  #ifdef IDEBUGSERIAL
  Serial.print("Conectando al WiFi ");
  #endif

  WiFi.begin(SSID, PASSWORD);

  clientmqttc.begin(MQTT_IPSERVER, atoi(MQTT_PORT), clientnetwf);
  clientmqttc.onMessage(Fun_MessageReceived);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    #ifdef IDEBUGSERIAL
    Serial.print(".");
    #endif
    delay(500);
  }
  #ifdef IDEBUGSERIAL
  Serial.println("");
  Serial.print("WiFi conectado con IP: ");
  Serial.println(WiFi.localIP());
  #endif

  #ifdef IDEBUGSERIAL
  Serial.print("Conectando al Servidor MQTT ");
  #endif
  while(!clientmqttc.connect(HOST_MQTT_DEVICE_ID)) 
  {
    #ifdef IDEBUGSERIAL
    Serial.print(".");
    #endif
    delay(1000);
  }

  #ifdef IDEBUGSERIAL
  Serial.println("");
  Serial.print("Conectado al Servidor MQTT: ");
  Serial.println(MQTT_IPSERVER);
  #endif
  
  if(clientmqttc.subscribe((String(MQTT_TOPIC) + String(MQTT_SUBTOPICGRAL)).c_str()))
  {
    Connection_Status = WIFI_UP_MQTT_UP;
    #ifdef IDEBUGSERIAL
    Serial.print("Suscrito al Topico: ");
    #endif
  }
  else
  {
    #ifdef IDEBUGSERIAL
    Serial.print("No se pudo suscribir al Topico: ");
    #endif
  }
  #ifdef IDEBUGSERIAL
  Serial.println(String(MQTT_TOPIC) + String(MQTT_SUBTOPIC));
  #endif
}

/*******************************************************************************
 * FUNCTION: Fun_HTTP_Web_Server_Config
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
 * Fun para configurar el servicio HTTTP
 *
 * PRECONDITIONS:
 * None.
 *
 * POSTCONDITIONS:
 * None.
 *
\******************************************************************************/
void Fun_HTTP_Web_Server_Config()
{
// Iniciar mDNS a direccion host.local
  if (!MDNS.begin(HOST_MQTT_DEVICE_ID)) 
  {             
    Serial.println("Error iniciando mDNS");
  }
  Serial.println("mDNS iniciado");

  httpUpdater.setup(&httpServer, UPDATE_PATH, UPDATE_USERNAME, UPDATE_PASSWORD);

  httpServer.on("/", [](){
    if(!httpServer.authenticate(UPDATE_USERNAME, UPDATE_PASSWORD))
      return httpServer.requestAuthentication();
    httpServer.send(200, "text/plain", "Hostname: " + String(HOST_MQTT_DEVICE_ID) + "\nMQTT Server: " + String(MQTT_IPSERVER) + "\nMQTT Port: " + String(MQTT_PORT) + "\nMQTT Out Topic: " + String(MQTT_TOPIC) + "\nTo update firmware go to: http://"+ String(HOST_MQTT_DEVICE_ID) + ".local" + String(UPDATE_PATH));
  });
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
}