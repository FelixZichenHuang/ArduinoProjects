/*
  Crane

  This crane is controlled by a joystick and is designed to stack blocks. The arm move up when joystick is pushed up, down when joystick is pushed down, left when joystick is pushed left, and right when joystick is pused right.

  The circuit:
  - Joystick
    Pin 1 (GND) of the joystick is connected to the ground pin on the Arduino board.
    Pin 2 (+5V) of the joystick is connected to the +5V pin on the Arduino board.
    Pin 3 (X-Axis) of the joystick is connected to the analog input A1.
    Pin 4 (Y-Axis) of the joystick is connected to the analog input A0.
    Pin 5 (Push button) of the joystick is connected to the digital input 2.
  - Servo Motors
    The servo motor used to control the left and right movement of the arm is attached to pin 8.
    The servo motor used to control the up and down movement of the arm is attached to pin 9.
 */

#include <Servo.h>
#include <EEPROM.h>

Servo myservo_8;  // create servo object to control a servo
Servo myservo_9;  // create servo object to control a servo

int x = 0;    // variable to read the value from the analog pin (for x)
int y = 0;    // variable to read the value from the analog pin (for y)
int mappedX;  // variable of the value from the to add to posX mapped from "x"
int mappedY;  // variable of the value from the to add to posY mapped from "y"
int posX;     // variable of the position of the x-motor
int posY;     // variable of the position of the y-motor
int toX;     // variable of the position of the x-motor
int toY;     // variable of the position of the y-motor
int save = 0; // State of saving
int state;    // The current state of the push button (Off = 1, On = 0)
bool override = false; 
bool release = true;    // Checks if the button has been relased since last pushed (used for debouncing)
bool action = true;     // Checks if an action has already been done since last pushed (used for memory memorizing, basically avoiding repeated repositioning)

void setup() {
  Serial.begin(9600);
  posX = EEPROM.read(3);   // read from address 3
  posY = EEPROM.read(4);   // read from address 4
  pinMode(A0, INPUT);       //JoyStick x input
  pinMode(A1, INPUT);       //JoyStick y input
  pinMode(2, INPUT_PULLUP); //Button input recieving
  pinMode(8, OUTPUT);       //Outputs value for the x motor
  pinMode(9, OUTPUT);       //Outputs value for the y motor
  myservo_8.attach(8);      // attaches the servo on pin 9 to the servo object (y-motor)
  myservo_9.attach(9);      // attaches the servo on pin 9 to the servo object (x-motor)
  save = EEPROM.read(2); // Reads if last move is saving position (if yes, next time button press will directly lead to the previous saved position)
}

void loop() {
  x = analogRead(A0);            // reads the value of the joystick x-value (value between 0 and 1023)
  y = analogRead(A1);            // reads the value of the joystick y-value (value between 0 and 1023)
  state = digitalRead(2);        // reads the state the pushbutton on the joystick (value is 0 or 1)
  Serial.print(state); 
  Serial.print("\n");
  if (state == LOW && release) {  //If state is 1(When button is pressed, for the first tick, increases the save
    save++;               //Changing between save state or move to state
    release = false;      //Gate for this only running once
    action = true;        //When there is action done, allows the if state to be activated once
  } else if (state == HIGH) {  //Reset to not pressed state when released
    release = true;
  }
  mappedX = map(x, 0, 1023, -1, 2);     // scale it to use it with the servo (value between 0 and 180)
  mappedY = map(y, 0, 1023, -1, 2);     // scale it to use it with the servo (value between 0 and 180)

  if (!override){
    if (posX > 179){ //positioning for x & y (when going out of bounds, return them to a value before they go out of bounds)
      posX = 179;
    } else if(posX < 2) {
      posX = 2;
    } else {
      posX += mappedX;
    }
    if (posY > 40) {
      posY = 40;
    } else if(posY < 0) {
      posY = 0;
    } else {
      posY += mappedY;
    }
  } else {      // If the crane is in override mode (e.g. going back to setted position, slowly moves the crane to the selected position)
    if (posX < toX) {
      posX++;
    } else if (posX > toX) {
      posX--;
    }
    if (posY < toY) {
      posY++;
    } else if (posY > toY) {
      posY--;
    }
    if (posX == toX && posY == toY) {
      override = false;
    }
  }

  if (action) {
    if (save % 2 == 1) {
      EEPROM.write(0, posX); 
      EEPROM.write(1, posY);
      EEPROM.write(2, save);
    } else if (save % 2 == 0) {
      toX = EEPROM.read(0);   // read from address 0
      toY = EEPROM.read(1);   // read from address 1
      override = true;        // makes the code to the override mode (ignores inputs until is moved to the last position)
    } 
    action = false; //Prevents this from running repeatedly
  }
  myservo_8.write(posX);  //Writes x pos to x motor
  myservo_9.write(posY);  //Wrties y pos to y motor
  EEPROM.write(3, posX); 
  EEPROM.write(4, posY); //Remembering last postion when power is disconnected
  delay(40);                                // waits for the servo to get there
}
