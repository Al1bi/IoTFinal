#include <Arduino.h>

class UltrasonicSensor {
private:
    byte triggerPin;
    byte echoPin;

public:
    UltrasonicSensor(byte triggerPin, byte echoPin);
    ~UltrasonicSensor();
    void dischargeTriggerElectricity();
    void excitePiezoElectric();
    float readDistancInCentimeters();
};
