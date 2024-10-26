#pragma once

#include "driver/adc.h"

class AnalogSensor {
 protected:
  adc1_channel_t adcChannel{};
  adc_bits_width_t adcWidth{};

 public:
  AnalogSensor(adc1_channel_t adcChannel, adc_bits_width_t adcWidth) {
    this->adcChannel = adcChannel;
    this->adcWidth = adcWidth;
    adc1_config_width(adcWidth);
    adc1_config_channel_atten(adcChannel, ADC_ATTEN_DB_12);
  }

  virtual float read() = 0;
};
