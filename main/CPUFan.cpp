
#include "CPUFan.hpp"
#include "driver/gpio.h"

CPUFan::CPUFan(
  gpio_num_t gpioPin,
  std::string name
) : gpioPin(gpioPin), name(name), fanSwitch(gpioPin, name) { }

void CPUFan::turnOn() {
  this->fanSwitch.on();
}

void CPUFan::turnOff() {
  this->fanSwitch.off();
}

bool CPUFan::isOn() {
  return this->fanSwitch.isOn();
}
