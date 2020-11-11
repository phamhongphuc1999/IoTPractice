const express = require('express')
const mongoose = require('mongoose');
const { mongoUrl } = require('./config');
const routes = require('./routes');

const app = express();
app.use('/', routes);

let server;
let port = process.env.PORT || 3000;

// mongoose.connect(mongoUrl, { useNewUrlParser: true, useUnifiedTopology: true }).then(() => {
//     console.log("connect to mongo")
//     server = app.listen(port, () =>{
//         console.log(`listen to ${port}`)
//     })
// })

server = app.listen(port, () =>{
    console.log(`listen to ${port}`)
})
