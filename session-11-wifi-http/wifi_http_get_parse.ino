#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.print("Connecting");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  HTTPClient http;

  http.begin("http://api.open-meteo.com/v1/forecast?latitude=32&longitude=34&current_weather=true");

  int httpCode = http.GET();

  Serial.print("HTTP Code: ");
  Serial.println(httpCode);

  if (httpCode == 200) {

    String payload = http.getString();

    StaticJsonDocument<1024> doc;

    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.println("JSON Parse Failed");
    } else {

      float temp = doc["current_weather"]["temperature"];
      float wind = doc["current_weather"]["windspeed"];

      Serial.print("Current temperature: ");
      Serial.print(temp);
      Serial.println(" C");

      Serial.print("Wind speed: ");
      Serial.print(wind);
      Serial.println(" km/h");
    }
  } else {
    Serial.println("GET Request Failed");
  }

  http.end();
}

void loop() {
}