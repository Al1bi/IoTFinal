#include "Led.h"

Led::Led(byte ledPin) {
    this->ledPin = ledPin;
    pinMode(ledPin, OUTPUT);
}

Led::~Led() {
}

void Led::turnOn() {
    digitalWrite(ledPin, HIGH);
}

void Led::turnOff() {
    digitalWrite(ledPin, LOW);
}

void Led::blinkWithFrequency(float seconds) {
    float milliseconds = seconds * 1000;
    turnOn();
    delay(milliseconds);
    turnOff();
    delay(milliseconds);
}
