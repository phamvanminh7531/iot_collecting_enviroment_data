#include <ArduinoJson.h>
//Include thư viện
#include <OneWire.h>
#include <DallasTemperature.h>

// Chân nối với Arduino
#define ONE_WIRE_BUS 7
//Thiết đặt thư viện onewire
OneWire oneWire(ONE_WIRE_BUS);
//Mình dùng thư viện DallasTemperature để đọc cho nhanh
DallasTemperature sensors(&oneWire);
#include <DHT.h> // Gọi thư viện DHT22
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); //TX, RX
// gnd SET_A and SET_B for Normal Mode (Send and Receive)

const int DHTPIN = A3; //Đọc dữ liệu từ DHT22 ở chân A3 trên mạch Arduino
const int DHTTYPE = DHT22; //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin(); // Khởi động cảm biến
  mySerial.begin(9600);
  sensors.begin();

}

void loop() {
  //Khong khi
  int AIR_sensorValue = analogRead(A0);

  //Anh sang
  int UV_sensor_value=analogRead(A5);
  int volts = UV_sensor_value * 5.0 / 1024.0;
  int UV_index = (volts * 10);

  //Nhiet do do am
  int h = dht.readHumidity(); //Đọc độ ẩm
  int t = dht.readTemperature(); //Đọc nhiệt độ
 

  //Nhiet do nuoc
  sensors.requestTemperatures();
  int waterTemp = sensors.getTempCByIndex(0);

  //pH
  int ecMeterReading = analogRead(A1);            // read the voltage from the EC Meter
  int ecVoltage= (ecMeterReading * 5.0);
  int ec=ecVoltage/1024;
  int ph=(ec*700);

  Serial.println( ph );
  Serial.println(AIR_sensorValue);
  Serial.println(UV_sensor_value);
  Serial.println(UV_index);
  Serial.println(h);
  Serial.println(t);
  Serial.println(waterTemp);

  StaticJsonBuffer<100> jBuffer;
  JsonObject& root=jBuffer.createObject();
  root["a"]=AIR_sensorValue;
  root["h"]=h;
  root["t"]=t;
  root["l"]=UV_sensor_value;
  root["u"]=UV_index;
  root["wt"]=waterTemp;
  root["ph"]=ph;
  root.prettyPrintTo(mySerial);
  mySerial.println();
  delay(6000);

}
