#include <WiFi.h>
#include <WebSocketClient.h>
#include <ArduinoJson.h>
#include <Ticker.h>
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* ws_host = "YOUR_WEBSOCKET_SERVER";
const int ws_port = 8080;
const char* ws_path = "/ws";
const char* device_id = "DISTRESS_001";
