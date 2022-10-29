#if !defined(ESP32)
  #error This code is intended to run only on the ESP32 boards ! Please check your Tools->Board setting.
#endif

#include <Arduino.h>

#include <EEPROM.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WebSocketsServer_Generic.h>
#include "ESPAsyncWebServer.h"
#include <FastLED.h>

#define USE_SERIAL Serial
#define PIN_LED_1     2
#define PIN_LED_2     13
#define PIN_LED_3     15
//#define PIN_LED_4     2
#define NUM_CHANNELS   3
#define NUM_LEDS     1024 // 23.25 FPS for 1200 LEDS
#define LED_BRIGHTNESS 85
#define WEBSOCKET_PORT 7171
#define EEPROM_SIZE 3072
const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
WebSocketsServer webSocket(WEBSOCKET_PORT);
WiFiMulti wiFiMulti;
AsyncWebServer  server = AsyncWebServer(80);
CRGB leds[NUM_CHANNELS][NUM_LEDS];
IPAddress apIP(192, 168, 1, 100);
byte mac[6];
String apSsid = "LedX";
String apPassword = "ledx1001";
bool newWsData = false;
int startTimeP=0, startTimeL=0,startTimeT=0, latencyL = 0,latencyP=0, latencyT = 0;

void startWebSocket(){
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  USE_SERIAL.println("== WebSocket started");
}
char buffer[EEPROM_SIZE];
int chan=0, i = 0, j=0,k=0, dataSize=0;
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  IPAddress ip = webSocket.remoteIP(num);
  IPAddress softIp = WiFi.softAPIP();
  switch(type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[WSc=%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      USE_SERIAL.printf("[WSc=%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      if(hasTemplate()){
        USE_SERIAL.printf("[WSc=%u] Has template!\n", num);
        sprintf(buffer, "{\"command\": \"IDENTIFY\", \"id\": \"%s\", \"ip\": \"%s\", \"type\": \"DRIVER\", \"state\": \"READY\", \"channels\": %d, \"leds\": %d, \"template\": \"%s\"}", apSsid.c_str(), WiFi.softAPIP().toString().c_str(), NUM_CHANNELS, NUM_LEDS, readTemplate().c_str());
//        sprintf(buffer, "{\"command\": \"IDENTIFY\", \"id\": \"%s\", \"ip\": \"%s\", \"type\": \"DRIVER\", \"state\": \"READY\", \"channels\": %d, \"leds\": %d}", apSsid.c_str(), WiFi.softAPIP().toString().c_str(), NUM_CHANNELS, NUM_LEDS);
      } else{
        USE_SERIAL.printf("[WSc=%u] Doesn't have template!\n", num);
        sprintf(buffer, "{\"command\": \"IDENTIFY\", \"id\": \"%s\", \"ip\": \"%s\", \"type\": \"DRIVER\", \"state\": \"READY\", \"channels\": %d, \"leds\": %d}", apSsid.c_str(), WiFi.softAPIP().toString().c_str(), NUM_CHANNELS, NUM_LEDS);
      }
      USE_SERIAL.printf("[WSc=%u] Handshake: %s\n", num, buffer);
      webSocket.sendTXT(num,buffer);
      break;
    case WStype_TEXT:
      USE_SERIAL.printf("[WSc=%u] get text: %s size:%d\n", num, payload, length);
      sprintf(buffer, "%s", payload);
      saveTemplate(String(buffer));
      break;
    case WStype_BIN:
//      USE_SERIAL.printf("[WSc=%u] get BIN size:%d\n", num, length);
      startTimeP = millis();
      j=0;
      for(chan=0; chan < NUM_CHANNELS && j < length; chan++){
        dataSize = payload[j]*20 + payload[j+1];
        j+=2;
        USE_SERIAL.printf("\n[channel=%d] bin: %d, mtx=[%d]\n", chan, length, dataSize);
        for(i=0; i < dataSize && j+3 <= length; i++, j+=3){
          if(i < NUM_LEDS){
            leds[chan][i] = CRGB(pgm_read_byte(&gamma8[payload[j]]), pgm_read_byte(&gamma8[payload[j+1]]), pgm_read_byte(&gamma8[payload[j+2]]));
          }
        }
        for(;i<NUM_LEDS;i++){
          leds[chan][i] = CRGB(0,0,0);
        }
      }
      newWsData = true;
      latencyP = millis()-startTimeP;
      break;
    case WStype_ERROR:      
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
      break;
  }
}

void startAP(){
  USE_SERIAL.println("Config Wifi SoftAP...");
  if (!WiFi.softAPConfig (apIP, apIP, IPAddress(255, 255, 255, 0))) {
    USE_SERIAL.println("SoftAP Config Failed.");
  }
  apSsid = "LedX-";
  apSsid += mac[5];
  apSsid += mac[3];
  if(!WiFi.softAP(apSsid.c_str(), apPassword.c_str())){
    USE_SERIAL.println("SoftAP Failed to start");
  }
  USE_SERIAL.printf("AP IP address: %s\n", WiFi.softAPIP().toString().c_str());
  delay(500);
}

#include "ledx3_html.h"
#include "global_css.h"
#include "global_js.h"
#include "canvas_js.h"
#include "connection_js.h"
#include "timeoutworker_js.h"
#include "templates_json.h"
void startServer(){
  server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request){
    request->send(200, "text/html", FPSTR(ledx3_html_h));
  });
  server.on("/global.css", HTTP_GET, [&](AsyncWebServerRequest *request){
    request->send(200, "text/css", FPSTR(global_css_h));
  });
  server.on("/canvas.js", HTTP_GET, [&](AsyncWebServerRequest *request){
    request->send(200, "application/javascript", FPSTR(canvas_js_h));
  });
  server.on("/global.js", HTTP_GET, [&](AsyncWebServerRequest *request){
    request->send(200, "application/javascript", FPSTR(global_js_h));
  });
  server.on("/connection.js", HTTP_GET, [&](AsyncWebServerRequest *request){
    request->send(200, "application/javascript", FPSTR(connection_js_h));
  });
  server.on("/timeout-worker.js", HTTP_GET, [&](AsyncWebServerRequest *request){
    request->send(200, "application/javascript", FPSTR(timeoutworker_js_h));
  });
  server.on("/templates.json", HTTP_GET, [&](AsyncWebServerRequest *request){
    request->send(200, "application/javascript", FPSTR(templates_json_h));
  });
  server.begin();
}

bool hasTemplate(){
  int newStrLen = EEPROM.read(0)*20+EEPROM.read(1);
  if(newStrLen==0){
    return false;
  }
  for (int i = 0; i < newStrLen; i++) {
    int c = EEPROM.read(2 + i);
    if(c< 32||c>126){
      return false;
    }
  }
  return true;
}

void saveTemplate(const String &eTemplate) {
  int len = eTemplate.length();
  EEPROM.write(0, len/20);
  EEPROM.write(1, len%20);
  for (int i = 0; i < len; i++) {
    EEPROM.write(2 + i, eTemplate[i]);
  }
  EEPROM.commit();
  USE_SERIAL.printf("== EEPROM saved: [%s]\n\n", eTemplate.c_str());
  readTemplate();
}

String readTemplate(){
  int newStrLen = EEPROM.read(0)*20+EEPROM.read(1);
  if(newStrLen==0){
    return "";
  }
  char eTemplate[newStrLen+1];
  for (int i = 0; i < newStrLen; i++) {
    eTemplate[i] = EEPROM.read(2 + i);
  }
  eTemplate[newStrLen] = '\0';
  USE_SERIAL.printf("== EEPROM read: size:[%d] text:[%s]\n\n", newStrLen,eTemplate);
  return String(eTemplate);
}

void setup() {
  USE_SERIAL.begin(115200);
  USE_SERIAL.setDebugOutput(true);
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();
  for(uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }
  USE_SERIAL.printf("== Serial started...\n");
  
  EEPROM.begin(EEPROM_SIZE);
  USE_SERIAL.println("== EEPROM started...\n");
  
  WiFi.macAddress(mac);
  USE_SERIAL.println("== Mac address read.");
  USE_SERIAL.printf("%d-%d-%d-%d-%d-%d\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
  
  FastLED.addLeds<WS2811, PIN_LED_1, RGB>(leds[0], NUM_LEDS);
  FastLED.addLeds<WS2811, PIN_LED_2, RGB>(leds[1], NUM_LEDS);
  FastLED.addLeds<WS2811, PIN_LED_3, RGB>(leds[2], NUM_LEDS);
//  FastLED.addLeds<WS2811, PIN_LED_4, RGB>(leds[3], NUM_LEDS);
  FastLED.setBrightness(LED_BRIGHTNESS);
  USE_SERIAL.printf("== Pixels started..\n");
  
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  delay(100);
  USE_SERIAL.println("== Setup WiFi.");
  
  startAP();
  USE_SERIAL.println("== Soft AP started.");
  
  startServer();
  USE_SERIAL.println("== Web Server started");

  startWebSocket();
  startTimeP = millis();
  startTimeL = millis();
  startTimeT = millis();
}

void loop() {
  webSocket.loop();
  if(newWsData){
    startTimeL = millis();
    FastLED.show(); // 25 ms for 800 Leds
    latencyL = millis()-startTimeL;
    latencyT = millis()-startTimeT;
    startTimeT = millis();
    newWsData = false;
  }
}
