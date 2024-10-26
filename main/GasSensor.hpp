#pragma once

#include "driver/adc.h"

#include "AnalogSensor.hpp"

class GasSensor : public AnalogSensor {
 public:
  GasSensor(adc1_channel_t adcChannel, adc_bits_width_t adcWidth);
  float read();
};
