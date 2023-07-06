# datalogger

Logging humidity and temperature with an ESP32 and SCD22 sensor.
The Sensor should work off grid so a battery- and SD card module will be included in this project.
Since the chip will go into deep sleep to save battery, we are going to add an RTC to accurately measure the time.

## Components

- ESP32 Microcontroller: The ESP32 D1 Mini will be used as the main controller for the data logger. It is well-suited for low-power applications.
- SCD22 Sensor
- Battery Module
- SD Card Module
- RTC Module: An RTC module will be integrated into the system to accurately measure and maintain the time even when the microcontroller is in deep sleep mode.

## Features

- Logging: The data logger will record humidity and temperature measurements at regular intervals and store them on the SD card.
- Battery-powered: The data logger will operate using batteries, providing portability and allowing it to be used in remote or off-grid locations.
- Deep sleep: To conserve power, the microcontroller will enter deep sleep mode between measurements, extending the battery life.
- Accurate timekeeping: The RTC module will ensure precise time measurements, even when the microcontroller is in deep sleep mode.
- Easy data retrieval: The logged data can be easily retrieved by accessing the SD card.

## Acknowledgments

Parts of this project were inspired by the work of Rui Santos from RandomNerdTutorials.com. 

For more details and tutorials, you can visit [https://RandomNerdTutorials.com](https://RandomNerdTutorials.com).

