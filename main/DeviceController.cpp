#include "driver/adc.h"
#include "esp_log.h"

#include "DeviceController.hpp"

static const char* TAG = "DeviceController";

DeviceController* DeviceController::deviceController = nullptr;

DeviceController::DeviceController() {
  // Singleton instance
  ESP_LOGI(TAG, "Initializing device controller");
  this->deviceController = this;

  // Initialize the device components
  ESP_LOGI(TAG, "Initializing device components");

  ESP_LOGI(TAG, "Initializing built-in LED");
  this->mBuiltinLED = new BuiltInLED();

  ESP_LOGI(TAG, "Initializing CPU fan");
  this->mCPUFan = new CPUFan(GPIO_NUM_15, GPIO_NUM_22, GPIO_NUM_23, "CPU Fan");

  ESP_LOGI(TAG, "Initializing current sensor");
  this->mCurrentSensor = new CurrentSensor(ADC1_CHANNEL_4 /* GPIO32 */, ADC_WIDTH_BIT_12);

  ESP_LOGI(TAG, "Initializing DHT sensors");
  this->mDHTSensor1 = new DHTSensor(GPIO_NUM_9, DHTSensor::SensorType::DHT_TYPE_DHT11, "DHT Sensor 1");
  this->mDHTSensor2 = new DHTSensor(GPIO_NUM_10, DHTSensor::SensorType::DHT_TYPE_DHT11, "DHT Sensor 2");

  ESP_LOGI(TAG, "Initializing gas sensor");
  this->mGasSensor = new GasSensor(ADC1_CHANNEL_6 /* GPIO34 */, ADC_WIDTH_BIT_12);

  ESP_LOGI(TAG, "Initializing voltage sensor");
  this->mVoltageSensor = new VoltageSensor(ADC1_CHANNEL_5 /* GPIO33 */, ADC_WIDTH_BIT_12);

  ESP_LOGI(TAG, "Device controller initialized");
}

DeviceController::~DeviceController() {
  // Deinitialize the device components
  ESP_LOGI(TAG, "Deinitializing device components");
  delete this->mBuiltinLED;
  delete this->mCPUFan;
  delete this->mCurrentSensor;
  delete this->mDHTSensor1;
  delete this->mDHTSensor2;
  delete this->mGasSensor;
  delete this->mVoltageSensor;
}
