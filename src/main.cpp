#include <Arduino.h>

const int chargePin = GPIO_NUM_32;
const int measurePin = GPIO_NUM_33;

unsigned long pulsInTimeBegin;
unsigned long pulsInTimeEnd;
bool newPulseDurationAvailable = false;

unsigned long pwmRead = 0;
int pwmValue = 0;

// Interrupt for non-blocking PWM reading
void measurePwm() {
  pulsInTimeEnd = micros();
  newPulseDurationAvailable = true;
  digitalWrite(chargePin, LOW);
};

void setup() {
  pinMode(chargePin, OUTPUT);
  pinMode(measurePin, INPUT);

  Serial.begin(115200);

  // // Interrupt for non-blocking PWM reading
  attachInterrupt(digitalPinToInterrupt(measurePin), measurePwm, RISING);
  digitalWrite(chargePin, HIGH);
  Serial.println("Device has been initialized");
}

void loop() {

  if (newPulseDurationAvailable) {
    newPulseDurationAvailable = false;
    pwmRead = pulsInTimeEnd - pulsInTimeBegin;

    digitalWrite(chargePin, HIGH);
    pulsInTimeBegin = micros();

    Serial.print("New PWM read: ");
    Serial.print(pwmRead);
    Serial.println(" us");
    delay(1);
  }
}
