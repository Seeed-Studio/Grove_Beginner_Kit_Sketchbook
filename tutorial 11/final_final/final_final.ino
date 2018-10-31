#include <Wire.h>
#include<NoDelayTimerTask.h>
#include <ChainableLED.h>
#include <MMA7660.h>
#include <rgb_lcd.h>
#include <DHT.h>

NoDelayTimerTask timer1(5000);

const int a = 440;
const int f = 349;
const int cH = 523;

#define DHTPIN 2
#define linefinderPin 3
#define tiltswitchPin 5
#define buzzerPin 6
#define FW 12      // forward motor
#define RE 13       // reverse motor
#define RT 10       // right motor
#define LT 11       // lift motor

#define NUM_LEDS  1
#define DHTTYPE DHT11   // DHT 11 
int counter = 0;

rgb_lcd lcd;
ChainableLED leds(7, 8, NUM_LEDS);
MMA7660 accelemeter;
DHT dht(DHTPIN, DHTTYPE);

unsigned long previous_FW_Millis = 0;
//----------------PID--------------
float Acceleration_angle[1];
float ax, ay, az;
float elapsedTime, time, timePrev;
int i;
float PID, pwmLeft, pwmRight, error, previous_error, pwmTurn;
float pid_p = 0;
float pid_i = 0;
float pid_d = 0;
/////////////////PID CONSTANTS/////////////////
double kp = 2.52; //3.55 2.52
double ki = 0.003; //0.003,0.025 0.5
double kd = 0.3; //0.3
///////////////////////////////////////////////
double throttle = 2000; //initial value of throttle to the motors
float desired_angle = 45; //This is the angle in which we whant the
//---------------------------------
class Flasher
{
    // Class Member Variables
    // These are initialized at startup
    int RON;
    int GON;
    int BON;
    int ROFF;
    int GOFF;
    int BOFF;
    long OnTime;     // milliseconds of on-time
    long OffTime;    // milliseconds of off-time

    // These maintain the current state
    int ledState;                 // ledState used to set the LED
    unsigned long previousMillis;   // will store last time LED was updated

    // Constructor - creates a Flasher
    // and initializes the member variables and state
  public:
    Flasher(int RcolorON, int GcolorON, int BcolorON, int RcolorOFF, int GcolorOFF, int BcolorOFF, long on, long off)
    {
      leds.init();
      OnTime = on;
      OffTime = off;

      RON = RcolorON;
      GON = GcolorON;
      BON = BcolorON;
      ROFF = RcolorOFF;
      GOFF = GcolorOFF;
      BOFF = BcolorOFF;

      ledState = LOW;
      previousMillis = 0;
    }

    void flash()
    {
      // check to see if it's time to change the state of the LED
      unsigned long currentMillis = millis();

      if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
      {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        leds.setColorRGB(0, RON, GON, BON);  // Update the actual LED
      }
      else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
      {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        leds.setColorRGB(0, ROFF, GOFF, BOFF);   // Update the actual LED
      }
    }
};

//Flasher warning(255, 128, 0, 0, 0, 0, 250, 250);
Flasher police(255, 0, 0, 0, 0, 255, 200, 200);

void setup() {
  accelemeter.init();
  lcd.begin(16, 2);
  pinMode(RE, OUTPUT);
  pinMode(FW, OUTPUT);
  pinMode(RT, OUTPUT);
  pinMode(LT, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(linefinderPin, INPUT);
  dht.begin();
  time = millis(); //Start counting time in milliseconds
  Serial.begin(9600);
  delay(2000);

}
bool lineread;
void loop() {

  timePrev = time;  // the previous time is stored before the actual time read
  time = millis();  // actual time read
  elapsedTime = (time - timePrev) / 500;
  accelemeter.getAcceleration(&ax, &ay, &az);
  double x_Buff = float(ax);
  double y_Buff = float(ay);
  double z_Buff = float(az);
  Acceleration_angle[1] = atan2(-y_Buff, -z_Buff) * 57.2957795 + 180;
  error = Acceleration_angle[1] - desired_angle;
  pid_p = kp * error;
  if (-2 < error < 2)
  {
    pid_i = pid_i + (ki * error);
  }
  pid_d = kd * ((error - previous_error) / elapsedTime);
  PID = pid_p + pid_i + pid_d;
  if (PID < -1000)
  {
    PID = -1000;
  }
  if (PID > 1000)
  {
    PID = 1000;
  }
  pwmLeft = throttle + PID;
  pwmRight = throttle - PID;
  if (pwmRight < 1000)
  {
    pwmRight = 0;
  }
  if (pwmRight > 2000)
  {
    pwmRight = 255;
  }
  //Left
  if (pwmLeft < 1000)
  {
    pwmLeft = 0;
  }
  if (pwmLeft > 2000)
  {
    pwmLeft = 255;
  }

  int8_t x;
  int8_t y;
  int8_t z;
  accelemeter.getXYZ(&x, &y, &z);
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  //  Serial.print(z);
  //  Serial.print(",");
  //  Serial.println(desired_angle);
  //  firstSection();
  if (timer1.isTimeUp())
  {

    if (HIGH == digitalRead(linefinderPin))
    {
      //      analogWrite(RT, 0);

      timer1.resetTimer();
      analogWrite(LT, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Line Detected");//prints on LCD
      if (digitalRead(tiltswitchPin) == HIGH)
      {
        leds.setColorRGB(0, 255, 128, 0);
      }
      else
      {
        leds.setColorRGB(0, 0, 0, 0);
      }
      Serial.print(z);
      digitalWrite(RE, HIGH);
      digitalWrite(FW, LOW);
      digitalWrite(6, HIGH);

    }
    else
    {

      analogWrite(LT, pwmLeft);
      analogWrite(RT, pwmRight);
      previous_error = error;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperature: ");
      lcd.setCursor(13, 0);
      lcd.print(t);
      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.setCursor(10, 1);
      lcd.print(h);
      if (digitalRead(tiltswitchPin) == HIGH)
      {
        leds.setColorRGB(0, 255, 0, 0);
      }
      else
      {
        leds.setColorRGB(0, 0, 0, 255);
      }
      digitalWrite(FW, HIGH);
      digitalWrite(RE, LOW);
      digitalWrite(6, LOW);

    }

  }


}
// end of code.
