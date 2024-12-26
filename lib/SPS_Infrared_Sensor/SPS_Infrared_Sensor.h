#ifndef SPS_INFRARED_SENSOR_H
#define SPS_INFRARED_SENSOR_H

class SPS_InfraredSensor {
public:
  /**
   * Manage all the infrared sensor in the
   * @param   irCar1      digital port in arduino uno that used by slot 1
   * @param   irCar2      digital port in arduino uno that used by slot 2
   * @param   irCar3      digital port in arduino uno that used by slot 3
   * @param   irCar4      digital port in arduino uno that used by slot 4
   * @param   irCar5      digital port in arduino uno that used by slot 5
   * @param   irCar6      digital port in arduino uno that used by slot 6
   * @param   irEnter    digital port in arduino uno that used by entrance gate
   * @param   irExit     digital port in arduino uno that used by exit gate
   */
  SPS_InfraredSensor(int irCar1, int irCar2, int irCar3, int irCar4, int irCar5,
                     int irCar6, int irEntryFront, int irEntryBack,
                     int irExitFront, int irExitBack);

  /**
   * Setup all infrared port connections. Must be called before using other
   * functions
   */
  void init();

  /**
   * Return true if parking sensor of slot i is filled, else return false
   */
  bool isParkingSensorDetected(int i);

  /**
   * Return true if the front sensor of the entry gate detects something, else
   * return false
   */
  bool isEntryFrontSensorDetected();

  /**
   * Return true if the back sensor of the entry gate detects something, else
   * return false
   */
  bool isEntryBackSensorDetected();

  /**
   * Return true if the front sensor of the exit gate detects something, else
   * return false
   */
  bool isExitFrontSensorDetected();

  /**
   * Return true if the back sensor of the exit gate detects something, else
   * return false
   */
  bool isExitBackSensorDetected();

private:
  const int TOTAL_PARKING_SLOTS = 6;
  int parkingSensors[6];

  int entryFrontSensor;
  int entryBackSensor;

  int exitFrontSensor;
  int exitBackSensor;
};

#endif