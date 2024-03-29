var amqp = require('amqplib/callback_api');

amqp.connect('amqp://localhost', function(error0, connection) {});

amqp.connect('amqp://localhost', function(error0, connection) {
  if (error0) {
    throw error0;
  }
  connection.createChannel(function(error1, channel) {
    if (error1) {
        throw error1;
      }
      var queue = 'hello';
      var msg = {
        "id": 11,
        "packet_no": 100,
        "temperature": 30,
        "humidity": 50,
        "tds": 1100,
        "pH": 50
      }
      channel.assertQueue(queue, {
        durable: false
      });
      var stringData = JSON.stringify(msg);
      console.log('Send data: ' + stringData);
      channel.sendToQueue(queue, Buffer.from(stringData));
  });
  setTimeout(function() {
    connection.close();
    process.exit(0);
}, 500);
});
