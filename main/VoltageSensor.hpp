#pragma once

#include "driver/adc.h"

class VoltageSensor {
 private:
  adc1_channel_t adcChannel{};
  adc_bits_width_t adcWidth{};
  const float VOLTAGE = 3.23;
  const float MAX_ADC = 4095;
  const float VOLTAGE_SENSOR_SENSITIVITY = 0.100;
  const float VOLTAGE_SENSOR_OFFSET = 2.35;
  const float CALIBRATION_FACTOR = 1.0427;
  const float RESISTER_VALUE_1 = 29800.0;
  const float RESISTER_VALUE_2 = 7400.0;

 public:
  VoltageSensor(adc1_channel_t adcChannel, adc_bits_width_t adcWidth);
  float read();
};
