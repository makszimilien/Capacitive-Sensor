#include <Arduino.h>

const int ledPin = GPIO_NUM_32;
const int touchPin = GPIO_NUM_33;

// int capacitance;

unsigned long pulsInTimeBegin;
unsigned long pulsInTimeEnd;
bool newPulseDurationAvailable = false;

unsigned long pwmRead = 0;
int pwmValue = 0;

// Interrupt for non-blocking PWM reading
void pwmPinInterrupt() {
  if (digitalRead(touchPin) == HIGH) {
    pulsInTimeBegin = micros();
  } else {
    pulsInTimeEnd = micros();
    newPulseDurationAvailable = true;
  }
};

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(touchPin, INPUT);
  Serial.begin(115200);

  // Interrupt for non-blocking PWM reading
  attachInterrupt(digitalPinToInterrupt(touchPin), pwmPinInterrupt, CHANGE);
}

void loop() {
  // capacitance = (touchRead(touchPin));
  // Serial.print("Sensor value: ");
  // Serial.println(capacitance);
  // delay(10);

  if (newPulseDurationAvailable) {
    newPulseDurationAvailable = false;
    pwmRead = pulsInTimeEnd - pulsInTimeBegin;

    Serial.print("New PWM read: ");
    Serial.print(pwmRead);
    Serial.println(" ms");
  }
}
