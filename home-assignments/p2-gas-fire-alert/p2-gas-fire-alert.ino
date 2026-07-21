/*******************************************************
   Kitchen Safety Alert System
   Board  : Arduino Uno R3
   Sensor : MQ-2 Gas Sensor + Flame Sensor
   Output : Green LED, Yellow LED, Red LED, Buzzer

   Connections:
   MQ-2 AOUT  -> A0
   Flame DO   -> D7
   Flame AO   -> A1 (Optional)

   Green LED  -> D10
   Yellow LED -> D11
   Red LED    -> D12
   Buzzer     -> D9
********************************************************/

//-------------------- Pins --------------------
const int gasPin = A0;
const int flameDigital = 7;

const int greenLED = 10;
const int yellowLED = 11;
const int redLED = 12;
const int buzzer = 9;

//-------------------- Variables --------------------
unsigned long previousMillis = 0;
unsigned long serialMillis = 0;

bool buzzerState = false;

void setup()
{
  Serial.begin(9600);

  pinMode(flameDigital, INPUT);

  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);

  pinMode(buzzer, OUTPUT);

  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(buzzer, LOW);

  Serial.println("======================================");
  Serial.println(" Kitchen Safety Alert System");
  Serial.println(" MQ-2 Warming Up...");
  Serial.println(" Wait 2 Minutes...");
  Serial.println("======================================");

  delay(120000);     // 2 minute warm-up

  Serial.println("MQ-2 Ready!");
}

void loop()
{
  //---------------- Read Sensors ----------------

  // Reference Code
  int gasValue = analogRead(A0);

  // Map to percentage
  int gasPercent = map(gasValue, 0, 1023, 0, 100);

  // Reference Code
  bool flameDetected = (digitalRead(7) == LOW); // active-low

  String status = "SAFE";

  //---------------- Decision Logic ----------------

  if (gasPercent > 60 && flameDetected)
  {
    status = "GAS + FLAME";
  }
  else if (flameDetected)
  {
    status = "FLAME";
  }
  else if (gasPercent > 60)
  {
    status = "DANGER";
  }
  else if (gasPercent > 30)
  {
    status = "WARNING";
  }
  else
  {
    status = "SAFE";
  }

  //---------------- LED & Buzzer ----------------

  if (status == "SAFE")
  {
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);

    digitalWrite(buzzer, LOW);
  }

  else if (status == "WARNING")
  {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);

    // Slow Beep (1 beep/sec) using millis()

    if (millis() - previousMillis >= 500)
    {
      previousMillis = millis();

      buzzerState = !buzzerState;
      digitalWrite(buzzer, buzzerState);
    }
  }

  else if (status == "DANGER")
  {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);

    digitalWrite(buzzer, HIGH);      // Continuous Alarm
  }

  else if (status == "FLAME")
  {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, HIGH);

    // Fast Beep (Different Alarm Pattern)

    if (millis() - previousMillis >= 150)
    {
      previousMillis = millis();

      buzzerState = !buzzerState;
      digitalWrite(buzzer, buzzerState);
    }
  }

  else if (status == "GAS + FLAME")
  {
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, HIGH);

    // Combined Pattern

    if (millis() - previousMillis >= 80)
    {
      previousMillis = millis();

      buzzerState = !buzzerState;
      digitalWrite(buzzer, buzzerState);
    }
  }

  //---------------- Serial Monitor ----------------

  if (millis() - serialMillis >= 1000)
  {
    serialMillis = millis();

    Serial.print("GAS: ");
    Serial.print(gasPercent);
    Serial.print("%");

    Serial.print(" | FLAME: ");

    if (flameDetected)
      Serial.print("DETECTED");
    else
      Serial.print("NONE");

    Serial.print(" | STATUS: ");
    Serial.println(status);
  }

}
