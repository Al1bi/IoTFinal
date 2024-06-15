#include <Arduino.h>
#include "HX711.h"

class WeightSensor {
private:
    byte loadcellDoutPin;
    byte loadcellSckPin;
    HX711 scale;
    float ownFactor = 52008.33/209.0;

public:
    WeightSensor(byte loadcellDoutPin, byte loadcellSckPin);
    ~WeightSensor();
    float senseWeightInGrams(int);
};
