#include <HardwareSerial.h>

HardwareSerial RpiSerial(1);

#define RXD_RPI 4
#define TXD_RPI 5

void setup() {
  Serial.begin(115200);
  RpiSerial.begin(115200, SERIAL_8N1, RXD_RPI, TXD_RPI);
  Serial.println("ESP32-C5 UART ready. Waiting for RPi...");
}

void loop() {
  if (RpiSerial.available()) {
    String msg = RpiSerial.readStringUntil('\n');
    msg.trim();

    Serial.println("Received:");
    Serial.println(msg);

    float angles[18];
    int index = 0;

    char buffer[128];
    msg.toCharArray(buffer, sizeof(buffer));

    char *token = strtok(buffer, ",");

    while (token != NULL && index < 18) {
      angles[index] = atof(token);
      token = strtok(NULL, ",");
      index++;
    }

    Serial.println("Parsed angles:");

    for (int i = 0; i < index; i++) {
      Serial.print("Angle ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(angles[i]);
    }

    if (index == 18) {
      RpiSerial.println("{\"received\":18,\"status\":\"OK\"}");
    } else {
      RpiSerial.println("{\"status\":\"ERROR\",\"reason\":\"bad_format\"}");
    }
  }
}
