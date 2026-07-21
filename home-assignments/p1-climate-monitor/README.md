# Project 1 - Smart Room Climate Monitor

## Components Used
- ESP32 Dev Board
- DHT11 Sensor
- OLED Display (SSD1306)
- Buzzer
- Red LED
- Green LED
- 220Ω Resistors
- Breadboard
- Jumper Wires

## Wiring Summary
- DHT11 DATA -> GPIO4
- OLED SDA -> GPIO21
- OLED SCL -> GPIO22
- Red LED -> GPIO25
- Green LED -> GPIO26
- Buzzer -> GPIO27

## How to Run
1. Install DHTesp library.
2. Install Adafruit SSD1306 library.
3. Connect components as per wiring.
4. Upload code to ESP32.
5. Open Serial Monitor at 115200 baud.

## Expected Output
- OLED displays temperature and humidity.
- Green LED glows during normal conditions.
- Red LED and buzzer activate when:
  - Temperature > 38°C
  - Humidity > 80%
- Serial Monitor logs data every 5 seconds.

## Author
Puthiyaraj P

## Demo Video
https://drive.google.com/file/d/1YMgbYx_4tTFUb71QpfiSyKKb2clf5zXA/view?usp=sharing
