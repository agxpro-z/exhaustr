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
 * @param fanPin The GPIO pin connected to the relay switch.
 * @param tachoPin The GPIO pin connected to the fan's tachometer output.
 * @param pwmPin The GPIO pin connected to the fan's PWM input.
 * @param name The name of the CPU fan.
 */
class CPUFan {
 private:
  gpio_num_t fanPin;
  gpio_num_t tachoPin;
  gpio_num_t pwmPin;
  std::string name;
  Switch fanSwitch;
  int fanRPM;
  int fanRPMCounter;
  int dutyStart;
  int dutyEnd;
  int fanSpeed; // 1-100

 public:
  CPUFan(gpio_num_t fanPin, gpio_num_t tachoPin, gpio_num_t pwmPin, std::string name);
  void turnOn();
  void turnOff();
  bool isOn();
  int getFanRPM();
  void setFanSpeed(int speed); // 1-100
  int getFanSpeed();
};
