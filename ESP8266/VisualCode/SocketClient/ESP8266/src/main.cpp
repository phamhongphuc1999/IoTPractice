#include <WifiConnecter.h>

SocketIOClient client;

const char* ssid = "Hacker"; 
const char* password = "tuanhiep99";

char* host = "192.168.1.34";
int port = 3484;

unsigned long previousMillis = 0;
unsigned long interval = 2000;

extern String RID;
extern String Rfull;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.printf("connect to network: %s\n", ssid);
  IPAddress ip = ConnectWifi(ssid, password);

  Serial.println();
  Serial.println("Connected wifi");
  Serial.print("IP address of ESP8266(Socket Client ESP8266): ");
  Serial.println(ip);

  if(!CreateClient(&client, host, port)) {
    Serial.println("Fail to connect socker server");
    return;
  }
  else SendMessage(&client, "connection", "message", "Connected");
}

void loop() {
  if(millis() - previousMillis > interval){
    previousMillis = millis();
    SendMessage(&client, "esp8266", 
          new String[2]{"temperature", "humidity"}, 
          new String[2]{"20", "30"}, 
          2);
  }

  if(client.monitor()){
    Serial.println("Receive from server in RID: " + RID + ", Message: " + Rfull);
  }

  if(!client.connected()){
    client.reconnect(host, port);
  }
}
