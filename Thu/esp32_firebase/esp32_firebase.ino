#include <ArduinoJson.h>

#include <FirebaseESP32.h>
#include <FirebaseESP32HTTPClient.h>
#include <FirebaseJson.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <SoftwareSerial.h>

#define WIFI_SSID "Hoang_Loan_Khach"
#define WIFI_PASSWORD "11111111"

#define FIREBASE_HOST "https://steam-94d76-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "SJmbpJvtV7noLMLGXqP2E2MtCrwPu6He8R5fMaEv"

FirebaseData firebasedata;

String nodo = "/Sensores/";
bool iterar = true;

SoftwareSerial mySerial(2, 4); //TX, RX

void setup() {
Serial.begin(9600);
mySerial.begin(9600);
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("Connecting to wifi");
while (WiFi.status()!= WL_CONNECTED){
  Serial.print(".");
  delay(300);
}
Serial.println();
Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
Firebase.reconnectWiFi(true);
}

void loop() {
if (mySerial.available() > 1) { //Read from arduino
String input = mySerial.readString();
Serial.println(input);

StaticJsonDocument<300> doc;
deserializeJson(doc,input);
float airValue=doc["air"];
float humidityValue=doc["humidity"];
float temperatureValue=doc["temp"];
float lightValue=doc["light"];
float uvValue=doc["uv"];
float watertempValue=doc["watertemp"];
float phValue=doc["ph"];

//float airValue=object.get<float>("air");
//float humidityValue=object.get<float>("humidity");
//float temperatureValue=object.get<float>("temp");
//float lightValue=object.get<float>("light");
//float uvValue=object.get<float>("uv");
//float watertempValue=object.get<float>("watertemp");
//float phValue=object.get<float>("ph");

Serial.println(watertempValue);

Firebase.setFloat(firebasedata, nodo + "/Air",airValue);
Firebase.setFloat(firebasedata, nodo + "/Humidity",humidityValue);
Firebase.setFloat(firebasedata, nodo + "/Light",lightValue);
Firebase.setFloat(firebasedata, nodo + "/Temperature",temperatureValue);
Firebase.setFloat(firebasedata, nodo + "/UV",uvValue);
Firebase.setFloat(firebasedata, nodo + "/WarterTemp",watertempValue);
Firebase.setFloat(firebasedata, nodo + "/pH",phValue);
}}
