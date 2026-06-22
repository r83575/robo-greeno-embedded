# Session 16 - Raspberry Pi ESP32 UART Bridge

## Description

UART communication project between a Raspberry Pi 3B and an ESP32-C5.

This session demonstrates two-way serial communication over UART, where the Raspberry Pi sends robot joint angle commands and the ESP32 receives, parses and acknowledges the data.

---

## Features

- Raspberry Pi UART communication
- ESP32 UART communication
- Full duplex serial communication
- pySerial
- HardwareSerial
- 18 joint angle transmission
- CSV message parsing
- JSON acknowledgement
- Serial Monitor debugging

---

## Hardware Used

- Raspberry Pi 3B
- ESP32-C5 DevKitC-1
- USB Cable
- Jumper Wires

---

## UART Wiring

| Raspberry Pi | ESP32-C5 |
|---|---|
| GPIO14 (TX) | GPIO4 (RX) |
| GPIO15 (RX) | GPIO5 (TX) |
| GND | GND |

UART Configuration

- Baud Rate: 115200
- Data Bits: 8
- Parity: None
- Stop Bits: 1 (8N1)

---

## Message Flow

```text
Raspberry Pi
      |
      | 90,60,120,...
      v
ESP32-C5
      |
      | Parse 18 angles
      v
{"received":18,"status":"OK"}
```

---

## Files

| File | Description |
|---|---|
| `rpi_uart_sender.py` | Raspberry Pi UART sender using pySerial |
| `esp32_uart_receiver.ino` | ESP32 UART receiver, CSV parser and JSON acknowledgement |
| `wiring_diagram.txt` | UART wiring reference |
| `README.md` | Session documentation |

---

## Technologies Used

- Raspberry Pi
- ESP32-C5
- Python
- Arduino Framework
- UART
- pySerial
- HardwareSerial
- JSON
- CSV

---

## Author

Ruth Cohen

Mentor: Dosithee Miet
