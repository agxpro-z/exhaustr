#include <cstring>
#include <string>

#include "esp_event.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#include "WiFi.hpp"

static const char* TAG = "WiFi";

static void apEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  WiFi* wifi = (WiFi*) arg;
  wifi_event_ap_staconnected_t* event_connected = nullptr;
  wifi_event_ap_stadisconnected_t* event_disconnected = nullptr;

  switch (event_id) {
    case WIFI_EVENT_AP_START:
      ESP_LOGI(TAG, "WiFi AP started");
      esp_wifi_connect();
      break;
    case WIFI_EVENT_AP_STACONNECTED:
      ESP_LOGI(TAG, "WiFi AP connected");
      event_connected = (wifi_event_ap_staconnected_t*) event_data;
      ESP_LOGI(TAG, "station " MACSTR " join, AID=%d", MAC2STR(event_connected->mac), event_connected->aid);
      break;
    case WIFI_EVENT_AP_STADISCONNECTED:
      ESP_LOGI(TAG, "WiFi AP disconnected");
      event_disconnected = (wifi_event_ap_stadisconnected_t*) event_data;
      ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d, reason=%d", MAC2STR(event_disconnected->mac), event_disconnected->aid, event_disconnected->reason);
      break;
    case WIFI_EVENT_AP_STOP:
      ESP_LOGI(TAG, "WiFi AP stopped");
      break;
    default:
      break;
  }
}

static void staEventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  WiFi* wifi = (WiFi*) arg;

  if (event_base == WIFI_EVENT) {
    switch (event_id) {
      case WIFI_EVENT_STA_START:
        ESP_LOGI(TAG, "WiFi started");
        esp_wifi_connect();
        break;
      case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG, "WiFi connected");
        break;
      case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG, "WiFi disconnected");
        if (wifi->retryCount < 3) {
          esp_wifi_connect();
          ++wifi->retryCount;
        }
        break;
      case WIFI_EVENT_STA_STOP:
        ESP_LOGI(TAG, "WiFi stopped");
        break;
      default:
        break;
    }
  } else if (event_base == IP_EVENT) {
    ip_event_got_ip_t* event = nullptr;

    switch (event_id) {
      case IP_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "WiFi got IP address");
        event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
        wifi->retryCount = 0;
        break;
      default:
        break;
    }
  }
}

void WiFi::setupAP(wifi_config_t& wifi_config) {
  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &apEventHandler, this, nullptr));
  wifi_config = {
    .ap = {
      // .ssid = "",
      // .password = "",
      .ssid_len = (uint8_t) this->ssid.length(),
      .channel = 1,
      .authmode = WIFI_AUTH_WPA_WPA2_PSK,
      .ssid_hidden = 0,
      .max_connection = 4,
      .beacon_interval = 100,
      .pmf_cfg = {
        .capable = true,
        .required = true
      }
    }
  };
}

void WiFi::setupSTA(wifi_config_t& wifi_config) {
  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &staEventHandler, this, nullptr));
  ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &staEventHandler, this, nullptr));
  wifi_config = {
    .sta = {
      // .ssid = "",
      // .password = "",
      .scan_method = WIFI_FAST_SCAN,
      .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
      .threshold = {
        .rssi = -127,
        .authmode = WIFI_AUTH_WPA_WPA2_PSK
      },
    }
  };
}

WiFi::WiFi(std::string ssid, std::string password, Type type) : ssid(ssid), password(password), type(type) {
  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // Initialize WiFi
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  if (this->type == Type::AP) {
    esp_netif_create_default_wifi_ap();
  } else {
    esp_netif_create_default_wifi_sta();
  }

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  wifi_config_t wifi_config;
  if (this->type == Type::AP) {
    this->setupAP(wifi_config);
  } else {
    this->setupSTA(wifi_config);
  }

  // Set the SSID and password
  memcpy(wifi_config.ap.ssid, this->ssid.c_str(), this->ssid.length());
  wifi_config.ap.ssid[this->ssid.length()] = '\0';
  memcpy(wifi_config.ap.password, this->password.c_str(), this->password.length());
  wifi_config.ap.password[this->password.length()] = '\0';

  if (type == Type::AP) {
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
  } else {
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
  }

  ESP_LOGI(TAG, "WiFi initialized");
}
