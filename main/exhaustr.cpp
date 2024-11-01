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
  DeviceController* deviceController =  DeviceController::getInstance();

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
  while (true) {
    if (deviceController->getMode() == DeviceController::Mode::AUTO) {
      float temperature1, humidity1;
      deviceController->getDHTSensor1().read(&temperature1, &humidity1);
      float temperature2, humidity2;
      deviceController->getDHTSensor2().read(&temperature2, &humidity2);
      float aqi = deviceController->getGasSensor().read();

      float tempDiff = temperature1 - temperature2;
      if (tempDiff >= 2) {
        if (tempDiff >= 10) {
          cpuFan.setFanSpeed(100);
        } else {
          cpuFan.setFanSpeed(10 * tempDiff);
        }
        cpuFan.turnOn();
      } else if (aqi > 200) {
        cpuFan.setFanSpeed(100);
        cpuFan.turnOn();
      } else {
        cpuFan.turnOff();
      }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

#ifdef __cplusplus
} // End extern "C"
#endif
