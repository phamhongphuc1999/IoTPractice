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

//send data to ESP8266
function sendTime(){
    var json = {
        a: "123",
        b: "321"
    }
    io.sockets.emit('atime', json);
}

io.on('connection', function(socket){
    console.log('Connected');
    socket.emit('welcome', {
        message: 'Connected !!!!'
    });

    socket.on('connection', function(message) {
        console.log(message);
    });

    //get json data from ESP8266
    socket.on('atime', function(data) {
        sendTime();
        console.log(data["1"]);
        console.log(data['2']);
    });

    socket.on('arduino', function (data) {
        io.sockets.emit('arduino', { message: 'R0' });
        console.log(data);
    });
})