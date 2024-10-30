#pragma once

#include <string>

#include "esp_event.h"
#include "esp_wifi.h"

class WiFi {
 public:
  enum Type {
    STA,
    AP
  };

 private:
  const std::string ssid;
  const std::string password;
  const Type type;

  void setupAP(wifi_config_t& wifi_config);
  void setupSTA(wifi_config_t& wifi_config);

 public:
  int retryCount;

  WiFi(const std::string ssid, const std::string password, Type type);
};
