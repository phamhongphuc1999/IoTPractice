#include <SocketIOClient.h>
#include <ESP8266WiFi.h>

String CreateJson(String* keys, String* values, int count);

IPAddress ConnectWifi(const char* ssid, const char* password);
IPAddress ConnectWifi(const char* ssid);
bool CreateClient(SocketIOClient* client, char* host, unsigned int port);
void SendMessage(SocketIOClient* client, String RID, String key, String value);
void SendMessage(SocketIOClient* client, String RID, String* keys, String* values, int count);
