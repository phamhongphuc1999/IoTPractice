#include <Arduino.h>

int LED = 2;

void setup() {
  Serial.begin(115200); 
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.printf("%s", "Led is albe to controlle\n");
  Serial.println(LED_BUILTIN);
}

void loop() {
  delay(1000);
  if(Serial.available() > 0){
    char bit = Serial.read();
    if(bit == '0'){
      digitalWrite(LED_BUILTIN, LOW);
    }
    else if (bit == '1')
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    int test = digitalRead(LED_BUILTIN);
    Serial.println(test);
  }
}