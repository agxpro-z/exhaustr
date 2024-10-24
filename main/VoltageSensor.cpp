#include "driver/adc.h"

#include "VoltageSensor.hpp"

VoltageSensor::VoltageSensor(adc1_channel_t adcChannel, adc_bits_width_t adcWidth) : adcChannel(adcChannel), adcWidth(adcWidth) {
  adc1_config_width(adcWidth);
  adc1_config_channel_atten(adcChannel, ADC_ATTEN_DB_11);
}

float VoltageSensor::read() {
  uint32_t adcValue = adc1_get_raw(adcChannel);
  float adcVoltage = (adcValue * VOLTAGE) / MAX_ADC;
  float voltage = adcVoltage * ((RESISTER_VALUE_1 + RESISTER_VALUE_2) / RESISTER_VALUE_2) * CALIBRATION_FACTOR;
  return voltage;
}
