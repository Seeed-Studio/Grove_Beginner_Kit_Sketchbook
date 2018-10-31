//add LCD library
#include <rgb_lcd.h>
//add accelemeter library
#include "MMA7660.h"
#include <TimeLib.h>

//assign default time as epoch time 1514764800 which is 00:00:00 Jan 1 2018
long DEFAULT_TIME = 1514764800;
//set drinkTime to 30mins, which is 1800s
long drinkTime = DEFAULT_TIME + 10;

//assign buzzer as pin 6
#define buzzer 6
//set title of pin 5 as tiltSwitch
#define tiltSwitch 5


//assign accelemeter as the name of MMA7660 accelemeter
MMA7660 accelemeter;

//assign lcd as the name of rgb_lcd screen
rgb_lcd lcd;

int tolerance = 50;
boolean calibrated = false;
boolean moveDetected = false;

//Accelerometer limits
double rMin; //Minimum x Value
double rMax; //Maximum x Value
double rVal; //Current x Value

double pMin; //Minimum y Value
double pMax; //Maximum y Value
double pVal; //Current y Value

int8_t x;
int8_t y;
int8_t z;

//set value 0.5 to alpha
const float alpha = 0.5;

//initialise fXg, fYg, fZg as double with value of 0
double fXg = 0;
double fYg = 0;
double fZg = 0;
//initialise pitch and roll as double
double p, r;

void setup()
{
  //
  setTime(DEFAULT_TIME);
  //initialise the accelemeter
  accelemeter.init();
  //initialise the lcd screen;
  //set up the lcd's number of columns and rows:
  lcd.begin(16, 2);
  //set pin 5(tilt switch) as input pin
  pinMode(tiltSwitch, INPUT);
  calibrateAccel();
  Serial.begin(9600);
  delay(1000);
}

void Accel() {

  accelemeter.getXYZ(&x, &y, &z);


  //Low Pass Filter to reduce the noise
  fXg = x * alpha + (fXg * (1.0 - alpha));
  fYg = y * alpha + (fYg * (1.0 - alpha));
  fZg = z * alpha + (fZg * (1.0 - alpha));

  r  = (atan2(-fYg, fZg) * 180.0) / M_PI;
  p = (atan2(fXg, sqrt(fYg * fYg + fZg * fZg)) * 180.0) / M_PI;
  r = map(r, -90, 90, 0, 180);
  p = map(p, -90, 90, 0, 180);
  return r;
  return p;
}

void calibrateAccel() {
  // reset alarm
  moveDetected = false;
  Accel();

  //  rVal = map(r, -90, 90, 0, 180);
  //  pVal = map(p, -90, 90, 0, 180);
  //initialise x,y,z variables
  if (80 < r < 100 && 80 < p < 100) {
    rVal = r;
    rMin = rVal;
    rMax = rVal;

    pVal = p;
    pMin = pVal;
    pMax = pVal;

    //  zVal = z;
    //  zMin = zVal;
    //  zMax = zVal;

    //    // Calibration sequence initialisation sound - 3 seconds before calibration begins
    //    buzz(3, 1000);

    //calibrate the Accelerometer (should take about 0.5 seconds)
    for (int i = 0; i < 50; i++) {
      Accel();
      rVal = r;
      if (rVal > rMax) {
        rMax = rVal;
      } else if (rVal < rMin) {
        rMin = rVal;
      }

      // Calibrate Y Values
      pVal = p;
      if (pVal > pMax) {
        pMax = pVal;
      } else if (pVal < pMin) {
        pMin = pVal;
      }

      //      // Calibrate Z Values
      //      zVal = z;
      //      if (zVal > zMax) {
      //        zMax = zVal;
      //      } else if (zVal < zMin) {
      //        zMin = zVal;
      //      }

      //Delay 10msec between readings
      delay(10);
    }

        printValues();
    calibrated = true;
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Place the bottle");
      lcd.setCursor(0, 1);
      lcd.print("at flat surface");
      calibrated = false;
    }
}

boolean checkMotion() {
  boolean tempB = false;
  Accel();
  rVal = r;
  pVal = p;

  if (rVal > (rMax + tolerance) || rVal < (rMin - tolerance)) {
    tempB = true;
  }

  if (pVal > (pMax + tolerance) || pVal < (pMin - tolerance)) {
    tempB = true;
  }
  //
  //  if (zVal > (zMax + tolerance) || zVal < (zMin - tolerance)) {
  //    tempB = true;
  //  }

  return tempB;
}



void Motion() {

  //don't check for movement until recalibrated again
  //  drinkTime = now() + 10;
  calibrated = false;
  //    int mode = 2;

}

int mode = 0;


void loop()
{
//  Accel();
//  //display title roll =
//  Serial.print("roll = ");
//  //display the roll value
//  Serial.println(r);
//  //display title pitch =
//  Serial.print("pitch = ");
//  //display the pitch value
//  Serial.println(p);

  //current time is greate or equals to drinkTime(30mins ahead), then set to case 1;
  if (now() >= drinkTime  ) {
    mode = 1;
  }

  if (calibrated) {
    if (checkMotion()) {
      mode = 2;
      moveDetected = true;
    }
  }
  if (moveDetected) {
    Motion();
  }

  switch (mode) {
    case 0:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(now());
//      Serial.println(drinkTime);
      break;
    case 1:
      //      tone(buzzer, 262, 300);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You need to");
      lcd.setCursor(0, 1);
      lcd.print("drink more water");

      //      if (calibrated) {
      //        if (checkMotion()) {
      //          mode = 2;
      //          moveDetected = true;
      //        }
      //      }
      //      if (moveDetected) {
      //        Motion();
      //      }
      break;
    case 2:
      drinkTime = now() + 10;
//      Serial.println("motion detected");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Well Done remind");
      lcd.setCursor(0, 1);
      lcd.print("you in 30mins");
      delay(5000);
      Accel();

      Serial.println(r>80 && r < 100 && p>80 && p < 100);
      if (r>80 && r < 100 && p>80 && p < 100) {
        if (!calibrated) {
          calibrateAccel();
        }
        else
        {
          mode = 0;
          drinkTime = now() + 10;
//          Serial.println(drinkTime);
          Serial.println("resset");
        }
      }
      else
      {
        mode = 3;
      }
      break;
    case 3:
      drinkTime = now() + 10;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please put down water");
      lcd.setCursor(0, 1);
      lcd.print("bottle when finished!");
      for (int positionCounter = 0; positionCounter < 5; positionCounter++) {
        // scroll one position left:
        lcd.scrollDisplayLeft();
        // wait a bit:
        delay(200);
      }
      for (int positionCounter = 0; positionCounter < 5; positionCounter++) {
        // scroll one position right:
        lcd.scrollDisplayRight();
        // wait a bit:
        delay(200);
      }
      for (int positionCounter = 0; positionCounter < 5; positionCounter++) {
        // scroll one position left:
        lcd.scrollDisplayLeft();
        // wait a bit:
        delay(200);
      }

      Accel();
      if (r>80 && r < 100 && p>80 && p < 100) {
        if (!calibrated) {
          calibrateAccel();
        }
        else
        {
          mode = 0;
          drinkTime = now() + 10;
          Serial.print("cal: ");
          Serial.print("resset");
        }
      }
      break;

  }
  delay(1);
}




void printValues() {
  Serial.print("rMin=");
  Serial.print(rMin);
  Serial.print(", rMax=");
  Serial.print(rMax);
  Serial.println();

  Serial.print("pMin=");
  Serial.print(pMin);
  Serial.print(", pMax=");
  Serial.print(pMax);
  Serial.println();
  //
  //  Serial.print("zMin=");
  //  Serial.print(zMin);
  //  Serial.print(", zMax=");
  //  Serial.print(zMax);
  //  Serial.println();

  //display title roll =
  Serial.print("roll = ");
  //display the roll value
  Serial.println(r);
  //display title pitch =
  Serial.print("pitch = ");
  //display the pitch value
  Serial.println(p);

  Serial.println("------------------------");
}
