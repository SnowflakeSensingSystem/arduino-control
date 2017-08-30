//Libraries
#include <DHT.h>
#define DHTPIN 12     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

////////////////////
//Pin Declaration//
//////////////////
int const picControl = 6;
int const laserControl = 5;

int const Actuator1 = 2;
int const Actuator2 = 3;

//int const FlashControl = 4;
//int const LaserControl = 9;
//int const usbEnable = 8;
int const TempFan = 10;
//int const TempHeat = 10;
int const TempSensor1 = 1;
int const TempSensor2 = 0;
int const TempSensor3 = 3;
int const TempSensor4 = 2;
int const TempSensor5 = 5;

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value
//////////////
//constants//
//////////// 

float Temp1 = 0;
float Temp2 = 0;
float Temp3 = 0;
float Temp4 = 0;
float Temp5 = 0;

float DewTemp = 0;
float Humidity = 0;
int TempFreeze = -10;
int TempCool = 4;
int TempWarm = 700;
int TempHot = 35;
float TempSum = 0;
float TempAverage = 0;
boolean coolActive = false;
boolean heatActive = false;

float HumiditySum = 0;
float DewTempSum = 0;
float Temp3Sum = 0;
float Temp4Sum = 0;
float Temp5Sum = 0;
float TempAverageSum = 0;

int timeOff = 0;
int countOff = 0;
int testPrintLatency = 5000;
int timeRound = 0;
int count = 0;
unsigned long twenty = 20000;

void runActuator(int time){
  pinMode(Actuator1, OUTPUT);
  pinMode(Actuator2, OUTPUT);
  digitalWrite(Actuator1, 0);
  digitalWrite(Actuator2, 1);
  delay(time);
  digitalWrite(Actuator2, 0);
  delay(time);
  digitalWrite(Actuator1, 1);
  delay(time*2);
  digitalWrite(Actuator1, 0);
}

void setup(){
  Serial.begin(9600);
  //dht.begin();
  pinMode(picControl, OUTPUT);
  pinMode(laserControl, OUTPUT);

  pinMode(Actuator1, OUTPUT);
  pinMode(Actuator2, OUTPUT);
  
  digitalWrite(picControl, LOW);

  while(Serial.available() == false){}
  char ch = '1';                                    //Serial.read();      Hard coded this to 1 since it's not needed for test. 3/31/17 jonathan

  //runActuator(5);                             //Commented this out 3/31/17 because actuator is not needed (jonathan)
  
  if (ch == '1'){
    digitalWrite(laserControl, LOW);
  }
  if (ch == '2'){
    digitalWrite(laserControl, HIGH);
  }
  else{
    digitalWrite(laserControl, LOW);
  }
  
  digitalWrite(picControl, HIGH);


  
}
void loop(){
    int timeA = millis();
    if (timeA - timeRound > testPrintLatency){
      timeRound = timeA;
      count += 1;
      //int reading = analogRead(TempSensor1);  
      //float voltage = reading * 5.0;
      //voltage /= 1024.0; 
      //Temp1 = (voltage - 0.5) * 100 ;
      //reading = analogRead(TempSensor2);  
      //voltage = reading * 5.0;
      //voltage /= 1024.0; 
      //Temp2 = (voltage - 0.5) * 100 ;
      int reading = analogRead(TempSensor3);  
      float voltage = reading * 5.0;
      voltage /= 1024.0; 
      Temp3 = (voltage - 0.5) * 100 ;
      reading = analogRead(TempSensor4);  
      voltage = reading * 5.0;
      voltage /= 1024.0; 
      Temp4 = (voltage - 0.5) * 100 ;
      reading = analogRead(TempSensor5);  
      voltage = reading * 5.0;
      voltage /= 1024.0; 
      Temp5 = (voltage - 0.5) * 100 ;
      Humidity = dht.readHumidity();
      DewTemp = dht.readTemperature();
      //TempSum += Temp1;
      //TempSum += Temp2;
      TempSum += Temp3;
      TempSum += Temp4;
      TempSum += Temp5;

      TempAverage = TempSum/3;

      TempSum = 0;
//      if (count = 100){
//        if (TempAverage <= TempFreeze){
//          digitalWrite(TempHeat, HIGH);
//          digitalWrite(TempFan, LOW);
//          digitalWrite(FlashControl, LOW);
//          Serial.println("Average Temperature got too cold");
//          while(1){}
//        }
//        if (TempAverage <= TempCool){
//          digitalWrite(TempHeat, HIGH);
//          digitalWrite(TempFan, LOW);
//          Serial.println("Average Temperature got too cold");
//        }
//        if(TempAverage >= TempHot){
//          digitalWrite(TempFan, HIGH);
//          digitalWrite(TempHeat, LOW);
//          Serial.println("Average Temperature got too hit");
//          while(1){}
//        }
        //Serial.print("Temp Sensor 1 (C), "); Serial.print(Temp1); Serial.println(",");
        //Serial.print("Temp Sensor 2 (C), "); Serial.print(Temp2); Serial.println(",");
        HumiditySum += Humidity;
        DewTempSum += DewTemp;
        Temp3Sum += Temp3;
        Temp4Sum += Temp4;
        Temp5Sum += Temp5;
        TempAverageSum += TempAverage;

        if (count == 12){
          Serial.print("Temp Sensor 3 (C), "); Serial.print(Temp3Sum / (count)); Serial.println(",");
          Serial.print("Temp Sensor 4 (C), "); Serial.print(Temp4Sum / (count)); Serial.println(",");
          Serial.print("Temp Sensor 5 (C), "); Serial.print(Temp5Sum / (count)); Serial.println(",");
          float humidityAverage = HumiditySum / (count);
          float dewTempAverage = DewTempSum / (count);
          Serial.print("Humidity (%), ");
          Serial.print(humidityAverage);
          Serial.print(", Temp (C), ");
          Serial.print(dewTempAverage);
          Serial.println(",");
          double gamma = log10(humidityAverage/100) + ((17.62 * dewTempAverage) / (243.5 + dewTempAverage));
          double dp = 243.5 * gamma / (17.62 - gamma);
          Serial.print("Dew point (C), ");
          Serial.print(dp);
          Serial.print(",");
          Serial.println();
          Serial.print("Average Temperature (C), "); Serial.print(TempAverageSum / (count)); Serial.println(",");
          Serial.println();
          count = 0;
          Temp3Sum = 0;
          Temp4Sum = 0;
          Temp5Sum = 0;
          HumiditySum = 0;
          DewTempSum = 0;
          TempAverageSum = 0;
        }
      }
      Temp1 = 0;
      Temp2 = 0;
      Temp3 = 0;
      Temp4 = 0;
      Temp5 = 0;

      Humidity = 0;
      DewTemp = 0;
    }




