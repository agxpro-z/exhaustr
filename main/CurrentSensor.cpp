#include "driver/adc.h"
#include "freertos/FreeRTOS.h"

#include "AnalogSensor.hpp"
#include "CurrentSensor.hpp"

CurrentSensor::CurrentSensor(
  adc1_channel_t adcChannel,
  adc_bits_width_t adcWidth
) : AnalogSensor(adcChannel, adcWidth) { }

float CurrentSensor::read() {
  int adcValue = 0;
  for (int i = 0; i < 256; i++) {
    adcValue += adc1_get_raw(adcChannel);
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
  float avgAdcValue = (float) adcValue / 256;
  float adcVoltage = (avgAdcValue * VOLTAGE) / MAX_ADC;
  float voltage = adcVoltage * ((RESISTER_VALUE_1 + RESISTER_VALUE_2) / RESISTER_VALUE_2) * CALIBRATION_FACTOR;
  float current = (voltage - CURRENT_SENSOR_OFFSET) / CURRENT_SENSOR_SENSITIVITY;
  return current;
}
