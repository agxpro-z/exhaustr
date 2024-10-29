#pragma once

#include "driver/gpio.h"

#include "Switch.hpp"

class BuiltInLED {
 private:
  const gpio_num_t LED_PIN = GPIO_NUM_2;
  Switch ledSwitch = Switch(LED_PIN, "Built-in LED");

 public:
  void on();
  void off();
  void blink(int ms);
};
