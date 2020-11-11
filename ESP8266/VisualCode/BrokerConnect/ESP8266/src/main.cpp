#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "PubSubClient.h"
#include "utility.h"
#include "config.h"
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT21

int status = WL_IDLE_STATUS;
float humidity = 0, temperature = 0;
bool statusSensor = false;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    if((char)payload[0] == '1'){
      digitalWrite(LED_BUILTIN, HIGH);
      int status = digitalRead(LED_BUILTIN);
      if(status == 1) 
        client.publish("returnresult", CreateJson(new String[1]{"status"}, new String[1]{"1"}, 1));
      else 
        client.publish("returnresult", CreateJson(new String[1]{"status"}, new String[1]{"0"}, 1));
    }
    else{
      digitalWrite(LED_BUILTIN, LOW);
      int status = digitalRead(LED_BUILTIN);
      if(status == 0) 
        client.publish("returnresult", CreateJson(new String[1]{"status"}, new String[1]{"1"}, 1));
      else 
        client.publish("returnresult", CreateJson(new String[1]{"status"}, new String[1]{"0"}, 1));
    }
    Serial.println();
    Serial.println("-----------------------");
}

void connectBroker(){
  while (!client.connected()){
    Serial.println("Connecting to public emqx mqtt broker.....");
    if ( client.connect("esp8266-client") ) {
      client.subscribe("lightchannel");
      Serial.println("Connected to public emqx mqtt broker");
    } else{
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void connectWifi(){
  WiFi.begin(ssid, password);
  Serial.print("connecting to WiFi network");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("\nConnected to the WiFi network: ");
  Serial.println(ssid);
}

void lightSwitchController(){
  if(Serial.available()){
    char status = Serial.read();
    Serial.println(status);
    if(status == '1') {
      digitalWrite(LED_BUILTIN, HIGH);
      client.publish("lightchanged", CreateJson(new String[1]{"on"}, new String[1]{"1"}, 1));
    }
    else {
      digitalWrite(LED_BUILTIN, LOW);
      client.publish("lightchanged", CreateJson(new String[1]{"on"}, new String[1]{"0"}, 1));
    }
  }
}

bool measureHumidityAdTemperature(float* humidity, float* temperature){
  delay(1000);
  *humidity = dht.readHumidity();
  *temperature = dht.readTemperature();
  if (isnan(*humidity) || isnan(*temperature)) 
  {
    Serial.println("Failed to read from DHT");
    return false;
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("started sensor");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  connectWifi();
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
  lightSwitchController();
  statusSensor = measureHumidityAdTemperature(&humidity, &temperature);
  if(statusSensor){
    char* result = CreateJson(new String[2]{"humidity", "temperature"}, new float[2]{humidity, temperature}, 2);
    Serial.println(result);
  }
  client.loop();
}
