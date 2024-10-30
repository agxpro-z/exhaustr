#pragma once

#include <string>
#include <vector>

#include "driver/gpio.h"

class Switch {
 private:
  gpio_num_t pin;
  std::string name;
  bool state;

 public:
  Switch(gpio_num_t pin, std::string name);
  bool isOn();
  void on();
  void off();
  void pattern(std::vector<bool> pattern, int delay);
};
