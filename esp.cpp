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
void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp = parseMessage(message, length);
  if (String(topic) == "emergency/control") {
      }
}

void IRAM_ATTR buttonInterrupt() {
  buttonPressed = true; // מסמן שהלחצן נלחץ
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonInterrupt, FALLING);
}

if (buttonPressed && currentState == NORMAL) {
  currentState = EMERGENCY;
  sendAlert(); 
}
enum SystemState { NORMAL, EMERGENCY, ACKNOWLEDGED };
SystemState currentState = NORMAL;
switch (currentState) {
  case NORMAL:
    blinkLedSlow(); 
    break;
  case EMERGENCY:
    blinkLedFast(); 
    activateBuzzer(); 
    break;
  case ACKNOWLEDGED:
    turnOffAlerts(); 
    break;
}

void blinkLedSlow() {
  if (millis() - previousMillis >= 600000) { 
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
  }
}
