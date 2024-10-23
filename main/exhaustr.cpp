#include <iostream>
#include <stdio.h>

#include "driver/adc.h"

#include "BuiltinLED.hpp"
#include "CurrentSensor.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void app_main() {
  BuiltInLED builtinLED;
  CurrentSensor currentSensor(ADC1_CHANNEL_0 /* GPIO36 */, ADC_WIDTH_BIT_12);

  while (true) {
    builtinLED.blink(1000);
    float current = currentSensor.read();
    printf("Current: %.2f A\n", current);
  }
}

#ifdef __cplusplus
} // End extern "C"
#endif
