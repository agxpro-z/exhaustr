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
  this->builtinLED = new BuiltInLED();

  ESP_LOGI(TAG, "Initializing CPU fan");
  this->cpuFan = new CPUFan(GPIO_NUM_15, GPIO_NUM_22, GPIO_NUM_23, "CPU Fan");

  ESP_LOGI(TAG, "Initializing current sensor");
  this->currentSensor = new CurrentSensor(ADC1_CHANNEL_4 /* GPIO32 */, ADC_WIDTH_BIT_12);

  ESP_LOGI(TAG, "Initializing DHT sensors");
  this->dhtSensor1 = new DHTSensor(GPIO_NUM_9, DHTSensor::SensorType::DHT_TYPE_DHT11, "DHT Sensor 1");
  this->dhtSensor2 = new DHTSensor(GPIO_NUM_10, DHTSensor::SensorType::DHT_TYPE_DHT11, "DHT Sensor 2");

  ESP_LOGI(TAG, "Initializing gas sensor");
  this->gasSensor = new GasSensor(ADC1_CHANNEL_6 /* GPIO34 */, ADC_WIDTH_BIT_12);

  ESP_LOGI(TAG, "Initializing voltage sensor");
  this->voltageSensor = new VoltageSensor(ADC1_CHANNEL_5 /* GPIO33 */, ADC_WIDTH_BIT_12);

  ESP_LOGI(TAG, "Device controller initialized");
}

DeviceController::~DeviceController() {
  // Deinitialize the device components
  ESP_LOGI(TAG, "Deinitializing device components");
  delete this->builtinLED;
  delete this->cpuFan;
  delete this->currentSensor;
  delete this->dhtSensor1;
  delete this->dhtSensor2;
  delete this->gasSensor;
  delete this->voltageSensor;
  ESP_LOGI(TAG, "Device components deinitialized");
}

DeviceController* DeviceController::getInstance() {
  if (deviceController == nullptr) {
    deviceController = new DeviceController();
  }
  return deviceController;
}

DeviceController::Mode DeviceController::getMode() {
  return this->mode;
}

void DeviceController::setMode(Mode mode) {
  this->mode = mode;
}

BuiltInLED& DeviceController::getBuiltInLED() {
  return *this->builtinLED;
}

CPUFan& DeviceController::getCPUFan() {
  return *this->cpuFan;
}

CurrentSensor& DeviceController::getCurrentSensor() {
  return *this->currentSensor;
}

DHTSensor& DeviceController::getDHTSensor1() {
  return *this->dhtSensor1;
}

DHTSensor& DeviceController::getDHTSensor2() {
  return *this->dhtSensor2;
}

GasSensor& DeviceController::getGasSensor() {
  return *this->gasSensor;
}

VoltageSensor& DeviceController::getVoltageSensor() {
  return *this->voltageSensor;
}
