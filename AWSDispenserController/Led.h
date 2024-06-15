#include <Arduino.h>

class Led {
private:
    byte ledPin;

public:
    Led(byte pinLed);
    ~Led();
    void turnOn();
    void turnOff();
    void blinkWithFrequency(float seconds);
};
