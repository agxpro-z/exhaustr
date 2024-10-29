#include <string>

#include "driver/gpio.h"
#include "dht.h"
#include "esp_err.h"
#include "esp_log.h"

#include "DHTSensor.hpp"

DHTSensor::DHTSensor(gpio_num_t gpioNum, dht_sensor_type_t sensorType, std::string name) {
  this->gpioNum = gpioNum;
  this->sensorType = sensorType;
  this->name = name;
  gpio_reset_pin(gpioNum);
  gpio_set_direction(gpioNum, GPIO_MODE_INPUT);
}

void DHTSensor::read(float* temperature, float* humidity) {
  esp_err_t err = dht_read_float_data(this->sensorType, this->gpioNum, humidity, temperature);
  if (err != ESP_OK) {
    ESP_LOGE("DHTSensor", "Failed to read data from %s sensor", this->name.c_str());
  }
}
