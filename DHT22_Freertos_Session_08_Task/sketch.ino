#include "DHTesp.h"

const int DHT_PIN = 4;
const int LED_PIN = 2;

DHTesp dht;
int counter = 0;

void dhtTask(void *param) {
  while (true) {
    TempAndHumidity data = dht.getTempAndHumidity();

    if (isnan(data.temperature) || isnan(data.humidity)) {
      Serial.println("ERROR: Failed to read DHT22");
    } else {
      counter++;

      float heatIndex = data.temperature + 0.33 * data.humidity - 4.0;
      bool alert = data.temperature > 30 || data.humidity > 70;

      Serial.println("--------------------------------");
      Serial.print("#"); Serial.print(counter);
      Serial.print(" | Time: "); Serial.print(millis() / 1000); Serial.println("s");

      Serial.print("Temperature: "); Serial.print(data.temperature, 1); Serial.println(" C");
      Serial.print("Humidity:    "); Serial.print(data.humidity, 1); Serial.println(" %");
      Serial.print("Heat Index:  "); Serial.print(heatIndex, 1); Serial.println(" C");

      if (alert) {
        Serial.println("WARNING: Temperature or humidity is too high!");
        digitalWrite(LED_PIN, HIGH);
      } else {
        digitalWrite(LED_PIN, LOW);
      }
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}


void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  dht.setup(DHT_PIN, DHTesp::DHT22);

  Serial.println("=== DHT22 FreeRTOS Project Started ===");

  xTaskCreate(dhtTask, "DHT Task", 4096, NULL, 1, NULL);
}

void loop() {
}