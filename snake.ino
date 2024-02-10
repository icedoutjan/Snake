#include <LedControl.h>
#include "snake.h"

class appel{
  public:
  void spwanNew();
  void bling();
  int appel_x;
  int appel_y;

  private:
  const float appelInterval = 200.0;  //to controll the speed of the blinking appel
  float lastAppelTime = 0.0;
};

//init of some importent classes
LedControl lc = LedControl(2, 3, 4, 1);
appel appel;
snake snake;

//to controll die fps of the main game
float intervalSpeed = 1.0;
float lastFrameTime = 0.0; // Zeit des letzten Frames
float frameInterval = 1000.0 / intervalSpeed; // Zeitintervall in Millisekunden
unsigned long currentTime;

bool gameOver = false;  //for performanc reasones 

void setup() {
  //notic with this funktion it tooks longer to break the arduino
  randomSeed(analogRead(0)); //for a realistic random effekt 
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  
  lc.clearDisplay(0);
  appel.spwanNew();
}

void loop() {
  
  while(!gameOver){

    //time and fps controll stuff
    currentTime = millis(); // Überprüfe, ob es Zeit für ein neues Frame ist
    if (currentTime - lastFrameTime >= frameInterval) {
      lastFrameTime = currentTime;
      frameInterval = 1000.0 / intervalSpeed; //for speedup reasons

      //game logic
      snake.move(); //edit the array that i use to print the snake based on the direction
      gameOver = !snake.alife(); // for performanc reasons 
      eat(); //does the snake eat teh appel and if yes speedup a bit and add a part to the snaek

      //visual stuff
      lc.clearDisplay(0);   //clear the led martix
      for(int i = 0;i < snake.len;i++){ //print the snake 
        lc.setLed(0,snake.body_x[i],snake.body_y[i],true);
      }
    }
    //also for visualisation but it have to be out side of the if() because the had another time logic 
    appel.bling();
    //input stuff
    snake.updateDirection();
  } 
}

// funktionen im file 
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

void eat(){
  if(snake.body_x[0] == appel.appel_x && snake.body_y[0] == appel.appel_y){
    snake.len++;
    appel.spwanNew();
  }
}

// appel funktionen
void appel::spwanNew(){ //spwan a new appel and speedup the game a bit 
  while(feldInUse(appel_x,appel_y)){
    appel_x = random(0,8); //just random() funktion is useless without randomSeed()
    appel_y = random(0,8);// i am not sure but i think the code is faster with randomSeed()
  }
  intervalSpeed += 0.1;
}

void appel::bling(){ //just a simpel way to let the led blinging wihtout delay
  if (currentTime - lastAppelTime >= appelInterval) {
    lc.setLed(0,appel_x,appel_y,true);
    lastAppelTime = currentTime;
  }
}
