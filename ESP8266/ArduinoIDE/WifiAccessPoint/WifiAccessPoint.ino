#include <ESP8266WiFi.h>

void setup()
{
WiFi.softAP("phamhongphuc", "123456789");
Serial.begin(115200);

}
void loop()
{
Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
delay(2000);
}
