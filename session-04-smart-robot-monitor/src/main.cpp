#include <Arduino.h>
#include <DHTesp.h>

#define DHT_PIN 15

#define TRIG_PIN 5
#define ECHO_PIN 6
#define LED_PIN 7

DHTesp dht;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2.0;
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  dht.setup(DHT_PIN, DHTesp::DHT22);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Smart Robot Status Monitor Started");
}

void loop() {
  TempAndHumidity data = dht.getTempAndHumidity();
  float distance = readDistanceCm();

  bool danger = distance > 0 && distance < 20;
  digitalWrite(LED_PIN, danger ? HIGH : LOW);

  Serial.println("----- Robot Status -----");

  Serial.print("Temperature: ");
  Serial.print(data.temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(data.humidity);
  Serial.println(" %");

  Serial.print("Distance: ");
  if (distance < 0) {
    Serial.println("Out of range");
  } else {
    Serial.print(distance);
    Serial.println(" cm");
  }

  Serial.print("Status: ");
  Serial.println(danger ? "DANGER" : "SAFE");

  delay(2000);
}