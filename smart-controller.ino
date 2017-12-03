#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "ZTEe";
const char* password = "6725516dasha";

bool isActive = false;
bool isConnected = false;

IPAddress ip(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0); 

ESP8266WebServer server(85);

void highImpulse(int pin) {
  digitalWrite(pin, 1);
  delay(100);
  digitalWrite(pin, 0);
}

void handleToggle() {
  isActive = !isActive;
  highImpulse(D7);
  server.send(200, "application/json", "{ \"message\": \"toggled\" }");
}

void handleSetupHigh() {
  highImpulse(D7);
  server.send(200, "application/json", "{ \"message\": \"toggled\" }");
}

void handleConnect() {
  isConnected = true;
  digitalWrite(D4, LOW);
  server.send(200, "application/json", "{ \"message\": \"connected\" }");
}

void handleDisconnect() {
  isConnected = false;
  digitalWrite(D4, HIGH);
  server.send(200, "application/json", "{ \"message\": \"disconnected\" }");
}

void handleStatus() {
  if (isActive) {
    server.send(200, "application/json", "{ \"status\": true }");
  } else {
    server.send(200, "application/json", "{ \"status\": false }");
  }
}

void setupPins() {
  pinMode(D7, OUTPUT);
  pinMode(D4, OUTPUT);
  
  digitalWrite(D4, HIGH);
  digitalWrite(D7, LOW);
}

void connectToWifi() {
  Serial.begin(115200);
  
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  
  Serial.println("");

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


}

void setup(void){
  setupPins();
  
  connectToWifi();
  
  server.on("/toggle", handleToggle);
  server.on("/setupHigh", handleSetupHigh);
  server.on("/connect", handleConnect);
  server.on("/disconnect", handleDisconnect);
  server.on("/status", handleStatus);
  
  server.begin();
}

void activatePins() {}

void loop(void){
  server.handleClient();
  activatePins();
}
