'use strict'

const addon = require('./build/Release/eas-rtsp')

addon.getBuffer((err, buffer) => {
  if (err) {
    console.log(err)
  } else {
    console.log(buffer.toString('hex'))
  }
})

// Code not reached
console.log('continue...')
