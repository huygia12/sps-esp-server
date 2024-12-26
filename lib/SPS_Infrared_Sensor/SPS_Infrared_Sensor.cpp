#include "SPS_Infrared_Sensor.h"
#include <Arduino.h>

#define DETECTED 0
#define NOT_DETECTED 1

SPS_InfraredSensor::SPS_InfraredSensor(int irCar1, int irCar2, int irCar3,
                                       int irCar4, int irCar5, int irCar6,
                                       int irEntryFront, int irEntryBack,
                                       int irExitFront, int irExitBack)
    : entryFrontSensor(irEntryFront), entryBackSensor(irEntryBack),
      exitFrontSensor(irExitFront),
      exitBackSensor(irExitBack), parkingSensors{irCar1, irCar2, irCar3,
                                                 irCar4, irCar5, irCar6} {}

void SPS_InfraredSensor::init() {
  pinMode(parkingSensors[0], INPUT);
  pinMode(parkingSensors[1], INPUT);
  pinMode(parkingSensors[2], INPUT);
  pinMode(parkingSensors[3], INPUT);
  pinMode(parkingSensors[4], INPUT);
  pinMode(parkingSensors[5], INPUT);
  pinMode(entryFrontSensor, INPUT);
  pinMode(entryBackSensor, INPUT);
  pinMode(exitFrontSensor, INPUT);
  pinMode(exitBackSensor, INPUT);
}

bool SPS_InfraredSensor::isParkingSensorDetected(int i) {
  if (i < 0 || i >= TOTAL_PARKING_SLOTS) {
    return false;
  }

  return digitalRead(parkingSensors[i]) == DETECTED;
}

bool SPS_InfraredSensor::isEntryFrontSensorDetected() {
  return digitalRead(entryFrontSensor) == DETECTED;
}

bool SPS_InfraredSensor::isEntryBackSensorDetected() {
  return digitalRead(entryBackSensor) == DETECTED;
}

bool SPS_InfraredSensor::isExitFrontSensorDetected() {
  return digitalRead(exitFrontSensor) == DETECTED;
}

bool SPS_InfraredSensor::isExitBackSensorDetected() {
  return digitalRead(exitBackSensor) == DETECTED;
}