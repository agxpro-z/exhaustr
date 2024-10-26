#include <iostream>
#include <stdio.h>

#include "driver/adc.h"
#include "freertos/FreeRTOS.h"

#include "BuiltinLED.hpp"
#include "CurrentSensor.hpp"
#include "GasSensor.hpp"
#include "Switch.hpp"
#include "VoltageSensor.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void app_main() {
  BuiltInLED builtinLED;
  Switch fanSwitch(GPIO_NUM_15, "Fan Switch");
  CurrentSensor currentSensor(ADC1_CHANNEL_0 /* GPIO36 */, ADC_WIDTH_BIT_12);
  GasSensor gasSensor(ADC1_CHANNEL_6 /* GPIO34 */, ADC_WIDTH_BIT_12);
  VoltageSensor voltageSensor(ADC1_CHANNEL_3 /* GPIO39 */, ADC_WIDTH_BIT_12);

  while (true) {
    builtinLED.blink(1000);
    float current = currentSensor.read();
    printf("Current: %.2f A\n", current);
    float voltage = voltageSensor.read();
    printf("Voltage: %.2f V\n", voltage);

    float airQuality = gasSensor.read();
    printf("Air Quality: %.2f\n", airQuality);

    fanSwitch.on();

    builtinLED.blink(1000);
    current = currentSensor.read();
    printf("Current: %.2f A\n", current);
    voltage = voltageSensor.read();
    printf("Voltage: %.2f V\n", voltage);

    vTaskDelay(10000 / portTICK_PERIOD_MS);
    fanSwitch.off();
  }
}

#ifdef __cplusplus
} // End extern "C"
#endif
