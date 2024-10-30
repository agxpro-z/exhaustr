#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_attr.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"

#include "CPUFan.hpp"

CPUFan::CPUFan(
  gpio_num_t fanPin,
  gpio_num_t tachoPin,
  gpio_num_t pwmPin,
  std::string name
) : fanSwitch(fanPin, name) {
  this->fanPin = fanPin;
  this->tachoPin = tachoPin;
  this->pwmPin = pwmPin;
  this->name = name;
  this->fanRPM = 0;
  this->fanRPMCounter = 0;
  this->dutyStart = 180;
  this->dutyEnd = 255;
  this->fanSpeed = 1;

  // Setup the tacho pin for RPM calculation
  gpio_reset_pin(this->tachoPin);
  gpio_set_direction(this->tachoPin, GPIO_MODE_INPUT);
  gpio_set_intr_type(this->tachoPin, GPIO_INTR_POSEDGE);
  gpio_install_isr_service(0);

  // Setup the PWM pin for fan speed control
  gpio_reset_pin(this->pwmPin);
  gpio_set_direction(this->pwmPin, GPIO_MODE_OUTPUT);

  ledc_timer_config_t timerConfig = {
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_8_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 25000,
    .clk_cfg = LEDC_AUTO_CLK,
    .deconfigure = false
  };
  ledc_timer_config(&timerConfig);

  ledc_channel_config_t channelConfig = {
    .gpio_num = this->pwmPin,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = LEDC_TIMER_0,
    .duty = (uint32_t) this->dutyStart,
    .hpoint = 0,
    .flags = {
      .output_invert = 0
    }
  };
  ledc_channel_config(&channelConfig);
}

// Interrupt service routine for the tachometer pin
void IRAM_ATTR rpm_isr_handler(void* arg) {
  int* fanRPMCounter = (int*) arg;
  ++(*fanRPMCounter);
}

void CPUFan::turnOn() {
  if (this->isOn()) {
    return;
  }
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

void CPUFan::setFanSpeed(int speed) {
  if (speed < 1) {
    speed = 1;
  } else if (speed > 100) {
    speed = 100;
  }
  this->fanSpeed = speed;
  int duty = this->dutyStart + (this->dutyEnd - this->dutyStart) * speed / 100;
  ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
  ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
}

int CPUFan::getFanSpeed() {
  return this->fanSpeed;
}
