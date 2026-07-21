# Project 3 - Ultrasonic Parking Assistant

## Components Used

- Arduino Uno R3
- HC-SR04 Ultrasonic Sensor
- Active Buzzer
- Green LED
- Yellow LED
- Red LED
- OLED Display (SSD1306 I2C)
- 220Ω Resistors
- Breadboard
- Jumper Wires

## Wiring Diagram (Text)

### HC-SR04 Ultrasonic Sensor
- VCC -> 5V
- GND -> GND
- TRIG -> D9
- ECHO -> D10

### OLED Display
- VCC -> 5V
- GND -> GND
- SDA -> A4
- SCL -> A5

### LEDs
- Green LED -> D2
- Yellow LED -> D3
- Red LED -> D4

### Buzzer
- Positive (+) -> D6
- Negative (-) -> GND

## Library Versions

- Adafruit SSD1306 v2.5.13
- Adafruit GFX v1.12.1

## How to Run

1. Connect all components according to the wiring diagram.
2. Install the required OLED libraries.
3. Upload the Arduino sketch.
4. Open Serial Monitor at 9600 baud.
5. Move an object toward the HC-SR04 sensor.
6. Observe the LEDs, buzzer alerts and OLED display.

## Distance Zones

### SAFE (> 60 cm)

- Green LED ON
- Buzzer OFF

### CAUTION (30–60 cm)

- Yellow LED ON
- Beep every 800 ms

### CLOSE (15–30 cm)

- Yellow LED + Red LED ON
- Beep every 300 ms

### DANGER (< 15 cm)

- Red LED ON
- Continuous Buzzer

## Expected Output

### SAFE

```text
Distance: 75 cm | Zone: SAFE
```

### CAUTION

```text
Distance: 45 cm | Zone: CAUTION
```

### CLOSE

```text
Distance: 22 cm | Zone: CLOSE
```

### DANGER

```text
Distance: 8 cm | Zone: DANGER
```

The OLED displays:

```text
42 cm
```

along with a proximity bar graph.

## Distance Calculation Formula

The HC-SR04 measures distance by sending an ultrasonic pulse and waiting for the echo to return.

Formula:

```cpp
distance = pulseIn(ECHO, HIGH) * 0.034 / 2;
```

Explanation:

- Speed of sound ≈ 0.034 cm/µs
- The pulse travels to the object and back.
- Therefore the measured time is divided by 2.
- The result gives the distance between the sensor and the object in centimeters.

Example:

```text
Echo Time = 2500 µs

Distance = 2500 × 0.034 ÷ 2
Distance = 42.5 cm
```

## Noise Reduction

To improve accuracy, 5 distance readings are taken and averaged.

Example:

```cpp
for(int i=0; i<5; i++)
{
    readings[i] = measureDistance();
}
```

The average value is used for zone detection.

## Serial Monitor Screenshot

Upload and include:

```text
serial-monitor-output.png
```

The screenshot should clearly show distance values and all zone labels.

## Demo Video

Google Drive Link:

https://drive.google.com/file/d/1lXKPi_mzC0GV1j_gPf_8OtPkclE6CMPY/view?usp=sharing

## Author

Puthiyaraj P
