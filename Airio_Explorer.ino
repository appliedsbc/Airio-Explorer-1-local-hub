#include <WiFiNINA.h>
#include <SPI.h>
#include <SD.h>
#include <TimeLib.h>
#include <Wire.h>

String ssid; // your network SSID (name)
String pass; // your network password (use for WPA, or use as key for WEP)
int timeZone = 0;
IPAddress ip;

int status = WL_IDLE_STATUS;
WiFiServer server(80);
WiFiClient client;


String CARBONDIOXIDE;
String OXYGEN;
String HUMIDITY;
String TEMPERATURE;
String PRESSURE;

String present_file = "/Data/06-01-22.CSV";

const int rate = 10; // sample period in seconds
unsigned long time_read = 0; //records the last time a datapoint was written

String dataString;
String fileList;

//-------------------------SETUP-----------------------------------------------------
void setup() {
  Wire.begin();
  Serial.begin(9600);
  delay(1000);
  Serial1.begin(9600);
  Serial1.print("M 1\r\n"); //initialize oxygen sensor
  delay(2000);

  pinMode(3, OUTPUT); //LED BLUE
  pinMode(5, OUTPUT); //LED GREEN
  pinMode(6, OUTPUT); //LED RED
  pinMode(8, OUTPUT); //BEEPER
  pinMode(10, OUTPUT); //RELAY 1
  pinMode(16, OUTPUT); //FAN
  pinMode(20, OUTPUT); //RELAY 2
  pinMode(21, OUTPUT); //SD CS


  //SD CARD INITIALIZE
  pinMode(9, INPUT); //SD DETECT INTTERUPT
  attachInterrupt(digitalPinToInterrupt(9), insertSD, CHANGE);
  delay(100);
  if (!SD.begin(21)) {
      Serial.println("SD card failed, or not present.");
      beep("error");
  }
  else {
  Serial.println("SD card initialized.");
  beep("connected");
  }

  ConnectToWiFi();

}



//-------------------------MAIN LOOP--------------------------------------------------
void loop() {
  // listen for incoming http requests
  client = server.available();
  if (server.available()) UserClick();


  // check if it's time to log measurements to SD card
  if (time_read <= (now() - rate)) {
    time_read = now();
    T67XX();
    LOX_02();
    HIH8000();
    logSD();
  }

}
