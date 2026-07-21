
/*******************************************************
  Bluetooth Home Control using ESP32
  Board: ESP32 Dev Board

  Bluetooth Name: IIT_IoT_HomeCtrl

  Commands:
  '1' -> Light1 ON
  '2' -> Light1 OFF
  '3' -> Light2 ON
  '4' -> Light2 OFF
  '5' -> ALL ON
  '6' -> ALL OFF
  '?' -> Status
*******************************************************/
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

//--------------- Pin Definitions ----------------

#define RELAY1     26
#define RELAY2     27

#define BUTTON1    0
#define BUTTON2    35

#define BUZZER     25

#define BT_LED     2      // Built-in LED

// Relay is Active LOW
#define RELAY_ON   LOW
#define RELAY_OFF  HIGH

//--------------- Variables ----------------

bool light1 = false;
bool light2 = false;

unsigned long lastBTCommand = 0;
const unsigned long timeoutPeriod = 1800000UL; //30 minutes

bool lastButton1 = LOW;
bool lastButton2 = LOW;

//------------------------------------------------

void setup()
{
  Serial.begin(115200);

  SerialBT.begin("IIT_IoT_HomeCtrl");      // Reference Code

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP);   // Boot Button
  pinMode(BUTTON2, INPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(BT_LED, OUTPUT);

  digitalWrite(RELAY1, RELAY_OFF);
  digitalWrite(RELAY2, RELAY_OFF);
  digitalWrite(BUZZER, LOW);
  digitalWrite(BT_LED, LOW);

  Serial.println("Bluetooth Home Controller Ready");
}

//------------------------------------------------

void loop()
{

  // Bluetooth Connected Indicator

  if (SerialBT.hasClient())
    digitalWrite(BT_LED, HIGH);
  else
    digitalWrite(BT_LED, LOW);

  //------------------------------------------------
  // Reference Code
  //------------------------------------------------

  if (SerialBT.available())
  {
    char cmd = SerialBT.read();

    lastBTCommand = millis();

    switch (cmd)
    {
      case '1':
        light1 = true;
        break;

      case '2':
        light1 = false;
        break;

      case '3':
        light2 = true;
        break;

      case '4':
        light2 = false;
        break;

      case '5':
        light1 = true;
        light2 = true;
        break;

      case '6':
        light1 = false;
        light2 = false;
        break;

      case '?':
        sendStatus();
        return;

      default:
        SerialBT.println("Invalid Command");
        return;
    }

    updateRelays();
    beep();
    sendStatus();
  }

  //------------------------------------------------
  // Manual Button 1
  //------------------------------------------------

  bool b1 = digitalRead(BUTTON1);

  if (lastButton1 == HIGH && b1 == LOW)
  {
    light1 = !light1;

    updateRelays();
    beep();
    sendStatus();

    delay(200);
  }

  lastButton1 = b1;

  //------------------------------------------------
  // Manual Button 2
  //------------------------------------------------

  bool b2 = digitalRead(BUTTON2);

  if (lastButton2 == LOW && b2 == HIGH)
  {
    light2 = !light2;

    updateRelays();
    beep();
    sendStatus();

    delay(200);
  }

  lastButton2 = b2;

  //------------------------------------------------
  // Auto Timeout
  //------------------------------------------------

  if (millis() - lastBTCommand > timeoutPeriod)
  {
    light1 = false;
    light2 = false;

    updateRelays();
  }

}

//------------------------------------------------

void updateRelays()
{

  // Reference Code
  // Relay Active LOW

  digitalWrite(RELAY1, light1 ? RELAY_ON : RELAY_OFF);
  digitalWrite(RELAY2, light2 ? RELAY_ON : RELAY_OFF);

}

//------------------------------------------------

void beep()
{
  digitalWrite(BUZZER, HIGH);
  delay(50);
  digitalWrite(BUZZER, LOW);
}

//------------------------------------------------

void sendStatus()
{

  String status = "";

  status += "L1:";
  status += (light1 ? "ON " : "OFF ");

  status += "L2:";
  status += (light2 ? "ON" : "OFF");

  Serial.println(status);
  SerialBT.println(status);

}
