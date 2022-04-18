/*
  Rui Santos
  Complete project details at our blog: https://RandomNerdTutorials.com/esp8266-data-logging-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <DFRobot_DHT11.h>
//DFRobot_DHT11 DHT;
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

 SoftwareSerial espSerial(D6, D5);
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "GUEST-ETC"
#define WIFI_PASSWORD "Etc63lvl@2021Q4"

// Insert Firebase project API Key
#define API_KEY "AIzaSyD-32DzcKDtIsaaxuTLIbWYthMZw5iNL78"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "nguyenkhacbac1233@gmail.com"
#define USER_PASSWORD "12345678"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://esp8266-a385a-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String tempPath = "/temperature";
String humPath = "/humidity";
String timePath = "/timestamp";

// Parent Node (to be updated in every loop)
String parentPath;

FirebaseJson json;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Variable to save current epoch time
int timestamp;

// BME280 sensor

float temperature;
float humidity;
int n=0;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 10000;

// Initialize BME280


// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  timeClient.update();
  unsigned long now = timeClient.getEpochTime();
  return now;
}

void setup(){

  Serial.begin(115200);
  espSerial.begin(115200);
  while (!Serial) continue;

  initWiFi();
  timeClient.begin();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);
  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";
//  Firebase.set("LED_STATUS",0);  
}

void loop(){
//  DHT.read(DHT11_PIN);
  // Send new readings to database
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& data = jsonBuffer.parseObject(espSerial);
    if (data == JsonObject::invalid()) {
    Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }
  Serial.println("JSON Object Recieved");
  Serial.print("Recieved Humidity:  ");
  float hum = data["humidity"];
  Serial.print("Recieved Temperature:  ");
  float temp = data["temperature"];

  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    //Get current timestamp
    timestamp = getTime();
    Serial.print ("time: ");
    Serial.println (timestamp);

    parentPath= databasePath + "/" + String(timestamp);

    json.set(tempPath.c_str(), String(temp));
    json.set(humPath.c_str(), String(hum));
//    json.set(presPath.c_str(), String(bme.readPressure()/100.0F));
    json.set(timePath, String(timestamp));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
    
  }


}
