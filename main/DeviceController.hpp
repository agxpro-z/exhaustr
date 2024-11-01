#pragma once

#include "BuiltinLED.hpp"
#include "CPUFan.hpp"
#include "CurrentSensor.hpp"
#include "DHTSensor.hpp"
#include "GasSensor.hpp"
#include "VoltageSensor.hpp"

class DeviceController {
 public:
  enum Mode {
    AUTO,
    MANUAL
  };

 private:
  // Singleton instance
  static DeviceController* deviceController;

  // Device mode
  Mode mode = Mode::AUTO;

  // Device components
  BuiltInLED* builtinLED;
  CPUFan* cpuFan;
  CurrentSensor* currentSensor;
  DHTSensor* dhtSensor1;
  DHTSensor* dhtSensor2;
  GasSensor* gasSensor;
  VoltageSensor* voltageSensor;

  DeviceController();

 public:
  static DeviceController* getInstance();
  ~DeviceController();

  Mode getMode();
  void setMode(Mode mode);
  BuiltInLED& getBuiltInLED();
  CPUFan& getCPUFan();
  CurrentSensor& getCurrentSensor();
  DHTSensor& getDHTSensor1();
  DHTSensor& getDHTSensor2();
  GasSensor& getGasSensor();
  VoltageSensor& getVoltageSensor();
};
