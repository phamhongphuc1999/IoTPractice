#include <ESP8266WiFi.h>

void setup()
{
WiFi.softAP("31/8/2017");
Serial.begin(115200);

}
void loop()
{
Serial.printf("Stations connected to soft-AP = %d \n", WiFi.softAPgetStationNum());
delay(2000); //delay trong 2s để kiểm tra xem có thiết bị nào mới kết nối với module không ?
}
