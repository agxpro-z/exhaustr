#include "BuiltinLED.hpp"

#include "driver/gpio.h"

void BuiltInLED::on() {
  this->ledSwitch.on();
}

void BuiltInLED::off() {
  this->ledSwitch.off();
}

void BuiltInLED::blink(int ms) {
  this->ledSwitch.pattern(this->blinkPattern, ms);
}
