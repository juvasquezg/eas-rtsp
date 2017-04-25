# EAS-RTSP

Extract and Send RTSP es un módulo nativo (C++) de Node.js que usa live555 para extraer el video de la cámara
usando el protocolo RTSP y enviarlo a un programa escrito en Node con javascript.

Usage
---

```js
const rtsp = require('eas-rtsp')

rtsp.initExtract()

rtsp.on('data', (data, size) => {
	http.send(data, size);
})
```

