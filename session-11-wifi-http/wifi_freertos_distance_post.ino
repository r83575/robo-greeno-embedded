#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define TRIG_PIN 4
#define ECHO_PIN 5

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

float g_distance = 0;
bool wifiStatus = false;

float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  float distance = duration * 0.034 / 2;

  return distance;
}

void sensorTask(void* parameter) {
  while (true) {
    g_distance = readDistance();

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void wifiTask(void* parameter) {
  while (true) {
    if (WiFi.status() == WL_CONNECTED) {
      wifiStatus = true;

      HTTPClient http;

      http.begin("https://postman-echo.com/post");
      http.addHeader("Content-Type", "application/json");

      StaticJsonDocument<200> doc;
      doc["distance"] = g_distance;
      doc["device"] = "ESP32";

      String body;
      serializeJson(doc, body);

      int httpCode = http.POST(body);

      Serial.print("POST Code: ");
      Serial.println(httpCode);

      http.end();
    } else {
      wifiStatus = false;
    }

    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void displayTask(void* parameter) {
  while (true) {
    Serial.print("Distance: ");
    Serial.print(g_distance);
    Serial.println(" cm");

    Serial.print("WiFi: ");
    Serial.println(wifiStatus ? "OK" : "FAIL");

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.localIP());

  xTaskCreate(sensorTask, "SENS", 2048, NULL, 1, NULL);
  xTaskCreate(wifiTask, "WIFI", 8192, NULL, 1, NULL);
  xTaskCreate(displayTask, "DISP", 2048, NULL, 2, NULL);
}

void loop() {
}

