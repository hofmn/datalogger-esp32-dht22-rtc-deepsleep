#include <DHT_U.h>
#include <DHT.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <Wire.h>
#include <RTClib.h>

// Define Name and ID of the Datalogger here, to easily change later
#define fileName "/data.txt"
#define dataloggerID "left"
// Define DHT-Sensor Type and Pin for Sensor input
#define dhtType DHT22
#define dhtPin 17
// Define some parameters to measure the battery state
#define sensivity (3.3 / 4095.0) // The max. input value of the ESP32 is 3.3V and the resolution is 16 bits, so 2^16 = 4095
#define measurementRatio (5/3.3) // Since the voltage of the battery can be higher than 3.3V, we connect it to the 5V pin of the ESP and use a voltage divider to measure it at a GPIO pin.
#define minVoltage (2.75/measurementRatio) // The min and max voltage depend on the battery and should be found in the datasheet
#define maxVoltage (4.2/measurementRatio)
float batteryPercentage;

RTC_DS3231 rtc;

DHT dht(dhtPin, dhtType);
float t;
float h;

// Function to append data to a file
void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

// Function to set up the SD card
void sdCardSetup() {
  if (!SD.begin(5)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

// Function to map a float value from one range to another
float mapf(float analog_read, float min_Voltage, float max_voltage,
           float zero, float hundred) {
  float a = analog_read - min_Voltage;
  float b = hundred - zero;
  float c = max_voltage - min_Voltage;
  return a * b / c + zero;
}

// Function to set up the battery measurement
void batterySetup() {
  float rawADC = analogRead(A0);
  Serial.println("RawADC: " + String(rawADC));
  float processedADC = rawADC * sensivity;
  Serial.println("processedADC: " + String(processedADC));
  batteryPercentage = mapf(processedADC, minVoltage, maxVoltage, 0, 100);
  Serial.println("batteryPercentage: " + String(batteryPercentage));
}

// Function to set up the RTC (Real-Time Clock)
void rtcSetup() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
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
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("RTC lost power");
    appendFile(SD, fileName, "--- RTC LOST POWER --- \n");
  }
}

// Function to set up the DHT sensor
void dhtSetup() {
  dht.begin();
  delay(2000);
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    appendFile(SD, fileName, "--- DHT READ FAILED --- \n");
  }
}

// Function to write data to the file in the inline Influxdb format
void writeData() {
  DateTime now = rtc.now();
  String time = String(now.unixtime());
  String writeData = "hydro_greenhouse,position=" + String(dataloggerID) + " temp=" + String(t) +
                     ",hum=" + String(h) + ",bat=" + String(batteryPercentage) + " " + time + "\n";
  appendFile(SD, fileName, (writeData).c_str());
  Serial.print(writeData);
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  batterySetup();
  sdCardSetup();
  rtcSetup();
  dhtSetup();
  writeData();
  esp_deep_sleep(5500000);
}

void loop() {
  // Empty loop as there's no need for continuous execution
}
