#include <LedControl.h>

//Snake Struct
typedef struct Snake Snake;
struct Snake{
  int head[2]; // snake head
  int body[40][2]; //a two-dimensional array
  int len; //length
  int dir[2];//direction to move snake
};

//Food Struct
typedef struct Apple Apple;
struct Apple{
  int rPos; //row indexi
  int cPos; //column indexi
};

//led matrix
const int DIN = 12;
const int CS = 11;
const int CLK = 10;
LedControl lc = LedControl(DIN, CLK, CS, 1);

const int varXPin = A3;//X for joistik
const int varYPin = A4; //Y for joistik

byte pic[8] = {0, 0, 0, 0, 0, 0, 0, 0};

Snake snake = {{1,5}, {{0,5}, {1,5}}, 2, {1,0}};//  for snake
Apple apple = {(int)random(0, 8), (int)random(0,8)};// for food

//GameMaster
float oldTime = 0;
float timer = 0;
float updateRate = 3;

int i,j;//counters


void setup(){
  lc.shutdown(0, false);
  //set brwkinvaleba to medium
  lc.setIntensity(0,8);
  //clear display
  lc.clearDisplay(0);

  //set joystick pins as input
  pinMode(varXPin, INPUT);
  pinMode(varYPin, INPUT);

}


void loop(){

  float deltaTime = calculateDeltaTime();
  timer += deltaTime;//unity reference lol

int xVal = analogRead(varXPin);
int yVal = analogRead(varYPin);

if(xVal< 100 && snake.dir[1] == 0){
    snake.dir[0] = 0;
    snake.dir[1] = -1;
  }else if(xVal > 920 && snake.dir[1] == 0){
    snake.dir[0] = 0;
    snake.dir[1] = 1;
  }else if(yVal< 100 && snake.dir[0] == 0){
    snake.dir[0] = -1;
    snake.dir[1] = 0;
  }else if(yVal > 920 && snake.dir[0] == 0){
    snake.dir[0] = 1;
    snake.dir[1] = 0;
  }
  
  
  if(timer > 1000 / updateRate){
    timer = 0;
    Update();
  }

//render
  Render();

}

float calculateDeltaTime(){
  float currentTime = millis();
  float dt = currentTime - oldTime;
  oldTime = currentTime;
  return dt;
}

void reset(){
  for(int i = 0; i < 8; i++){
    pic[i] = 0;
  }
}

void Update(){
  reset(); //clear

  int newHead[2] = {snake.head[0] + snake.dir[0], snake.head[1] + snake.dir[1]};

  if(newHead[0] == 8){
    newHead[0] = 0;
  } else if(newHead[0] == -1){
    newHead[0] = 7;
  }else if(newHead[1] == 8){
    newHead[1] = 0;
  }else if(newHead[1] == -1){
    newHead[1] = 7;
  }

  //if snake eats itself
  for(i = 0; i < snake.len; i++){
    if(snake.body[i][0] == newHead[0] && snake.body[i][1] == newHead[1]){
      //wait for 1wami and reset
      delay(1000);
      snake = {{1,5},{{0,5}, {1,5}}, 2, {1,0}};//reinitialize  snake 
      apple = {(int)random(0,8),(int)random(0,8)};//reinitialize  food 
      return;
    }
  }
  
  
  //if snake eat food
  if(newHead[0] == apple.rPos && newHead[1] ==apple.cPos){
    snake.len = snake.len+1;
    apple.rPos = (int)random(0,8);
    apple.cPos = (int)random(0,8);
  }else{
    removeFirst();
  }
  
  snake.body[snake.len-1][0]= newHead[0];
  snake.body[snake.len-1][1]= newHead[1];
  
  snake.head[0] = newHead[0];
  snake.head[1] = newHead[1];

  for(i = 0; i < snake.len; i++){
    pic[snake.body[i][0]] |= 128 >> snake.body[i][1];
  }
  pic[apple.rPos] |= 128 >> apple.cPos;
  
}

void Render(){
  
   for(j =0 ; j < 8; j++){
    lc.setRow(0, j, pic[j]);
   }
}

void removeFirst(){
  for(i = 1;i < snake.len; i++){
    snake.body[i-1][0] = snake.body[i][0];
    snake.body[i-1][1] = snake.body[i][1];
  }
}

