#include <Arduino.h>

const int ledPin = GPIO_NUM_32;
const int touchPin = GPIO_NUM_33;

int capacitance;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(touchPin, INPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
}

void loop() {
  capacitance = (touchRead(touchPin));
  Serial.print("Sensor value: ");
  Serial.println(capacitance);
}
