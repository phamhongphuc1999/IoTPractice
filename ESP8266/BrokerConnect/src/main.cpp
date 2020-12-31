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
String* head = new String[4]{"espID", "deviceID", "status", "data"};

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

bool CheckID(String espId, String deviceId){
  int index = -1;
  if(espId == "1") index = esp1->indexOf(deviceId);
  else if(espId == "2") index = esp2->indexOf(deviceId);
  return (index > -1);
}

bool ValidMessage(char* message){
  int count = 0, index = 0;
  while(message[index] != '\0'){
    if(message[index] == '.') count++;
    if(count > 2) return false;
    index++;
  }
  return (count == 2);
}

String GetEsp1Status(){
  int status = digitalRead(LED_PIN1);
  String* h = new String[3]{"deviceID", "type", "status"};
  String* body;
  if(status == 0) body = new String[3]{"01", "light", "0"};
  else body = new String[3]{"01", "light", "1"};
  String result = "{\"espID\":\"1\",\"device\":[";
  result += CreateJson(h, body, 3) + "]}";
  return result;
}

String GetEsp2Status(){
  int status = digitalRead(LED_PIN2);
  String* h = new String[3]{"deviceID", "type", "status"};
  String* body;
  if(status == 0) body = new String[3]{"02", "light", "0"};
  else body = new String[3]{"02", "light", "1"};
  String result = "{\"espID\":\"2\",\"device\":[";
  result += CreateJson(h, body, 3) + "]}";
  return result;
}

String GetAllStatus(){
  String result = "{\"esp\":[";
  result += GetEsp1Status() + ",";
  result += GetEsp2Status() + "]}";
  return result;
}

void lightSwitchController(){
  if(Serial.available()){
    char status = Serial.read();
    Serial.println(status);
    String* body;
    if(status == '1') {
      digitalWrite(LED_PIN1, HIGH);
      body = new String[4]{"1", "01", "true", CreateJson(new String[1]{"on"}, new String[1]{"true"}, 1)};
    }
    else if(status == '0'){
      digitalWrite(LED_PIN1, LOW);
      body = new String[4]{"1", "01", "true", CreateJson(new String[1]{"on"}, new String[1]{"false"}, 1)};
    }
    else if(status == '2'){
      digitalWrite(LED_PIN2, LOW);
      body = new String[4]{"1", "02", "true", CreateJson(new String[1]{"on"}, new String[1]{"false"}, 1)};
    }
    else if(status == '3'){
      digitalWrite(LED_PIN2, HIGH);
      body = new String[4]{"1", "02", "true", CreateJson(new String[1]{"on"}, new String[1]{"true"}, 1)};
    }
    bool* isString = new bool[4]{true, true, true, false};
    String payload = CreateJson(head, body, isString, 4);
    client.publish("lightchanged", payload.c_str());
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
  int control = (char)status[0] - '0', currentStatus = 0;
  if(deviceId == "01"){
      digitalWrite(LED_PIN1, control);
      currentStatus = digitalRead(LED_PIN1);
    }
    else if(deviceId == "02"){
      digitalWrite(LED_PIN2, control);
      currentStatus = digitalRead(LED_PIN2);
    }
    String* body;
    if(currentStatus == control) body = new String[4]{"1", deviceId, "true", CreateJson(new String[1]{"status"}, new String[1]{"1"}, 1)};
    else body = new String[4]{"1", deviceId, "true", CreateJson(new String[1]{"status"}, new String[1]{"1"}, 1)};
    bool* isString = new bool[4]{true, true, true, false};
    String payload = CreateJson(head, body, isString, 4);
    client.publish("returnresult", payload.c_str());
    Serial.println("\n-----------------------");
}

void controldevice_callback(byte* payload){
  String* result = SplitString((char*)payload);
  if(!CheckID(result[0], result[1])){
    String* body = new String[4]{result[0], result[1], "false", CreateJson(new String[1]{"reason"}, new String[1]{"error accountId or error deviceId"}, 1)};
    bool* isString = new bool[4]{true, true, true, false};
    String payload = CreateJson(head, body, isString, 4);
    client.publish("returnresult", payload.c_str());
  }
  else if(result[1][0] == '0') light_callback(result[1], result[2]);
  else client.publish("returnresult", CreateJson(new String[1]{"error"}, new String[1]{"1"}, 1).c_str());
}

void getdevicestatus_callback(byte* payload){
  client.publish("on_online", GetAllStatus().c_str());
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    if(ValidMessage((char*)payload)){
      if (strcmp(topic, "controldevice") == 0) controldevice_callback(payload);
      else if(strcmp(topic, "getdevicestatus") == 0) getdevicestatus_callback(payload);
    }
    else {
      String* head = new String[2]{"status", "reason"};
      String* body = new String[2]{"false", "payload error"};
      client.publish("returnresult", CreateJson(head, body, 2).c_str());
    }
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
  if(statusSensor){
    String* body = new String[4]{"1", "11", "true", CreateJson(new String[2]{"humidity", "temperature"}, new float[2]{humidity, temperature}, 2)};
    bool* isString = new bool[4]{true, true, true, false};
    String result = CreateJson(head, body, isString, 4);
    Serial.println(result);
    client.publish("sensorinformation", result.c_str());
  }
  client.loop();
}
