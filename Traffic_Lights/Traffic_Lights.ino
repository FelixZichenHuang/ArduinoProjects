void setup() {
  for (int i = 2; i <= 11; i++) {
    pinMode(i, OUTPUT);
  }
}

void clearAll() {
  for (int i = 5; i <= 11; i++) {
    digitalWrite(i, LOW);
  }
}

void digitalOne() {
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
}

void digitalTwo() {
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(11, HIGH);
}

void digitalThree() {
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(11, HIGH);
}

void digitalFour() {
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
}

void digitalFive() {
  digitalWrite(5, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
}

void digitalSix() {
  digitalWrite(5, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
}

void digitalSeven() {
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
}

void digitalEight() {
  for (int i = 5; i <= 11; i++) {
    digitalWrite(i, HIGH);
  }
}

void digitalNine() {
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
}

void loop() {
  digitalWrite(2, HIGH);
  delay(167000);
  digitalWrite(2, LOW);
  delay(200);
  digitalWrite(2, HIGH);
  delay(2800);
  digitalNine();
  delay(1000);
  clearAll();
  digitalEight();
  delay(1000);
  clearAll();
  digitalSeven();
  delay(1000);
  clearAll();
  digitalSix();
  delay(1000);
  clearAll();
  digitalFive();
  delay(1000);
  clearAll();
  digitalFour();
  delay(1000);
  clearAll();
  digitalThree();
  delay(1000);
  clearAll();
  digitalTwo();
  delay(1000);
  clearAll();
  digitalOne();
  delay(1000);
  clearAll();
  digitalWrite(2, LOW);

  digitalWrite(4, HIGH);
  delay(47000);
  digitalWrite(4, LOW);
  delay(200);
  digitalWrite(4, HIGH);
  delay(2800);
  digitalNine();
  delay(1000);
  clearAll();
  digitalEight();
  delay(1000);
  clearAll();
  digitalSeven();
  delay(1000);
  clearAll();
  digitalSix();
  delay(1000);
  clearAll();
  digitalFive();
  delay(1000);
  clearAll();
  digitalFour();
  delay(1000);
  clearAll();
  digitalThree();;
  digitalWrite(4, LOW);
  delay(500);
  digitalWrite(4, HIGH);
  delay(500);
  clearAll();
  digitalTwo();
  digitalWrite(4, LOW);
  delay(500);
  digitalWrite(4, HIGH);
  delay(500);
  clearAll();
  digitalOne();
  digitalWrite(4, LOW);
  delay(500);
  digitalWrite(4, HIGH);
  delay(500);
  clearAll();
  digitalWrite(4, LOW);

  digitalWrite(3, HIGH);
  digitalThree();
  delay(1000);
  clearAll();
  digitalTwo();
  delay(1000);
  clearAll();
  digitalOne();
  delay(1000);
  clearAll();
  digitalWrite(3, LOW);
}