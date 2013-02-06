/*     
 * SmokeDuino BBQ monitor
 * Read temperature from DS18B20
 * Turn on heat via PowerSwitch Tail
 * Report current temperature and desired temperature via LCD
 * Set desired temperature via potentiometer
 */

#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* Pin connected to PowerSwitch Tail */
/* I have an LED connected in parallel */
/* so both will turn on at the same time */
int powerpin = 4;

/* Analog pin for potentiometer */
int potPin = 1;

/* Low and high points for temperature settings */
/* see getdesire() below */
int templow = 100;
int temphigh = 200;

int counter = 0;
int temperature = 0;
int desiredtemp = 0;

/* Variable for One Wire Bus */
#define ONE_WIRE_BUS 3 /*-(Connect to Pin 3 )-*/

/* Set up a oneWire instance to communicate with any OneWire device */
/* By using this, we don't need to know what the address for the */
/* device */
OneWire ourWire(ONE_WIRE_BUS);

/* Tell Dallas Temperature Library to use oneWire Library */
DallasTemperature sensors(&ourWire);

/* Set up LCD display on pins 7-12 */
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
  /* initial LCD display */
  lcd.begin(16,2);
  lcd.print("Cur:");
  lcd.setCursor(0,1);
  lcd.print("Set:");
  /*-( Start up the DallasTemperature library )-*/
  sensors.begin();
  /* Set up powerpin to be output */
  pinMode(powerpin,OUTPUT);
  /* Make sure PowerSwitch Tail is off */
  digitalWrite(powerpin,LOW);
}
 
void loop()                     // run over and over again
{
 sensors.requestTemperatures(); // Send the command to get temperatures

 /* Grab the temperature in F */
 temperature=sensors.getTempFByIndex(0);
 
 /* What temperature are we looking for? */
 desiredtemp=int(getdesire(potPin));
 
 /* Don't delay too often when turning on */
 /* and off the temperature */
 delay(100);
 if ((counter == 10)) {
   lcd.setCursor(5,0);
   lcd.print(temperature);
   lcd.print("  ");
   lcd.setCursor(9,0);
   lcd.print("F");
   counter = 0;
   if ((temperature < desiredtemp) && ( temperature > 0 )) {
     digitalWrite(powerpin, HIGH);
   } else {
     digitalWrite(powerpin, LOW);
   }
 }
 /* Print out the temperature we want to be set to */
 /* We update this continually so there's a smooth change */
 /* when turning the knob */
 lcd.setCursor(5,1);
 lcd.print(desiredtemp);
 lcd.print("  ");
 lcd.setCursor(9,1);
 lcd.print("F");
 counter = counter + 1;
}

/* Not sure why I made this a function, but whatever */
/* Translate the value from the pot to a value between */
/* templow and temphigh */
float getdesire(int pin){
  return (map(analogRead(pin), 0, 1023, temphigh, templow)) ;
}
