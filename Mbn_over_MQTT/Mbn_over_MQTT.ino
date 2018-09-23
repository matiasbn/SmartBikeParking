#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <math.h>
#include "BigNumber.h"
#include <string.h>
 
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "maco.net"
#define WLAN_PASS       "JeBn*20471052-K#"

/************************* Adafruit.io Setup *********************************/

#define MQTT_SERVER      "m13.cloudmqtt.com"
#define MQTT_PORT  16342                   // use 8883 for SSL
#define MQTT_USER    "jdlolrja"
#define MQTT_PASS         "AG5uhSft5ylk"
 
WiFiClient espClient;
PubSubClient client(espClient);

//Private Keys
int b = 1;
int n = 8;
int mu = 7;
int p = 200;
int i = 10;
int j = 128;
BigNumber ten = 10;
BigNumber RCF = 0;
 
void setup() {
  BigNumber::begin ();
  Serial.begin(115200);
  delay(10);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", MQTT_USER, MQTT_PASS )) {
 
      Serial.println("Connected to the MQTT Broker");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  client.subscribe("mensaje_encriptado");
}
 
void callback(char* topic, byte* payload, int length) {
  char inputChar;
  BigNumber encryptedMessage;
  Serial.print("Mensaje encriptado:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    inputChar = (char)payload[i];
    encryptedMessage+=((BigNumber)inputChar-(BigNumber)48)*ten.pow(length-i-1);
  }
  BigNumber decryptedMessage = encryptedMessage/RCF;
  
  Serial.println();
  Serial.print("Mensaje desencriptado ");
  Serial.println(decryptedMessage);
  if(decryptedMessage%BigNumber(52)==0){
    Serial.println("Mensaje 1 recibido");
  }else if(decryptedMessage%BigNumber(53)==0){
    Serial.println("Mensaje 2 recibido");
  }else {
    Serial.println("Mensaje no reconocido");
  }
  
  Serial.println("-----------------------");
 
}
 
void loop() {
  //Decryptor
  BigNumber numbAmount = ten.pow(p);
  BigNumber::setScale(p);
  BigNumber M = BigNumber(b)/BigNumber(pow(10,n)-mu);
  BigNumber::setScale(0);
  BigNumber SCF = M *numbAmount;
  RCF = (SCF%ten.pow(i+j)-SCF%ten.pow(i))/ten.pow(i);
  client.loop();
}
