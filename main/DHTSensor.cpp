#include <string>

#include "driver/gpio.h"
#include "dht.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "DHTSensor.hpp"

DHTSensor::DHTSensor(gpio_num_t gpioNum, dht_sensor_type_t sensorType, std::string name) {
  this->gpioNum = gpioNum;
  this->sensorType = sensorType;
  this->name = name;
  this->lastReadTime = 0;
  this->temperature = -1;
  this->humidity = -1;
  gpio_reset_pin(gpioNum);
  gpio_set_direction(gpioNum, GPIO_MODE_INPUT);
}

void DHTSensor::read() {
  if (esp_timer_get_time() - this->lastReadTime < 2'000'000) {
    ESP_LOGI("DHTSensor", "Skipping reading from %s sensor", this->name.c_str());
    return;
  }
  esp_err_t err = dht_read_float_data(this->sensorType, this->gpioNum, &this->humidity, &this->temperature);
  if (err == ESP_OK) {
    this->lastReadTime = esp_timer_get_time();
  } else {
    this->temperature = -1;
    this->humidity = -1;
    ESP_LOGE("DHTSensor", "Failed to read data from %s sensor", this->name.c_str());
  }
}

void DHTSensor::read(float* temperature, float* humidity) {
  this->read();
  *temperature = this->temperature;
  *humidity = this->humidity;
}

float DHTSensor::getTemperature() {
  this->read();
  return this->temperature;
}

float DHTSensor::getHumidity() {
  this->read();
  return this->humidity;
}
