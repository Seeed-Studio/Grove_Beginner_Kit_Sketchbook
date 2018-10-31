//add DHT sensor library
#include <DHT.h>

//set digital pin2 as DHTPIN
#define DHTPIN 2
//set the sensor type as DHT 11
#define DHTTYPE DHT11

/*assign dht as the name of DHT sensor
  set the sensor pin as DHTPIN(pin2),
  set the sensor type as DHTTYPE(DHT11)
*/
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  //initialise the dht sensor
  dht.begin();
  // opens serial port, sets data rate to 9600 bps
  Serial.begin(9600);
  //wait for 2s to initialise the board
  delay(2000);
}

void loop() {
  //store the humidity value to h
  int h = dht.readHumidity();
  //store the temperature value to t(in Celsius)
  int t = dht.readTemperature();
  //store the converted temperature value in fahrenheit to f
  int f = dht.convertCtoF(t);
  //display the title Temperature in C:
  Serial.print("Temperature in C: ");
  //display the temperature value t
  Serial.print(t);
  /* note the difference Serial.print()
     and Serial.println(),
     Serial.print() print the data in the same line
     Serial.println() print the data on the new line
     display the temperature unit ºC and change new line
  */
  Serial.println("ºC");
  //display the title Temperature in F:
  Serial.print("Temperature in F: ");
  //display the temperature value f
  Serial.print(f);
  //display the temperature unit ºF and change new line
  Serial.println("ºF");
  //display the title Humidity:
  Serial.print("Humidity: ");
  //display the Humidity value h
  Serial.print(h);
  //display the % sign
  Serial.println("%");
}
