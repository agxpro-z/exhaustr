#pragma once

#include <string>

#include "driver/gpio.h"

class Switch {
 private:
  gpio_num_t pin;
  std::string name;

 public:
  Switch(gpio_num_t pin, std::string name);
  bool isOn();
  void on();
  void off();
};
