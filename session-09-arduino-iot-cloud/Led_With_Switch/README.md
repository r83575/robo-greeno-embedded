# LED With Switch - Arduino IoT Cloud

## Description
ESP32 project that controls an LED remotely using the Arduino IoT Cloud dashboard.

The ESP32 connects to WiFi and receives cloud variable updates from the dashboard button.

---

## Features

- Arduino IoT Cloud integration
- Remote LED control
- WiFi communication
- MQTT/TLS connection
- Real-time dashboard updates

---

## Components

- ESP32 DevKit
- LED
- 220Ω Resistor
- WiFi Network

---

## How It Works

The dashboard button changes a cloud variable.

When the variable changes:
- `true` → LED ON
- `false` → LED OFF

The ESP32 receives the update and controls the LED in real time.

---

## Files

```txt
sketch.ino
thingProperties.h
arduino_secrets.example.h
```

---

## Dashboard

[Open Arduino IoT Cloud Dashboard](https://app.arduino.cc/dashboards/3ac7e57e-2f48-4bed-872d-2b675d4eeb63)

---

## Author

Ruth Cohen