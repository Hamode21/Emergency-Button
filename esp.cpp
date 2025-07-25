#include <WiFi.h>           
#include <PubSubClient.h>   
#include <ArduinoJson.h>   
const int buttonPin = 4;   
const int ledPin = 5;      
const int buzzerPin = 18; 
const char* ssid = "SSID_שלך";
const char* mqtt_server = "broker.hivemq.com";
void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}
PubSubClient client(espClient);
client.setServer(mqtt_server, 1883);
client.setCallback(callback); 
