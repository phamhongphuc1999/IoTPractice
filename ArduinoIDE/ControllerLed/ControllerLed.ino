#include <Arduino.h>

int led = 13;

void setup() {
  Serial.begin(115200); 
  pinMode(led, OUTPUT);
  Serial.printf("Led is albe to controlle");
}

void loop() {
  // if (Serial.available() > 0){
  //   delay(1000);
  //   Serial1.printf("abc");
  // }
  delay(1000);
  Serial.printf("ab");
}
