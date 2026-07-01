# Session 17 - JSON UART Protocol

## Description

JSON-based communication protocol between a Raspberry Pi 3B and an ESP32-C5 for reliable robot command transmission.

This session introduces a structured UART protocol using JSON messages, acknowledgements (ACK), status reporting, automatic retry logic and protocol-level error handling.

The Raspberry Pi sends robot joint commands while the ESP32 validates, parses and applies the received joint angles to the PCA9685 servo controller.

---

## Features

* JSON-based UART communication
* Raspberry Pi ↔ ESP32 protocol
* Command sequence numbers
* ACK confirmation
* Automatic retry on timeout
* Periodic STATUS messages
* Protocol error handling
* PCA9685 servo output
* 18 joint command support
* Serial Monitor debugging

---

## Hardware Used

* Raspberry Pi 3B
* ESP32-C5 DevKitC-1
* PCA9685 16-Channel PWM Driver
* Servo Motors
* USB Cable
* Jumper Wires

---

## UART Wiring

| Raspberry Pi | ESP32-C5   |
| ------------ | ---------- |
| GPIO14 (TX)  | GPIO4 (RX) |
| GPIO15 (RX)  | GPIO5 (TX) |
| GND          | GND        |

UART Configuration

* Baud Rate: 115200
* Data Bits: 8
* Parity: None
* Stop Bits: 1 (8N1)

---

## I2C Wiring

| ESP32-C5    | PCA9685 |
| ----------- | ------- |
| GPIO2 (SDA) | SDA     |
| GPIO3 (SCL) | SCL     |
| 3.3V        | VCC     |
| GND         | GND     |

I2C Address

* PCA9685: `0x40`

---

## Protocol Messages

### CMD

The Raspberry Pi sends a command containing robot joint angles.

Example:

```json
{
  "type": "CMD",
  "seq": 1,
  "ts": 123456789,
  "data": {
    "joints": [90,60,120,...],
    "speed": 25
  }
}
```

---

### ACK

Returned after a valid command has been received and processed.

Example:

```json
{
  "type": "ACK",
  "seq": 1,
  "data": {
    "status": "OK",
    "applied": 16,
    "received": 18,
    "pca9685": "OK"
  }
}
```

---

### STATUS

The ESP32 periodically reports system information.

Example:

```json
{
  "type": "STATUS",
  "seq": 1001,
  "data": {
    "temp": 24.5,
    "hum": 60.0,
    "dist": 30.0,
    "battery": 87,
    "uptime": 15234,
    "active_servos": 16,
    "total_joints": 18,
    "pca9685_found": true
  }
}
```

---

### ERR

Returned whenever an invalid frame is received.

Example:

```json
{
  "type": "ERR",
  "seq": 3,
  "data": {
    "code": "BAD_SIZE",
    "msg": "Need 18 joints"
  }
}
```

---

## Communication Flow

```text
Raspberry Pi
      |
      | JSON CMD
      v
ESP32-C5
      |
      | Validate
      | Parse
      | Apply Servo Angles
      v
ACK

ESP32-C5
      |
      | STATUS every 2 seconds
      v
Raspberry Pi
```

---

## Reliability Features

* Command sequence numbering
* ACK verification
* Automatic retry (up to 3 attempts)
* STATUS heartbeat monitoring
* JSON validation
* Command size validation
* UART timeout detection

---

## Servo Output

The protocol accepts **18 joint angles** representing a complete hexapod robot.

Current hardware applies the first **16 joints** through a single PCA9685 PWM controller.

Joint indices **16** and **17** are received and validated but are not physically driven because one PCA9685 provides 16 PWM channels.

---

## Files

| File                | Description                                                                                     |
| ------------------- | ----------------------------------------------------------------------------------------------- |
| `protocol.py`       | Raspberry Pi JSON UART protocol implementation with ACK handling, retries and STATUS monitoring |
| `uart_protocol.ino` | ESP32-C5 JSON protocol receiver, parser, PCA9685 controller and protocol responses              |
| `README.md`         | Session documentation                                                                           |

---

## Technologies Used

* Raspberry Pi
* ESP32-C5
* Python
* Arduino Framework
* UART
* JSON
* pySerial
* ArduinoJson
* PCA9685
* I2C
* ServoKit
* HardwareSerial

---

## Author

Ruth Cohen

Mentor: Dosithee Miet
