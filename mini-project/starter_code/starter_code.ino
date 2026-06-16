#include <DHT.h>

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

SecurityState state = STATE_NORMAL;
SecurityState lastState = STATE_NORMAL;

float temperature = 0;
float distanceCm = 0;
int alarmCount = 0;

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
  bool distAlert = distanceCm > 0 && distanceCm < 30;
  bool distWarn  = distanceCm >= 30 && distanceCm <= 100;

  bool tempAlert = temperature > 35;
  bool tempWarn  = temperature >= 32 && temperature <= 35;

  lastState = state;

  if (distAlert || tempAlert) {
    state = STATE_ALERT;
  } else if (distWarn || tempWarn) {
    state = STATE_WARNING;
  } else {
    state = STATE_NORMAL;
  }

  if (state == STATE_ALERT && lastState != STATE_ALERT) {
    alarmCount++;
  }
}

void updateLeds() {
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);

  if (state == STATE_NORMAL) {
    digitalWrite(GREEN, HIGH);
  } else if (state == STATE_WARNING) {
    digitalWrite(YELLOW, HIGH);
  } else if (state == STATE_ALERT) {
    digitalWrite(RED, HIGH);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);

  dht.begin();
}

void loop() {
  temperature = dht.readTemperature();
  distanceCm = readDistance();

  if (isnan(temperature)) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, HIGH);
    delay(300);
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    delay(300);
    return;
  }

  evaluateState();
  updateLeds();

  delay(300);
}