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
int count = 0;
String homeId = "";

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

bool CheckID(String homeId, String deviceId){
    if(homeId == "1"){
      int index = home1->indexOf(deviceId);
      return (index == -1);
    }
    else if(homeId == "2"){
      int index = home2->indexOf(deviceId);
      return (index == -1);
    }
}

char* GetAllDeviceStatus(String homeId){
  if(homeId == "1"){
    int status = digitalRead(LED_PIN1);
    String result = "{";
    result += "\"homeID\":\"" + homeId + "\",";
    result += "\"esp\":[{\"espID\": \"1\","
  }
  else if(homeId == "2"){
    int status = digitalRead(LED_PIN2);
  }
}

void lightSwitchController(){
  if(Serial.available()){
    char status = Serial.read();
    Serial.println(status);
    if(status == '1') {
      digitalWrite(LED_PIN1, HIGH);
      char* payload = CreateJson("1", "1", "01", "true", CreateJson(new String[1]{"on"}, new String[1]{"true"}, 1));
      client.publish("lightchanged", payload);
    }
    else if(status == '0'){
      digitalWrite(LED_PIN1, LOW);
      char* payload = CreateJson("1", "1", "01", "true", CreateJson(new String[1]{"on"}, new String[1]{"false"}, 1));
      client.publish("lightchanged", payload);
    }
    else if(status == '2'){
      digitalWrite(LED_PIN2, LOW);
      char* payload = CreateJson("1", "1", "02", "true", CreateJson(new String[1]{"on"}, new String[1]{"true"}, 1));
      client.publish("lightchanged", payload);
    }
    else if(status == '3'){
      digitalWrite(LED_PIN2, HIGH);
      char* payload = CreateJson("1", "1", "02", "true", CreateJson(new String[1]{"on"}, new String[1]{"false"}, 1));
      client.publish("lightchanged", payload);
    }
  }
}

bool measureHumidityAdTemperature(float* humidity, float* temperature){
  if(!isSensorOn) return false;
  count += 1;
  delay(1000);
  if(count >= DELAY_TIME){
    count = 0;
    *humidity = dht.readHumidity();
    *temperature = dht.readTemperature();
    if (isnan(*humidity) || isnan(*temperature)) 
    {
      Serial.println("Failed to read from DHT");
      return false;
    }
    return true;
  }
  else return false;
}

void light_callback(String deviceId, String status){
  int control = (char)status[0] - '0';
  bool check = false;
  if(deviceId == "01"){
      digitalWrite(LED_PIN1, control);
      int status = digitalRead(LED_PIN1);
      check = (status == control);
    }
    else if(deviceId == "02"){
      digitalWrite(LED_PIN2, control);
      int status = digitalRead(LED_PIN2);
      check = (status == control);
    }
    char* payload;
    if(check) payload = CreateJson("1", "1", deviceId, "true", CreateJson(new String[1]{"status"}, new String[1]{"1"}, 1));
    else payload = CreateJson("1", "1", deviceId, "true", CreateJson(new String[1]{"status"}, new String[1]{"0"}, 1));
    client.publish("returnresult", payload);
    Serial.println();
    Serial.println("-----------------------");
}

void controldevice_callback(byte* payload){
  String* result = SplitString((char*)payload);
  if(!CheckID(result[0], result[2])){
      char* payload = CreateJson(result[0], result[1], result[3], "false", CreateJson(new String[1]{"reason"}, new String[1]{"error accountId or error deviceId"}, 1));
      client.publish("returnresult", payload);
  }
  homeId = result[0];
  if(result[2][0] == '0') light_callback(result[2], result[3]);
  else client.publish("returnresult", CreateJson(new String[1]{"error"}, new String[1]{"1"}, 1));
}

void getdevicestatus_callback(byte* payload){
  String* result = SplitString((char*)payload);
  if(!CheckID(result[0], result[2])){
      char* payload = CreateJson(result[0], result[1], result[3], "false", CreateJson(new String[1]{"reason"}, new String[1]{"error accountId or error deviceId"}, 1));
      client.publish("returnresult", payload);
  }
  homeId = result[0];
  client.publish("on_online", GetAllDeviceStatus(homeId));
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.println("Message: ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    if (strcmp(topic, "controldevice") == 0) controldevice_callback(payload);
    else if(strcmp(topic, "getdevicestatus") == 0) getdevicestatus_callback(payload);
}

void connectBroker(){
  while (!client.connected()){
    Serial.println("Connecting to public emqx mqtt broker.....");
    if ( client.connect("ESP8266Client", "admin", "admin") ) {
      client.subscribe("controldevice");
      client.subscribe("getdevicestatus");
      Serial.println("Connected to public emqx mqtt broker in topic: controldevice");
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

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("started sensor");
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  digitalWrite(LED_PIN1, LOW);
  digitalWrite(LED_PIN2, LOW);
  connectWifi();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  connectBroker();
}

void loop() {
  status = WiFi.status();
  if (status != WL_CONNECTED){
    Serial.print("Reconnected to the WiFi network: ");
    Serial.println(ssid);
    connectWifi();
    Serial.print("Connected to the WiFi network: ");
    Serial.println(ssid);
  }
  lightSwitchController();
  statusSensor = measureHumidityAdTemperature(&humidity, &temperature);
  if(statusSensor && homeId == "2"){
    char* result = CreateJson("2", "1", "11", "true", CreateJson(new String[2]{"humidity", "temperature"}, new float[2]{humidity, temperature}, 2));
    Serial.println(result);
    client.publish("sensorinformation", result);
  }
  client.loop();
}
