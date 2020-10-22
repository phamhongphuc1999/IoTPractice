#include <ESP8266WiFi.h>

IPAddress local_IP(192,168,4,22);
IPAddress gateway(192,168,4,9);
IPAddress subnet(255,255,255,0);

void setup()
{
  Serial.begin(115200);
  // bool check = WiFi.softAPConfig(local_IP, gateway, subnet);
  // if(check) Serial.println("Wifi ready\n");
  // else Serial.println("Wifi fail\n");
  WiFi.softAP("PhamHongPhuc", "123456789");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}
void loop()
{
  Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
  delay(2000);
}
