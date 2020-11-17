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

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

#pragma region ==================CONTROL LIGHT=======================
void lightSwitchController(){
  if(Serial.available()){
    char status = Serial.read();
    Serial.println(status);
    if(status == '1') {
      digitalWrite(LED_PIN1, HIGH);
      client.publish("lightchanged", CreateJson(new String[2]{"on", "id"}, new String[2]{"1", "1"}, 2));
    }
    else if(status == '0'){
      digitalWrite(LED_PIN1, LOW);
      client.publish("lightchanged", CreateJson(new String[2]{"on", "id"}, new String[2]{"0", "1"}, 2));
    }
    else if(status == '2'){
      digitalWrite(LED_PIN2, LOW);
      client.publish("lightchanged", CreateJson(new String[2]{"on", "id"}, new String[2]{"0", "2"}, 2));
    }
    else if(status == '3'){
        digitalWrite(LED_PIN2, HIGH);
        client.publish("lightchanged", CreateJson(new String[2]{"on", "id"}, new String[2]{"1", "2"}, 2));
    }
  }
}
#pragma endregion

#pragma region =====================SENSOR===========================
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

void sensorSwitchController(bool sign){
  isSensorOn = sign;
  if(sign) count = DELAY_TIME - 1;
}
#pragma endregion

#pragma region ===================================CALLBACK HANDLE==================================
void light_callback(byte* payload, unsigned int length){
  if((char)payload[1] == '1'){
      int control = (char)payload[2] - '0';
      digitalWrite(LED_PIN1, control);
      int status = digitalRead(LED_PIN1);
      if(control == status)
        client.publish("returnresult", CreateJson(new String[2]{"status", "id"}, new String[2]{"1", "1"}, 2));
      else client.publish("returnresult", CreateJson(new String[2]{"status", "id"}, new String[2]{"0", "1"}, 2));
    }
    else if((char)payload[1] == '2'){
      int control = (char)payload[2] - '0';
      digitalWrite(LED_PIN2, control);
      int status = digitalRead(LED_PIN2);
      if(control == status)
        client.publish("returnresult", CreateJson(new String[2]{"status", "id"}, new String[2]{"1", "2"}, 2));
      else client.publish("returnresult", CreateJson(new String[2]{"status", "id"}, new String[2]{"0", "2"}, 2));
    }
    Serial.println();
    Serial.println("-----------------------");
}

void sensor_callback(byte* payload, unsigned int length){
  if((char)payload[2] == '1') sensorSwitchController(true);
  else if((char)payload[2] == '0') sensorSwitchController(false);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message: ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    if((char)payload[0] == 'l') light_callback(payload, length);
    else if((char)payload[0] == 's') sensor_callback(payload, length);
    else client.publish("returnresult", CreateJson(new String[1]{"error"}, new String[1]{"1"}, 1));
}
#pragma endregion

void connectBroker(){
  while (!client.connected()){
    Serial.println("Connecting to public emqx mqtt broker.....");
    if ( client.connect("esp8266-client") ) {
      client.subscribe("controldevice");
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
    char* result = CreateJson(new String[3]{"id", "humidity", "temperature"}, new float[3]{1, humidity, temperature}, 3);
    Serial.println(result);
    client.publish("sensorinformation", result);
  }
  client.loop();
}
