var amqp = require('amqplib/callback_api');

amqp.connect('amqp://localhost', function(error0, connection) {
    if (error0) {
        throw error0;
    }
    connection.createChannel(function(error1, channel) {
        if (error1) {
            throw error1;
        }

        var queue = 'hello';

        channel.assertQueue(queue, {
            durable: false
        });

        console.log(" [*] Waiting for messages in %s.", queue);

        channel.consume(queue, function(msg) {
            var jsonData = JSON.parse(msg.content.toString());
            console.log(`id: ${jsonData['id']}`);
            console.log(`packet_no: ${jsonData['packet_no']}`);
            console.log(`temperature: ${jsonData['temperature']}`);
            console.log(`humidity: ${jsonData['humidity']}`);
            console.log(`tds: ${jsonData['tds']}`);
            console.log(`pH: ${jsonData['pH']}`);
        }, {
            noAck: true
        });
    });
});