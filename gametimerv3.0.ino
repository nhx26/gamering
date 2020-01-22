#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN 5 // On Trinket or Gemma, suggest changing this to 1

#define NUMPIXELS 16 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500

#include "MPU9250.h"

#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

#include <SparkFunMPU9250-DMP.h>

MPU9250_DMP imu;

int status;

int mode = 0;

boolean flip = true;

int prev_pixel = 8;
int current_pixel;

double total_z = 0;
int count_data = 0;

int level = 0;
int level_tap = 0;

int target = 10;

int score = 0;
int score_tap = 0;

boolean finish = false;

boolean gameover = false;

boolean gameover_tap = false;

int enemy = 50;

boolean showscore = true;

int timer = 0;

int brightness = 30;

int current_pos = 0;
int prev_pos;

float avg_z;

int passby = 0;

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin();

  Serial.begin(115200);

  randomSeed(analogRead(0));

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);


  //tap detection
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }
  // Enable tap detection in the DMP. Set FIFO sample rate to 10Hz.
  imu.dmpBegin(DMP_FEATURE_TAP, 10);
  // dmpSetTap parameters, in order, are:
  // x threshold: 1-1600 (0 to disable)
  // y threshold: 1-1600 (0 to disable)
  // z threshold: 1-1600 (0 to disable)
  // (Threshold units are mg/ms)
  // taps: Minimum number of taps needed for interrupt (1-4)
  // tap time: milliseconds between valid taps
  // tap time multi: max milliseconds between multi-taps
  unsigned short xThresh = 0;   // Disable x-axis tap
  unsigned short yThresh = 0;   // Disable y-axis tap
  unsigned short zThresh = 100; // Set z-axis tap thresh to 100 mg/ms
  unsigned char taps = 1;       // Set minimum taps to 1
  unsigned short tapTime = 100; // Set tap time to 100ms
  unsigned short tapMulti = 1000;// Set multi-tap time to 1s
  imu.dmpSetTap(xThresh, yThresh, zThresh, taps, tapTime, tapMulti);

}

void loop() {

  mpu6050.update();

  //    Serial.print("angleX : ");
  //    Serial.print(mpu6050.getAngleX());
  //    Serial.print("\tangleY : ");
  //    Serial.print(mpu6050.getAngleY());
  //    Serial.print("\tangleZ : ");
  //    Serial.println(mpu6050.getAngleZ());
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
  //

  if (detect_flip()) {
    unsigned long time_now = millis();
    while (millis() < time_now + 2000) {
      all_pixels(0, 0, brightness);
    }
    mode++;
    Serial.print("mode: ");
    Serial.print(mode);
    Serial.println();
    time_now = millis();
    while (millis() < time_now + 3000) {
      all_pixels(0, 0, 0);
    }
  }

  if (mode % 4 == 0) {
    catch_the_coin();
  }

  if (mode % 4 == 1) {
    tap_game();
  }

  if (mode % 4 == 2) {
    dice_roll();
  }

  if (mode % 4 == 3) {
    spinner();
  }

}

void all_pixels(int r, int g, int b) {

  for (int i = 0; i <= 15 ; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
  pixels.show();
}

void dice_roll() {

  if ((abs(mpu6050.getAccX()) > 1) || abs(mpu6050.getAccY()) > 1) {

    for (int i = 0; i < rand; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    int rand = random(1, 12);


    for (int i = 0; i < rand ; i += 2) {
      unsigned long time_now = millis();
      while (millis() < time_now + 500) {
        pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
        pixels.show();
      }
    }

    for (int i = 0; i < rand ; i += 2) {
      pixels.setPixelColor(i, pixels.Color(brightness, 0, brightness));
    }

    unsigned long time_now = millis();
    while (millis() < time_now + 2000) {
      pixels.show();
    }
  }

  for (int i = 0; i < rand; i++) {
    pixels.setPixelColor(i, pixels.Color(brightness, brightness, 0));
  }
  pixels.show();

}

void spinner() {

  if ((abs(mpu6050.getAccX()) > 1) || abs(mpu6050.getAccY()) > 1) {

    for (int i = 0; i < rand; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    int rand = random(1, 16);

    for (int i = 0; i < rand; i++) {

      pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
      pixels.setPixelColor(i + 1, pixels.Color(brightness, brightness, brightness));
      unsigned long time_now = millis();
      while (millis() < time_now + 200) {
        pixels.show();
      }

      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.setPixelColor(i + 1, pixels.Color(0, 0, 0));
      pixels.show();
    }
    pixels.setPixelColor(rand, pixels.Color(brightness, brightness, brightness));
    pixels.setPixelColor(rand + 1, pixels.Color(brightness, brightness, brightness));
    unsigned long time_now = millis();
    while (millis() < time_now + 200) {
      pixels.show();
    }
    pixels.setPixelColor(rand, pixels.Color(brightness, brightness, 0));
    pixels.setPixelColor(rand + 1, pixels.Color(brightness, brightness, 0));
    time_now = millis();
    while (millis() < time_now + 2000) {
      pixels.show();
    }
  }

  for (int i = 0; i < rand; i++) {
    pixels.setPixelColor(i, pixels.Color(brightness, brightness, 0));
  }
  pixels.show();

}

void hourglass() {

  if (mpu6050.getAngleY() > 0 and flip) {
    for (int i = 0; i < 16 ; i++) {
      pixels.setPixelColor(i, pixels.Color(brightness, brightness, brightness));
      unsigned long time_now = millis();
      while (millis() < time_now + 200) {
        pixels.show();
      }
      flip = false;
    }
  }

  if (mpu6050.getAngleY() < 0 and !flip) {
    for (int i = 0; i < 16 ; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      pixels.show();
      unsigned long time_now = millis();
      while (millis() < time_now + 200) {
        pixels.show();
      }
      flip = true;
    }
  }

  if (!flip) {
    //  detect_mode();
  }


}

void pause_player(int period) {

  unsigned long time_now = millis();
  while (millis() < time_now + period) {
    if ((current_pos == target) && (detect_tap() == true)) {
      pixels.setPixelColor(target, pixels.Color(brightness, 0, 0));
      unsigned long time_now = millis();
      while (millis() < time_now + 500) {
        pixels.show();
      }
      level++;
      score_tap++;
      target = random_pixel(0, 16);
      passby = 0;
    }
  }
}

boolean detect_tap() {
  if ( imu.fifoAvailable() )
  {
    // DMP FIFO must be updated in order to update tap data
    imu.dmpUpdateFifo();
    // Check for new tap data by polling tapAvailable
    if ( imu.tapAvailable() )
    {
      // If a new tap happened, get the direction and count
      // by reading getTapDir and getTapCount
      unsigned char tapDir = imu.getTapDir();
      unsigned char tapCnt = imu.getTapCount();

      if (tapDir == TAP_Z_DOWN) {
        Serial.println("tap");
        return true;
      }

      else {
        return false;
      }

    }
    else {
      return false;
    }
  }
  return false;
}


boolean detect_flip() {

  if (mpu6050.getAccZ() < -1) {
    Serial.println("flip");
    return true;
  }
  else {
    Serial.println("Not flip");
    return false;
  }
}


void catch_the_coin() {

  float avg_y;
  float avg_x;

  avg_y = mpu6050.getAccY();
  avg_x = mpu6050.getAccX();

  int angle = atan2(avg_y, avg_x) * 180 / 3.14f;

  int angleDash = (angle + 360) % 360;

  int current_pixel = (int)(((angleDash / 360.0f) * 16.0f));
  int player = 15 - current_pixel;


  if (!gameover) {

    unsigned long time_now = millis();
    while (millis() < time_now + 50) {

      for (int i = 0; i < 16; i++) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }
    }

    if (prev_pixel == player) {
      pixels.setPixelColor(player, pixels.Color(brightness, brightness, brightness));
      pixels.show();
    }

    else {
      pixels.setPixelColor(prev_pixel, pixels.Color(0, 0, 0));
      pixels.setPixelColor(player, pixels.Color(brightness, brightness, brightness));
      pixels.show();
      prev_pixel = player;
    }

    pixels.setPixelColor(target, pixels.Color(brightness, brightness, 0));
    pixels.show();


    if (target == player) {
      score++;
      unsigned long time_now = millis();
      while (millis() < time_now + 1000) {
        pixels.setPixelColor(target, pixels.Color(0, brightness, 0));
        pixels.show();
      }
      target = random_pixel(0, 16);
      level++;
      if (level > 0) {
        finish = true;
      }
    }


    if (level > 0 && finish and !gameover) {

      timer++;
      if (timer % 10 == 0) {
        enemy = random_pixel(0, 16);
        while (enemy == target || enemy == player) {
          enemy = random_pixel(0, 16);
        }
      }
      pixels.setPixelColor(enemy, pixels.Color(brightness, 0, 0));
      pixels.show();
    }



    if (player == enemy) {
      pixels.setPixelColor(enemy, pixels.Color(brightness, 0, brightness));
      pixels.show();
      gameover = true;

    }
  }

  if (gameover) {
    show_coin_score(score);
  }
}

void show_coin_score(int score_made) {

  if (showscore) {
    for (int i = 0; i < 16; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    showscore = false;
  }

  while (score_made > 0) {
    for (int i = 0; i < score_made; i++) {
      pixels.setPixelColor(i, pixels.Color(brightness, brightness, 0));
      pixels.show();
      unsigned long time_now = millis();
      while (millis() < time_now + 500) {
        mpu6050.update();
        if ((abs(mpu6050.getAccX()) > 1) || (abs(mpu6050.getAccY()) > 1)) {
          gameover = false;
          showscore = true;
          score = 0;
          level = 0;
          enemy = 50;

        //      //blink
              unsigned long time_now = millis();
      while (millis() < time_now + 1000) {
          all_pixels(brightness, brightness, brightness);
}
   
          while (millis() < time_now + 1000) {
          all_pixels(0, 0,0);
}
        }

      }
    }

    score_made -= 15;
    for (int i = 0; i < 16; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    unsigned long time_now = millis();
    while (millis() < time_now + 1000) {
          pixels.show();
    }

  }

}


void tap_game() {

if (!gameover_tap) {

    pixels.setPixelColor(current_pos, pixels.Color(0, 0, brightness));
    pixels.show();
    if (prev_pos != target) {
      pixels.setPixelColor(prev_pos, pixels.Color(0, 0, 0));
      pixels.show();
    }

    if (level == 0) {
      pause_player(1000);
    }

    if (level > 0 && level < 3) {
      pause_player(500);
    }

    if (level > 2 && level < 4) {
      pause_player(300);
    }

    if (level > 3 && level < 6) {
      pause_player(100);
    }

    if (level > 5) {
      pause_player(50);
    }

    if ((current_pos == target) && (detect_tap() == true)) {
      pixels.setPixelColor(target, pixels.Color(brightness, 0, 0));
      unsigned long time_start = millis();
      while (millis() < time_start + 500) {
        pixels.show();
      }
      level++;
      score_tap++;
      target = random_pixel(0, 16);
      passby = 0;
    }

    if ((current_pos == target) && (detect_tap() == false)) {
      passby++;
    }

    if (passby == 2) {
      gameover_tap = true;
    }

    prev_pos = current_pos;
    current_pos++;

    pixels.setPixelColor(target, pixels.Color(0, brightness, 0));
    pixels.show();


    if (current_pos > 15) {
      current_pos = 0;
    }

  }

  if (gameover_tap) {
      show_tap_score(score_tap);
  }
}


void show_tap_score(int score_made) {

  if (showscore) {
    for (int i = 0; i < 16; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    showscore = false;
  }

 mpu6050.update();
        if ((abs(mpu6050.getAccX()) > 1) || (abs(mpu6050.getAccY()) > 1)) {
          gameover_tap = false;
          showscore = true;
          score_tap = 0;
          level = 0;
          passby = 0;

        //      blink
       unsigned long time_now = millis();
      while (millis() < time_now + 1000) {
          all_pixels(brightness, brightness, brightness);
}
   
          all_pixels(0, 0,0);
        }
  

  while (score_made > 0) {
    for (int i = 0; i < score_made; i++) {
      pixels.setPixelColor(i, pixels.Color(brightness, brightness, 0));
      pixels.show();
      unsigned long time_now = millis();
      while (millis() < time_now + 500) {
        mpu6050.update();
        if ((abs(mpu6050.getAccX()) > 1) || (abs(mpu6050.getAccY()) > 1)) {
          gameover_tap = false;
          showscore = true;
          score_tap = 0;
          level = 0;
          passby = 0;

        //      //blink
              unsigned long time_now = millis();
      while (millis() < time_now + 1000) {
          all_pixels(brightness, brightness, brightness);
}
   
          while (millis() < time_now + 1000) {
          all_pixels(0, 0,0);
}
        }

      }
    }

    score_made -= 15;
    for (int i = 0; i < 16; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    unsigned long time_now = millis();
    while (millis() < time_now + 1000) {
          pixels.show();
    }

  }

}




void check_tap() {
  if ( imu.fifoAvailable() )
  {
    // DMP FIFO must be updated in order to update tap data
    imu.dmpUpdateFifo();
    // Check for new tap data by polling tapAvailable
    if ( imu.tapAvailable() )
    {
      // If a new tap happened, get the direction and count
      // by reading getTapDir and getTapCount
      unsigned char tapDir = imu.getTapDir();
      unsigned char tapCnt = imu.getTapCount();
      switch (tapDir)
      {
        case TAP_X_UP:
          Serial.print("Tap X+ ");
          break;
        case TAP_X_DOWN:
          Serial.print("Tap X- ");
          break;
        case TAP_Y_UP:
          Serial.print("Tap Y+ ");
          break;
        case TAP_Y_DOWN:
          Serial.print("Tap Y- ");
          break;
        case TAP_Z_UP:
          Serial.print("Tap Z+ ");
          break;
        case TAP_Z_DOWN:
          Serial.print("Tap Z- ");
          break;
      }
      Serial.println(tapCnt);
    }
  }
}

int random_pixel(int min_, int max_) {
  int target = random(min_ , max_);
  pixels.setPixelColor(target, pixels.Color(brightness, 0, 0));
  pixels.show();
  return target;
}
