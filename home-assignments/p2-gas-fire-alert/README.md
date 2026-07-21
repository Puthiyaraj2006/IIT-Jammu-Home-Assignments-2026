# Project 2 - Gas & Fire Safety Alert System

## Components Used

- Arduino Uno R3
- MQ-2 Gas Sensor
- Flame Sensor
- Active Buzzer
- Green LED
- Yellow LED
- Red LED
- 220Ω Resistors
- Breadboard
- Jumper Wires

## Wiring Diagram (Text)

### MQ-2 Gas Sensor
- VCC -> 5V
- GND -> GND
- AOUT -> A0

### Flame Sensor
- VCC -> 5V
- GND -> GND
- DO -> D7
- AO -> A1 (Optional)

### LEDs
- Green LED Anode -> D10
- Yellow LED Anode -> D11
- Red LED Anode -> D12
- All LED Cathodes -> 220Ω Resistor -> GND

### Buzzer
- Positive (+) -> D9
- Negative (-) -> GND

## How to Run

1. Connect all components according to the wiring diagram.
2. Power the Arduino Uno.
3. Wait approximately 1–2 minutes for the MQ-2 sensor warm-up.
4. Upload the Arduino sketch.
5. Open Serial Monitor at 9600 baud.
6. Observe gas percentage, flame status and alert level.
7. Adjust gas sensor values or trigger the flame sensor to test different states.

## Alert Levels

### SAFE State
- Gas Level < 30%
- Flame Not Detected
- Green LED ON
- Buzzer OFF

### WARNING State
- Gas Level 30%–60%
- Yellow LED ON
- Slow Beep (1 beep per second)

### DANGER State
- Gas Level > 60%
- Flame Detected
- Red LED ON
- Continuous Alarm

## Expected Output

### SAFE

```text
GAS: 20% | FLAME: NONE | STATUS: SAFE
```

- Green LED glows.
- No buzzer sound.

### WARNING

```text
GAS: 45% | FLAME: NONE | STATUS: WARNING
```

- Yellow LED glows.
- Buzzer beeps once every second.

### DANGER

```text
GAS: 75% | FLAME: NONE | STATUS: DANGER
```

or

```text
GAS: 40% | FLAME: DETECTED | STATUS: DANGER
```

- Red LED glows.
- Continuous buzzer alarm.

## MQ-2 Warm-Up Requirement

The MQ-2 gas sensor requires approximately 1–2 minutes of warm-up time after power is applied. During this period, sensor readings may fluctuate and should not be considered accurate. Once warmed up, the readings become stable and reliable.

## Why is the Flame Sensor Active-Low?

The flame sensor used in this project is active-low.

This means:

- HIGH = No Flame Detected
- LOW = Flame Detected

Example:

```cpp
bool flameDetected = (digitalRead(7) == LOW);
```

When a flame is detected, the sensor output automatically becomes LOW, which triggers the danger alert.

## Serial Monitor Screenshot

Upload and include:

```text
serial-monitor-output.png
```

The screenshot should clearly show SAFE, WARNING and DANGER states.

## Demo Video

Google Drive Link:

https://drive.google.com/file/d/1WNJU54dnbAP_AV0-yP1nvAS0Y5OzAZ77/view?usp=sharing

## Author

Puthiyaraj P
