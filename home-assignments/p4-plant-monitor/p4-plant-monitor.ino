// Project 4 - Smart Plant Watering Monitor

#include <Wire.h>
#include <DHTesp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_W 128
#define OLED_H 64

Adafruit_SSD1306 oled(OLED_W, OLED_H, &Wire, -1);
DHTesp climateSensor;

// Pin configuration
const byte MOISTURE_SENSOR = 34;
const byte DHT_SENSOR = 4;
const byte WATER_PUMP = 26;
const byte EXTRA_RELAY = 27;
const byte CONTROL_BUTTON = 13;
const byte DRY_LED = 25;
const byte READY_LED = 33;
const byte ALERT_BUZZER = 32;

// Soil sensor calibration
const int SOIL_DRY = 3200;
const int SOIL_WET = 1200;

// Soil thresholds
const int DRY_LIMIT = 30;
const int STOP_LIMIT = 40;

int moisture = 0;
float tempC = 0;
float humidity = 0;

bool watering = false;
bool buttonWatering = false;

unsigned long sensorTimer = 0;
unsigned long wateringTimer = 0;

const unsigned long READ_TIME = 5000;
const unsigned long MANUAL_TIME = 5000;

String plantCondition = "OPTIMAL";

void setup()
{
  Serial.begin(115200);

  pinMode(WATER_PUMP, OUTPUT);
  pinMode(EXTRA_RELAY, OUTPUT);
  pinMode(DRY_LED, OUTPUT);
  pinMode(READY_LED, OUTPUT);
  pinMode(ALERT_BUZZER, OUTPUT);
  pinMode(CONTROL_BUTTON, INPUT_PULLUP);

  // Relay modules are active LOW
  digitalWrite(WATER_PUMP, HIGH);
  digitalWrite(EXTRA_RELAY, HIGH);

  digitalWrite(DRY_LED, LOW);
  digitalWrite(READY_LED, HIGH);
  digitalWrite(ALERT_BUZZER, LOW);

  climateSensor.setup(DHT_SENSOR, DHTesp::DHT11);

  Wire.begin(21, 22);

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED initialization failed");
    while (1);
  }

  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setTextSize(1);
  oled.setCursor(18, 22);
  oled.println("Smart Plant");
  oled.setCursor(25, 36);
  oled.println("Monitor");
  oled.display();

  delay(2000);
}

void loop()
{
  if (millis() - sensorTimer >= READ_TIME)
  {
    sensorTimer = millis();

    collectSensorData();
    controlPlantWatering();
    showInformation();
    sendDataToSerial();
  }

  readWaterButton();
  handleManualWatering();
}

void collectSensorData()
{
  int sensorReading = analogRead(MOISTURE_SENSOR);

  moisture = map(sensorReading, SOIL_DRY, SOIL_WET, 0, 100);
  moisture = constrain(moisture, 0, 100);

  TempAndHumidity environment =
    climateSensor.getTempAndHumidity();

  tempC = environment.temperature;
  humidity = environment.humidity;
}

void controlPlantWatering()
{
  if (buttonWatering)
    return;

  if (moisture < DRY_LIMIT)
  {
    watering = true;
    plantCondition = "DRY";

    digitalWrite(DRY_LED, HIGH);
    digitalWrite(READY_LED, LOW);
    digitalWrite(ALERT_BUZZER, HIGH);
  }
  else
  {
    if (moisture > STOP_LIMIT)
    {
      watering = false;
    }

    if (moisture <= 70)
    {
      plantCondition = "OPTIMAL";
    }
    else
    {
      plantCondition = "WET";
    }

    digitalWrite(DRY_LED, LOW);
    digitalWrite(READY_LED, HIGH);
    digitalWrite(ALERT_BUZZER, LOW);
  }

  digitalWrite(WATER_PUMP, watering ? LOW : HIGH);
}

void readWaterButton()
{
  static int previousButton = HIGH;
  int currentButton = digitalRead(CONTROL_BUTTON);

  if (previousButton == HIGH && currentButton == LOW)
  {
    buttonWatering = true;
    wateringTimer = millis();

    digitalWrite(WATER_PUMP, LOW);

    Serial.println("Manual Watering Started");

    delay(200);
  }

  previousButton = currentButton;
}

void handleManualWatering()
{
  if (buttonWatering &&
      millis() - wateringTimer >= MANUAL_TIME)
  {
    buttonWatering = false;

    digitalWrite(WATER_PUMP, HIGH);

    Serial.println("Manual Watering Finished");
  }
}

void showInformation()
{
  oled.clearDisplay();

  oled.setCursor(0, 0);
  oled.print("Soil: ");
  oled.print(moisture);
  oled.println("%");

  oled.print("Temp: ");
  oled.print(tempC, 1);
  oled.println(" C");

  oled.print("Status: ");
  oled.println(plantCondition);

  oled.drawRect(5, 52, 100, 8, SSD1306_WHITE);

  int level = map(moisture, 0, 100, 0, 98);
  oled.fillRect(6, 53, level, 6, SSD1306_WHITE);

  oled.display();
}

void sendDataToSerial()
{
  Serial.print("SOIL: ");
  Serial.print(moisture);
  Serial.print("% | TEMP: ");
  Serial.print(tempC);
  Serial.print(" C | HUM: ");
  Serial.print(humidity);
  Serial.print("% | STATUS: ");
  Serial.println(plantCondition);
}
