const PORT = 3484;
var http = require('http');
var socketoi = require('socket.io');
var ip = require('ip');
var app = http.createServer();
var io = socketoi(app);
app.listen(PORT);
console.log(`Server run in IP: ${ip.address()} and port: ${PORT}`);

function ParseJson(jsondata) {
    try {
        return JSON.parse(jsondata);
    } catch (error) {
        return null;
    }
}

io.on('connection', function(socket){
    //esp8266 connect to server by RID connection
    socket.on('connection', function(message) {
        console.log('Connected');
        socket.emit('welcome', {
            message: 'Connected !!!!'
        });
        console.log(message);
    });

    //get json data from ESP8266 by RID esp8266
    socket.on('esp8266', function(data) {
        socket.emit('esp8266', {
            id: '123',
            name: 'phuc'
        })
        console.log(data)
    });

    //get json data from ESP32 by RID esp32
    socket.on('esp32', function (data) {
        io.sockets.emit('esp32', { 
            message: 'R0' 
        });
        console.log(data);
    });
})