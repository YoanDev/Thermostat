

/**
 * @brief Initializes instance of a variable delay
 * 
 * @param void * context passed from main function
 * @return void
 */
static void vReadDisplayTemp( void *pvParameters )
{
  int tempval;
  VAR lValueToSend;
  lValueToSend.id = 0;
  while(1)
  {
    sensors.requestTemperatures();
    tempval = sensors.getTempCByIndex(0); // get temperature
    lValueToSend.val = tempval;
    Serial1.println(tempval);
    xQueueSendToBack( xQueueLCD, &lValueToSend, portMAX_DELAY ); // send tempature to LCD manager
    xQueueSendToBack( xQueue, &lValueToSend, portMAX_DELAY ); // send tempature to heater manager
    vTaskDelay( 1000 / portTICK_PERIOD_MS );//stop the task
  }
}

/**
 * @brief Initializes instance of a variable delay
 * 
 * @param void * context passed from main function
 * @return void
 */
static void vReadDisplayPot( void *pvParameters )
{
  int setpval;
  VAR lValueToSend;
  lValueToSend.val = 0;
  lValueToSend.id = 1;
  Potentiometer a;
  while(1)
  {
    setpval = a.pot_val();
    String inByte = Serial1.readString();
    Serial.println(inByte);
    //lValueToSend.val = setpval;
    if(inByte.toInt())
    { 
      lValueToSend.val = inByte.toInt();
      Serial.println(inByte);
    } 
    xQueueSendToBack( xQueueLCD, &lValueToSend, portMAX_DELAY ); // send set point to LCD manager
    xQueueSendToBack( xQueue, &lValueToSend, portMAX_DELAY ); // send set point to heater manager
    vTaskDelay( 100 / portTICK_PERIOD_MS ); //stop the task
  }
}

/**
 * @brief Initializes instance of a variable delay
 * 
 * @param void * context passed from main function
 * @return void
 */
static void vMangageHeater( void *pvParameters )
{
  VAR lReceivedValue;
  int tempval = 0;
  int setpval = 0;
  
  while(1)
  {
    if(xQueueReceive( xQueue, (VAR *)&lReceivedValue, 10 )==pdPASS) 
    {
      if(lReceivedValue.id == 0) // Reception temperature
      {
        tempval = lReceivedValue.val;
      }
      else // Reception set point
      {
        setpval = lReceivedValue.val;
      }
    }
    if (tempval>setpval) //Turn off led
    {
      digitalWrite(HEATER_LED, LOW);
    }
    else // Turn on led
    {
      digitalWrite(HEATER_LED, HIGH);
    }
  }
}

/**
 * @brief Initializes instance of a variable delay
 * 
 * @param void * context passed from main function
 * @return void
 */
static void vManageLum( void *pvParameters )
{
  uint16_t luminosity;
  VAR lValueToSend;
  lValueToSend.id = 2;
  LDR_c b;
  while(1)
  {
    luminosity = b.ldr_val(); // Read luminosity
    lValueToSend.val = luminosity; 
    xQueueSendToBack( xQueueLCD, &lValueToSend, portMAX_DELAY ); // Send luminosity to the lcd controler
    vTaskDelay( 1000 / portTICK_PERIOD_MS );
  }
}

/**
 * @brief Initializes instance of a variable delay
 * 
 * @param void * context passed from main function
 * @return void
 */
static void vLcdControl( void *pvParameters )
{
  VAR lReceivedValue;
  int tempval = 0;
  int setpval = 0;
  int luminosity = 0;
  while(1)
  {
    if(xQueueReceive( xQueueLCD, (VAR *)&lReceivedValue, 10 )==pdPASS)
    {
      if(lReceivedValue.id == 0)
      {
        tempval = lReceivedValue.val;
        lcd.setCursor(0,0);
        lcd.print("Temperature:   "); //add spaces to erase previous value
        lcd.setCursor(13,0);
        lcd.print(tempval);
      }
      else if(lReceivedValue.id == 1)
      {
        setpval = lReceivedValue.val;
        lcd.setCursor(0,1);
        lcd.print("Setpoint:   "); //add spaces to erase previous value
        lcd.setCursor(10,1);
        lcd.print(setpval);
      }
      else
      {
        luminosity = lReceivedValue.val;
        analogWrite(BACKLIGHT,analogRead(LDR)/4); // Controle the luminosity of lcd 
      }
    }
  }
}
