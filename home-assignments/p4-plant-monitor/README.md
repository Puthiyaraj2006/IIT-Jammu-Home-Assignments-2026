# Project 4 - Smart Plant Watering Monitor

## Components Used

- ESP32 Dev Board
- Capacitive Soil Moisture Sensor
- DHT11 Temperature & Humidity Sensor
- 2-Channel Relay Module
- OLED Display (SSD1306 I2C)
- Red LED
- Green LED
- Active Buzzer
- Push Button
- Breadboard
- Jumper Wires

## Wiring Diagram (Text)

### Soil Moisture Sensor
- VCC -> 3.3V
- GND -> GND
- AOUT -> GPIO34

### DHT11 Sensor
- VCC -> 3.3V
- GND -> GND
- DATA -> GPIO4

### Relay Module
- VCC -> 5V
- GND -> GND
- IN1 -> GPIO26 (Water Pump Relay)
- IN2 -> GPIO27

### OLED Display
- VCC -> 3.3V
- GND -> GND
- SDA -> GPIO21
- SCL -> GPIO22

### LEDs
- Red LED -> GPIO25
- Green LED -> GPIO33

### Buzzer
- Positive (+) -> GPIO32
- Negative (-) -> GND

### Push Button
- One Side -> GPIO13
- Other Side -> GND
- INPUT_PULLUP mode used in code

## Library Versions

- DHTesp
- Adafruit SSD1306
- Adafruit GFX

## How to Run

1. Connect all components according to the wiring diagram.
2. Install the required libraries.
3. Upload the code to ESP32.
4. Open Serial Monitor at 115200 baud.
5. Observe soil moisture, temperature and humidity readings.
6. Test using dry and wet soil conditions.
7. Press the push button to activate manual watering mode.

## Soil Moisture Calibration

### Dry Value

```text
3200
```

### Wet Value

```text
1200
```

### Conversion Formula

```cpp
moisture = map(sensorReading, 3200, 1200, 0, 100);
moisture = constrain(moisture, 0, 100);
```

## Soil States

### DRY (<30%)

- Water Pump ON
- Red LED ON
- Buzzer ON
- Status = DRY

### OPTIMAL (30% - 70%)

- Water Pump OFF
- Green LED ON
- Buzzer OFF
- Status = OPTIMAL

### WET (>70%)

- Water Pump OFF
- Green LED ON
- Buzzer OFF
- Status = WET

## Hysteresis Logic

To prevent rapid relay switching:

### Pump ON

```text
Moisture < 30%
```

### Pump OFF

```text
Moisture > 40%
```

This hysteresis gap improves stability and prevents frequent relay toggling.

## Manual Override

Pressing the push button connected to GPIO13 starts manual watering.

Features:

- Pump turns ON immediately.
- Pump remains ON for 5 seconds.
- System automatically returns to automatic mode.
- Serial Monitor displays:

```text
Manual Watering Started
Manual Watering Finished
```

## OLED Display Output

Example:

```text
Soil: 45%
Temp: 27.0 C
Status: OPTIMAL
```

A moisture bar graph is displayed at the bottom of the OLED screen.

## Expected Serial Output

```text
SOIL: 22% | TEMP: 28.0 C | HUM: 61% | STATUS: DRY
SOIL: 27% | TEMP: 28.0 C | HUM: 62% | STATUS: DRY
SOIL: 35% | TEMP: 27.0 C | HUM: 63% | STATUS: OPTIMAL
SOIL: 48% | TEMP: 27.0 C | HUM: 64% | STATUS: OPTIMAL
SOIL: 72% | TEMP: 26.0 C | HUM: 65% | STATUS: WET
```

## Relay Logic

The relay module is active-LOW.

```cpp
digitalWrite(WATER_PUMP, LOW);
```

means:

```text
Pump ON
```

and

```cpp
digitalWrite(WATER_PUMP, HIGH);
```

means:

```text
Pump OFF
```

## Serial Log Screenshot

Upload and include:

```text
soil-moisture-log.png
```

The screenshot should show at least 5 moisture readings.

## Demo Video

Google Drive Link:

https://drive.google.com/file/d/15hbwtoMmDq06AsR_iePAb-TbkpOrcrF2/view?usp=sharing

## Author

Puthiyaraj P
