var mqtt = require('mqtt');

var client  = mqtt.connect('tcp://broker.hivemq.com:1883',{
    protocolId: 'MQIsdp',
    protocolVersion: 3
});

client.publish("/my_test", JSON.stringify({
    "name": "Pham Hong Phuc",
    "age": 20
}), {qos: 2, retain: true}, (error, packet) =>{
    console.log(packet)
    client.end()
})
