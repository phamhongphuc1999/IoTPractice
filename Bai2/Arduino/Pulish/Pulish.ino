#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(172, 16, 0, 100);
const char* server = "broker.example.com";

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

void setup() {
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
  mqttClient.setServer(server, 1883);

  if (mqttClient.connect("myClientID")) {
    // connection succeeded
  } 
  else {
     mqttClient.state()
  }

}

void loop() {
  mqttClient.loop();
}
