# Esp32 API Rest

Basic example of an ESP32 API server using PlatformIO

In this API, the Esp32 is connected to a DHT11 sensor to read temperature and humidity data. Also included is a ws2812b rgb ribbon for sending and color changing. In addition, for practical purposes, the reading of the internal temperature of the device was included. Therefore, the API server on Esp32 will expose 4 different endpoints:

- read the temperature
- read the internal temperature
- read humidity
- send rgb color

## lib_deps

```bash
lib_deps = 
	bblanchon/ArduinoJson@^6.19.4
	adafruit/Adafruit NeoPixel@^1.10.4
	adafruit/DHT sensor library@^1.4.3
	adafruit/Adafruit Unified Sensor@^1.1.5
``` 

## Api

> Use **GET** temperature
```
http://192.168.0.17/temperature
```
#### Response
```json
{
	"type": "temperature",
	"value": 27.29999924,
	"unit": "°C"
}
```


> Use **GET** humidity
```
http://192.168.0.17/humidity
```
#### Response
```json
{
	"type": "humidity",
	"value": 43,
	"unit": "%"
}
```

> Use **GET** internal
```
http://192.168.0.17/internal
```
#### Response
```json
{
	"type": "internal",
	"value": 53.33333206,
	"unit": "mBar"
}
```

> Use **POST** led
```
http://192.168.0.17/led
```
#### Body
```json
{
	"red":133,
	"blue":222,
	"green":244
}
```