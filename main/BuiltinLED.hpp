#pragma once

#include "driver/gpio.h"

class BuiltInLED {
 private:
  const gpio_num_t LED_PIN = GPIO_NUM_2;

 public:
  BuiltInLED();
  void on();
  void off();
  void blink(int ms);
};
