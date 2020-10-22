#include <ESP8266WiFi.h>

void setup()
{
WiFi.softAP("31/8/2017");
Serial.begin(115200);

}
void loop()
{
Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
delay(2000);
}
