#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        9 // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 16 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 

#include "MPU9250.h"

#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

MPU9250 IMU(Wire,0x68);

int status;

int mode = 0;

boolean flip = true; 

int prev_pixel = 8;
int current_pixel;

double total_z = 0;
int count_data = 0;

int level = 0;

int target = 10;

int score = 0;

boolean finish = false;

boolean gameover = false;

int enemy = 50;

boolean showscore = true;

int timer = 0;

void setup() {

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels.begin(); 
  
  Serial.begin(115200);


  status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }

  randomSeed(analogRead(0));

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

}

void loop() {

  mpu6050.update();
//  Serial.print("angleX : ");
//  Serial.print(mpu6050.getAngleX());
//  Serial.print("\tangleY : ");
//  Serial.print(mpu6050.getAngleY());
//  Serial.print("\tangleZ : ");
//  Serial.println(mpu6050.getAngleZ());
  
  IMU.readSensor();
//  Serial.print(IMU.getAccelX_mss(),6);
//  Serial.print("\t");
//  Serial.print(IMU.getAccelY_mss(),6);
//  Serial.print("\t");
//  Serial.println(IMU.getAccelZ_mss(),6);
//  Serial.print("\t");
//  Serial.print(IMU.getGyroX_rads(),6);
//  Serial.print("\t");
//  Serial.print(IMU.getGyroY_rads(),6);
//  Serial.print("\t");
//  Serial.print(IMU.getGyroZ_rads(),6);
//  Serial.print("\t");
//  Serial.print(IMU.getMagX_uT(),6);
//  Serial.print("\t");
//  Serial.print(IMU.getMagY_uT(),6);
//  Serial.print("\t");
//  Serial.print(IMU.getMagZ_uT(),6);
//  Serial.print("\t");
//  Serial.println(IMU.getTemperature_C(),6);
//  pause(500);


//
//  detect_mode();
//detect_tap();
//
//rotate_light();

//rotate_game();

rotation();
//Serial.println(gameover);

if (mode == 1){
  //spinner();

}

if (mode == 2){
  dice_roll();
}

if (mode == 3){
  hourglass();
}


}

void all_pixels(int r, int g, int b){

  for (int i = 0; i <=15 ; i++){
     pixels.setPixelColor(i, pixels.Color(r, g, b));
 }
    pixels.show(); 
}

void dice_roll(){
  
  if(IMU.getAccelX_mss()> 5){

  for (int i = 0; i < rand; i++){
  pixels.setPixelColor(i, pixels.Color(0, 0,0));
  }
  pixels.show();
  int rand = random(1,12);
    for (int i = 0; i <rand ; i+=2){
     pixels.setPixelColor(i, pixels.Color(20,20,20));
     pixels.show(); 
     pause(200);
    }

    for (int i = 0; i <rand ; i+=2){
     pixels.setPixelColor(i, pixels.Color(20,0,20));
      
     
    }
    pixels.show();
    pause(2000);
  }

  for (int i = 0; i < rand; i++){
  pixels.setPixelColor(i, pixels.Color(20, 0,0));
  }
  pixels.show();
   
}

void spinner(){

  if(IMU.getAccelX_mss()> 5) {
    
  for (int i = 0; i < rand; i++){
  pixels.setPixelColor(i, pixels.Color(0, 0,0));
  }
  pixels.show();
  int rand = random(1,16);

  for (int i = 0; i < rand; i++){
  pixels.setPixelColor(i, pixels.Color(20, 20,20));
  pixels.setPixelColor(i+1, pixels.Color(20, 20,20));
  pixels.show();
  pause(200);
  pixels.setPixelColor(i, pixels.Color(0, 0,0));
  pixels.setPixelColor(i+1, pixels.Color(0, 0,0));
  pixels.show();
  }
  pixels.setPixelColor(rand, pixels.Color(20, 20,20));
  pixels.setPixelColor(rand+1, pixels.Color(20, 20,20));
  pixels.show();
  pause(200);
  pixels.setPixelColor(rand, pixels.Color(20, 20,0));
  pixels.setPixelColor(rand+1, pixels.Color(20, 20,0));
  pixels.show();
  pause(2000);
  } 

  for (int i = 0; i < rand; i++){
  pixels.setPixelColor(i, pixels.Color(20, 0,0));
  }
  pixels.show();
  
}

void hourglass(){

if (mpu6050.getAngleY() > 0 and flip){
    for (int i = 0; i < 16 ; i++){
     pixels.setPixelColor(i, pixels.Color(20,10,30));
     pixels.show(); 
     pause(200);
     flip = false;
    }
}

if (mpu6050.getAngleY() < 0 and !flip){
    for (int i = 0; i < 16 ; i++){
     pixels.setPixelColor(i, pixels.Color(0,0,0));
     pixels.show(); 
     pause(200);
     flip = true;
    }
} 

if(!flip){
  detect_mode();
}


}

void pause(int period){

  unsigned long time_now = millis();
  while(millis() < time_now + period){
    
  }
}

void detect_mode(){

  if(IMU.getAccelY_mss() > 20){
    mode++;
    if(mode > 3){
      mode = 0;
    }
    //Serial.println(mode);
    all_pixels(10,10,10);
    pause(500);
    all_pixels(0,0,0);
    pause(500);
    for (int i = 0; i < mode ; i++){
    pixels.setPixelColor(i, pixels.Color(0,10,0));
    }
    pixels.show();
    pause(500);
  }
}


void rotate_light_z(){

  int deg;
  if(mpu6050.getAngleZ() > 0){
   deg = abs(int(mpu6050.getAngleZ())) %360;
  }
  
  else{
   deg = (360-abs(int(mpu6050.getAngleZ()))) %360;
  }

  current_pixel = deg / 22.5;

  if(prev_pixel == current_pixel){
  pixels.setPixelColor(15 - prev_pixel, pixels.Color(0,0,10));
  pixels.show();
  }

  else{
  pixels.setPixelColor(15 - prev_pixel, pixels.Color(0,0,0));
  pixels.setPixelColor(15 - current_pixel, pixels.Color(0,0,10));
  
  pixels.show();
  prev_pixel = current_pixel;
  }
  
}

boolean detect_tap(){
   double avg_z;
  if(count_data == 0){
  unsigned long time_now = millis();
  while(millis() < time_now + 5000){
    total_z += IMU.getAccelZ_mss();
    Serial.print(count_data);
    count_data++;
  }
  avg_z = total_z / count_data;  
  }

  if( abs(avg_z - IMU.getAccelZ_mss()) > 1){
    pixels.setPixelColor(3, pixels.Color(10,0,0));
    pixels.show();
    Serial.println("tap");
    return true;
    unsigned long time_now = millis();
    while(millis() < time_now + 500){
   
    }
    while(millis() < time_now + 2000){
   
    }
    pixels.setPixelColor(3, pixels.Color(0,0,0));
    pixels.show();
    }

    if( abs(avg_z - IMU.getAccelZ_mss()) > 2){
      
    }
  }

void rotate_game(){
  
 int deg;
  if(mpu6050.getAngleZ() > 0){
   deg = abs(int(mpu6050.getAngleZ())) %360;
  }
  
  else{
   deg = (360-abs(int(mpu6050.getAngleZ()))) %360;
  }

  current_pixel = deg / 22.5;

  if(prev_pixel == current_pixel){
  pixels.setPixelColor(15 - prev_pixel, pixels.Color(0,0,20));
  
  pixels.setPixelColor(15 - prev_pixel + 1, pixels.Color(0,0,5));
  pixels.show();
  }

  else{
  pixels.setPixelColor(15 - prev_pixel, pixels.Color(0,0,0));
  pixels.setPixelColor(15 - prev_pixel + 1, pixels.Color(0,0,0));
  pixels.setPixelColor(15 - current_pixel, pixels.Color(0,0,20));
  pixels.setPixelColor(15 - current_pixel + 1, pixels.Color(0,0,5));
  pixels.show();
  prev_pixel = current_pixel;
  }

  pixels.setPixelColor(target, pixels.Color(20,0,0));
    
  if (target == (15-current_pixel)){
    pixels.setPixelColor(target, pixels.Color(20,20,0));
    pixels.show();
    pause(1000);
    target = random_pixel(0,16);
  }
  
}

int random_pixel(int min_, int max_){
    int target = random(min_ ,max_);
    pixels.setPixelColor(target, pixels.Color(10,0,0));
    pixels.show();
    return target;
}

void rotation(){
//  int sumx=0;
//  int sumy=0;
//
  float avg_y;
  float avg_x;

 
//  for (int i = 0;i<10;i++){
//    sumx+=mpu6050.getAccX();
//    sumy+=mpu6050.getAccY();
//    
//  }

  
  avg_y=mpu6050.getAccY();
  avg_x=mpu6050.getAccX();

  
  int angle = atan2(avg_y,avg_x) * 180 / 3.14f;
  Serial.println(angle);
  int angleDash = (angle + 360) % 360;

  int current_pixel = (int)(((angleDash / 360.0f) * 16.0f));
  int player = 15 - current_pixel;
  //Serial.println(player);
  pause(50);
//Serial.println(enemy);
  if(!gameover){
  for (int i = 0; i < 16; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }

  if(prev_pixel == player){
  pixels.setPixelColor(player, pixels.Color(10, 10, 10));
  pixels.show();
  }
  
  else{
  pixels.setPixelColor(prev_pixel, pixels.Color(0, 0, 0));
  pixels.setPixelColor(player, pixels.Color(10, 10, 10));
  pixels.show();
  prev_pixel = player;
  }
  pixels.setPixelColor(target, pixels.Color(10,10,0));
  pixels.show();
  
  if(target == player){
     score++;
     pixels.setPixelColor(target, pixels.Color(0,10,0));
     pixels.show();
     pause(1000);
     target = random_pixel(0,16);
     level++;
     if(level > 1){
     finish = true;}
           Serial.print("zero");
  }
  
   if(level > 1 && level < 5 && finish){
    enemy = random_pixel(0,16);
    
    while(enemy == target || enemy == player){
      enemy = random_pixel(0,16);
      Serial.print("one");
    }
          Serial.print("two");
    pixels.setPixelColor(enemy,pixels.Color(10,0,0));
    pixels.show(); 
   
    finish = false;
   }

   if(level > 1 && level < 5 && !finish){
    pixels.setPixelColor(enemy,pixels.Color(10,0,0));
    pixels.show();
          Serial.print("three");
   }

    if(level > 4 && finish){
      timer++;
      if(timer % 10 == 0){
    enemy = random_pixel(0,16);
      }
    pixels.setPixelColor(enemy,pixels.Color(10,0,0));
    pixels.show();
   }

   
   //Serial.println(gameover);
   if(player == enemy){
   pixels.setPixelColor(enemy,pixels.Color(10,0,10));
   pixels.show();
   gameover = true;
         Serial.print("four");
  }
  }

  if(gameover){
    show_score(score);
    Serial.print("five");
    if (IMU.getAccelX_mss() > 15){
     gameover = false;
     showscore = true;
     score = 0;
     pause(1000);
   }
  }

  }

  void show_score(int score){
      if(showscore){
      for (int i = 0; i < 16; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  }
  pixels.show();
      pause(2000);
      showscore = false;
      }
      
   for (int i = 0; i < score; i++){
     pixels.setPixelColor(i, pixels.Color(10, 10, 0));
     pixels.show();
     pause(500);
   }
  }
