#include "rgb_lcd.h"

//create object to control an LCD.  
rgb_lcd lcd;
long previousLCDMillis = 0;    // for LCD screen update
long lcdInterval = 4000;

// screen to show 
int screen = 0;    
int screenMax = 2;
bool screenChanged = true;   // initially we have a new screen,  by definition 
// defines of the screens to show
#define TEMPERATURE 0
#define HUMIDITY       1
#define TIME              2

//////////////////////////////////////
//
// display functions
//
void showWelcome()
{
  lcd.clear();
  lcd.setCursor(0, 0);           
  lcd.print("Sensor");
  lcd.setCursor(0, 1);          
  lcd.print("initialising...");      
}

void showTemperature(int T)
{
  lcd.clear();
  lcd.setCursor(0, 0);           
  lcd.print("Temperature");
  lcd.setCursor(0, 1);           
  lcd.print(T);       
  lcd.print((char)223);          
  lcd.print("C");
}

void showHumidity(int H)
{
  lcd.clear();
  lcd.setCursor(0, 0);           
  lcd.print("Humidity");
  lcd.setCursor(0, 1);          
  lcd.print(H);
  lcd.print("%");
}

void showTime()
{
  lcd.clear();
  lcd.setCursor(0, 0);           
  lcd.print("Time in seconds");
  lcd.setCursor(0, 1);          
  lcd.print(millis() / 1000);
}


void setup()
{
  Serial.begin(9600);
  Serial.println("Sensor is Starting Up");
  
  lcd.begin(16, 2);
  showWelcome();
  
  delay(2000);   // to show message on screen}
}


void loop()
{ 
  unsigned long currentLCDMillis = millis();

  // MUST WE SWITCH SCREEN?
  if(currentLCDMillis - previousLCDMillis > lcdInterval)              // save the last time you changed the display 
  {
    previousLCDMillis = currentLCDMillis; 
    screen++;
    if (screen > screenMax) screen = 0;  // all screens done? => start over
    screenChanged = true; 
  }

  // debug Serial.println(screen);


  // DISPLAY CURRENT SCREEN
  if (screenChanged)   // only update the screen if the screen is changed.
  {
    screenChanged = false; // reset for next iteration
    switch(screen)
    {
    case TEMPERATURE: 
      showTemperature(40); 
      break;
    case HUMIDITY: 
      showHumidity(50);
      break;
    case TIME:
      showTime();
      break;
    default:
      // cannot happen -> showError() ?
      break;
    }
  }

}
