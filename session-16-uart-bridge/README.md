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
| `rpi_uart_1000_test.py` | UART reliability test that sends 1000 messages and measures communication success |
| `esp32_uart_receiver.ino` | ESP32 UART receiver, CSV parser and JSON acknowledgement |
| `wiring_diagram.txt` | UART wiring reference |
| `demo/` | Demonstration video showing successful two-way UART communication |
| `README.md` | Session documentation |

---

## Bonus Tasks

### Error Handling

If the received UART message does not contain exactly 18 joint angles, the ESP32 returns:

```json
{"status":"ERROR","reason":"bad_format"}
```

### UART Reliability Test

A stress test was performed by transmitting **1000** UART messages from the Raspberry Pi to the ESP32.

Result:

- Messages Sent: **1000**
- Messages Received: **1000**
- Success Rate: **100%**

### Demonstration

A video recording of the successful two-way UART communication is included in the `demo/` directory.

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
