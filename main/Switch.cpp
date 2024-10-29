#include <string>

#include "driver/gpio.h"
#include "esp_log.h"

#include "Switch.hpp"

Switch::Switch(gpio_num_t pin, std::string name) : pin(pin), name(name) {
  gpio_reset_pin(pin);
  gpio_set_direction(pin, GPIO_MODE_OUTPUT);
}

bool Switch::isOn() {
  return gpio_get_level(pin);
}

void Switch::on() {
  gpio_set_level(pin, 1);
  ESP_LOGI("Switch", "%s is on", name.c_str());
}

void Switch::off() {
  gpio_set_level(pin, 0);
  ESP_LOGI("Switch", "%s is off", name.c_str());
}
