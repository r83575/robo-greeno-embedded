#include <Arduino.h>

HardwareSerial mySerial(2);

void setup() {

  Serial.begin(115200);

  mySerial.begin(9600, SERIAL_8N1, 16, 17);

  Serial.println("Loopback Test Started");
}

void loop() {

  mySerial.println("HELLO");

  delay(1000);

  while (mySerial.available()) {

    char c = mySerial.read();

    Serial.print(c);
  }
}