/******************************************************
  ESP32 Mini Weather Station
  Components:
  - ESP32 Dev Board
  - DHT11 Sensor
  - OLED Display SSD1306 (I2C)
  - Red LED
  - Green LED
  - Active Buzzer
******************************************************/

#include <Wire.h>
#include <DHTesp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//---------------- OLED Settings ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//---------------- Pin Definitions ----------------
#define DHTPIN      4
#define RED_LED     25
#define GREEN_LED   26
#define BUZZER      27

//---------------- Objects ----------------
DHTesp dht;

//---------------- Variables ----------------
unsigned long lastSensorRead = 0;
unsigned long lastCSVLog = 0;

float temperature = 0;
float humidity = 0;
String status = "COMFORT";

void setup() {

  Serial.begin(115200);

  // Initialize DHT11
  dht.setup(DHTPIN, DHTesp::DHT11);

  // Initialize OLED
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Output Pins
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BUZZER, LOW);

  display.setCursor(0,0);
  display.println("Mini Weather");
  display.println("Station");
  display.display();

  delay(2000);
}

void loop() {

  // Read DHT11 every 2 seconds
  if (millis() - lastSensorRead >= 2000) {

    lastSensorRead = millis();

    TempAndHumidity data = dht.getTempAndHumidity();

    temperature = data.temperature;
    humidity = data.humidity;

    // Print Raw Values
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" C\tHumidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    //------------- Comfort Index -------------

    if (temperature > 38 || humidity > 80) {
      status = "DANGER";
    }
    else if (temperature >= 34 || humidity >= 70) {
      status = "HOT";
    }
    else if (temperature >= 25 && temperature <= 33 &&
             humidity >= 40 && humidity <= 69) {
      status = "COMFORT";
    }
    else {
      status = "COOL";
    }

    //------------- OLED Display -------------

    display.clearDisplay();

    display.setCursor(0,0);
    display.print("Temp: ");
    display.print(temperature,1);
    display.println(" C");

    display.setCursor(0,20);
    display.print("Hum : ");
    display.print(humidity,0);
    display.println(" %");

    display.setCursor(0,40);
    display.print("Status: ");
    display.println(status);

    display.display();

    //------------- Threshold Logic -------------

    if (temperature > 38 || humidity > 80) {

      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);

      digitalWrite(BUZZER, HIGH);
      delay(1000);
      digitalWrite(BUZZER, LOW);
    }
    else {

      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BUZZER, LOW);
    }
  }

  //------------- CSV Logging every 5 sec -------------

  if (millis() - lastCSVLog >= 5000) {

    lastCSVLog = millis();

    Serial.print(millis());
    Serial.print(",");
    Serial.print(temperature);
    Serial.print(",");
    Serial.print(humidity);
    Serial.print(",");
    Serial.println(status);
  }

}
