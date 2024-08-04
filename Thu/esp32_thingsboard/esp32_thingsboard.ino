#include "ThingsBoard.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 4); //TX, RX

#include <WiFi.h>


#define WIFI_SSID             "CNTT"
#define WIFI_PASSWORD       "Bdu@Cntt"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN               "4kG8cdhkqORFdrR1wxU7"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   9600

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

void setup() {
  // initialize serial for debugging
  mySerial.begin(SERIAL_DEBUG_BAUD);
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to wifi");
  while (WiFi.status()!= WL_CONNECTED){
  Serial.print(".");
  delay(300);
  }
}

void loop() {
  delay(1000);

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }
  if (mySerial.available() > 1) { //Read from arduino
  String input = mySerial.readString();
  Serial.println(input);
  StaticJsonDocument<200> doc;
  deserializeJson(doc,input);
  float airValue=doc["a"];
  float humidityValue=doc["h"];
  float temperatureValue=doc["t"];
  float lightValue=doc["l"];
  float uvValue=doc["uv"];
  float watertempValue=doc["wt"];
  float phValue=doc["ph"];
  Serial.println(watertempValue);
  Serial.println("Sending data...");
  if (airValue!=0){
  tb.sendTelemetryFloat("air",airValue);
  }
  if (humidityValue!=0){
  tb.sendTelemetryFloat("humidity",humidityValue);
  }
  if (temperatureValue!=0){
  tb.sendTelemetryFloat("temperature",temperatureValue);
  }
  if (lightValue!=0){
  tb.sendTelemetryFloat("light",lightValue);
  }
  if (uvValue!=0){
  tb.sendTelemetryFloat("UV",uvValue);
  }
  if (watertempValue!=0){
  tb.sendTelemetryFloat("watertemp",watertempValue);
  }
  if (phValue!=0){
  tb.sendTelemetryFloat("ph",phValue);
  }

  tb.loop();
}
}
