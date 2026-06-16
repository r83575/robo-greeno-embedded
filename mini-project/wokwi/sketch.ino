#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";
const char* SERVER = "http://httpbin.org/post";

volatile bool g_wifi_ok = false;

#define DHTPIN 4
#define DHTTYPE DHT22

#define TRIG_PIN 5
#define ECHO_PIN 18

#define LED_GREEN 13
#define LED_YELLOW 27
#define LED_RED 2

#define BUZZER 26

DHT dht(DHTPIN, DHTTYPE);

typedef enum {
  STATE_NORMAL = 0,
  STATE_WARNING = 1,
  STATE_ALERT = 2
} SecurityState;

volatile SecurityState g_state = STATE_NORMAL;
volatile float g_temperature = 0.0;
volatile float g_distance = 0.0;
volatile int g_alarm_count = 0;

void sensorTask(void *pvParameters);
void alertTask(void *pvParameters);
void displayTask(void *pvParameters);
void bleTask(void *pvParameters);
void wifiTask(void *pvParameters);

float readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.034 / 2;
}

void evaluateState() {
  bool distAlert = (g_distance > 0 && g_distance < 30.0);
  bool distWarn = (g_distance > 0 && g_distance < 100.0);

  bool tempAlert = (g_temperature > 35.0);
  bool tempWarn = (g_temperature > 32.0);

  if (distAlert || tempAlert) {
    if (g_state != STATE_ALERT) {
      g_alarm_count++;
    }
    g_state = STATE_ALERT;
  } else if (distWarn || tempWarn) {
    g_state = STATE_WARNING;
  } else {
    g_state = STATE_NORMAL;
  }
}

void updateOutputs() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  noTone(BUZZER);

  if (g_state == STATE_NORMAL) {
    digitalWrite(LED_GREEN, HIGH);
  } else if (g_state == STATE_WARNING) {
    digitalWrite(LED_YELLOW, HIGH);
    tone(BUZZER, 1000);
  } else if (g_state == STATE_ALERT) {
    digitalWrite(LED_RED, HIGH);
    tone(BUZZER, 2000);
  }
}

const char* stateToText(SecurityState state) {
  switch (state) {
    case STATE_NORMAL:
      return "NORMAL";
    case STATE_WARNING:
      return "WARNING";
    case STATE_ALERT:
      return "ALERT";
    default:
      return "UNKNOWN";
  }
}

String buildPayload() {
  return String((int)g_state) +
         ",T:" + String(g_temperature, 1) +
         ",D:" + String(g_distance, 1) +
         ",A:" + String(g_alarm_count);
}

void setup() {
  Serial.begin(115200);

  dht.begin();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  pinMode(BUZZER, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);

  xTaskCreate(sensorTask, "Sensor Task", 3072, NULL, 1, NULL);
  xTaskCreate(alertTask, "Alert Task", 2048, NULL, 3, NULL);
  xTaskCreate(displayTask, "Display Task", 2048, NULL, 2, NULL);
  xTaskCreate(bleTask, "BLE SIM Task", 2048, NULL, 1, NULL);
  xTaskCreate(wifiTask, "WiFi Task", 8192, NULL, 1, NULL);
}

void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void sensorTask(void *pvParameters) {
  while (true) {
    float temp = dht.readTemperature();

    if (!isnan(temp)) {
      g_temperature = temp;
    }

    g_distance = readDistanceCM();

    evaluateState();

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void alertTask(void *pvParameters) {
  while (true) {
    updateOutputs();

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void displayTask(void *pvParameters) {
  while (true) {
    Serial.printf(
      "Temp: %.2f C | Distance: %.2f cm | State: %s | Alarm Count: %d | WiFi: %s\n",
      g_temperature,
      g_distance,
      stateToText(g_state),
      g_alarm_count,
      g_wifi_ok ? "OK" : "DOWN"
    );

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void bleTask(void *pvParameters) {
  while (true) {
    Serial.printf("BLE SIM: %s\n", buildPayload().c_str());
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void wifiTask(void *pvParameters) {
  while (true) {
    if (WiFi.status() != WL_CONNECTED) {
      g_wifi_ok = false;

      Serial.println("WiFi connecting...");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      continue;
    }

    g_wifi_ok = true;

    HTTPClient http;
    http.begin(SERVER);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<256> doc;
    doc["temperature"] = g_temperature;
    doc["distance"] = g_distance;
    doc["state"] = stateToText(g_state);
    doc["alarm_count"] = g_alarm_count;

    String json;
    serializeJson(doc, json);

    int code = http.POST(json);

    Serial.print("HTTP POST code: ");
    Serial.println(code);

    http.end();

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}