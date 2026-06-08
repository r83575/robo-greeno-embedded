# Session 11 - WiFi HTTP GET and POST

## Description
ESP32 WiFi communication project using HTTP requests, JSON data and FreeRTOS tasks.

This session demonstrates how an ESP32 connects to WiFi, receives data from an API using HTTP GET, and sends sensor telemetry using HTTP POST.

---

## Projects

| Project | Description |
|---|---|
| `wifi_http_get_parse.ino` | HTTP GET request and JSON parsing |
| `wifi_freertos_distance_post.ino` | FreeRTOS distance sensor telemetry using HTTP POST |

---

## Features
- ESP32 WiFi connection
- HTTP GET request
- HTTP POST request
- JSON parsing with ArduinoJson
- JSON serialization
- FreeRTOS multitasking
- HC-SR04 distance sensor
- Serial Monitor output

---

## Part 1 - HTTP GET and JSON Parsing

The ESP32 sends a GET request to the Open-Meteo API and parses the JSON response.

Data printed:
- Current temperature
- Wind speed

---

## Part 2 - FreeRTOS Distance POST

The ESP32 reads distance from an HC-SR04 sensor and sends the value to a server as JSON.

### FreeRTOS Tasks

| Task | Description |
|---|---|
| `sensorTask` | Reads distance every 2 seconds |
| `wifiTask` | Sends HTTP POST every 5 seconds |
| `displayTask` | Prints distance and WiFi status every 1 second |

---

## JSON Example

```json
{
  "distance": 195.52,
  "device": "ESP32"
}
```

---

## Example Serial Output

```txt
WiFi: OK
Distance: 195.52 cm
WiFi: OK
Distance: 195.38 cm
WiFi: OK
```

---

## Hardware Used
- ESP32
- HC-SR04 Ultrasonic Sensor
- WiFi Network

---

## Pin Connections

| HC-SR04 Pin | ESP32 Pin |
|---|---|
| TRIG | GPIO 4 |
| ECHO | GPIO 5 |

---

## Files

```txt
wifi_http_get_parse.ino
wifi_freertos_distance_post.ino
README.md
```

---

## Technologies Used
- ESP32
- WiFi
- HTTP
- REST API
- ArduinoJson
- FreeRTOS
- HC-SR04

---

## Author
Ruth Cohen