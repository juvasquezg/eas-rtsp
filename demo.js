'use strict'

const addon = require('./build/Release/eas-rtsp')
const fs = require('fs')


addon.getBuffer((err, buffer) => {
  if (err) {
    console.log(err)
  } else {
    console.log(buffer.toString('hex'))
	console.log('*********Otro buffer...**********')
	addon.getBuffer((err, buffer) => {
	  if (err) {
		console.log(err)
	  } else {
		console.log(buffer.toString('hex'))
	  }
	})
	// fs.writeFile('stream.h264', buffer, 'binary', function(err) {
	//   if(err) {
    //     console.log(err)
	//   } else {
    //     console.log("The file was saved!")
	//   }
    // })
  }
})

// Code not reached
console.log('continue...')
