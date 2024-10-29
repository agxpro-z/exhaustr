#include <iostream>
#include <stdio.h>


#include "driver/adc.h"
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

void app_main() {
  BuiltInLED builtinLED;
  CPUFan cpuFan(GPIO_NUM_15, "CPU Fan");

  DHTSensor dhtSensor1(GPIO_NUM_9, DHTSensor::SensorType::DHT_TYPE_DHT11, "DHT Sensor 1");
  DHTSensor dhtSensor2(GPIO_NUM_10, DHTSensor::SensorType::DHT_TYPE_DHT11, "DHT Sensor 2");

  CurrentSensor currentSensor(ADC1_CHANNEL_4 /* GPIO32 */, ADC_WIDTH_BIT_12);
  VoltageSensor voltageSensor(ADC1_CHANNEL_5 /* GPIO33 */, ADC_WIDTH_BIT_12);

  GasSensor gasSensor(ADC1_CHANNEL_6 /* GPIO34 */, ADC_WIDTH_BIT_12);

  while (true) {
    builtinLED.blink(1000);
    float current = currentSensor.read();
    printf("Current: %.2f A\n", current);
    float voltage = voltageSensor.read();
    printf("Voltage: %.2f V\n", voltage);

    float airQuality = gasSensor.read();
    printf("Air Quality: %.2f\n", airQuality);

    cpuFan.turnOn();

    builtinLED.blink(1000);
    current = currentSensor.read();
    printf("Current: %.2f A\n", current);
    voltage = voltageSensor.read();
    printf("Voltage: %.2f V\n", voltage);

    printf("Reading data from DHT sensor 1\n");
    printf("Temperature: %.2f°C, Humidity: %.2f%%\n", dhtSensor1.getTemperature(), dhtSensor1.getHumidity());

    printf("Reading data from DHT sensor 2\n");
    printf("Temperature: %.2f°C, Humidity: %.2f%%\n", dhtSensor2.getTemperature(), dhtSensor2.getHumidity());

    vTaskDelay(10000 / portTICK_PERIOD_MS);
    cpuFan.turnOff();
  }
}

#ifdef __cplusplus
} // End extern "C"
#endif
