#include <string>

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "Switch.hpp"

Switch::Switch(gpio_num_t pin, std::string name) : pin(pin), name(name), state(false) {
  gpio_reset_pin(pin);
  gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

bool Switch::isOn() {
  return this->state;
}

void Switch::on() {
  gpio_set_level(pin, 1);
  this->state = true;
  ESP_LOGI("Switch", "%s is on", name.c_str());
}

void Switch::off() {
  gpio_set_level(pin, 0);
  this->state = false;
  ESP_LOGI("Switch", "%s is off", name.c_str());
}

void Switch::pattern(std::vector<bool> pattern, int delay) {
  for (bool state : pattern) {
    if (state) {
      gpio_set_level(pin, 1);
    } else {
      gpio_set_level(pin, 0);
    }
    vTaskDelay(delay / portTICK_PERIOD_MS);
  }
}
