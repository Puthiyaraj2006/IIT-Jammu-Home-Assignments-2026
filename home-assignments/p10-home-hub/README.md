# Project 10 - Full IoT Home Automation Hub

## Components Used

- ESP32 Dev Board
- DHT11 Temperature & Humidity Sensor
- MQ-2 Gas Sensor
- PIR Motion Sensor HC-SR501
- LDR Sensor
- 10kΩ Resistor
- 2-Channel Relay Module
- OLED Display SSD1306 (0.96" I2C)
- Active Buzzer
- Red LED
- Green LED
- Yellow LED
- Push Button x2
- Breadboard
- Jumper Wires

---

# System Architecture Diagram

```text
DHT11 --------\
MQ-2 ----------\
PIR ------------> ESP32 ---> Wi-Fi ---> Web Dashboard
LDR ------------/     |
                     |
                     ---> MQTT Broker (HiveMQ)

ESP32 ---> OLED Display

ESP32 ---> Relay 1 (Fan)
ESP32 ---> Relay 2 (Light)

ESP32 ---> Buzzer
ESP32 ---> Status LEDs

ESP32 <--- Manual Override Buttons
```

---

# Wiring Summary

## DHT11 Sensor

- VCC → 3.3V
- GND → GND
- DATA → GPIO4

## MQ-2 Gas Sensor

- VCC → 5V
- GND → GND
- AOUT → GPIO35

## PIR Motion Sensor

- VCC → 5V
- GND → GND
- OUT → GPIO13

## LDR Sensor

- One Side → 3.3V
- Other Side → GPIO34
- 10kΩ Resistor → GND

## Relay Module

### Relay 1 (Fan)

- IN1 → GPIO26

### Relay 2 (Light)

- IN2 → GPIO27

### Relay Power

- VCC → 5V
- GND → GND

**Note:** Relay is Active-LOW

```text
LOW  = ON
HIGH = OFF
```

## OLED Display

- SDA → GPIO21
- SCL → GPIO22
- VCC → 3.3V
- GND → GND

## Buzzer

- Positive → GPIO14
- Negative → GND

## LEDs

### Red LED (Gas Alert)

- GPIO25

### Green LED (System OK)

- GPIO2

### Yellow LED

- GPIO33

## Manual Override Buttons

### Fan Override Button

- GPIO0
- INPUT_PULLUP

### Light Override Button

- GPIO32
- INPUT_PULLUP

---

# Libraries Used

- WiFi.h
- AsyncTCP.h
- ESPAsyncWebServer.h
- PubSubClient.h
- DHTesp.h
- Adafruit SSD1306
- Adafruit GFX
- Wire.h

---

# Problem Statement

Design a complete IoT-based Home Automation Hub that monitors environmental conditions and automatically controls household devices. The system should monitor temperature, humidity, gas levels, room lighting conditions, and human presence while providing remote monitoring through a web dashboard and MQTT communication.

---

# Features

### Real-Time Environmental Monitoring

- Temperature Monitoring
- Humidity Monitoring
- Gas Leak Detection
- Motion Detection
- Ambient Light Detection

### Smart Automation

- Automatic Fan Control
- Automatic Light Control
- Gas Safety Shutdown

### Remote Monitoring

- Web Dashboard
- MQTT Cloud Monitoring

### Local Monitoring

- OLED Display
- Status LEDs
- Buzzer Alerts

---

# Automation Rules

## Rule 1 - Fan Automation

### Fan ON

```text
Temperature > 32°C
```

### Fan OFF

```text
Temperature < 28°C
```

### Hysteresis

```text
ON  at 32°C
OFF at 28°C
```

This prevents rapid switching of the relay.

---

## Rule 2 - Light Automation

### Light ON

```text
LDR < 20%
AND
PIR detects motion
```

### Light OFF

```text
LDR > 60%
OR
No motion detected for 3 minutes
```

---

## Rule 3 - Gas Alert Safety System

### Trigger Condition

```text
Gas Level > 60%
```

### Actions

```text
Red LED ON
Buzzer ON
Fan OFF
Light OFF
MQTT Alert Published
```

This provides emergency safety shutdown.

---

# Manual Override Logic

## Fan Override

Press Fan Button (GPIO0)

```text
Manual Control Enabled
Duration: 10 Minutes
```

After 10 minutes:

```text
Automatic Mode Resumes
```

## Light Override

Press Light Button (GPIO32)

```text
Manual Control Enabled
Duration: 10 Minutes
```

After 10 minutes:

```text
Automatic Mode Resumes
```

---

# MQTT Configuration

## Broker

```text
broker.hivemq.com
```

## Topic

```text
iitjammu/puthiyaraj/home
```

## Publish Interval

```text
Every 30 Seconds
```

## Sample JSON Message

```json
{
  "temp": 29,
  "humidity": 65,
  "gas": 15,
  "pir": 1,
  "light": 18,
  "fan": 1,
  "light_relay": 1,
  "alert": 0
}
```

---

# Web Dashboard

The ESP32 hosts a web dashboard on Port 80.

### Dashboard Displays

- Temperature
- Humidity
- Gas Level
- PIR Status
- LDR Reading
- Fan Status
- Light Status
- Alert Status
- System Uptime

### Dashboard Controls

- Fan ON/OFF Toggle
- Light ON/OFF Toggle

---

# OLED Display Screens

The OLED automatically changes screens every 5 seconds.

## Screen 1

```text
Temperature
Humidity
```

Example:

```text
Temp: 29°C
Hum : 65%
```

---

## Screen 2

```text
Gas Level
PIR Status
LDR Value
```

Example:

```text
Gas : 15%
PIR : Motion
LDR : 18%
```

---

## Screen 3

```text
Fan Status
Light Status
System Uptime
```

Example:

```text
Fan   : ON
Light : OFF
Up    : 0d 04h 15m
```

---

# Expected Serial Monitor Output

```text
TEMP: 29°C
HUMIDITY: 65%

GAS: 15%
PIR: MOTION
LDR: 18%

FAN: ON
LIGHT: ON

ALERT: NO

UPTIME: 0d 04h 15m
----------------------------------
```

---

# Expected Working

### Normal Condition

```text
Green LED ON
Fan OFF
Light OFF
Buzzer OFF
```

### High Temperature

```text
Fan ON
Relay 1 Activated
```

### Dark Room + Motion

```text
Light ON
Relay 2 Activated
```

### Gas Leak

```text
Red LED ON
Buzzer ON
All Relays OFF
MQTT Alert Sent
```

---

# Deliverables

## GitHub Folder

```text
/home-assignments/p10-home-hub/
```

Contents:

```text
README.md
p10-home-hub.ino
web-dashboard.png
mqtt-explorer.png
```

---

# Demo Video

Google Drive Link:

https://drive.google.com/file/d/1BtisUNc1l9Oln10Dk7PTQMvlaMjMv5r0/view?usp=sharing

The demo should show:

- Temperature-based fan control
- Motion-based light control
- LDR operation
- Gas alert activation
- OLED updates
- Web dashboard
- MQTT messages

---


# Author

Puthiyaraj P

