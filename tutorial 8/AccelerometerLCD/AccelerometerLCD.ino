//add accelemeter library
#include "MMA7660.h"
//add LCD library
#include <rgb_lcd.h>


//assign name tiltswitchPin to pin 5
#define tiltswitchPin 5

//assign accelemeter as the name of MMA7660 accelemeter
MMA7660 accelemeter;
//assign lcd as the name of rgb_lcd screen
rgb_lcd lcd;

//set value 0.5 to alpha
const float alpha = 0.5;

//initialise fXg, fYg, fZg as double with value of 0
double fXg = 0;
double fYg = 0;
double fZg = 0;
//initialise pitch and roll as double

void setup()
{
  //initialise the accelemeter
  accelemeter.init();
  //initialise the lcd screen;
  //set up the lcd's number of columns and rows:
  lcd.begin(16, 2);
  //set pinMode of tiltswitchPin to input
  pinMode(tiltswitchPin, INPUT);
  //wait for 2s
  delay(2000);

}
void loop()
{
  /*if tilt switch is on display X, Y, Z, Roll and Pitch data
    if tilt switch is off display X, Y, Z acceleration data
  */
  if (HIGH == digitalRead(tiltswitchPin))
  {
    //initialise x, y, z as int8_t, pitch and roll as double
    int8_t x;
    int8_t y;
    int8_t z;
    double roll;
    double pitch;
    //get x y z offset value from accelemeter
    accelemeter.getXYZ(&x, &y, &z);

    //Low Pass Filter to reduce the noise
    fXg = x * alpha + (fXg * (1.0 - alpha));
    fYg = y * alpha + (fYg * (1.0 - alpha));
    fZg = z * alpha + (fZg * (1.0 - alpha));

    //Roll & Pitch Equations
    roll  = (atan2(-fYg, fZg) * 180.0) / M_PI;
    pitch = (atan2(fXg, sqrt(fYg * fYg + fZg * fZg)) * 180.0) / M_PI;
    //reset the lcd screen
    lcd.clear();
    //set the LCD cursor to column 0, line 0
    lcd.setCursor(0, 0);
    //display text x:
    lcd.print("x:");
    //display value of x
    lcd.print(x);
    //set the LCD cursor to column 5, line 0
    lcd.setCursor(5, 0);
    //display text y:
    lcd.print("y:");
    //display value of y
    lcd.print(y);
    //set the LCD cursor to column 10, line 0
    lcd.setCursor(10, 0);
    //display text z:
    lcd.print("z:");
    //display value of z
    lcd.print(z);

    //set the LCD cursor to column 0, line 1
    lcd.setCursor(0, 1);
    //display text R:
    lcd.print("R:");
    //display value of roll
    lcd.print(roll);
    //set the LCD cursor to column 8, line 1
    lcd.setCursor(8, 1);
    //display text P:
    lcd.print("P:");
    //display value of pitch
    lcd.print(pitch);
  } else
  {
    //initialise ax, ay, az as float
    float ax, ay, az;
    //get ax ay az acceleration value from accelemeter
    accelemeter.getAcceleration(&ax, &ay, &az);
    //reset the lcd screen
    lcd.clear();
    //set the LCD cursor to column 0, line 0
    lcd.setCursor(0, 0);
    //display text ax:
    lcd.print("ax:");
    //display value of ax
    lcd.print(ax);
    //set the LCD cursor to column 8, line 0
    lcd.setCursor(8, 0);
    //display text ay:
    lcd.print("ay:");
    //display value of ay
    lcd.print(ay);
    //set the LCD cursor to column 0, line 1
    lcd.setCursor(0, 1);
    //display text az:
    lcd.print("az:");
    //display value of az
    lcd.print(az);
  }
  //wait 0.5s
  delay(500);
}
