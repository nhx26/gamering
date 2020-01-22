#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define DELAYVAL 500

#include "MPU9250.h"

#include <MPU6050_tockn.h>

#include <Wire.h>

#include <SparkFunMPU9250-DMP.h>

#define PIN 5 // WS2812 leds are connected to this pin 

#define NUMPIXELS 16 // 16 leds are used on the game ring pcb

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

MPU6050 mpu6050(Wire);

MPU9250_DMP imu;

int status;

int mode = 0; // Game ring mode

boolean flip = true; // Flip state to change mode

int prev_pixel = 8; // LED to start pixel
int current_pixel;

int level = 0; // Level for coin game
int level_tap = 0; // Level for tap game

int target = 10; // Target LED for coin game

int score = 0; // Score for coin game
int score_tap = 0; // Score for tap game

boolean finish = false;

boolean gameover = false; // Gameover for coin game

boolean gameover_tap = false; // Gameover for tap game

int enemy = 50; // Placeholder value

boolean showscore = true;

int timer = 0;

int brightness = 30; // Brightness for all LEDs

int current_pos = 0;
int prev_pos;

int passby = 0; // Counter for tap game

bool reaction_start = true; // Starting reaction game
bool first = true;
bool second = true;

//Start times for reaction game
float p1s = 0;
float p2s = 0;

//End times for reaction game
float p1e = 0;
float p2e = 0;

bool p1 = true;
bool p2 = true;

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

  mpu6050.update(); // Update values for sensor

  //  Mode flipping indicator (blue)
  show_flip();

  if (mode % 5 == 0) {
    catch_the_coin();
  }

  if (mode % 5 == 1) {
    tap_game();
  }

  if (mode % 5 == 2) {
    reaction_game();
  }

  if (mode % 5 == 3) {
    dice_roll();
  }

  if (mode % 5 == 4) {
    spinner();
  }


}


void show_flip() {
  if (detect_flip()) {
    unsigned long time_now = millis();
    while (millis() < time_now + 2000) {
      all_pixels(0, 0, brightness);
    }
    mode++;
    time_now = millis();
    while (millis() < time_now + 3000) {
      all_pixels(0, 0, 0);
    }
  }
}

// Random number generator
int random_pixel(int min_, int max_) {
  return random(min_ , max_);
}

// All LEDs control
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

//Checking tap on target for tap game
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
            all_pixels(0, 0, 0);
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

    all_pixels(0, 0, 0);
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
            all_pixels(0, 0, 0);
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

void reaction_game() {
  //Start indicator
  if (reaction_start) {
    unsigned long time_now = millis();
    while (millis() < time_now + 500) {
      all_pixels(brightness, brightness, brightness);
    }
    time_now = millis();
    while (millis() < time_now + 500) {
      all_pixels(0, 0, 0);
    }


    time_now = millis();
    while (millis() < time_now + 500) {
      all_pixels(brightness, brightness, brightness);
    }
    time_now = millis();
    while (millis() < time_now + 500) {
      all_pixels(0, 0, 0);
    }

    time_now = millis();
    while (millis() < time_now + 500) {
      all_pixels(brightness, brightness, brightness);
    }

    time_now = millis();
    while (millis() < time_now + 500) {
      all_pixels(0, 0, 0);
    }
    reaction_start = false;
  }

  int random_time = random(3000, 10000);

  unsigned long time_now = millis();
  while (millis() < time_now + random_time) {

  }

  //player 1 - blue
  if (p1) {
    if (first) {
      p1s = millis();
      first = false;
    }
    all_pixels(0, 0, brightness);
    while (!detect_tap()) {
    }
    all_pixels(0, 0, 0);
    p1e = millis();
    p1 = false;
  }


  //change mode

  all_pixels(0, 0, 0);

  time_now = millis();
  while (millis() < time_now + 1000) {

  }

  all_pixels(0, 0, 0);

  random_time = random(3000, 10000);

  time_now = millis();
  while (millis() < time_now + random_time) {

  }

  //player 2 - red
  if (p2) {
    if (first) {
      p2s = millis();
      first = false;
    }
    all_pixels( brightness, 0, 0);
    while (!detect_tap()) {
    }
    all_pixels(0, 0, 0);
    p2e = millis();
    p2 = false;
  }


  //Winner
  time_now = millis();
  while (millis() < time_now + 500) {
    all_pixels(brightness, 0, brightness);

  }
  time_now = millis();
  while (millis() < time_now + 500) {
    all_pixels(0, 0, 0);

  }

  time_now = millis();
  while (millis() < time_now + 500) {
    all_pixels(brightness, 0, brightness);
  }
  time_now = millis();
  while (millis() < time_now + 500) {
    all_pixels(0, 0, 0);

  }

  time_now = millis();
  while (millis() < time_now + 500) {
    all_pixels(brightness, 0, brightness);

  }
  time_now = millis();
  while (millis() < time_now + 500) {
    all_pixels(0, 0, 0);

  }



  if ( (p1e-p1s) < (p2e-p2s)) {
    time_now = millis();
    while (millis() < time_now + 2000) {
      all_pixels(0, 0, brightness);

    }
  }

  else {
    time_now = millis();
    while (millis() < time_now + 2000) {
      all_pixels(brightness, 0, 0);

    }
    all_pixels(0, 0, 0);
  }
  reaction_start = true;
  p1 = true;
  p2 = true;
  first = true;
  second = true;

  time_now = millis();
  while (millis() < time_now + 5000) {
    all_pixels(0, 0, 0);
  }
}


