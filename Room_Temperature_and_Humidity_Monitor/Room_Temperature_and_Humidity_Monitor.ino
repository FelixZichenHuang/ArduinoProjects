#include <DHT.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

int inputPort = 3;

int sensorPort = 13;
DHT Sensor(sensorPort, DHT11);

int portRS = 2;
int portE = 6;
int portD4 = 8;
int portD5 = 9;
int portD6 = 10;
int portD7 = 11;
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

int mode;
int buttonState;
int lastButtonState = 0;

float humidity;
float temperature;

bool APH;
bool APT;

void setup() {
  pinMode(inputPort, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  Sensor.begin();
  lcd.begin(16, 2);
  lcd.createChar(0, degree);
}

void loop() {
  mode = EEPROM.read(0);
  buttonState = digitalRead(inputPort);
  if (buttonState == 1 && lastButtonState == 0) {
    if (mode == 0) {
      mode = 1;
    } else if (mode == 1) {
      mode = 0;
    }
    EEPROM.write(0, mode);
  }

  humidity = Sensor.readHumidity();
  if (humidity <= 60 && humidity >= 30) {
    APH = true;
  } else {
    APH = false;
  }
  if (mode == 0) {
    temperature = Sensor.readTemperature();
    if (temperature < 28 && temperature >= 18) {
      APT = true;
    } else {
      APT = false;
    }
  } else if (mode == 1) {
    temperature = Sensor.readTemperature(true);
    if (temperature < 82 && temperature >= 64) {
      APT = true;
    } else {
      APT = false;
    }
  }
  
  lcd.setCursor(0, 0);
  lcd.print("H: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("T: ");
  lcd.print(temperature);
  lcd.write(byte(0));
  if (mode == 0) {
    lcd.print("C");
  } else if (mode == 1) {
    lcd.print("F");
  }

  if (APH && APT) {
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    digitalWrite(5, HIGH);
  }
  if (!APH) {
    digitalWrite(5, LOW);
    digitalWrite(7, HIGH);
  }
  if (!APT) {
    digitalWrite(5, LOW);
    digitalWrite(4, HIGH);
  }

  lastButtonState = buttonState;
}