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
- 10kΩ Pull-up Resistor between DATA and 3.3V

### Relay Module

- VCC -> 5V
- GND -> GND
- IN1 -> GPIO26
- IN2 -> GPIO27

### OLED Display

- VCC -> 3.3V
- GND -> GND
- SDA -> GPIO21
- SCL -> GPIO22

### LEDs

- Red LED -> GPIO18
- Green LED -> GPIO19

### Buzzer

- Positive (+) -> GPIO25
- Negative (-) -> GND

### Push Button

- One Side -> GPIO0
- Other Side -> GND

(INPUT_PULLUP used in code)

## Library Versions

- DHTesp v1.19.0
- Adafruit SSD1306 v2.5.13
- Adafruit GFX v1.12.1

## How to Run

1. Connect all components according to the wiring diagram.
2. Install required libraries.
3. Upload the code to ESP32.
4. Open Serial Monitor at 115200 baud.
5. Observe soil moisture percentage, temperature and humidity.
6. Test with dry soil and wet soil.
7. Press the manual override button to force watering.

## Soil Moisture Calibration

### Dry Calibration Value

```text
3200
```

### Wet Calibration Value

```text
1200
```

### Conversion Formula

```cpp
int moisturePercent = map(rawMoisture, 3200, 1200, 0, 100);
moisturePercent = constrain(moisturePercent, 0, 100);
```

### Note

Replace the above calibration values with the actual values measured from your own sensor.

## Soil States

### DRY (<30%)

- Red LED ON
- Relay ON (Pump ON)
- Buzzer Alert

### OPTIMAL (30%–70%)

- Green LED ON
- Pump OFF

### WET (>70%)

- Green LED ON
- Pump OFF

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

This creates a hysteresis gap and improves relay stability.

## Manual Override

Press the push button connected to GPIO0.

Actions:

- Pump turns ON for 5 seconds
- Watering is forced manually
- System automatically returns to auto mode

## Expected Output

Example OLED Display:

```text
Soil: 45%
Temp: 27°C
Status: OPTIMAL
```

A moisture bar graph is also displayed on the OLED.

Example Serial Monitor Output:

```text
Soil: 22% | Temp: 28°C | Status: DRY
Soil: 27% | Temp: 28°C | Status: DRY
Soil: 35% | Temp: 27°C | Status: OPTIMAL
Soil: 48% | Temp: 27°C | Status: OPTIMAL
Soil: 72% | Temp: 26°C | Status: WET
```

## Relay Logic

The relay module is active-LOW.

```cpp
digitalWrite(RELAY1, LOW);
```

means:

```text
Pump ON
```

and

```cpp
digitalWrite(RELAY1, HIGH);
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
- 10kΩ Pull-up Resistor between DATA and 3.3V

### Relay Module

- VCC -> 5V
- GND -> GND
- IN1 -> GPIO26
- IN2 -> GPIO27

### OLED Display

- VCC -> 3.3V
- GND -> GND
- SDA -> GPIO21
- SCL -> GPIO22

### LEDs

- Red LED -> GPIO18
- Green LED -> GPIO19

### Buzzer

- Positive (+) -> GPIO25
- Negative (-) -> GND

### Push Button

- One Side -> GPIO0
- Other Side -> GND

(INPUT_PULLUP used in code)

## Library Versions

- DHTesp v1.19.0
- Adafruit SSD1306 v2.5.13
- Adafruit GFX v1.12.1

## How to Run

1. Connect all components according to the wiring diagram.
2. Install required libraries.
3. Upload the code to ESP32.
4. Open Serial Monitor at 115200 baud.
5. Observe soil moisture percentage, temperature and humidity.
6. Test with dry soil and wet soil.
7. Press the manual override button to force watering.

## Soil Moisture Calibration

### Dry Calibration Value

```text
3200
```

### Wet Calibration Value

```text
1200
```

### Conversion Formula

```cpp
int moisturePercent = map(rawMoisture, 3200, 1200, 0, 100);
moisturePercent = constrain(moisturePercent, 0, 100);
```

### Note

Replace the above calibration values with the actual values measured from your own sensor.

## Soil States

### DRY (<30%)

- Red LED ON
- Relay ON (Pump ON)
- Buzzer Alert

### OPTIMAL (30%–70%)

- Green LED ON
- Pump OFF

### WET (>70%)

- Green LED ON
- Pump OFF

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

This creates a hysteresis gap and improves relay stability.

## Manual Override

Press the push button connected to GPIO0.

Actions:

- Pump turns ON for 5 seconds
- Watering is forced manually
- System automatically returns to auto mode

## Expected Output

Example OLED Display:

```text
Soil: 45%
Temp: 27°C
Status: OPTIMAL
```

A moisture bar graph is also displayed on the OLED.

Example Serial Monitor Output:

```text
Soil: 22% | Temp: 28°C | Status: DRY
Soil: 27% | Temp: 28°C | Status: DRY
Soil: 35% | Temp: 27°C | Status: OPTIMAL
Soil: 48% | Temp: 27°C | Status: OPTIMAL
Soil: 72% | Temp: 26°C | Status: WET
```

## Relay Logic

The relay module is active-LOW.

```cpp
digitalWrite(RELAY1, LOW);
```

means:

```text
Pump ON
```

and

```cpp
digitalWrite(RELAY1, HIGH);
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

