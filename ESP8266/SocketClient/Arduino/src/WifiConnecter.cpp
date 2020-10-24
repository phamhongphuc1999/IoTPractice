#include <WifiConnecter.h>

String CreateJson(String* keys, String* values, int count){
    String result = "{";
    for (int i = 0; i < count - 1; i++)
    {
        result += "\"" + keys[i] + "\":\"" + values[i] + "\",";
    }
    result += "\"" + keys[count - 1] + "\":\"" + values[count - 1] + "\"}";
    Serial.println(result);
    return result;
}

IPAddress ConnectWifi(const char* ssid, const char* password){
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    return WiFi.localIP();
}

bool CreateClient(SocketIOClient* client, char* host, unsigned int port){
    if (!client->connect(host, port)) {
        
        return false;
    }
 
    if (client->connected()) {
        return true;
    }
    return false;
}

void SendMessage(SocketIOClient* client, String RID, String key, String value){
    client->send(RID, key, value);
}

void SendMessage(SocketIOClient* client, String RID, String* keys, String* values, int count){
    client->send(RID, CreateJson(keys, values, count));
}
