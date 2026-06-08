#include "arduino_secrets.h"
#include "thingProperties.h"

#define TRIG_PIN 4
#define ECHO_PIN 5
#define LED_PIN 3

void setup() {
  Serial.begin(115200);
  delay(1500);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  long duration = readDuration();
  durationUs = duration;

  if (duration == 0) {
    distanceCm = -1;
    ledState = false;
    digitalWrite(LED_PIN, LOW);
  } else {
    float distance = duration * 0.0343 / 2;
    distanceCm = distance;

    if (distance < 20) {
      digitalWrite(LED_PIN, HIGH);
      ledState = true;
    } else {
      digitalWrite(LED_PIN, LOW);
      ledState = false;
    }
  }

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  delay(1000);
}

long readDuration() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  return pulseIn(ECHO_PIN, HIGH, 30000);
}