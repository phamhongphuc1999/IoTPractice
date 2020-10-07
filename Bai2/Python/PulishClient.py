import paho.mqtt.client as paho
import time


def on_connect(client, userdata, flags, rc):
    print("CONNACK received with code %d." %(rc))


def on_publish(client, userdata, mid):
    print("mid: "+str(mid))


client = paho.Client()

# add event
client.on_connect = on_connect
client.on_publish = on_publish

client.connect("broker.mqttdashboard.com", 1883)
client.loop_start()

while True:
    (rc, mid) = client.publish("encyclopedia/temperature", "bom nguyen tu", qos=1, retain=True)
    time.sleep(3)
