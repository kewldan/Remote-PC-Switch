#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define SSID F("Covex67")
#define PASSWORD F("135742694")
#define PIN D8
 
ESP8266WebServer server(80);
 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN, OUTPUT);

  digitalWrite(LED_BUILTIN, 0);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  
  static bool blink = false;
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, blink);
    blink ^= 1;
    delay(100);
  }
  digitalWrite(LED_BUILTIN, 1);
 
  MDNS.begin(F("esp"));
  
  server.on(F("/signal"), []() {
    static char* buffer = new char[32];
    snprintf(buffer, 32, "UPTIME: %ldms", millis());

    server.send(200, F("text/plain"), buffer);
    
    digitalWrite(PIN, 1);
    delay(400);
    digitalWrite(PIN, 0);
  });
 
  server.begin();
}
 
void loop() {
  server.handleClient();
  MDNS.update();
}