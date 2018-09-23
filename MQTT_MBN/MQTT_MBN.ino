#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Semola con Leche"
#define WLAN_PASS       "Sanluis296"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "m13.cloudmqtt.com"
#define AIO_SERVERPORT  16342                   // use 8883 for SSL
#define AIO_USERNAME    "jdlolrja"
#define AIO_KEY         "AG5uhSft5ylk"
#define LAMPARA 13
#define CALEFACTOR 12
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT,AIO_USERNAME,AIO_KEY);

/****************************** Feeds ***************************************/
//Adafruit_MQTT_Publish pub_prueba = Adafruit_MQTT_Publish(&mqtt, "prueba");
Adafruit_MQTT_Subscribe sub_lampara = Adafruit_MQTT_Subscribe(&mqtt, "prueba/lampara");
Adafruit_MQTT_Subscribe sub_calefactor = Adafruit_MQTT_Subscribe(&mqtt, "prueba/calefactor");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LAMPARA, OUTPUT);
  pinMode(CALEFACTOR, OUTPUT);
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&sub_lampara);
  mqtt.subscribe(&sub_calefactor);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &sub_lampara) {
      Serial.print(F("Got: "));
      Serial.println((char *)sub_lampara.lastread);
      if(strcmp((char *)sub_lampara.lastread,"1")==0){
        digitalWrite(LAMPARA, HIGH);
        }
      if(strcmp((char *)sub_lampara.lastread,"0")==0){
        digitalWrite(LAMPARA, LOW);
        }   
      }
    if (subscription == &sub_calefactor) {
      Serial.print(F("Got: "));
      Serial.println((char *)sub_calefactor.lastread);
      if(strcmp((char *)sub_calefactor.lastread,"1")==0){
        digitalWrite(CALEFACTOR, HIGH);
        }
      if(strcmp((char *)sub_calefactor.lastread,"0")==0){
        digitalWrite(CALEFACTOR, LOW);
        }   
      }
    }
  }

//  // Now we can publish stuff!
//  Serial.print(F("\nSending pub_prueba val "));
//  Serial.print(x);
//  Serial.print("...");
//  if (! pub_prueba.publish(x++)) {
//    Serial.println(F("Failed"));
//  } else {
//    Serial.println(F("OK!"));
//  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
//}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
