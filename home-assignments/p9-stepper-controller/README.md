# Project 9 - Stepper Motor Precision Positioner

## Components Used

- Arduino Uno R3
- 28BYJ-48 Stepper Motor
- ULN2003 Driver Board
- 10kΩ Potentiometer
- Push Button x3
- OLED Display (SSD1306 I2C)
- LED
- 220Ω Resistor
- Breadboard
- Jumper Wires

## Wiring Diagram (Text)

### ULN2003 Driver

- IN1 -> Pin 8
- IN2 -> Pin 9
- IN3 -> Pin 10
- IN4 -> Pin 11
- VCC -> 5V
- GND -> GND

### Stepper Motor

- Connect motor cable directly to ULN2003 driver board

### Potentiometer

- Left Pin -> GND
- Right Pin -> 5V
- Middle Pin (Wiper) -> A0

### Push Buttons

#### Clockwise Button
- Pin 2
- 10kΩ Pull-down Resistor

#### Counter Clockwise Button
- Pin 3
- 10kΩ Pull-down Resistor

#### Home Button
- Pin 4
- 10kΩ Pull-down Resistor

### OLED Display

- SDA -> A4
- SCL -> A5
- VCC -> 5V
- GND -> GND

### Position Indicator LED

- Pin 13 through 220Ω resistor

## Libraries Used

- Stepper Library
- Adafruit SSD1306
- Adafruit GFX
- Wire

## How to Run

1. Connect all components according to the wiring diagram.
2. Install required libraries.
3. Upload the code to Arduino Uno.
4. Open Serial Monitor at 115200 baud.
5. Rotate potentiometer to set target angle.
6. Use CW and CCW buttons to move the motor.
7. Press HOME button to reset current position.

## 28BYJ-48 Stepper Motor Specification

The 28BYJ-48 stepper motor contains an internal gearbox.

### Step Resolution

```text
4096 Steps = 360 Degrees
```

This means:

```text
1 Step ≈ 0.0879°
```

### Why 4096 Steps?

The motor itself has fewer internal steps, but the gearbox increases positioning precision.

Because of the gear reduction ratio, one complete output shaft rotation requires approximately:

```text
4096 Steps
```

This provides high resolution and accurate positioning.

## Target Angle Calculation

The potentiometer controls the desired position.

### Mapping

```cpp
targetAngle = map(analogRead(A0), 0, 1023, 0, 360);
```

Example:

```text
Potentiometer = 50%
Target Angle = 180°
```

## Position Tracking

Current motor position is stored as:

```cpp
currentSteps
```

Angle calculation:

```cpp
angle = (currentSteps % 4096) * 360.0 / 4096.0;
```

## Button Functions

### CW Button

Moves motor:

```text
+512 Steps
```

Equivalent to:

```text
45 Degrees Clockwise
```

### CCW Button

Moves motor:

```text
-512 Steps
```

Equivalent to:

```text
45 Degrees Counter Clockwise
```

### HOME Button

```text
currentSteps = 0
```

Motor does not move.

Current position becomes logical home position.

## OLED Display

Example:

```text
Current: 135°
Target : 270°
Dir    : CW
Steps  : 1750
```

Displayed Information:

- Current Angle
- Target Angle
- Direction
- Total Steps

## Expected Serial Monitor Output

```text
Move 1 : CW  | Steps: 512  | Angle: 45°
Move 2 : CW  | Steps: 1024 | Angle: 90°
Move 3 : CW  | Steps: 1536 | Angle: 135°
Move 4 : CCW | Steps: 1024 | Angle: 90°
Move 5 : CW  | Steps: 1536 | Angle: 135°
Move 6 : CW  | Steps: 2048 | Angle: 180°
Move 7 : CCW | Steps: 1536 | Angle: 135°
Move 8 : CW  | Steps: 2048 | Angle: 180°
Move 9 : CW  | Steps: 2560 | Angle: 225°
Move 10: CW  | Steps: 3072 | Angle: 270°
```

## Important Notes

- Maximum recommended speed: 15 RPM
- Higher speed may cause skipped steps
- Position tracking accuracy depends on avoiding missed steps
- Follow the correct wiring order:

```text
IN1 → IN3 → IN2 → IN4
```

for reliable operation with the Stepper library.

## Serial Monitor Screenshot

Upload and include:

```text
stepper-position-log.png
```

The screenshot should show at least 10 motor movements.



The wokwi should show:

- Potentiometer changing target angle
- CW button operation
- CCW button operation
- HOME button reset
- OLED updating in real time

## Wokwi Link
https://wokwi.com/projects/470243464085273601

## Author

Puthiyaraj P
