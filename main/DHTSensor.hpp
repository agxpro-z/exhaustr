#pragma once

#include <string>

#include "driver/gpio.h"
#include "dht.h"

class DHTSensor {
 private:
  gpio_num_t gpioNum;
  dht_sensor_type_t sensorType;
  std::string name;

 public:
  DHTSensor(gpio_num_t gpioNum, dht_sensor_type_t sensorType, std::string name);
  void read(float *temperature, float *humidity);

  typedef dht_sensor_type_t SensorType;
};
