#include <DHT_U.h>
#include <DHT.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Wire.h>
#include <RTClib.h>


#define fileName "/data.txt"
#define dataloggerID "left"
#define dhtPin 17
#define dhtType DHT22
#define sensivity (3.3 / 4095.0)
#define measurementRatio (5/3.3)
#define minVoltage (2.75*measurementRatio)
#define maxVoltage (4.2*measurementRatio)

DHT dht(dhtPin, dhtType);
RTC_DS3231 rtc;
float t;
float h;
float batteryPercentage;

float mapf(float analog_read, float min_Voltage, float max_voltage, 
float zero, float hundred) {
 float a = analog_read - min_Voltage;
 float b = hundred - zero;
 float c = max_voltage - min_Voltage;
 return a * b / c + zero;
}

void batterySetup(){
  float rawADC = analogRead(A0);
  float processedADC = rawADC*sensivity;
  batteryPercentage = mapf(processedADC, minVoltage, maxVoltage, 0, 100);
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void sdCardSetup(){
  if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void rtcSetup(){
  if (!rtc.begin()) {
    Serial.println("Couldnt´t find RTC");
    appendFile(SD, fileName, "--- Couldn't find RTC --- \n");
    return;
  }
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.disableAlarm(1);
  rtc.disableAlarm(2);
  rtc.disable32K();
  rtc.writeSqwPinMode(DS3231_OFF);

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
    Serial.println("RTC lost power");
    appendFile(SD, fileName, "--- RTC LOST POWER --- \n");
  }
}

void dhtSetup(){
  dht.begin();
  delay(2000);
  h = dht.readHumidity();
  t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    appendFile(SD, fileName, "--- DHT READ FAILED --- \n");
  }
}

void writeData(){
  DateTime now = rtc.now();
  String time = String(now.unixtime());
  String writeData = "hydro_greenhouse,position=" + String(dataloggerID) + " temp=" + String(t) 
  + ",hum=" + String(h) + ",bat=" + String(batteryPercentage) + " " + time + "\n";
  appendFile(SD, fileName, (writeData).c_str());
  Serial.print(writeData);
}

void setup(){
  delay(1000);
  Serial.begin(115200);
  batterySetup();
  sdCardSetup();
  rtcSetup();
  dhtSetup();
  writeData();
  esp_deep_sleep(5500000);
}

void loop(){
}