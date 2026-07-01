#include <HardwareSerial.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

HardwareSerial RpiSerial(1);

#define RXD_RPI 4
#define TXD_RPI 5

#define I2C_SDA 2
#define I2C_SCL 3

#define PCA9685_ADDR 0x40

#define SERVO_MIN_US 500
#define SERVO_MAX_US 2500
#define SERVO_FREQ 50

#define ACTIVE_SERVO_CHANNELS 16
#define TOTAL_JOINTS 18

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PCA9685_ADDR);

float g_joints[TOTAL_JOINTS] = {
  90,90,90,90,90,90,
  90,90,90,90,90,90,
  90,90,90,90,90,90
};

unsigned long lastStatusTime = 0;
unsigned long lastI2CCheckTime = 0;
uint16_t statusSeq = 1000;
bool pcaFound = false;

int angleToPulse(float angle) {
  angle = constrain(angle, 0, 180);
  int us = map((int)angle, 0, 180, SERVO_MIN_US, SERVO_MAX_US);
  return (us * 4096L) / 20000L;
}

bool checkI2CAddress(byte address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}

void scanI2C() {
  byte count = 0;

  Serial.println();
  Serial.println("===== I2C SCAN START =====");

  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found: 0x");
      if (addr < 16) Serial.print("0");
      Serial.println(addr, HEX);
      count++;
    }
  }

  Serial.print("I2C devices count: ");
  Serial.println(count);
  Serial.println("===== I2C SCAN END =====");
  Serial.println();
}

void applyJointsToServos(float joints[TOTAL_JOINTS]) {
  if (!pcaFound) {
    Serial.println("OUTPUT: PCA9685 not found, servos not updated");
    return;
  }

  Serial.println("OUTPUT: applying joints to PCA9685");

  for (int i = 0; i < ACTIVE_SERVO_CHANNELS; i++) {
    int pulse = angleToPulse(joints[i]);
    pwm.setPWM(i, 0, pulse);

    Serial.print("OUTPUT: channel ");
    Serial.print(i);
    Serial.print(" angle=");
    Serial.print(joints[i]);
    Serial.print(" pulse=");
    Serial.println(pulse);
  }

  Serial.println("OUTPUT: joints 16 and 17 received but not physically applied");
}

void sendACK(uint16_t seq) {
  StaticJsonDocument<160> doc;
  doc["type"] = "ACK";
  doc["seq"] = seq;
  doc["ts"] = millis();
  doc["data"]["status"] = "OK";
  doc["data"]["applied"] = ACTIVE_SERVO_CHANNELS;
  doc["data"]["received"] = TOTAL_JOINTS;
  doc["data"]["pca9685"] = pcaFound ? "OK" : "NOT_FOUND";

  String out;
  serializeJson(doc, out);
  RpiSerial.println(out);
  Serial.print("UART OUT: ");
  Serial.println(out);
}

void sendERR(uint16_t seq, const char* code, const char* msg) {
  StaticJsonDocument<192> doc;
  doc["type"] = "ERR";
  doc["seq"] = seq;
  doc["ts"] = millis();
  doc["data"]["code"] = code;
  doc["data"]["msg"] = msg;

  String out;
  serializeJson(doc, out);
  RpiSerial.println(out);
  Serial.print("UART OUT ERR: ");
  Serial.println(out);
}

void sendSTATUS() {
  StaticJsonDocument<320> doc;
  doc["type"] = "STATUS";
  doc["seq"] = statusSeq++;
  doc["ts"] = millis();
  doc["data"]["temp"] = 24.5;
  doc["data"]["hum"] = 60.0;
  doc["data"]["dist"] = 30.0;
  doc["data"]["battery"] = 87;
  doc["data"]["uptime"] = millis();
  doc["data"]["active_servos"] = ACTIVE_SERVO_CHANNELS;
  doc["data"]["total_joints"] = TOTAL_JOINTS;
  doc["data"]["pca9685_found"] = pcaFound;
  doc["data"]["i2c_sda"] = I2C_SDA;
  doc["data"]["i2c_scl"] = I2C_SCL;

  String out;
  serializeJson(doc, out);
  RpiSerial.println(out);
  Serial.print("UART OUT STATUS: ");
  Serial.println(out);
}

void handleFrame(String raw) {
  raw.trim();

  Serial.print("UART IN: ");
  Serial.println(raw);

  StaticJsonDocument<512> doc;
  DeserializationError err = deserializeJson(doc, raw);

  if (err) {
    sendERR(0, "BAD_JSON", err.c_str());
    return;
  }

  const char* type = doc["type"];
  uint16_t seq = doc["seq"] | 0;

  if (!type || strcmp(type, "CMD") != 0) {
    sendERR(seq, "BAD_TYPE", "Expected CMD");
    return;
  }

  JsonArray joints = doc["data"]["joints"];

  if (joints.size() != TOTAL_JOINTS) {
    sendERR(seq, "BAD_SIZE", "Need 18 joints");
    return;
  }

  for (int i = 0; i < TOTAL_JOINTS; i++) {
    g_joints[i] = joints[i].as<float>();
  }

  Serial.print("OUTPUT: CMD seq=");
  Serial.print(seq);
  Serial.println(" parsed successfully");

  applyJointsToServos(g_joints);
  sendACK(seq);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("ESP32-C5 JSON UART + PCA9685 starting");
  Serial.println("UART: RX GPIO4, TX GPIO5");
  Serial.println("I2C: SDA GPIO2, SCL GPIO3");

  RpiSerial.begin(115200, SERIAL_8N1, RXD_RPI, TXD_RPI);

  Wire.begin(I2C_SDA, I2C_SCL);

  scanI2C();

  pcaFound = checkI2CAddress(PCA9685_ADDR);

  if (pcaFound) {
    Serial.println("OUTPUT: PCA9685 found at 0x40");
    pwm.begin();
    pwm.setPWMFreq(SERVO_FREQ);
    delay(500);
    applyJointsToServos(g_joints);
  } else {
    Serial.println("OUTPUT: PCA9685 NOT FOUND at 0x40");
  }

  Serial.println("ESP32-C5 JSON UART + PCA9685 ready");
}

void loop() {
  if (RpiSerial.available()) {
    String raw = RpiSerial.readStringUntil('\n');
    handleFrame(raw);
  }

  if (millis() - lastI2CCheckTime >= 5000) {
    lastI2CCheckTime = millis();
    pcaFound = checkI2CAddress(PCA9685_ADDR);

    Serial.print("OUTPUT: I2C PCA9685 0x40 = ");
    Serial.println(pcaFound ? "OK" : "NOT_FOUND");
  }

  if (millis() - lastStatusTime >= 2000) {
    lastStatusTime = millis();
    sendSTATUS();
  }
}