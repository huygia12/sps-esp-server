#ifndef SPS_Gate_H
#define SPS_Gate_H

#include <Servo.h>

class SPS_Gate {
public:
  /**
   * @param   servoPin    pin for servo
   * @param   delayInMs   gate opening/closing speed
   */
  SPS_Gate(int servoPin, int delayInMs);

  /**
   * @param   servoPin    pin for servo
   * @param   delayInMs   gate opening/closing speed
   * @param   maxDegree   max degree servo can turn
   * @param   minDegree   min degree servo can turn
   */
  SPS_Gate(int servoPin, int delayInMs, int maxDegree, int minDegree,
           int speed);

  /**
   * setup pin for servo, must be used before other functions
   */
  void init();

  /**
   * open the gate from current position to MAX_DEGREE
   */
  void open();

  /**
   * close the gate from current position to MIN_DEGREE
   */
  void close();

private:
  const int MAX_DEGREE = 90;
  const int MIN_DEGREE = 0;
  const int SPEED = 90;
  int servoPin;
  Servo servo;
  int delayInMs;
  int preMovementTime;
};

#endif