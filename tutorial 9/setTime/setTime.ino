//add LCD library
#include <rgb_lcd.h>
#include <TimeLib.h>

//assign lcd as the name of rgb_lcd screen
rgb_lcd lcd;

long DEFAULT_TIME = 1514764800;
long waterTime = DEFAULT_TIME+86400;
void setup()  {
  setTime(DEFAULT_TIME);
  
  //initialise the lcd screen;
  //set up the lcd's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
}
  
void loop() {
  if (timeStatus() != timeNotSet) {
    digitalClockDisplay();
  }
long watertime1 = 86400;

  Serial.println(watertime);
  
  delay(1000);
}

void digitalClockDisplay() {
    //set the LCD cursor to column 0, line 0
  lcd.setCursor(0, 0);
  //Print text temperature: to the LCD
  lcd.print(hour());
  //set the LCD cursor to column 12, line 0
  printDigits(minute());
  printDigits(second());
  lcd.setCursor(0, 1);
  lcd.print(day());
  lcd.print("/");
  lcd.print(month());
  lcd.print("/" );
  lcd.print(year());
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  lcd.print(":");
  if (digits < 10)
    lcd.print('0');
  lcd.print(digits);
}
