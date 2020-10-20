#include <Arduino.h>

int led = 13;

void setup() {
  Serial.begin(115200); 
  pinMode(led, OUTPUT);
  while(!Serial); 
  Serial.printf("Led is albe to controlle");
}

void loop() {
  if (Serial.available() > 0){
    delay(10);
    char bit = Serial.read();
    Serial.println(bit);
  }
  else{
    Serial.printf("error");
  }
}