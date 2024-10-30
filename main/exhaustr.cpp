#include <iostream>
#include <stdio.h>

#include "driver/adc.h"
#include "esp_pthread.h"
#include "freertos/FreeRTOS.h"

#include "BuiltinLED.hpp"
#include "CPUFan.hpp"
#include "CurrentSensor.hpp"
#include "DeviceController.hpp"
#include "DHTSensor.hpp"
#include "GasSensor.hpp"
#include "RESTServer.hpp"
#include "VoltageSensor.hpp"
#include "WiFi.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void* blink_thread(void* arg) {
  BuiltInLED& builtinLED = DeviceController::getInstance()->getBuiltInLED();
  while (true) {
    builtinLED.blink(250);
  }
  return nullptr;
}

void app_main() {
  // Initialize the device controller
  DeviceController::getInstance();

  WiFi wifi("ESP32 WiFi", "password", WiFi::Type::AP);

  // Create a thread to blink the built-in LED
  esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
  esp_pthread_set_cfg(&cfg);
  pthread_t thread;
  int res = pthread_create(&thread, nullptr, blink_thread, nullptr);
  if (res != 0) {
    printf("Failed to create thread: %d\n", res);
  }

  RESTServer restServer;
  restServer.start();

  CPUFan& cpuFan = DeviceController::getInstance()->getCPUFan();
  int speed = 0;
  while (true) {
    printf("Fan speed: %d\n", speed);
    cpuFan.turnOn();
    vTaskDelay(15000 / portTICK_PERIOD_MS);
    cpuFan.turnOff();
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    speed = (speed + 10) % 110;
    cpuFan.setFanSpeed(speed);
  }
}

#ifdef __cplusplus
} // End extern "C"
#endif
