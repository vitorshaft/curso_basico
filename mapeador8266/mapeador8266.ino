//copiado do exemplo HelloServer na cara dura

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "SHAFT"
#define STAPSK  "recife2014"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

const int encEsq = 12;
const int encDir = 14;

float pEsq = 0.0;
float pDir = 0.0;

void esq(){
  pEsq+=0.1;
}
void dir(){
  pDir+=0.1;
}

void setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  attachInterrupt(digitalPinToInterrupt(encEsq),esq,CHANGE);
  attachInterrupt(digitalPinToInterrupt(encDir),dir,CHANGE);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  MDNS.update();

}
