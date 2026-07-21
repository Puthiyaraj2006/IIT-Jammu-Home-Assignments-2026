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

## Wiring Diagram 

DHT11
- VCC -> 3.3V
- GND -> GND
- DATA -> GPIO4

OLED
- VCC -> 3.3V
- GND -> GND
- SDA -> GPIO21
- SCL -> GPIO22

Buzzer
- Positive -> GPIO27
- Negative -> GND

## Library Versions
- DHTesp
- Adafruit SSD1306
- Adafruit GFX

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
