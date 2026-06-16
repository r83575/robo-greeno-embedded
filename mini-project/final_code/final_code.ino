#include "arduino_secrets.h"
#include "thingProperties.h"

#include <DHT.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>
// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEAdvertising.h>

// #define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
// #define CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// BLECharacteristic *pCharacteristic;

volatile bool wifiOk = false;
const char* SERVER = "http://postman-echo.com/post";

#define DHTPIN 4
#define DHTTYPE DHT22

#define TRIG_PIN 5
#define ECHO_PIN 8

#define GREEN 13
#define YELLOW 7
#define RED 10


DHT dht(DHTPIN, DHTTYPE);

enum SecurityState {
  STATE_NORMAL = 0,
  STATE_WARNING = 1,
  STATE_ALERT = 2
};


volatile SecurityState g_state = STATE_NORMAL;
volatile SecurityState g_lastState = STATE_NORMAL;

volatile float g_temperature = 0;
volatile float g_distanceCm = 0;
volatile int g_alarmCount = 0;
volatile bool g_alertJustStarted = false;

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.034 / 2.0;
}

void evaluateState() {
  bool distAlert = g_distanceCm > 0 && g_distanceCm < 30;
  bool distWarn  = g_distanceCm >= 30 && g_distanceCm <= 100;

  bool tempAlert = g_temperature > 35;
  bool tempWarn  = g_temperature >= 32 && g_temperature <= 35;

  g_lastState = g_state;

  if (distAlert || tempAlert) {
    g_state = STATE_ALERT;
  } else if (distWarn || tempWarn) {
    g_state = STATE_WARNING;
  } else {
    g_state = STATE_NORMAL;
  }

  if (g_state == STATE_ALERT && g_lastState != STATE_ALERT) {
    g_alarmCount++;
    g_alertJustStarted = true;
  }
}

void updateLeds() {
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);

  if (g_state == STATE_NORMAL) {
    digitalWrite(GREEN, HIGH);
  } else if (g_state == STATE_WARNING) {
    digitalWrite(YELLOW, HIGH);
  } else if (g_state == STATE_ALERT) {
    digitalWrite(RED, HIGH);
  }
}

void sensorTask(void *pvParameters);
void alertTask(void *pvParameters);
void displayTask(void *pvParameters);
// void bleTask(void *pvParameters);
void cloudTask(void *pvParameters);
void wifiTask(void *pvParameters);

void setup() {
  Serial.begin(115200);
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);

  dht.begin();

  // BLEDevice::init("SecurityMonitor-ESP32");

  // BLEServer *pServer = BLEDevice::createServer();
  // BLEService *pService = pServer->createService(SERVICE_UUID);

  // pCharacteristic = pService->createCharacteristic(
  //   CHAR_UUID,
  //   BLECharacteristic::PROPERTY_READ |
  //   BLECharacteristic::PROPERTY_NOTIFY
  // );

  // pCharacteristic->setValue("Starting...");
  // pService->start();

  // BLEAdvertising *pAdv = BLEDevice::getAdvertising();
  // pAdv->addServiceUUID(SERVICE_UUID);
  // pAdv->setScanResponse(true);

  // BLEDevice::startAdvertising();

  // Serial.println("BLE advertising started!");

  xTaskCreate(sensorTask, "sensorTask", 3072, NULL, 1, NULL);
  xTaskCreate(alertTask, "alertTask", 2048, NULL, 3, NULL);
  xTaskCreate(displayTask, "displayTask", 2048, NULL, 2, NULL);
  // xTaskCreate(bleTask, "bleTask", 4096, NULL, 1, NULL);
  xTaskCreate(cloudTask, "cloudTask", 8192, NULL, 1, NULL);
  xTaskCreate(wifiTask, "wifiTask", 4096, NULL, 1, NULL);
}

void loop() {
  ArduinoCloud.update();
  delay(100);
}

void sensorTask(void *pvParameters) {
  while (true) {
    g_temperature = dht.readTemperature();
    g_distanceCm = readDistance();

    if (!isnan(g_temperature)) {
      evaluateState();
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void alertTask(void *pvParameters) {
  while (true) {
    updateLeds();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void displayTask(void *pvParameters) {
  while (true) {
    Serial.print("Temp: ");
    Serial.print(g_temperature);
    Serial.print(" C | Distance: ");
    Serial.print(g_distanceCm);
    Serial.print(" cm | State: ");
    Serial.print((int)g_state);
    Serial.print(" | Alarm Count: ");
    Serial.println(g_alarmCount);

    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// void bleTask(void *pvParameters) {
//   while (true) {
//     String payload = String((int)g_state) +
//                      ",T:" + String(g_temperature, 1) +
//                      ",D:" + String(g_distanceCm, 1) +
//                      ",A:" + String(g_alarmCount);

//     pCharacteristic->setValue(payload.c_str());
//     pCharacteristic->notify();

//     Serial.print("BLE: ");
//     Serial.println(payload);

//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//   }
// }

void cloudTask(void *pvParameters) {
  while (true) {

    wifiOk = (WiFi.status() == WL_CONNECTED);

    temperature = g_temperature;
    distanceCm = g_distanceCm;
    alarmCount = g_alarmCount;
    state = (int)g_state;
    systemOnline = wifiOk;

    if (wifiOk && g_alertJustStarted) {
      g_alertJustStarted = false;

      StaticJsonDocument<256> doc;
      doc["temperature"] = g_temperature;
      doc["distance"] = g_distanceCm;
      doc["alarmCount"] = g_alarmCount;
      doc["state"] = (int)g_state;

      String json;
      serializeJson(doc, json);

      WiFiClient client;
      HttpClient http(client, "postman-echo.com", 80);

      http.beginRequest();
      http.post("/post");
      http.sendHeader("Content-Type", "application/json");
      http.sendHeader("Content-Length", json.length());
      http.beginBody();
      http.print(json);
      http.endRequest();

      int httpCode = http.responseStatusCode();

      Serial.print("HTTP Response: ");
      Serial.println(httpCode);
    }

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void wifiTask(void *pvParameters) {
  while (true) {
    wifiOk = (WiFi.status() == WL_CONNECTED);

    Serial.print("WiFi: ");
    Serial.println(wifiOk ? "CONNECTED" : "DISCONNECTED");

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

/*
  Since AlarmCount is READ_WRITE variable, onAlarmCountChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onAlarmCountChange()  {
  // Add your code here to act upon AlarmCount change
}
/*
  Since DistanceCm is READ_WRITE variable, onDistanceCmChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onDistanceCmChange()  {
  // Add your code here to act upon DistanceCm change
}
/*
  Since Temperature is READ_WRITE variable, onTemperatureChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTemperatureChange()  {
  // Add your code here to act upon Temperature change
}
/*
  Since State is READ_WRITE variable, onStateChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onStateChange()  {
  // Add your code here to act upon State change
}