#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "PubSubClient.h"

const char *ssid = "Hacker";
const char *password = "tuanhiep99";
const char *mqtt_broker = "broker.hivemq.com";
const int mqtt_port = 1883;

int status = WL_IDLE_STATUS;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}

void connectBroker(){
  while (!client.connected()){
    Serial.println("Connecting to public emqx mqtt broker.....");
    if ( client.connect("esp8266-client") ) {
      client.subscribe("esp8266");
      Serial.println("Public emqx mqtt broker connected");
    } else{
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void connectWifi(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
}

void setup() {
  Serial.begin(9600);
  connectWifi();
  Serial.print("Connected to the WiFi network: ");
  Serial.println(ssid);
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  connectBroker();
}

void loop() {
  status = WiFi.status();
  if ( status != WL_CONNECTED){
    Serial.print("Reconnected to the WiFi network: ");
    Serial.println(ssid);
    connectWifi();
    Serial.print("Connected to the WiFi network: ");
    Serial.println(ssid);
  }
  client.loop();
}