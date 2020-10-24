import pika
import json

connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel = connection.channel()
msg = json.dumps({
    'id': 11,
    'packet_no': 100,
    'temperature': 60,
    'humidity': 50,
    'tds': 50,
    'pH': 50
})

channel.queue_declare(queue='hello')
channel.basic_publish(exchange='', routing_key='hello', body=msg)
print("Send: " + msg)
connection.close()
