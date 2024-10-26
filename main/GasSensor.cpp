#include "driver/adc.h"
#include "freertos/FreeRTOS.h"

#include "AnalogSensor.hpp"
#include "GasSensor.hpp"

GasSensor::GasSensor(
  adc1_channel_t adcChannel,
  adc_bits_width_t adcWidth
) : AnalogSensor(adcChannel, adcWidth) { }

float GasSensor::read() {
  return (float) adc1_get_raw(adcChannel);
}
