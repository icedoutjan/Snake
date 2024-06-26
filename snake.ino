#include <LedControl.h>
#include "snake.h"
#include <LiquidCrystal_I2C.h>  //LiquidCrystal I2C by Frank de Brabander
#include <EEPROM.h>

int hightScor;

class apple{
  public:
  void spwanNew();
  void bling();
  int apple_x;
  int apple_y;

  private:
  const float appleInterval = 200.0;  //to controll the speed of the blinking appel
  float lastAppleTime = 0.0;
};

//init of some importent classes
LiquidCrystal_I2C lcd(0x27, 16, 2);
LedControl lc = LedControl(2, 3, 4, 1);
apple apple;
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
  //for martix
  lc.shutdown(0, false);
  lc.setIntensity(0, 1);
  lc.clearDisplay(0);
  //for lcd display
  lcd.init();
  lcd.backlight();
  //set up for game engien 
  snake.setup();
  apple.spwanNew();

  //hightScor
  EEPROM.get(0, hightScor);
}

void loop() {
  
  while(!gameOver){
    //time and fps controll stuff
    currentTime = millis(); // Überprüfe, ob es Zeit für ein neues Frame ist
    if (currentTime - lastFrameTime >= frameInterval) {
      lastFrameTime = currentTime;
      frameInterval = 1000.0 / intervalSpeed; //for speedup reasons

      //game logic
      snake.move(); 
      //check after moving the snake does the snake hit a forbind point 
      gameOver = !snake.alife(); // for performanc reasons 
      eat();
      //visual stuff
      lcdPrint();
      lc.clearDisplay(0);   //clear the led martix
      for(int i = 0;i < snake.len;i++){   //print the snake 
        lc.setLed(0,snake.body_x[i],snake.body_y[i],true);
      }
      
    }
    //also for visualisation but it have to be out side of the if() because the had another time logic 
    apple.bling();
    //input stuff
    snake.updateDirectionJoystick();
  } 
  dead();
  snake.reset();
  gameOver = false;
}

// funktionen im file 
bool feldInUse(int x,int y){  // prüf ob das feld von der schlange belegt ist
  for(int i = 0;i < snake.len;i++){ 
    if (snake.body_x[i] == x){  
      for(int l = 0;l < snake.len;l++){
        if (snake.body_y[l] == y){
          return true;
        }
      }
    }
  } 
  return false;
}

void eat(){ //does the snake eat the appel and if yes speedup a bit and add a part to the snaek
  if(snake.body_x[0] == apple.apple_x && snake.body_y[0] == apple.apple_y){
    snake.len++;
    
    if(snake.len > hightScor){
      hightScor = snake.len;
      EEPROM.put(0, hightScor);
    }
    apple.spwanNew();
  }
}

void lcdPrint() { // vereinfacht die ausgabe auf dem lcd display 
  lcd.clear();
  
  lcd.print("scor ");
  lcd.print(snake.len);
  lcd.setCursor(0, 1);
  
  lcd.print("hight ");
  lcd.print(hightScor);
  
}

void dead(){
  lcd.clear();
  lcd.print("yuor are dead");
  lcd.setCursor(0, 1);
  lcd.print("scor ");
  lcd.print(snake.len);
}

// appel funktionen
void apple::spwanNew(){ //spwan a new appel and speedup the game a bit 
  while(feldInUse(apple_x,apple_y)){
    apple_x = random(0,8); //just random() funktion is useless without randomSeed()
    apple_y = random(0,8);// i am not sure but i think the code is faster with randomSeed()
  }
  intervalSpeed += 0.1;
}

void apple::bling(){ //just a simpel way to let the led blinging wihtout delay
  if (currentTime - lastAppleTime >= appleInterval) {
    lc.setLed(0,apple_x,apple_y,true);
    lastAppleTime = currentTime;
  }
}
