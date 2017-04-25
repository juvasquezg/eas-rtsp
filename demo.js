//'use strict'

const addon = require('./build/Release/eas-rtsp')
var dgram = require('dgram')


var PORT = 33333;
var HOST = '127.0.0.1';

var client = dgram.createSocket('udp4');


addon.getBuffer((err, buffer) => {
  if (err) {
    console.log(err)
  } else {
    console.log(buffer.length)
    // client.send(buffer, 0, buffer.length, PORT, HOST, (err, bytes) => {
    //   if (err) throw err
    //   console.log('UDP message sent to ' + HOST +':'+ PORT)
    //   //client.close();
    // })
  }
})

// console.log(buffer.toString('ascii'))
