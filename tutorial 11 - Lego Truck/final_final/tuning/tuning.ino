#include <Wire.h>
#include<NoDelayTimerTask.h>
#include <MMA7660.h>

NoDelayTimerTask timer1(5000);

#define linefinderPin 3

#define RT 11       // right motor
#define LT 10       // lift motor

int counter = 0;


MMA7660 accelemeter;

unsigned long previous_FW_Millis = 0;
//----------------PID--------------
float Acceleration_angle[1];
float ax, ay, az;
float elapsedTime, time, timePrev;
int i;
float PID, pwmLeft, pwmRight, error, previous_error;
float pid_p = 0;
float pid_i = 0;
float pid_d = 0;
/////////////////PID CONSTANTS/////////////////
double kp = 2.52; //3.55
double ki = 0.025; //0.003, 0.025
double kd = 3; //2.05
///////////////////////////////////////////////
double throttle = 1500; //initial value of throttle to the motors
float desired_angle = 45; //This is the angle in which we whant the
//---------------------------------
void setup() {
  accelemeter.init();
  pinMode(RT, OUTPUT);
  pinMode(LT, OUTPUT);
  pinMode(linefinderPin, INPUT);
  time = millis(); //Start counting time in milliseconds
  Serial.begin(9600);
  delay(2000);

}
bool lineread;
void loop() {
  
  timePrev = time;  // the previous time is stored before the actual time read
  time = millis();  // actual time read
  elapsedTime = (time - timePrev) / 1000;
  accelemeter.getAcceleration(&ax, &ay, &az);
  double x_Buff = float(ax);
  double y_Buff = float(ay);
  double z_Buff = float(az);
  Acceleration_angle[1] = atan2(-y_Buff, -z_Buff) * 57.2957795 + 180;
  error = Acceleration_angle[1] - desired_angle;
  pid_p = kp * error;
  if (-3 < error < 3)
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
    pwmRight = 1000;
  }
  if (pwmRight > 2000)
  {
    pwmRight = 2000;
  }
  //Left
  if (pwmLeft < 1000)
  {
    pwmLeft = 1000;
  }
  if (pwmLeft > 2000)
  {
    pwmLeft = 2000;
  }

  int8_t x;
  int8_t y;
  int8_t z;
  accelemeter.getXYZ(&x, &y, &z);

  Serial.print(Acceleration_angle[1]);
  Serial.print(",");
  Serial.println(desired_angle);
  //  firstSection();
  if (timer1.isTimeUp())
  {

    if (HIGH == digitalRead(linefinderPin))
    {

    }
    else
    {
      analogWrite(LT, pwmLeft);
      analogWrite(RT, pwmRight);
      previous_error = error;
    }

  }


}
// end of code.
