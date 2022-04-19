/*!
 * @file readDHT11.ino
 * @brief DHT11 is used to read the temperature and humidity of the current environment. 
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Wuxiao](xiao.wu@dfrobot.com)
 * @version  V1.0
 * @date  2018-09-14
 * @url https://github.com/DFRobot/DFRobot_DHT11
 */
#include <SoftwareSerial.h>

#include <ArduinoJson.h>
#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 7
SoftwareSerial espSerial(5, 6);

float temp;
float hum;
void setup(){
  
  Serial.begin(115200);
  espSerial.begin(115200);
  Serial.println("Program started");
}

void loop() {
  DHT.read(DHT11_PIN);
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  //Obtain Temp and Hum data
  dht11_func();


  //Assign collected data to JSON Object
  data["humidity"] = hum;
  data["temperature"] = temp; 

  //Send data to NodeMCU
  data.printTo(espSerial);
  jsonBuffer.clear();

  delay(2000);
}
void dht11_func() {

  hum = DHT.humidity;
  temp = DHT.temperature;
  Serial.print("Humidity: ");
  Serial.println(hum);
  Serial.print("Temperature: ");
  Serial.println(temp);

}
