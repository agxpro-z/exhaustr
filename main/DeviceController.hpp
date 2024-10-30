#pragma once

#include "BuiltinLED.hpp"
#include "CPUFan.hpp"
#include "CurrentSensor.hpp"
#include "DHTSensor.hpp"
#include "GasSensor.hpp"
#include "VoltageSensor.hpp"

class DeviceController {
 private:
  // Singleton instance
  static DeviceController* deviceController;

  // Device components
  BuiltInLED* mBuiltinLED;
  CPUFan* mCPUFan;
  CurrentSensor* mCurrentSensor;
  DHTSensor* mDHTSensor1;
  DHTSensor* mDHTSensor2;
  GasSensor* mGasSensor;
  VoltageSensor* mVoltageSensor;

  DeviceController();

 public:
  static DeviceController* getInstance() {
    if (deviceController == nullptr) {
      deviceController = new DeviceController();
    }
    return deviceController;
  }

  ~DeviceController();

  BuiltInLED& getBuiltInLED() {
    return *this->mBuiltinLED;
  }

  CPUFan& getCPUFan() {
    return *this->mCPUFan;
  }

  CurrentSensor& getCurrentSensor() {
    return *this->mCurrentSensor;
  }

  DHTSensor& getDHTSensor1() {
    return *this->mDHTSensor1;
  }

  DHTSensor& getDHTSensor2() {
    return *this->mDHTSensor2;
  }

  GasSensor& getGasSensor() {
    return *this->mGasSensor;
  }

  VoltageSensor& getVoltageSensor() {
    return *this->mVoltageSensor;
  }
};
