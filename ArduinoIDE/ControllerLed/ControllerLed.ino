int led = 13;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  if (Serial.available() > 0){
    char bit = Serial.read();
    if(bit == '0'){
      digitalWrite(led, HIGH);
    }
    if(bit == '1'){
      digitalWrite(led, LOW);
    }
    Serial.println(bit);
  }
}
