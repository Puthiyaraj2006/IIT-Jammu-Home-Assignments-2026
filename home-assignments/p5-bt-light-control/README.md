# Project 5 - Bluetooth Home Light Controller

## Components Used

- ESP32 Dev Board
- 2-Channel Relay Module
- 2 LEDs (Light Simulation)
- 220Ω Resistors
- Push Button x2
- Active Buzzer
- Breadboard
- Jumper Wires

## Wiring Diagram (Text)

### Relay Module

Relay IN1
- IN1 -> GPIO26

Relay IN2
- IN2 -> GPIO27

Relay Power
- VCC -> 5V (ESP32 VIN)
- GND -> GND

### Simulated Lights

Light 1 LED
- Connected through Relay Channel 1

Light 2 LED
- Connected through Relay Channel 2

### Push Buttons

Button 1
- GPIO0
- 10kΩ Pull-down Resistor

Button 2
- GPIO35
- 10kΩ Pull-down Resistor

### Buzzer

- Positive (+) -> GPIO25
- Negative (-) -> GND

### Bluetooth Status Indicator

- Built-in LED -> GPIO2

## Library Versions

- BluetoothSerial (ESP32 Built-in Library)

## Pairing Instructions

1. Upload the code to ESP32.
2. Enable Bluetooth on your Android phone.
3. Install a Bluetooth terminal application.
   - Recommended: Serial Bluetooth Terminal
4. Search for Bluetooth devices.
5. Pair with:

```text
IIT_IoT_HomeCtrl
```

6. Connect to the ESP32 device.
7. Send commands through the terminal application.

## Supported Commands

| Command | Function |
|----------|----------|
| 1 | Light 1 ON |
| 2 | Light 1 OFF |
| 3 | Light 2 ON |
| 4 | Light 2 OFF |
| 5 | All Lights ON |
| 6 | All Lights OFF |
| ? | Status Query |

## Example Communication

### Phone Sends

```text
1
```

### ESP32 Responds

```text
L1:ON L2:OFF
```

### Phone Sends

```text
5
```

### ESP32 Responds

```text
L1:ON L2:ON
```

### Phone Sends

```text
?
```

### ESP32 Responds

```text
L1:OFF L2:ON
```

## Manual Override

Two push buttons are provided for local control.

### Button 1 (GPIO0)

- Toggles Light 1
- Sends updated status through Bluetooth

### Button 2 (GPIO35)

- Toggles Light 2
- Sends updated status through Bluetooth

## Bluetooth Connection Indicator

The built-in LED on GPIO2 indicates Bluetooth connection status.

### Connected

- GPIO2 LED ON

### Disconnected

- GPIO2 LED OFF

## Auto Timeout Feature

For energy saving, the controller automatically turns OFF all lights if no Bluetooth command is received for 30 minutes.

## Expected Output

### Light 1 ON

```text
L1:ON L2:OFF
```

### Light 2 ON

```text
L1:OFF L2:ON
```

### All Lights ON

```text
L1:ON L2:ON
```

### All Lights OFF

```text
L1:OFF L2:OFF
```

A short confirmation beep is generated whenever a valid command is received.

## Relay Logic Explanation

The relay module is active-LOW.

```cpp
digitalWrite(RELAY1, LOW);
```

means:

```text
Relay ON
Light ON
```

and

```cpp
digitalWrite(RELAY1, HIGH);
```

means:

```text
Relay OFF
Light OFF
```

## Bluetooth Terminal Screenshot

Upload and include:

```text
bluetooth-terminal-output.png
```

The screenshot should clearly show command and response exchanges.

## Demo Video

Google Drive Link:

https://drive.google.com/file/d/14uXZH-1g8kAGtEdPjGadxZiyZ9tjE06k/view?usp=sharing

## Author

Puthiyaraj P
