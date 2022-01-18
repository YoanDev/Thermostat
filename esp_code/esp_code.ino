/*
 * This code allow a connection wifi to thermostat.
*/

#include <WiFi.h>
#include <ThingSpeak.h>    

// pin uart2 for esp32
#define RXp2 16
#define TXp2 17

//Change with your ID
char* ssid = "XXXXXXXXXXXX";
const char* mdp= "XXXXXXXXXXXXX";

WiFiClient  client;
unsigned long counterChannelNumber = 1111111;                // Channel ID
unsigned long counterChannelNumber2 = 2222222;                // Channel ID
const char * myCounterReadAPIKey = "XXXXXXXXXXXXXXXXX";      // Read API Key
const char * myCounterWriteAPIKey = "XXXXXXXXXXXXXXXXX";      // Write API Key
const int FieldNumber1 = 1;                                 // The field you wish to read
String inByte;;

void setup()
{
  Serial.begin(115200); //Communication with PC 
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2); // Communication with arduino
  Serial.println();
  WiFi.begin(ssid, mdp);                 // write wifi name & password           
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop() 
{
  int setpoint = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey); // Read setpoint
  Serial.println(setpoint);
  Serial2.println(setpoint); // Send setpoint to the arduino
  while (Serial2.available() > 0) {
    inByte = Serial2.readString(); // Read temperature
    ThingSpeak.setField(1,inByte); 
    ThingSpeak.writeFields(counterChannelNumber2,myCounterWriteAPIKey); s// Send temperature to the ap
  }
  delay(1000);
}
