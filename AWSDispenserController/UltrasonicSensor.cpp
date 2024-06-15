#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(byte triggerPin, byte echoPin) {
    this->triggerPin = triggerPin;
    this->echoPin = echoPin;
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

UltrasonicSensor::~UltrasonicSensor() {
}

void UltrasonicSensor::dischargeTriggerElectricity() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
}

void UltrasonicSensor::excitePiezoElectric() {
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
}

float UltrasonicSensor::readDistancInCentimeters() {
    dischargeTriggerElectricity();
    excitePiezoElectric();
    float roundTripTimeInMicroSeconds = pulseIn(echoPin, HIGH);
    float distanceMeasured = 0.01725 * roundTripTimeInMicroSeconds; 
    return distanceMeasured;
}
