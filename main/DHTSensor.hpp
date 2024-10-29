#pragma once

#include <string>

#include "driver/gpio.h"
#include "dht.h"
#include "esp_timer.h"

class DHTSensor {
 private:
  gpio_num_t gpioNum;
  dht_sensor_type_t sensorType;
  std::string name;
  int64_t lastReadTime;
  float temperature;
  float humidity;

  void read();

 public:
  DHTSensor(gpio_num_t gpioNum, dht_sensor_type_t sensorType, std::string name);
  void read(float *temperature, float *humidity);

  typedef dht_sensor_type_t SensorType;
};
