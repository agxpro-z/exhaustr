
#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"

#include "CPUFan.hpp"

CPUFan::CPUFan(
  gpio_num_t fanPin,
  gpio_num_t tachoPin,
  std::string name
) : fanSwitch(fanPin, name) {
  this->fanPin = fanPin;
  this->tachoPin = tachoPin;
  this->name = name;
  this->fanRPM = 0;
  this->fanRPMCounter = 0;

  // Setup the tacho pin for RPM calculation
  gpio_reset_pin(this->tachoPin);
  gpio_set_direction(this->tachoPin, GPIO_MODE_INPUT);
  gpio_set_intr_type(this->tachoPin, GPIO_INTR_POSEDGE);
  gpio_install_isr_service(0);
}

void IRAM_ATTR rpm_isr_handler(void* arg) {
  int* fanRPMCounter = (int*) arg;
  ++(*fanRPMCounter);
}

void CPUFan::turnOn() {
  gpio_isr_handler_add(tachoPin, rpm_isr_handler, &this->fanRPMCounter);
  this->fanSwitch.on();

  // Create a task to calculate the fan RPM
  xTaskCreate(
    [](void* arg) {
      CPUFan* cpuFan = (CPUFan*) arg;
      while (cpuFan->isOn()) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        cpuFan->fanRPM = cpuFan->fanRPMCounter * 60 / 2; // 2 pulses per revolution
        cpuFan->fanRPMCounter = 0;
      }
      cpuFan->fanRPM = 0;
      vTaskDelete(nullptr);
    },
    "Fan RPM Task",
    2048,
    this,
    5,
    nullptr
  );
}

void CPUFan::turnOff() {
  this->fanSwitch.off();
  gpio_isr_handler_remove(tachoPin);
}

bool CPUFan::isOn() {
  return this->fanSwitch.isOn();
}

int CPUFan::getFanRPM() {
  return this->fanRPM;
}
