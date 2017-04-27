'use strict'

const addon = require('./build/Release/eas-rtsp')


console.log(`native addon whoami: ${addon.getBuffer()}`)

// Code not reached
console.log('continue...')
