#include <LedControl.h>
//#include "appel.h"
#include <Keypad.h>

const byte ROWS = 4; // Anzahl der Zeilen des Keypads
const byte COLS = 4; // Anzahl der Spalten des Keypads

// Definition der Zeilen- und Spaltenpins des Keypads
byte rowPins[ROWS] = { 53, 52, 51, 50 }; // Verbinde die Zeilen des Keypads mit diesen Pins
byte colPins[COLS] = { 49, 48, 47, 46 }; // Verbinde die Spalten des Keypads mit diesen Pins

// Definition der Tastatur
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

LedControl lc = LedControl(2, 3, 4, 1);
int snakeDirection = 3; // Anfangsrichtung: 0 = links, 1 = rechts, 2 = oben, 3 = unten

bool debug = true;
bool gameOver =false;
class snake{
  public:
  int len = 4;
  int body_x[64];
  int body_y[64];
  void move(int input);
  bool alife();
};
class appel : public snake{
  public:
  void spwanNew();
  void blick();

  
  int appel_x;
  int appel_y;
  int appelInterval = 10;
};
appel appel;
snake snake;

float intervalSpeed = 0.7;

 float lastFrameTime = 0.0; // Zeit des letzten Frames
 float frameInterval = 1000.0 / intervalSpeed; // Zeitintervall in Millisekunden

void setup() {
  
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  
  lc.clearDisplay(0);
  appel.spwanNew();
}

void loop() {
  // Überprüfe, ob es Zeit für ein neues Frame ist
  while(!gameOver){
  unsigned long currentTime = millis();
  if (currentTime - lastFrameTime >= frameInterval) {
    float t = currentTime - lastFrameTime;  // display the speed in case of debug
    lastFrameTime = currentTime;
    frameInterval = 1000.0 / intervalSpeed; //for speedup reasons 
    lc.clearDisplay(0);

    snake.move(snakeDirection);
    if(!snake.alife()){
      gameOver = true;
    }
    
    eat();
    appel.blick();
    for(int i = 0;i < snake.len;i++){
      lc.setLed(0,snake.body_x[i],snake.body_y[i],true);
    }

    if(debug){
      Serial.print("x ");
      Serial.println(snake.body_x[0]);
      Serial.print("y ");
      Serial.println(snake.body_y[0]);
      Serial.print("Aktualisiere Frame ");
      Serial.println(t);
    }
  }
  
  //input stuff
  char key = keypad.getKey();
  if (key) {
    if (key == '8' && snakeDirection != 3) snakeDirection = 2; // Nach oben
    else if (key == '2' && snakeDirection != 2) snakeDirection = 3; // Nach unten
    else if (key == '4' && snakeDirection != 1) snakeDirection = 0; // Nach links
    else if (key == '6' && snakeDirection != 0) snakeDirection = 1; // Nach rechts
  }

 
 } 
}



void appel::spwanNew(){ //spwan a new appel and speedup the game a bit 
  while(feldInUse(appel_x,appel_y)){
  appel_x = random(8);
  appel_y = random(8);
  }
  intervalSpeed += 0.1;
}

void appel::blick(){
  lc.setLed(0,appel_x,appel_y,true);
}

bool feldInUse(int x,int y){  // prüf ob das feld von der schlange belegt ist
  for(int i = 0;i < snake.len;i++){ 
    if (snake.body_x[i] == x){
      for(int l = 0;l < snake.len;i++){
        if (snake.body_y[l] == y){
          return true;
        }
      }
    }
  } 
  return false;
}

void snake::move(int input){
  for (int i = len - 1; i > 0; i--) { 
    body_x[i] = body_x[i - 1];
    body_y[i] = body_y[i - 1];
  }
  switch (input) {
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
}
bool snake::alife(){
  if(body_y[0] == 9 | body_y[0] == -1 | body_x[0] == 9 | body_x[0] == -1){
    return false;
  }
  for(int i = 1;i < len;i++){
    if(body_y[0] == body_y[i] && body_x[0] == body_x[i]){
      return false;
    }
  }
  return true;
}
void eat(){
  if(snake.body_x[0] == appel.appel_x && snake.body_y[0] == appel.appel_y){
    snake.len++;
    appel.spwanNew();
  }
}
