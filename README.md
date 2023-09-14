# datalogger

Logging humidity and temperature with an ESP32 and SCD22 sensor. The Sensor should work off grid so a battery- and SD card module will be included in this project. Since the chip will go into deep sleep to save battery, we are going to add an RTC to accurately measure the time.

## Components

- ESP32 Microcontroller
- DHT22 Sensor
- Battery Module
- SD Card Module
- RTC Module

## Features

- Logging: The data logger will record humidity and temperature measurements at regular intervals and store them on the SD card.
- Battery-powered: The data logger will operate using batteries, providing portability and allowing it to be used in remote or off-grid locations.
- Deep sleep: To conserve power, the microcontroller will enter deep sleep mode between measurements, extending the battery life.
- Accurate timekeeping: The RTC module will ensure precise time measurements, even when the microcontroller is in deep sleep mode.
- Easy data retrieval: The logged data can be easily retrieved by accessing the SD card.

## Getting Started

### Hardware Setup

1. Connect the DHT22 sensor to the ESP32 board as follows:
   - VCC pin of the DHT22 to the 3.3V pin of the ESP32.
   - GND pin of the DHT22 to the GND pin of the ESP32.
   - Data pin of the DHT22 to a digital pin (e.g., D35) of the ESP32.

2. Connect the SD card module to the ESP32 board as follows:
   - VCC pin of the SD card module to the 3.3V pin of the ESP32.
   - GND pin of the SD card module to the GND pin of the ESP32.
   - MISO pin of the SD card module to the GPIO pin (e.g., D18) of the ESP32.
   - MOSI pin of the SD card module to the GPIO pin (e.g., D23) of the ESP32.
   - SCK pin of the SD card module to the GPIO pin (e.g., D19) of the ESP32.
   - CS pin of the SD card module to the GPIO pin (e.g., D5) of the ESP32.

3. Connect the Battery Module to the ESP32 board as follows:
   - Connect the positive terminal of the Battery Module to the VIN pin of the ESP32.
   - Connect the negative terminal of the Battery Module to the GND pin of the ESP32.

4. Connect the Voltage Divider (Spannungsteiler) to the ESP32 board as follows:
   - Connect a resistor (e.g., 10k ohm) between the VIN pin of the ESP32 and the positive terminal of the Battery Module.
   - Connect a resistor (e.g., 20k ohm) between the connection point of the first resistor and the GND pin of the ESP32.
   - Connect the junction between the two resistors to an analog input pin (e.g., A0) of the ESP32.

Note: The voltage divider is used to measure the battery voltage. It scales down the voltage from the Battery Module to a range suitable for the ESP32's analog input.

**Disclaimer:**
Please note that the provided hardware setup instructions are for reference purposes only. It is essential to consult the datasheets and documentation of your specific components to ensure correct pin connections and voltage compatibility. Always double-check the pin connections and verify the correct voltage levels before connecting the microcontroller to a power source. Improper connections or voltage levels may damage the components or pose a risk of electrical hazards. The author and contributors of this project disclaim any responsibility for damages or injuries that may occur as a result of following these instructions or using the provided information. It is the user's responsibility to exercise caution and perform proper due diligence when working with electronic components and power sources.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

Parts of this project were inspired by the work of Rui Santos from RandomNerdTutorials.com. For more details and tutorials, you can visit [https://RandomNerdTutorials.com](https://RandomNerdTutorials.com).
