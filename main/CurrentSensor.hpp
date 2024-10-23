#pragma once

#include "driver/adc.h"

class CurrentSensor {
 private:
  adc1_channel_t adcChannel{};
  adc_bits_width_t adcWidth{};
  const float VOLTAGE = 3.23;
  const float MAX_ADC = 4095;
  const float CURRENT_SENSOR_SENSITIVITY = 0.100;
  const float CURRENT_SENSOR_OFFSET = 2.35;
  const float CALIBRATION_FACTOR = 1.097;
  const float RESISTER_VALUE_1 = 979.0;
  const float RESISTER_VALUE_2 = 1958.0;

 public:
  CurrentSensor(adc1_channel_t adcChannel, adc_bits_width_t adcWidth);
  float read();
};
