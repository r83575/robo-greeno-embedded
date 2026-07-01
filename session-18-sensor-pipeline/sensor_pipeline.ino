#include <HardwareSerial.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "DHTesp.h"

HardwareSerial RpiSerial(1);

#define RXD_RPI 4
#define TXD_RPI 5

#define I2C_SDA 2
#define I2C_SCL 3
#define PCA9685_ADDR 0x40

#define DHT_PIN 6
#define HCSR04_TRIG 7
#define HCSR04_ECHO 8

#define SERVO_MIN_US 500
#define SERVO_MAX_US 2500
#define SERVO_FREQ 50

#define ACTIVE_SERVO_CHANNELS 16
#define TOTAL_JOINTS 18

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PCA9685_ADDR);
DHTesp dht;

float g_joints[TOTAL_JOINTS] = {
  90,90,90,90,90,90,
  90,90,90,90,90,90,
  90,90,90,90,90,90
};

volatile float g_temp = 0;
volatile float g_hum = 0;
volatile float g_dist = 0;

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

float readDistanceCm() {
  digitalWrite(HCSR04_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(HCSR04_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(HCSR04_TRIG, LOW);

  long duration = pulseIn(HCSR04_ECHO, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return duration / 58.0;
}

void applyJointsToServos(float joints[TOTAL_JOINTS]) {
  if (!pcaFound) {
    Serial.println("OUTPUT: PCA9685 not found, servos not updated");
    return;
  }

  for (int i = 0; i < ACTIVE_SERVO_CHANNELS; i++) {
    int pulse = angleToPulse(joints[i]);
    pwm.setPWM(i, 0, pulse);
  }
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
  Serial.print("UART OUT ACK: ");
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
  doc["data"]["temp"] = g_temp;
  doc["data"]["hum"] = g_hum;
  doc["data"]["dist"] = g_dist;
  doc["data"]["battery"] = 85;
  doc["data"]["uptime"] = millis();
  doc["data"]["active_servos"] = ACTIVE_SERVO_CHANNELS;
  doc["data"]["total_joints"] = TOTAL_JOINTS;
  doc["data"]["pca9685_found"] = pcaFound;

  String out;
  serializeJson(doc, out);
  RpiSerial.println(out);
  Serial.print("UART OUT STATUS: ");
  Serial.println(out);
}

void handleFrame(String raw) {
  raw.trim();

  if (raw.length() == 0) {
    return;
  }

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

  applyJointsToServos(g_joints);
  sendACK(seq);
}

void sensorTask(void *p) {
  while (true) {
    TempAndHumidity data = dht.getTempAndHumidity();

    if (!isnan(data.temperature)) {
      g_temp = data.temperature;
      g_hum = data.humidity;
    }

    float dist = readDistanceCm();
    if (dist >= 0) {
      g_dist = dist;
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

void statusTask(void *p) {
  while (true) {
    sendSTATUS();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void uartTask(void *p) {
  while (true) {
    if (RpiSerial.available()) {
      String raw = RpiSerial.readStringUntil('\n');
      handleFrame(raw);
    }

    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

void i2cTask(void *p) {
  while (true) {
    pcaFound = checkI2CAddress(PCA9685_ADDR);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Session 18 - ESP32 Sensor Pipeline");

  RpiSerial.begin(115200, SERIAL_8N1, RXD_RPI, TXD_RPI);
  RpiSerial.setTimeout(50);

  Wire.begin(I2C_SDA, I2C_SCL);

  pcaFound = checkI2CAddress(PCA9685_ADDR);

  if (pcaFound) {
    pwm.begin();
    pwm.setPWMFreq(SERVO_FREQ);
    delay(500);
    applyJointsToServos(g_joints);
    Serial.println("PCA9685 found");
  } else {
    Serial.println("PCA9685 not found");
  }

  dht.setup(DHT_PIN, DHTesp::DHT22);

  pinMode(HCSR04_TRIG, OUTPUT);
  pinMode(HCSR04_ECHO, INPUT);

  xTaskCreate(sensorTask, "SENSOR", 4096, NULL, 1, NULL);
  xTaskCreate(statusTask, "STATUS", 4096, NULL, 1, NULL);
  xTaskCreate(uartTask, "UART", 4096, NULL, 3, NULL);
  xTaskCreate(i2cTask, "I2C", 2048, NULL, 1, NULL);

  Serial.println("Session 18 ready");
}

void loop() {
}