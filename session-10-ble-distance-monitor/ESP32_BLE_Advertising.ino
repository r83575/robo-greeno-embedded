#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define TRIG_PIN 4
#define ECHO_PIN 5

BLECharacteristic *pCharacteristic;

volatile float g_distanceCm = -1;

void sensorTask(void *parameter) {
  while (true) {
    long duration = readDuration();

    if (duration == 0) {
      g_distanceCm = -1;
    } else {
      g_distanceCm = duration * 0.0343 / 2;
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void bleTask(void *parameter) {
  while (true) {
    String data = "D:" + String(g_distanceCm) + "cm";

    pCharacteristic->setValue(data.c_str());
    pCharacteristic->notify();

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void displayTask(void *parameter) {
  while (true) {
    Serial.print("Distance: ");
    Serial.print(g_distanceCm);
    Serial.println(" cm");

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  BLEDevice::init("RoboGreeno-ESP32");

  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHAR_UUID,
    BLECharacteristic::PROPERTY_READ |
    BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->setValue("Starting...");
  pService->start();

  BLEAdvertising *pAdv = BLEDevice::getAdvertising();
  pAdv->addServiceUUID(SERVICE_UUID);
  pAdv->setScanResponse(true);

  BLEDevice::startAdvertising();

  Serial.println("BLE advertising started!");

  xTaskCreate(sensorTask, "SENS", 2048, NULL, 1, NULL);
  xTaskCreate(bleTask, "BLE", 4096, NULL, 1, NULL);
  xTaskCreate(displayTask, "DISP", 2048, NULL, 2, NULL);
}

void loop() {
}

long readDuration() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  return pulseIn(ECHO_PIN, HIGH, 30000);
}