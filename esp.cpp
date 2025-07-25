#include <WiFi.h>
#include <WebSocketClient.h>
#include <ArduinoJson.h>
#include <Ticker.h>
// WiFi settings לשנות בהתאם
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
//websocket
const char* ws_host = "YOUR_WEBSOCKET_SERVER";
const int ws_port = 8080;
const char* ws_path = "/ws";
const char* device_id = "DISTRESS_001";
// Pins
const int buttonPin = 4;  
const int ledPin = 5;     
const int buzzerPin = 18

// State machine
enum State { NORMAL, DISTRESS, PENDING };
State currentState = NORMAL;

// Timing variables
Ticker normalBlinkTimer;
Ticker distressBlinkTimer;
Ticker buzzerTimer;
bool ledState = false;
int buzzerFreq = 500;
// WebSocket client
WebSocketClient wsClient;
WiFiClient client;
// Button pressed
volatile bool buttonPressed = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
// JSON buffer
StaticJsonDocument<200> doc;
void IRAM_ATTR handleButton() {
    if (millis() - lastDebounceTime > debounceDelay) {
        buttonPressed = true;
        lastDebounceTime = millis();
    }
}
void normalBlink() {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    normalBlinkTimer.once(600, normalBlink);
}
void distressBlink() {
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
}

