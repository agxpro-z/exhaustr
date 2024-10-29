#include "BuiltinLED.hpp"

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

void BuiltInLED::on() {
  this->ledSwitch.on();
}

void BuiltInLED::off() {
  this->ledSwitch.off();
}

void BuiltInLED::blink(int ms) {
  this->ledSwitch.on();
  vTaskDelay(ms / portTICK_PERIOD_MS);
  this->ledSwitch.off();
  vTaskDelay(ms / portTICK_PERIOD_MS);
}
