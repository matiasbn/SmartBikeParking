#include <ESP8266WiFi.h>
#include <Wire.h>
#include <math.h>
#include "BigNumber.h"
#include <string.h>
const char* ssid = "maco.net";
const char* password = "JeBn*20471052-K#";

//Led
int ledPin = BUILTIN_LED;

//Private Keys
int b = 1;
int n = 8;
int mu = 7;
int p = 200;
int i = 10;
int j = 90;
BigNumber ten = 10;
                                  
IPAddress ip(192, 168, 1, 17);
IPAddress gateway(192,168,1,1);
IPAddress subnet (255,255,255,0);
WiFiServer server(301); 

void setup() {
  BigNumber::begin ();
  Serial.begin(115200);
  delay(10);
  WiFi.config(ip,gateway,subnet);
  Serial.println(WiFi.localIP());

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected"); 
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Generar número
  BigNumber numbAmount = ten.pow(p);
  BigNumber::setScale(p);
  BigNumber M = BigNumber(b)/BigNumber(pow(10,n)-mu);
  BigNumber::setScale(0);
  BigNumber SCF = M *numbAmount;
  BigNumber RCF = (SCF%ten.pow(i+j)-SCF%ten.pow(i))/ten.pow(i);
  //Print
  Serial.print("privKey = ");
  Serial.println(RCF);
  delay(5000);
}     
