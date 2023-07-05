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
