/****************************************************
   FULL IoT HOME AUTOMATION HUB
   ESP32 - IIT Jammu Summer School 2026

   Sensors:
   DHT11
   MQ-2
   PIR
   LDR

   Outputs:
   2-Channel Relay
   OLED
   Buzzer
   Red LED
   Green LED

   
****************************************************/

#include <WiFi.h>
#include <WebServer.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ================= WIFI =================

const char* ssid = "Surya.the.actor";
const char* password = "instagram";

// ================= MQTT =================

const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

String mqttTopic = "iitjammu/puthiyaraj/home";

// ================= PIN DEFINITIONS =================

// DHT11
#define DHTPIN 4
#define DHTTYPE DHT11

// Sensors
#define MQ2_PIN 35
#define PIR_PIN 13
#define LDR_PIN 34

// Relay
#define FAN_RELAY 26
#define LIGHT_RELAY 27

// Alert
#define BUZZER_PIN 14
#define RED_LED 25
#define GREEN_LED 2

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 21
#define OLED_SCL 22

// ================= OBJECTS =================

DHT dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

WebServer server(80);

// ================= GLOBAL VARIABLES =================

float temperature = 0;
float humidity = 0;

int gasValue = 0;
int gasPercent = 0;

int ldrValue = 0;
int lightPercent = 0;

int pirState = 0;

bool fanState = false;
bool lightState = false;
bool gasAlert = false;

unsigned long lastSensorRead = 0;
unsigned long lastMQTTPublish = 0;
unsigned long lastOLEDUpdate = 0;

int oledScreen = 0;

// Light motion timer
unsigned long lastMotionTime = 0;

// ================= SETUP =================

void setup()
{
  Serial.begin(115200);

  // Pin modes
  pinMode(MQ2_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  pinMode(FAN_RELAY, OUTPUT);
  pinMode(LIGHT_RELAY, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Relay OFF initially
  digitalWrite(FAN_RELAY, HIGH);
  digitalWrite(LIGHT_RELAY, HIGH);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  // DHT start
  dht.begin();

  // OLED start
  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C
      ))
  {
    Serial.println("OLED not found");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("IoT HOME HUB");
  display.println("Starting...");
  display.display();

  delay(2000);

  // Wi-Fi
  connectWiFi();

  // MQTT
  mqttClient.setServer(
    mqtt_server,
    1883
  );

  // Web routes
  server.on("/", handleRoot);

  server.on(
    "/fan/on",
    []()
    {
      fanState = true;
      digitalWrite(FAN_RELAY, LOW);
      server.send(200, "text/plain", "Fan ON");
    }
  );

  server.on(
    "/fan/off",
    []()
    {
      fanState = false;
      digitalWrite(FAN_RELAY, HIGH);
      server.send(200, "text/plain", "Fan OFF");
    }
  );

  server.on(
    "/light/on",
    []()
    {
      lightState = true;
      digitalWrite(LIGHT_RELAY, LOW);
      server.send(200, "text/plain", "Light ON");
    }
  );

  server.on(
    "/light/off",
    []()
    {
      lightState = false;
      digitalWrite(LIGHT_RELAY, HIGH);
      server.send(200, "text/plain", "Light OFF");
    }
  );

  server.begin();

  Serial.println("Web Server Started");
}

// ================= LOOP =================

void loop()
{
  server.handleClient();

  if (!mqttClient.connected())
  {
    reconnectMQTT();
  }

  mqttClient.loop();

  // Read sensors every 5 seconds
  if (millis() - lastSensorRead >= 5000)
  {
    readSensors();

    applyAutomationRules();

    updateOLED();

    lastSensorRead = millis();
  }

  // MQTT every 30 seconds
  if (millis() - lastMQTTPublish >= 30000)
  {
    publishMQTT();

    lastMQTTPublish = millis();
  }
}

// ================= WIFI =================

void connectWiFi()
{
  Serial.print("Connecting WiFi");

  WiFi.begin(
    ssid,
    password
  );

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);

    Serial.print(".");
  }

  Serial.println();

  Serial.println("WiFi Connected");

  Serial.print("IP Address: ");

  Serial.println(
    WiFi.localIP()
  );
}

// ================= READ SENSORS =================

void readSensors()
{
  // DHT11
  float newTemperature = dht.readTemperature();

  float newHumidity = dht.readHumidity();

  if (!isnan(newTemperature))
  {
    temperature = newTemperature;
  }

  if (!isnan(newHumidity))
  {
    humidity = newHumidity;
  }

  // MQ2
  gasValue = analogRead(MQ2_PIN);

  gasPercent = map(
    gasValue,
    0,
    4095,
    0,
    100
  );

  gasPercent = constrain(
    gasPercent,
    0,
    100
  );

  // PIR
  pirState = digitalRead(PIR_PIN);

  // LDR
  ldrValue = analogRead(LDR_PIN);

  lightPercent = map(
    ldrValue,
    0,
    4095,
    0,
    100
  );

  lightPercent = constrain(
    lightPercent,
    0,
    100
  );

  if (pirState == HIGH)
  {
    lastMotionTime = millis();
  }

  Serial.println("--------------------");

  Serial.print("Temperature: ");
  Serial.println(temperature);

  Serial.print("Humidity: ");
  Serial.println(humidity);

  Serial.print("Gas: ");
  Serial.println(gasPercent);

  Serial.print("PIR: ");
  Serial.println(pirState);

  Serial.print("Light: ");
  Serial.println(lightPercent);
}

// ================= AUTOMATION =================

void applyAutomationRules()
{
  // ================= GAS SAFETY =================

  if (gasPercent > 60)
  {
    gasAlert = true;

    // Safety shutdown
    fanState = false;
    lightState = false;

    digitalWrite(
      FAN_RELAY,
      HIGH
    );

    digitalWrite(
      LIGHT_RELAY,
      HIGH
    );

    digitalWrite(
      BUZZER_PIN,
      HIGH
    );

    digitalWrite(
      RED_LED,
      HIGH
    );

    digitalWrite(
      GREEN_LED,
      LOW
    );

    return;
  }

  else
  {
    gasAlert = false;

    digitalWrite(
      BUZZER_PIN,
      LOW
    );

    digitalWrite(
      RED_LED,
      LOW
    );

    digitalWrite(
      GREEN_LED,
      HIGH
    );
  }

  // ================= FAN AUTOMATION =================

  // Temperature above 32°C
  if (!fanState && temperature > 32)
  {
    fanState = true;

    digitalWrite(
      FAN_RELAY,
      LOW
    );

    Serial.println("Fan AUTO ON");
  }

  // Temperature below 28°C
  if (fanState && temperature < 28)
  {
    fanState = false;

    digitalWrite(
      FAN_RELAY,
      HIGH
    );

    Serial.println("Fan AUTO OFF");
  }

  // ================= LIGHT AUTOMATION =================

  // Dark + Motion
  if (
    lightPercent < 20 &&
    pirState == HIGH
  )
  {
    lightState = true;

    digitalWrite(
      LIGHT_RELAY,
      LOW
    );

    Serial.println("Light AUTO ON");
  }

  // Daylight
  if (lightPercent > 60)
  {
    lightState = false;

    digitalWrite(
      LIGHT_RELAY,
      HIGH
    );

    Serial.println("Light OFF - Daylight");
  }

  // No motion for 3 minutes
  if (
    millis() - lastMotionTime >= 180000
  )
  {
    lightState = false;

    digitalWrite(
      LIGHT_RELAY,
      HIGH
    );

    Serial.println(
      "Light OFF - No Motion"
    );
  }
}

// ================= MQTT =================

void reconnectMQTT()
{
  while (!mqttClient.connected())
  {
    Serial.print("Connecting MQTT...");

    String clientID = "ESP32HomeHub";

    clientID += String(
      random(0xffff),
      HEX
    );

    if (
      mqttClient.connect(
        clientID.c_str()
      )
    )
    {
      Serial.println(
        "MQTT Connected"
      );
    }

    else
    {
      Serial.print(
        "Failed, rc="
      );

      Serial.println(
        mqttClient.state()
      );

      delay(2000);
    }
  }
}

// ================= MQTT PUBLISH =================

void publishMQTT()
{
  String json = "{";

  json += "\"temp\":";
  json += String(temperature, 1);

  json += ",\"humidity\":";
  json += String(humidity, 1);

  json += ",\"gas\":";
  json += String(gasPercent);

  json += ",\"pir\":";
  json += String(pirState);

  json += ",\"light\":";
  json += String(lightPercent);

  json += ",\"fan\":";
  json += String(fanState ? 1 : 0);

  json += ",\"light_relay\":";
  json += String(lightState ? 1 : 0);

  json += ",\"alert\":";
  json += String(gasAlert ? 1 : 0);

  json += "}";

  mqttClient.publish(
    mqttTopic.c_str(),
    json.c_str()
  );

  Serial.println(
    "MQTT Published:"
  );

  Serial.println(json);
}

// ================= OLED =================

void updateOLED()
{
  if (
    millis() - lastOLEDUpdate >= 5000
  )
  {
    oledScreen++;

    if (oledScreen > 2)
    {
      oledScreen = 0;
    }

    lastOLEDUpdate = millis();
  }

  display.clearDisplay();

  display.setCursor(0, 0);

  // Screen 1
  if (oledScreen == 0)
  {
    display.println(
      "ENVIRONMENT"
    );

    display.println();

    display.print(
      "Temp: "
    );

    display.print(
      temperature
    );

    display.println(
      " C"
    );

    display.print(
      "Humidity: "
    );

    display.print(
      humidity
    );

    display.println(
      " %"
    );
  }

  // Screen 2
  else if (oledScreen == 1)
  {
    display.println(
      "SENSORS"
    );

    display.println();

    display.print(
      "Gas: "
    );

    display.print(
      gasPercent
    );

    display.println(
      " %"
    );

    display.print(
      "PIR: "
    );

    display.println(
      pirState ? "MOTION" : "NO MOTION"
    );

    display.print(
      "LDR: "
    );

    display.print(
      lightPercent
    );

    display.println(
      " %"
    );
  }

  // Screen 3
  else
  {
    display.println(
      "SYSTEM STATUS"
    );

    display.println();

    display.print(
      "Fan: "
    );

    display.println(
      fanState ? "ON" : "OFF"
    );

    display.print(
      "Light: "
    );

    display.println(
      lightState ? "ON" : "OFF"
    );

    display.print(
      "Alert: "
    );

    display.println(
      gasAlert ? "YES" : "NO"
    );
  }

  display.display();
}

// ================= WEB DASHBOARD =================

void handleRoot()
{
  String html = "";

  html += "<!DOCTYPE html>";

  html += "<html>";

  html += "<head>";

  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";

  html += "<meta http-equiv='refresh' content='5'>";

  html += "<title>IoT Home Hub</title>";

  html += "<style>";

  html += "body{font-family:Arial;text-align:center;background:#f2f2f2;}";

  html += ".card{background:white;margin:10px;padding:15px;border-radius:10px;}";

  html += "button{padding:10px;margin:5px;font-size:16px;}";

  html += "</style>";

  html += "</head>";

  html += "<body>";

  html += "<h1>ESP32 IoT Home Automation Hub</h1>";

  html += "<div class='card'>";

  html += "<h2>Environment</h2>";

  html += "<p>Temperature: ";

  html += String(
    temperature
  );

  html += " °C</p>";

  html += "<p>Humidity: ";

  html += String(
    humidity
  );

  html += " %</p>";

  html += "</div>";

  html += "<div class='card'>";

  html += "<h2>Sensors</h2>";

  html += "<p>Gas: ";

  html += String(
    gasPercent
  );

  html += " %</p>";

  html += "<p>PIR: ";

  html += pirState ? "MOTION" : "NO MOTION";

  html += "</p>";

  html += "<p>Light: ";

  html += String(
    lightPercent
  );

  html += " %</p>";

  html += "</div>";

  html += "<div class='card'>";

  html += "<h2>Relay Status</h2>";

  html += "<p>Fan: ";

  html += fanState ? "ON" : "OFF";

  html += "</p>";

  html += "<a href='/fan/on'>";

  html += "<button>Fan ON</button>";

  html += "</a>";

  html += "<a href='/fan/off'>";

  html += "<button>Fan OFF</button>";

  html += "</a>";

  html += "<p>Light: ";

  html += lightState ? "ON" : "OFF";

  html += "</p>";

  html += "<a href='/light/on'>";

  html += "<button>Light ON</button>";

  html += "</a>";

  html += "<a href='/light/off'>";

  html += "<button>Light OFF</button>";

  html += "</a>";

  html += "</div>";

  html += "<div class='card'>";

  html += "<h2>Gas Alert: ";

  html += gasAlert ? "DANGER" : "SAFE";

  html += "</h2>";

  html += "</div>";

  html += "</body>";

  html += "</html>";

  server.send(
    200,
    "text/html",
    html
  );
}
