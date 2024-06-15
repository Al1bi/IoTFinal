#include "Dispenser.h"

Dispenser::Dispenser(WeightSensor* cupWeightSensor, WeightSensor* tankWeightSensor, WaterPump* waterPump, Led* led){
  this->cupWeightSensor = cupWeightSensor;
  this->tankWeightSensor = tankWeightSensor;
  this->waterPump = waterPump;
  this->led = led;
}

Dispenser::~Dispenser(){
  delete cupWeightSensor;
  delete tankWeightSensor;
  delete waterPump;
  delete led;
}

float Dispenser::senseWeightOnTheTray(int quantityOfSenses = 1){
  return cupWeightSensor->senseWeightInGrams(quantityOfSenses);
}

float Dispenser::senseWeightInTheTank(int quantityOfSenses = 1){
  return tankWeightSensor->senseWeightInGrams(quantityOfSenses);
}

void Dispenser::turnOnWaterPump(){
  waterPump->turnOn();
}

void Dispenser::turnOffWaterPump(){
  waterPump->turnOff();
}

const char* Dispenser::getWaterPumpState(){
  return waterPump->getState();
}

void Dispenser::turnOnLed(){
  led->turnOn();
}

void Dispenser::turnOffLed(){
  led->turnOff();
}
