
#include "WeightSensor.h"

WeightSensor::WeightSensor(byte loadcellDoutPin, byte loadcellSckPin): loadcellDoutPin(loadcellDoutPin), loadcellSckPin(loadcellSckPin){
    scale.begin(loadcellDoutPin, loadcellSckPin);
    scale.tare();
    scale.set_scale(ownFactor);
}

WeightSensor::~WeightSensor(){
}

float WeightSensor::senseWeightInGrams(int x = 1){
  return max(scale.get_units(x), 0.f);
}

