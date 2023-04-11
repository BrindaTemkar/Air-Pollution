#include <SdsDustSensor.h>
#include <LiquidCrystal.h>      //Header file for LCD

#include <DHT.h>
DHT dht(7, DHT11);
int dataPin=6;

const int rs=12, en=11, d4=5, d5=4, d6=3, d7=2; //pins of LCD connected to Arduino
LiquidCrystal lcd(rs,en,d4,d5,d6,d7); //lcd function from LiquidCrystal
int rxPin = 0;
int txPin = 1;
SdsDustSensor sds(rxPin, txPin);
#define Mq135sensor A0

#define Mq2sensor  A1
#define Mq7sensor  A2



#define Mq135_LED    9      
#define Mq2_LED      7
#define BUZZER       1  // for Mq135 sensor   

void setup() {
  Serial.begin(9600);
  dht.begin();
  sds.begin();
 
  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setActiveReportingMode().toString()); // ensures sensor is in 'active' reporting mode
  Serial.println(sds.setContinuousWorkingPeriod().toString()); // ensures sensor has continuous working period - default but not recommended
  pinMode (BUZZER,OUTPUT);     // buzzer is connected as Output from Arduino
  pinMode (Mq135_LED,OUTPUT);     // led is connected as output from Arduino
  pinMode (Mq2_LED,OUTPUT); 
  pinMode(Mq135sensor,A0);
  pinMode(Mq2sensor,A1);
  pinMode(Mq7sensor,A2);
  pinMode(dataPin,6);
  
   // print sensor name on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Mq135sensor= ");//Mq135 Sensor data will be displayed at (3,0)
 
   
  
  
}
  
void loop() {
  /*
     read continously from each sensor
  */
  //print digital values from Mq135 Sensor
  Mq135sensor_Readings();
  
  Mq2sensor_Readings();

  Mq7sensor_Readings();

  DHT11_Readings();

  SdsDustSensor_Readings();
  

}

void Mq135sensor_Readings()
{
  int threshold = 250;  
  int ppm = analogRead(Mq135sensor); //read MQ135 analog outputs at A0 and store it in ppm

  Serial.print("Air Quality: ");  //print message in serail monitor
  Serial.println(ppm);            //print value of ppm in serial monitor

  lcd.setCursor(0,0);             // set cursor of lcd to 1st row and 1st column
  lcd.print("Air Quality: ");      // print message on lcd
  lcd.print(ppm);                 // print value of MQ135

  if (ppm > threshold)            // check is ppm is greater than threshold or not
    {
      lcd.setCursor(1,1);         //jump here if ppm is greater than threshold
      lcd.print("AQ Level HIGH");
      Serial.println("AQ Level HIGH");     
      tone(Mq135_LED,1000,200);         //blink led with turn on time 1000mS, turn off time 200mS
      digitalWrite(BUZZER,HIGH);     //Turn ON Buzzer
    }
  else
    {
      digitalWrite(Mq135_LED,LOW);   //jump here if ppm is not greater than threshold and turn off LED
      digitalWrite(BUZZER,LOW);   //Turn off Buzzer
      lcd.setCursor(1,1);
      lcd.print ("AQ Level Good");
      Serial.println("AQ Level Good");
    }  
    delay(500);
}
void Mq2sensor_Readings(){
int gas_value = digitalRead(Mq2sensor);

if(gas_value==HIGH)
{
  digitalWrite( Mq2_LED  , HIGH);
  Serial.println("Mq2 gas cocentration is High");
  delay(200);
  
  
}
else
{
 
 digitalWrite( Mq2_LED  , LOW); 
 Serial.println("Mq2 gas concentration is low");
 delay(200);
}
}
void Mq7sensor_Readings(){
int gas_value1 = digitalRead(Mq7sensor);

if(gas_value1==HIGH)
{
  
  Serial.println("Mq7 gas cocentration is High");
  delay(200);
  
  
}
else
{
 
 
 Serial.println("Mq7 gas concentration is low");
 delay(200);
}
}
void DHT11_Readings()
{
  float temp=dht.readTemperature();
  float hum=dht.readHumidity();
  Serial.print("Temperature ");
  Serial.print(temp);
  Serial.print("C");
  Serial.print("     Humidity ");
  Serial.print(hum);
  Serial.print(" %");
  Serial.println();
  delay(1000);
  

}
void SdsDustSensor_Readings()
{
  PmResult pm = sds.readPm();
  if (pm.isOk()) {
    Serial.print("PM2.5 = ");
    Serial.print(pm.pm25);
    Serial.print(", PM10 = ");
    Serial.println(pm.pm10);
 
    // if you want to just print the measured values, you can use toString() method as well
    Serial.println(pm.toString());
  } else {
    // notice that loop delay is set to 0.5s and some reads are not available
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }
 
  delay(500);
}