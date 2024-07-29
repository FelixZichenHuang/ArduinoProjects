#include <DHT.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

int trigPort = 0;
int echoPort = 1;

int sensorPort = 12;
DHT Sensor(sensorPort, DHT11);

int portRS = 2;
int portE = 4;
int portD4 = 7;
int portD5 = 8;
int portD6 = 9;
int portD7 = 10;
LiquidCrystal lcd(portRS, portE, portD4, portD5, portD6, portD7);
byte degree[8] = {
  0b00110,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

int fanPort = 13;

int buttonOnePin = 3;
int buttonTwoPin = 5;
int buttonThreePin = 6;
int buttonFourPin = 11;
int potentiometerPin = A0;

int temperatureUnitMode;
float duration;
int distance;
int displayMode = 1;
int buttonOneState;
int buttonOnePreviousState = 0;
int buttonTwoState;
int buttonTwoPreviousState = 0;
int buttonThreeState;
int buttonThreePreviousState = 0;
int buttonFourState;
int buttonFourPreviousState = 0;
bool fanOn = false;
unsigned long fanOnTime;

void setup() {
  pinMode(trigPort, OUTPUT);
  pinMode(echoPort, INPUT);
  pinMode(fanPort, OUTPUT);
  pinMode(buttonOnePin, INPUT);
  pinMode(buttonTwoPin, INPUT);
  pinMode(buttonThreePin, INPUT);
  pinMode(buttonFourPin, INPUT);
  pinMode(potentiometerPin, INPUT);
  Sensor.begin();
  lcd.begin(16, 2);
  lcd.createChar(0, degree);
}

void detectDistance() {
  digitalWrite(trigPort, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPort, LOW);
  duration = pulseIn(echoPort, HIGH);
  distance = duration / 2 * 0.0343;
}

void unitSwitching() {
  if (buttonTwoState == 1 && buttonTwoPreviousState == 0) {
    if (displayMode == 1 || displayMode == 3) {
      if (temperatureUnitMode == 1) {
        temperatureUnitMode = 2;
      } else if (temperatureUnitMode == 2) {
        temperatureUnitMode = 1;
      }
      EEPROM.write(3, temperatureUnitMode);
    }
  }
  if (temperatureUnitMode == 1) {
    lcd.print("C");
  } else if (temperatureUnitMode == 2) {
    lcd.print("F");
  }
  buttonTwoPreviousState = buttonTwoState;
}

void homeMode() {
  int maximumDistance;
  float temperature;
  float maximumTemperatureCelcius;
  float maximumTemperatureFarenheit;
  if (EEPROM.read(0) == 255) {
    maximumDistance = 10;
  } else {
    maximumDistance = EEPROM.read(0);
  }
  if (EEPROM.read(1) == 255) {
    maximumTemperatureCelcius = 22;
  }
  if (EEPROM.read(2) == 255) {
    maximumTemperatureFarenheit = 72;
  } else {
    maximumTemperatureCelcius = EEPROM.read(1);
    maximumTemperatureFarenheit = EEPROM.read(2);
  }
  if (EEPROM.read(3) == 255) {
    temperatureUnitMode = 1;
  } else {
    temperatureUnitMode = EEPROM.read(3);
  }
  lcd.clear();
  detectDistance();  
  if (temperatureUnitMode == 1) {
    temperature = Sensor.readTemperature();
  } else if (temperatureUnitMode == 2) {
    temperature = Sensor.readTemperature(true);
  }
  if (distance <= maximumDistance) {
    if (temperatureUnitMode == 1 && temperature >= maximumTemperatureCelcius) {
      fanOnTime = millis();
      digitalWrite(fanPort, HIGH);
      fanOn = true;
    } else if (temperatureUnitMode == 2 && temperature >= maximumTemperatureFarenheit) {
      fanOnTime = millis();
      digitalWrite(fanPort, HIGH);
      fanOn = true;
    } else {
      digitalWrite(fanPort, LOW);
      fanOn = false;
    }
  } else {
    digitalWrite(fanPort, LOW);
    fanOn = false;
  }
  buttonTwoState = digitalRead(buttonTwoPin);
  lcd.setCursor(0, 0);
  lcd.print(temperature);
  lcd.write(byte(0));
  unitSwitching();
  lcd.setCursor(0, 1);
  lcd.print("Fan ");
  if (fanOn == true) {
    lcd.print("On");
  } else {
    lcd.print("Off");
  }
  delay(100);
}

void distanceSettingMode() {
  lcd.clear();
  detectDistance();
  lcd.setCursor(0, 0);
  lcd.print("Set Distance: ");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");
  buttonOneState = digitalRead(3);
  if (buttonOneState == 1 && buttonOnePreviousState == 0) {
    EEPROM.write(0, distance);
  }
  buttonOneState == buttonOnePreviousState;
  delay(100);
}

void temperatureSettingMode() {
  int temperatureCelcius;
  int temperatureFarenheit;
  int potentiometerState;
  if (EEPROM.read(3) == 255) {
    temperatureUnitMode = 1;
  } else {
    temperatureUnitMode = EEPROM.read(3);
  }
  lcd.clear();
  potentiometerState = analogRead(potentiometerPin);
  temperatureCelcius = map(potentiometerState, 0, 1023, 16, 30);
  EEPROM.write(1, temperatureCelcius);
  temperatureFarenheit = map(potentiometerState, 0, 1023, 61, 86);
  EEPROM.write(2, temperatureFarenheit);
  buttonTwoState = digitalRead(buttonTwoPin);
  lcd.setCursor(0, 0);
  lcd.print("Set Temperature: ");
  lcd.setCursor(0, 1);
  if (temperatureUnitMode == 1) {
    lcd.print(temperatureCelcius);
  } else if (temperatureUnitMode == 2) {
    lcd.print(temperatureFarenheit);
  }
  lcd.write(byte(0));
  unitSwitching();
  delay(100);
}

void powerConsumptionMonitoringMode() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Power Consumed: ");
  lcd.setCursor(0, 1);
  lcd.print(fanOnTime / 1000 * 3.5 * 0.06);
  lcd.print(" J");
  delay(100);
}

void loop() {
  buttonThreeState = digitalRead(buttonThreePin);
  buttonFourState = digitalRead(buttonFourPin);
  if (buttonThreeState == 1 && buttonThreePreviousState == 0) {
    if (displayMode == 3) {
      displayMode = 1;
    } else {
      displayMode += 1;
    }
  }
  if (buttonFourState == 1 && buttonFourPreviousState == 0) {
    if (displayMode == 4) {
      displayMode = 1;
    } else {
      displayMode = 4;
    }
  }
  if (displayMode == 1) {
    homeMode();
  } else if (displayMode == 2) {
    digitalWrite(fanPort, LOW);
    fanOn = false;
    distanceSettingMode();
  } else if (displayMode == 3) {
    temperatureSettingMode();
  } else if (displayMode == 4) {
    digitalWrite(fanPort, LOW);
    fanOn = false;
    powerConsumptionMonitoringMode();
  }
  buttonThreePreviousState = buttonThreeState;
  buttonFourPreviousState = buttonFourState;
}