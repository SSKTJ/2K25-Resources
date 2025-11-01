/**
ESP8266 To Node-Red Transmission
--------------------------------
Gets data from the Arduino through Serial port and sends it to Node-red via wifi
LED at D8 is ON when module is connected to wifi
To be uploaded in the ESP8266 module
**/
#include <ArduinoWiFiServer.h>
#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiGratuitous.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>

#include <string.h>

const char* ssid = "RASFI";
const char* password = "RASFI678";

const char* mqtt_server = "192.168.182.38";

WiFiClient espClient22;
PubSubClient client(espClient22);
SoftwareSerial mySUART(4, 5);

const char* topic_encod_in = "encoding/inp";
//const char* topic_encod_out = "encoding/out";
const char* topic_arduino_serial = "arduino/serial";
const char* topic_decod_out = "decoding/out";

void setup_wifi() {
  delay(10);
  
  Serial.println();
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - NodeMCU IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageInfo = "1_";
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageInfo += (char)message[i];
  }
  Serial.println();
  if (topic == topic_encod_in) {
    mySUART.println(messageInfo);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    if (client.connect("ESP8266Client22")) {
      Serial.println("connected");  
      digitalWrite(D8, HIGH);
      client.subscribe(topic_encod_in);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  mySUART.begin(9600);
  Serial.begin(9600);
  setup_wifi();
  pinMode(D8, OUTPUT);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (mySUART.available()) {
    String arduinoData = mySUART.readStringUntil('\n');
    Serial.println(arduinoData);
    if (arduinoData.length() > 0 && arduinoData[0] == '1' && arduinoData[1] == '_') {
      arduinoData = arduinoData.substring(2);
      client.publish(topic_arduino_serial, arduinoData.c_str());
      client.publish(topic_decod_out, "-");
      Serial.print("Published to ");
      Serial.print(topic_arduino_serial);
      Serial.print(": ");
      Serial.println(arduinoData);
    } else if (arduinoData.length() > 2 && arduinoData[0] == '0' && arduinoData[1] == '_'){
      arduinoData = arduinoData.substring(2);
      client.publish(topic_decod_out, arduinoData.c_str());
      client.publish(topic_arduino_serial, "-");
      Serial.print("Published to ");
      Serial.print(topic_decod_out);
      Serial.print(": ");
      Serial.println(arduinoData);
    } 
  }
}