#include <ESP8266WiFi.h>
const char* ssid = "Stationssid";   //agin ssid
const char* password = "1234567890";//agin password
const char* host="192.168.4.1";   // hosted host
const char* apssid = "APssid";    // I will broadcast the ssid name of the network
const char* appassword = "1234567890"; // I will broadcast the encryption of the network
String cgv; // my variable to read the data from the client
int k;
const int port=5001; // port i am connected to
WiFiServer server(80); // AP server description
WiFiClient clientSTA; // client mod for station mod
WiFiClient clientAP; // Client name for AP mode

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA); /*1*/ //ESP8266 works in both AP mode and station mode
  //WiFi.mode(WIFI_STA); /*2*/  // ESP8266 works in station mode
  WiFi.begin(ssid, password); // given the network

  Serial.print(ssid);
  Serial.print("connecting to ");
  while (WiFi.status() != WL_CONNECTED) {
    // not connected to the network
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print(ssid);
  Serial.println(" Connected to WiFi address.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());     // Printing IP address
  Serial.print("Signal Power: ");
  Serial.println(WiFi.RSSI());        // printing signal strength
  clientSTA.connect(host,port);
  Serial.println("setup settings made.");
}

void loop() {
  if(!clientSTA.connect(host,port)) {
    // If there is no connect from host and port given
    Serial.println("No Station connection");
    k=0;
    return;
  }
  while(clientSTA.connected()) // while in station mode
  {
    if(k==0)
    { 
      Serial.println("Station connection available");
     // WiFi.mode(WIFI_AP_STA);  /*setup for wifi mode 2 * / / If WiFi is in modem station mode, then it will work in both modes after connect
      WiFi.softAP(apssid, appassword); // Start AP mode
      IPAddress myIP = WiFi.softAPIP(); // IP address for AP mode
      Serial.print("AP IP address: ");
      Serial.println(myIP);
      clientAP=server.available(); // Replace client mode of AP mode with server
      server.begin(); // server  start
      k=1;
    }
    while(clientSTA.available()) {
      // If data is coming from the station
      Serial.println("Data is coming from the station");
      cgv=clientSTA.readStringUntil('\r'); // read incoming data
      Serial.print("Data: ");
      Serial.println(cgv);
      clientSTA.flush();  // clear clientSTA
      cgv="";
      if(clientAP) {
        // If clientAP is active
        Serial.println("There is an AP link"); 
        clientAP.print(cgv); // send incoming data to clientAP
      }
    }
    if(clientAP.connected()) {
      // If clientAP is connected
      while(clientAP.available()) {
        // If the clientAP da data is coming
        Serial.println("Data comes from access point");
        cgv=clientAP.readStringUntil('\r'); // read incoming data
        Serial.print("AP data: ");
        Serial.println(cgv);
        clientAP.flush(); // clear clientAP
        clientSTA.print(cgv); // send incoming data to station
        cgv="";
      }
    }
  }
}