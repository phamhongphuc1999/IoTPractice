//wifi
const char *ssid = "iPhone";
const char *password = "999999999";

//broker
const char *mqtt_broker = "broker.emqx.io";
const int mqtt_port = 1883;

//sensor
const int DELAY_TIME = 10;
bool isSensorOn = false;

//led
const int LED_PIN1 = 5;
const int LED_PIN2 = 16;

//demo
const String* home1 = new String[1]{"01"};
const String* home2 = new String[2]{"02", "11"};
