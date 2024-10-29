#include <iostream>
#include <stdio.h>


#include "driver/adc.h"
#include "esp_pthread.h"
#include "freertos/FreeRTOS.h"

#include "BuiltinLED.hpp"
#include "CPUFan.hpp"
#include "CurrentSensor.hpp"
#include "DHTSensor.hpp"
#include "GasSensor.hpp"
#include "VoltageSensor.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void* blink_thread(void* arg) {
  BuiltInLED builtinLED;
  while (true) {
    builtinLED.blink(1000);
  }
  return nullptr;
}

void* dht_thread(void* args) {
  DHTSensor dhtSensor1(GPIO_NUM_9, DHTSensor::SensorType::DHT_TYPE_DHT11, "DHT Sensor 1");
  DHTSensor dhtSensor2(GPIO_NUM_10, DHTSensor::SensorType::DHT_TYPE_DHT11, "DHT Sensor 2");
  GasSensor gasSensor(ADC1_CHANNEL_6 /* GPIO34 */, ADC_WIDTH_BIT_12);

  while (true) {
    float airQuality = gasSensor.read();
    printf("Air Quality: %.2f\n", airQuality);

    printf("DHT Sensor 1: Temperature: %.2f°C, Humidity: %.2f%%\n", dhtSensor1.getTemperature(), dhtSensor1.getHumidity());
    printf("DHT Sensor 2: Temperature: %.2f°C, Humidity: %.2f%%\n", dhtSensor2.getTemperature(), dhtSensor2.getHumidity());

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void app_main() {
  CPUFan cpuFan(GPIO_NUM_15, "CPU Fan");

  CurrentSensor currentSensor(ADC1_CHANNEL_4 /* GPIO32 */, ADC_WIDTH_BIT_12);
  VoltageSensor voltageSensor(ADC1_CHANNEL_5 /* GPIO33 */, ADC_WIDTH_BIT_12);

  // Create a thread to blink the built-in LED
  esp_pthread_cfg_t cfg = esp_pthread_get_default_config();
  esp_pthread_set_cfg(&cfg);
  pthread_t thread;
  int res = pthread_create(&thread, nullptr, blink_thread, nullptr);
  if (res != 0) {
    printf("Failed to create thread: %d\n", res);
  }

  // Create a thread to read data from the DHT sensors and gas sensor
  res = pthread_create(&thread, nullptr, dht_thread, nullptr);
  if (res != 0) {
    printf("Failed to create thread: %d\n", res);
  }

  while (true) {
    float current = currentSensor.read();
    printf("Current: %.2f A\n", current);
    float voltage = voltageSensor.read();
    printf("Voltage: %.2f V\n", voltage);

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    cpuFan.turnOn();

    current = currentSensor.read();
    printf("Current: %.2f A\n", current);
    voltage = voltageSensor.read();
    printf("Voltage: %.2f V\n", voltage);

    vTaskDelay(15000 / portTICK_PERIOD_MS);
    cpuFan.turnOff();
  }
}

#ifdef __cplusplus
} // End extern "C"
#endif
