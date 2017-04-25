# EAS-RTSP

Extract and Send RTSP es un módulo nativo (C++) de Node.js que usa live555 para extraer el video de la cámara
usando el protocolo RTSP y enviarlo a un programa escrito en Node con javascript.

Usage
---

```js
'use strict'

const addon = require('./build/Release/eas-rtsp')

addon.getBuffer((err, buffer) => {
  if (err) {
    console.log(err)
  } else {
    console.log(buffer.toString('hex'))
  }
})
```

Build
---

Compile live555 con el flag -fPIC

```bash
node-gyp configure
node-gyp build
```
