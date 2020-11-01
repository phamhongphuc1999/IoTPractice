var mqtt = require('mqtt');

var client  = mqtt.connect('tcp://broker.hivemq.com:1883',{
    protocolId: 'MQIsdp',
    protocolVersion: 3
});

client.subscribe("esp8266", {qos: 2}, (err, granted) =>{
    console.log("Subscribe");
});

client.on('message', function (topic, message) {
    console.log(message.toString());
    //client.end();
});
