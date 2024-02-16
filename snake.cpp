#include "Arduino.h"
#include <Keypad.h>
#include "snake.h"


#define VRX_PIN  A0 // Arduino pin connected to VRX pin
#define VRY_PIN  A1 // Arduino pin connected to VRY pin
#define SW_PIN   A2

// keypad or input stuff
const byte ROWS = 4; // Anzahl der Zeilen des Keypads
const byte COLS = 4; // Anzahl der Spalten des Keypads

// Definition der Zeilen- und Spaltenpins des Keypads
byte rowPins[ROWS] = { 53, 52, 51, 50 }; // Verbinde die Zeilen des Keypads mit diesen Pins
byte colPins[COLS] = { 49, 48, 47, 46 }; // Verbinde die Spalten des Keypads mit diesen Pins

// Definition der Tastatur
const char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void snake::move(){ // edit the array that i use to print the snake based on the direction
  for (int i = len - 1; i > 0; i--) { // move the body forward
    body_x[i] = body_x[i - 1];
    body_y[i] = body_y[i - 1];
  }
  switch (direction) {  // check for direction and move in the head
    case 0: // Links
      body_x[0] -= 1;
      break;
    case 1: // Rechts
      body_x[0] += 1;
      break;
    case 2: // Oben
      body_y[0] -= 1;
      break;
    case 3: // Unten
      body_y[0] += 1;
      break;
  }
  lastDirection = direction;
}

bool snake::alife(){  // check for colliton 
  if(body_y[0] == 8 | body_y[0] == -1 | body_x[0] == 8 | body_x[0] == -1){  // against the wall
    return false;
  }
  for(int i = 4;i < len;i++){ // the loop starts at 4 becaus its impossible that the snake hits them self in this parts 
    if(body_y[0] == body_y[i] && body_x[0] == body_x[i]){ //against the snake 
      return false;
    }
  }
  return true;
}

void snake::updateDirection(){  // get the input and update the direction 
  char key = keypad.getKey();
  if (key) {
    if (key == '8' && lastDirection != 3) direction = 2; // Nach oben
    else if (key == '2' && lastDirection != 2) direction = 3; // Nach unten
    else if (key == '4' && lastDirection != 1) direction = 0; // Nach links
    else if (key == '6' && lastDirection != 0) direction = 1; // Nach rechts
  }
}
void snake::updateDirectionJoystick(){
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);

  if ((xValue >= 975 && xValue <= 1023) && lastDirection != 3) direction = 2; // Nach oben
    else if ((xValue >= 0 && xValue <= 50) && lastDirection != 2) direction = 3; // Nach unten
    else if ((yValue >= 975 && yValue <= 1023) && lastDirection != 1) direction = 0; // Nach links
    else if ((yValue >= 0 && yValue <= 50) && lastDirection != 0) direction = 1; // Nach rechts
}
void snake::setup(){
  for(int i = len;i < maxlen;i++){
    body_y[i] = -1;
    body_x[i] = -1;
  }
}
void snake::reset(){
  len = 3;
  lastDirection = 3;
  direction = 3;

  for(int i = 0;i < len;i++){
    body_y[i] = 0;
    body_x[i] = 0;
  }
  for(int i = len;i < maxlen;i++){
    body_y[i] = -1;
    body_x[i] = -1;
  }
  while(true){
    //wait till press the stick down
    if(analogRead(SW_PIN) == 0){
      break;
    }
  }
}
