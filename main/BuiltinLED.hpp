#pragma once

#include <vector>

#include "driver/gpio.h"

#include "Switch.hpp"

class BuiltInLED {
 private:
  const gpio_num_t LED_PIN = GPIO_NUM_2;
  Switch ledSwitch = Switch(LED_PIN, "Built-in LED");
  std::vector<bool> blinkPattern = {1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1};

 public:
  void on();
  void off();
  void blink(int ms);
};
