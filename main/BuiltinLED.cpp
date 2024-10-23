#include "BuiltinLED.hpp"

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

BuiltInLED::BuiltInLED() {
  gpio_reset_pin(LED_PIN);
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
}

void BuiltInLED::on() {
  gpio_set_level(LED_PIN, 1);
  ESP_LOGI("BuiltinLED", "LED is on");
}

void BuiltInLED::off() {
  gpio_set_level(LED_PIN, 0);
  ESP_LOGI("BuiltinLED", "LED is off");
}

void BuiltInLED::blink(int ms) {
  this->on();
  vTaskDelay(ms / portTICK_PERIOD_MS);
  this->off();
  vTaskDelay(ms / portTICK_PERIOD_MS);
}
