#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <semphr.h>
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "tasks.h"
#include "PotentiometerAndLDR.h"

#define DS18B20_PIN 10 

#define HEATER_LED 9
#define BACKLIGHT 2

// create I/O objects
LiquidCrystal_I2C lcd(0x27,20,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
OneWire oneWire(DS18B20_PIN); // set up oneWire to pin 10 (DS18B20 connection)
DallasTemperature sensors(&oneWire); 

typedef struct
{
  int id;
  int val;
}VAR;

/**
 * @brief Initializes instance of a variable delay
 * 
 * @param void 
 * @return void
 */
void hardware_setup(void)
{ 
  lcd.init(); // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Hardware setup");
  sensors.begin(); // initialize the sensor
  pinMode(HEATER_LED, OUTPUT); // initialize the LED
}


void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  Serial1.begin(9600);
  hardware_setup();//hardware initialization sensors, display, output controler...
  xQueue = xQueueCreate( 5, sizeof( VAR ) ); // 5 uint32_t elements
  xQueueLCD = xQueueCreate( 5, sizeof( VAR ) ); // 5 uint32_t elements
  if( xQueue != NULL )// if queue object is created
  {
    if(xTaskCreate( vReadDisplayTemp, "Task1", 200, NULL, 3, NULL )!=pdTRUE)
    {
      Serial.println( "Could not create the task vReadDisplayTemp" );
    }
    if(xTaskCreate( vReadDisplayPot, "Task2", 200, NULL, 3, NULL )!=pdTRUE)
    {
      Serial.println( "Could not create the task vReadDisplayPo" );
    }
    if(xTaskCreate( vMangageHeater, "Task3", 200, NULL, 1, NULL )!=pdTRUE)
    {
      Serial.println( "Could not create the task vMangageHeater" );
    }
    if(xTaskCreate( vManageLum, "Task4", 200, NULL,3 , NULL )!=pdTRUE)
    {
      Serial.println( "Could not create the task vManageLum" );
    }
    if(xTaskCreate( vLcdControl, "Task5", 200, NULL,2 , NULL )!=pdTRUE)
    {
      Serial.println( "Could not create the task vLcdControl" );
    }
  }
  else
      Serial.println( "Could not create the queue." );
}

void loop() 
{
}
