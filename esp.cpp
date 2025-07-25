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
void buzzerSound() {
    static bool ascending = true;
    if (currentState == DISTRESS) {
        if (ascending) {
            buzzerFreq += 50;
            if (buzzerFreq >= 2000) ascending = false;
        } else {
            buzzerFreq -= 50;
            if (buzzerFreq <= 500) ascending = true;
        }
        tone(buzzerPin, buzzerFreq);
    }
}
void sendStatus() {
    doc.clear();
    doc["device_id"] = device_id;
    doc["status"] = currentState == DISTRESS ? "distress" : 
                    currentState == PENDING ? "pending" : "normal";
    
    String json;
    serializeJson(doc, json);
    wsClient.sendData(json);
}
void setup() {
    // Initialize pins
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(ledPin, OUTPUT);
    pinMode(buzzerPin, OUTPUT);
    
    // Initialize serial
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Connect to WebSocket
    wsClient.begin(ws_host, ws_port, ws_path);
    
    // Setup interrupt
    attachInterrupt(digitalPinToInterrupt(buttonPin), handleButton, FALLING);
    
    // Start normal state blinking
    normalBlinkTimer.attach(600, normalBlink); // 10 minutes
}
void loop() {
    // Handle WebSocket events
    wsClient.onMessage([](String message) {
        doc.clear();
        deserializeJson(doc, message);
    if (doc["command"] == "cancel" && currentState == DISTRESS) {
            currentState = PENDING;
            noTone(buzzerPin);
            distressBlinkTimer.detach();
            digitalWrite(ledPin, LOW);
            sendStatus();
        }
    });
 // Handle button press
    if (buttonPressed && currentState != DISTRESS) {
        currentState = DISTRESS;
        normalBlinkTimer.detach();
        distressBlinkTimer.attach_ms(200, distressBlink); // Fast blinking
        buzzerTimer.attach_ms(100, buzzerSound); // Start buzzer
        sendStatus();
        buttonPressed = false;
    }
    
