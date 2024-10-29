#pragma once

#include <string>

#include "driver/gpio.h"

#include "Switch.hpp"

/**
 * @brief Control the 4-pin CPU fan using a relay switch connected to a GPIO pin.
 *        The relay switch is used to turn the fan on and off.
 *
 * @note  The fan is turned on when the relay switch is closed and turned off when the relay switch is open.
 *
 * @param gpioPin The GPIO pin connected to the relay switch.
 * @param name The name of the CPU fan.
 */
class CPUFan {
 private:
  gpio_num_t gpioPin;
  std::string name;
  Switch fanSwitch;

 public:
  CPUFan(gpio_num_t gpioPin, std::string name);
  void turnOn();
  void turnOff();
  bool isOn();
};
