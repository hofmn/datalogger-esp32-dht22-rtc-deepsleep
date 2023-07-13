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

DHT dht(dhtPin, dhtType);
RTC_DS3231 rtc;
float t;
float h;

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

void writeData(float t, float h, String ID){
  DateTime now = rtc.now();
  String time = String(now.unixtime());
  String writeData = "hydro_greenhouse,position=" + ID + " temp=" + String(t) 
  + ",hum=" + String(h) + " " + time + "\n";
  appendFile(SD, fileName, (writeData).c_str());
  Serial.print(writeData);
}

void setup(){
  Serial.begin(115200);
  delay(1000);
  sdCardSetup();
  delay(1000);
  rtcSetup();
  delay(1000);
  dhtSetup();
  writeData(t, h, dataloggerID);
  esp_deep_sleep(55*1000000);
}

void loop(){
  //This is not going to be called
}