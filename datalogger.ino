#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define dataloggerID "/left.txt"

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
  if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }
  
  uint8_t cardType = SD.cardType();
  appendFile(SD, dataloggerID, "home,room=Living\ Room temp=21.1,hum=35.9,co=0i 1641024000\n");
  appendFile(SD, dataloggerID, "home,room=Kitchen temp=21.0,hum=35.9,co=0i 1641024000\n");
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop(){

}