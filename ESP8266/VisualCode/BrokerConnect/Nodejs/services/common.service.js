const mqtt = require('mqtt');
const { brokerUrl } = require('../config');

var client  = mqtt.connect(brokerUrl, {
    protocolId: 'MQIsdp',
    protocolVersion: 3
});

const subscribe = (chanel, _qos) => {
    client.subscribe(chanel, {qos: _qos}, (err, branted) => {
        console.log(`subcribe: ${chanel}`);
    })
    client.on('message', function(topic, message){
        client.end();
        return message;
    })
}

const publish = (chanel, _qos, _retain, jsonData) => {
    client.publish(chanel, jsonData, {qos: _qos, retain: _retain}, (err, packet) =>{
        client.end();
        return packet;
    })
}

module.exports = {
    subscribe,
    publish
}
