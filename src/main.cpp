#include <Arduino.h>

const int chargePin = GPIO_NUM_32;
const int measurePin = GPIO_NUM_33;

unsigned long pulsInTimeBegin;
volatile unsigned long pulsInTimeEnd;
bool newPulseDurationAvailable = false;

unsigned long pulseRead = 0;
unsigned long pulseMedian = 0;

const int measurementsArraySize = 50;
unsigned long measurementsArray[measurementsArraySize];

void measurePulse() {
  pulsInTimeEnd = micros();
  newPulseDurationAvailable = true;
  digitalWrite(chargePin, LOW);
};

unsigned long calculatePulseWidht() {
  newPulseDurationAvailable = false;
  pulseRead = pulsInTimeEnd - pulsInTimeBegin;

  digitalWrite(chargePin, HIGH);
  pulsInTimeBegin = micros();
  return pulseRead;
};

void pushNewPulse(unsigned long newPulse) {
  for (int i = measurementsArraySize - 1; i > 0; i--) {
    measurementsArray[i] = measurementsArray[i - 1];
  }

  measurementsArray[0] = newPulse;
};

unsigned long calculateMedian(unsigned long array[], int size) {
  // Create a copy of the array to avoid modifying the original
  unsigned long tempArray[size];
  for (int i = 0; i < size; i++) {
    tempArray[i] = array[i];
  }

  // Perform an unoptimized bubble sort to find the median
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (tempArray[j] > tempArray[j + 1]) {
        // Swap elements if they are in the wrong order
        unsigned long temp = tempArray[j];
        tempArray[j] = tempArray[j + 1];
        tempArray[j + 1] = temp;
      }
    }
  }

  // Find the middle element(s)
  if (size % 2 == 0) {
    // If the size is even, average the two middle values
    return (tempArray[size / 2 - 1] + tempArray[size / 2]) / 2;
  } else {
    // If the size is odd, return the middle value
    return tempArray[size / 2];
  }
}

void setup() {
  pinMode(chargePin, OUTPUT);
  pinMode(measurePin, INPUT);

  Serial.begin(115200);

  attachInterrupt(digitalPinToInterrupt(measurePin), measurePulse, RISING);
  digitalWrite(chargePin, HIGH);
  Serial.println("Device has been initialized");
}

void loop() {

  if (newPulseDurationAvailable) {
    pushNewPulse(calculatePulseWidht());
    pulseMedian = calculateMedian(measurementsArray, measurementsArraySize);

    int barLength = map(pulseMedian, 40, 80, 0, 40);

    Serial.print("Pulse width: ");
    Serial.print(pulseMedian);
    Serial.print("us ");
    for (int i = 0; i <= barLength; i++) {
      Serial.print("=");
    }
    Serial.println("");
    delay(1);
  }
}
