#ifndef WATERPUMP_H
#define WATERPUMP_H

#include <Arduino.h>

class WaterPump {
private:
    const char* state;
    byte controlPinA;
    byte controlPinB;

public:
    WaterPump(byte controlPinA, byte controlPinB);
    ~WaterPump();
    void turnOn();
    void turnOff();
    const char* getState();
};

#endif