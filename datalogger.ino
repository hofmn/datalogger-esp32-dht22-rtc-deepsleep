#include <DHT_U.h>
#include <DHT.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Wire.h>
#include <RTClib.h>


#define dataloggerID "/left.txt"
#define dhtPin 17
#define dhtType DHT22
#define rtcSDA 26
#define rtcSCL 18
DHT dht(dhtPin, dhtType);
RTC_DS3231 rtc;

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


void setup(){
  Serial.begin(115200);

  // if(!SD.begin(5)){
  //   Serial.println("Card Mount Failed");
  //   return;
  // }
  // uint8_t cardType = SD.cardType();
  // Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  // Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
  
  dht.begin();

  if (!rtc.begin()) {
    Serial.println("Couldnt´t find RTC");
    while(1);
  }
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));


}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  DateTime now = rtc.now();
  String time = String(now.unixtime());
  String writeData = "hydro_greenhouse,position=left temp=" + String(t) + ",hum=" + String(h) + " " + time + "\n";
  // appendFile(SD, dataloggerID, (writeData).c_str());
  Serial.print(writeData);
}