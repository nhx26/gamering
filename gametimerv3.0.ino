#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        5 // On Trinket or Gemma, suggest changing this to 1

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

int brightness = 30;

void setup() {

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif

  pixels.begin(); 
  
  Serial.begin(115200);


//  status = IMU.begin();
//  if (status < 0) {
//    Serial.println("IMU initialization unsuccessful");
//    Serial.println("Check IMU wiring or try cycling power");
//    Serial.print("Status: ");
//    Serial.println(status);
//    while(1) {}
//  }

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
//  
//  Serial.print("temp : ");Serial.println(mpu6050.getTemp());
//  Serial.print("accX : ");Serial.print(mpu6050.getAccX());
//  Serial.print("\taccY : ");Serial.print(mpu6050.getAccY());
//  Serial.print("\taccZ : ");Serial.println(mpu6050.getAccZ());
//  
//  Serial.print("gyroX : ");Serial.print(mpu6050.getGyroX());
//  Serial.print("\tgyroY : ");Serial.print(mpu6050.getGyroY());
//  Serial.print("\tgyroZ : ");Serial.println(mpu6050.getGyroZ());
//  
//  Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
//  Serial.print("\taccAngleY : ");Serial.println(mpu6050.getAccAngleY());
//  
//  Serial.print("gyroAngleX : ");Serial.print(mpu6050.getGyroAngleX());
//  Serial.print("\tgyroAngleY : ");Serial.print(mpu6050.getGyroAngleY());
//  Serial.print("\tgyroAngleZ : ");Serial.println(mpu6050.getGyroAngleZ());
//  IMU.readSensor();
//  Serial.print(mpu6050.getAccX(),6);
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
//if(detect_flip() == true){
//  mode++;
//  Serial.print("mode: ");
//  Serial.print(mode);
//  Serial.println();
//  pause(2000);
//}
//
//if (mode % 4 == 1){
//  spinner();
//}
//
//if (mode % 4 == 2){
//  dice_roll();
//}
//
//if (mode % 4 == 3){
//  hourglass();
//}
//
//if(mode % 4 == 0){
//  rotation();
//}
rotation();

}

void all_pixels(int r, int g, int b){

  for (int i = 0; i <=15 ; i++){
     pixels.setPixelColor(i, pixels.Color(r, g, b));
 }
    pixels.show(); 
}

void dice_roll(){
  
  if(mpu6050.getAccX()> 5){

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

  if(mpu6050.getAccX()> 5) {
    
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
//  detect_mode();
}


}

void pause(int period){

  unsigned long time_now = millis();
  while(millis() < time_now + period){
    
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

 boolean detect_flip(){
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

if(IMU.getAccelZ_mss() < -avg_z){
  Serial.println("flip");
  return true;
}
else{
  return false;
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
//  Serial.println(angle);
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
  pixels.setPixelColor(player, pixels.Color(brightness, brightness, brightness));
  pixels.show();
  }
  
  else{
  pixels.setPixelColor(prev_pixel, pixels.Color(0, 0, 0));
  pixels.setPixelColor(player, pixels.Color(brightness, brightness, brightness));
  pixels.show();
  prev_pixel = player;
  }
  pixels.setPixelColor(target, pixels.Color(brightness,brightness,0));
  pixels.show();
  
  if(target == player){
     score++;
     pixels.setPixelColor(target, pixels.Color(0,brightness,0));
     pixels.show();
     pause(1000);
     target = random_pixel(0,16);
     level++;
     if(level > 0){
     finish = true;}
  }
  

    if(level > 0 && finish and !gameover){
      
      timer++;
      if(timer % 10 == 0){
    enemy = random_pixel(0,16);
        while(enemy == target || enemy == player){
      enemy = random_pixel(0,16);
        }
    }
    pixels.setPixelColor(enemy,pixels.Color(brightness,0,0));
    pixels.show();
   }

   
   //Serial.println(gameover);
   if(player == enemy){
   pixels.setPixelColor(enemy,pixels.Color(brightness,0,brightness));
   pixels.show();
   gameover = true;

  }
  }

  if(gameover){
    show_score(score);
    Serial.println("over");
    if ((abs(mpu6050.getAccX()) > 1)||(abs(mpu6050.getAccY()) > 1)){
     gameover = false;
     showscore = true;
     score = 0;
     pause(1000);
     level = 0;

     //blink
     all_pixels(brightness, brightness, brightness);
     pause(1000);
     all_pixels(0, 0, 0);
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
     pixels.setPixelColor(i, pixels.Color(brightness, brightness, 0));
     pixels.show();
     pause(500);
   }
  }
