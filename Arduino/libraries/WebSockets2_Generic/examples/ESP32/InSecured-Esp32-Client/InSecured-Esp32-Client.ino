/****************************************************************************************************************************
  InSecured-ESP32-Client.ino
  For ESP32.
  
  Based on and modified from Gil Maimon's ArduinoWebsockets library https://github.com/gilmaimon/ArduinoWebsockets
  to support STM32F/L/H/G/WB/MP1, nRF52 and SAMD21/SAMD51 boards besides ESP8266 and ESP32


  The library provides simple and easy interface for websockets (Client and Server).
  
  Built by Khoi Hoang https://github.com/khoih-prog/Websockets2_Generic
  Licensed under MIT license
 *****************************************************************************************************************************/
/*
 InSecured ESP32 Websockets Client

  This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server (using WSS) in inSecured mode
           Still using encryption, but just not checking CA Certs 
        3. Sends the websockets server a message ("Hello Server from ......")
        4. Sends the websocket server a "ping"
        5. Prints all incoming messages while the connection is open

    NOTE:
    The sketch dosen't check or indicate about errors while connecting to
    WiFi or to the websockets server. For full example you might want
    to try the example named "-Client" (And use the ssl methods).

    This so-called "insecured mode"
    
    1) permits you to actually connect to a TLS server (port 443,9443, etc., especially with expired CA Certs). 
    2) It won’t let you connect to plain non-SSL server because full encryption is still required.
    3) You never have to worry about CA Certs’ update and to include every trusted CA root certificate of every TLS server
    4) Still requires encryption. It just does not validate the certificate or fingerprint. 
    5) Insecure because we could be subject to a MITM (Man-In-The-Middle) attack.
    6) It’s still much better than plain non-SSL mode which just communicates using no encryption at all.    

  Hardware:
        For this sketch you only need an ESP32 board.

  Originally Created  : 15/02/2019
  Original Author     : By Gil Maimon
  Original Repository : https://github.com/gilmaimon/ArduinoWebsockets

*****************************************************************************************************************************/

#include "defines.h"

#include <WebSockets2_Generic.h>
#include <WiFi.h>

using namespace websockets2_generic;

void onMessageCallback(WebsocketsMessage message) 
{
  Serial.print("Got Message: ");
  Serial.println(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data) 
{
  (void) data;
  
  if (event == WebsocketsEvent::ConnectionOpened) 
  {
    Serial.println("Connnection Opened");
  } 
  else if (event == WebsocketsEvent::ConnectionClosed) 
  {
    Serial.println("Connnection Closed");
  } 
  else if (event == WebsocketsEvent::GotPing) 
  {
    Serial.println("Got a Ping!");
  } 
  else if (event == WebsocketsEvent::GotPong) 
  {
    Serial.println("Got a Pong!");
  }
}

WebsocketsClient client;

void setup() 
{
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\nStarting WebSockets InSecured-ESP32-Client on " + String(ARDUINO_BOARD));
  Serial.println(WEBSOCKETS2_GENERIC_VERSION);
  
  // Connect to wifi
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) 
  {
    Serial.print(".");
    delay(1000);
  }

  // Check if connected to wifi
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("No Wifi!");
    return;
  }

  Serial.print("\nConnected to Wifi, Connecting to WebSockets Server @");
  Serial.println(websockets_connection_string);

  // run callback when messages are received
  client.onMessage(onMessageCallback);

  // run callback when events are occuring
  client.onEvent(onEventsCallback);

#if USING_INSECURE_MODE
  client.setInsecure();
#else
  // Before connecting, set the ssl fingerprint of the server
  client.setCACert(echo_org_ssl_ca_cert);
#endif
  
  // Connect to server
  bool connected = client.connect(websockets_connection_string);

  if (connected) 
  {
    Serial.println("Connected!");

    String WS_msg = String("Hello to Server from ") + ARDUINO_BOARD;
    client.send(WS_msg);

    // Send a ping
    client.ping();
  } 
  else 
  {
    Serial.println("Not Connected!");
  }
}

void loop() 
{
  client.poll();
}
