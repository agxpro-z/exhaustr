#include <string>

#include "esp_http_server.h"
#include "esp_chip_info.h"
#include "esp_log.h"
#include "cJSON.h"

#include "DeviceController.hpp"
#include "RESTServer.hpp"

static const char* TAG = "RESTServer";

static const int REST_SERVER_PORT = 80;

/*
 * Handler for the /chip/info URI
 */
static esp_err_t chipInfoHandler(httpd_req_t* req);

/*
 * Device status handler /status URI
 */
static esp_err_t statusHandler(httpd_req_t* req);

/*
 * Fan state GET handler /fan/state URI
 */
static esp_err_t fanStateGetHandler(httpd_req_t* req);

/*
 * Fan state POST handler /fan/state URI
 */
static esp_err_t fanStatePostHandler(httpd_req_t* req);

/*
 * Fan speed GET handler /fan/speed URI
 */
static esp_err_t fanSpeedGetHandler(httpd_req_t* req);

/*
 * Fan speed POST handler /fan/speed URI
 */
static esp_err_t fanSpeedPostHandler(httpd_req_t* req);

void RESTServer::start() {
  // Start the REST server
  httpd_handle_t server = nullptr;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = REST_SERVER_PORT;
  config.uri_match_fn = httpd_uri_match_wildcard;

  ESP_LOGI(TAG, "Starting REST server on port: '%d'", config.server_port);
  esp_err_t err = httpd_start(&server, &config);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failed to start the REST server: %s", esp_err_to_name(err));
    return;
  }

  // Register the URI handlers
  // Get the chip information
  httpd_uri_t chipInfoUri = {
    .uri = "/chip/info",
    .method = HTTP_GET,
    .handler = chipInfoHandler,
    .user_ctx = nullptr
  };
  httpd_register_uri_handler(server, &chipInfoUri);

  // Get the device status
  httpd_uri_t statusUri = {
    .uri = "/status",
    .method = HTTP_GET,
    .handler = statusHandler,
    .user_ctx = nullptr
  };
  httpd_register_uri_handler(server, &statusUri);

  // Get the fan state
  httpd_uri_t fanStateUri = {
    .uri = "/fan/state",
    .method = HTTP_GET,
    .handler = fanStateGetHandler,
    .user_ctx = nullptr
  };
  httpd_register_uri_handler(server, &fanStateUri);

  // Set the fan state
  httpd_uri_t fanStatePostUri = {
    .uri = "/fan/state",
    .method = HTTP_POST,
    .handler = fanStatePostHandler,
    .user_ctx = nullptr
  };
  httpd_register_uri_handler(server, &fanStatePostUri);

  // Get the fan speed
  httpd_uri_t fanSpeedUri = {
    .uri = "/fan/speed",
    .method = HTTP_GET,
    .handler = fanSpeedGetHandler,
    .user_ctx = nullptr
  };
  httpd_register_uri_handler(server, &fanSpeedUri);

  // Set the fan speed
  httpd_uri_t fanSpeedPostUri = {
    .uri = "/fan/speed",
    .method = HTTP_POST,
    .handler = fanSpeedPostHandler,
    .user_ctx = nullptr
  };
  httpd_register_uri_handler(server, &fanSpeedPostUri);
}

// Handler for the /chip/info URI
static esp_err_t chipInfoHandler(httpd_req_t* req) {
  esp_chip_info_t chipInfo;
  esp_chip_info(&chipInfo);

  std::string model;
  switch (chipInfo.model) {
    case CHIP_ESP32:
      model = "ESP32";
      break;
    case CHIP_ESP32S2:
      model = "ESP32-S2";
      break;
    case CHIP_ESP32S3:
      model = "ESP32-S3";
      break;
    case CHIP_ESP32C3:
      model = "ESP32-C3";
      break;
    case CHIP_ESP32C2:
      model = "ESP32-C2";
      break;
    case CHIP_ESP32C6:
      model = "ESP32-C6";
      break;
    case CHIP_ESP32H2:
      model = "ESP32-H2";
      break;
    case CHIP_ESP32P4:
      model = "ESP32-P4";
      break;
    case CHIP_ESP32C61:
      model = "ESP32-C61";
      break;
    case CHIP_POSIX_LINUX:
      model = "POSIX/Linux";
      break;
    default:
      model = "Unknown";
      break;
  }

  cJSON* root = cJSON_CreateObject();
  cJSON_AddStringToObject(root, "model", model.c_str());
  cJSON_AddNumberToObject(root, "cores", chipInfo.cores);
  cJSON_AddNumberToObject(root, "revision", chipInfo.revision);
  cJSON_AddNumberToObject(root, "features", chipInfo.features);

  char* json = cJSON_Print(root);
  cJSON_Delete(root);

  httpd_resp_set_type(req, "application/json");
  httpd_resp_send(req, json, strlen(json));
  free(json);
  return ESP_OK;
}

// Device status handler /status URI
static esp_err_t statusHandler(httpd_req_t* req) {
  DeviceController* deviceController = DeviceController::getInstance();
  cJSON* root = cJSON_CreateObject();
  cJSON_AddStringToObject(root, "temp-inside", std::to_string(deviceController->getDHTSensor1().getTemperature()).c_str());
  cJSON_AddStringToObject(root, "humidity-inside", std::to_string(deviceController->getDHTSensor1().getHumidity()).c_str());
  cJSON_AddStringToObject(root, "temp-outside", std::to_string(deviceController->getDHTSensor2().getTemperature()).c_str());
  cJSON_AddStringToObject(root, "humidity-outside", std::to_string(deviceController->getDHTSensor2().getHumidity()).c_str());
  cJSON_AddStringToObject(root, "aqi", std::to_string(deviceController->getGasSensor().read()).c_str());
  cJSON_AddStringToObject(root, "fan-state", deviceController->getCPUFan().isOn() ? "on" : "off");
  cJSON_AddStringToObject(root, "fan-speed", std::to_string(deviceController->getCPUFan().getFanSpeed()).c_str());
  cJSON_AddStringToObject(root, "fan-rpm", std::to_string(deviceController->getCPUFan().getFanRPM()).c_str());
  cJSON_AddStringToObject(root, "fan-current", std::to_string(deviceController->getCurrentSensor().read()).c_str());
  cJSON_AddStringToObject(root, "fan-voltage", std::to_string(deviceController->getVoltageSensor().read()).c_str());

  char* json = cJSON_Print(root);
  cJSON_Delete(root);

  httpd_resp_set_type(req, "application/json");
  httpd_resp_send(req, json, strlen(json));
  free(json);
  return ESP_OK;
}

// Fan state GET handler /fan/state URI
static esp_err_t fanStateGetHandler(httpd_req_t* req) {
  DeviceController* deviceController = DeviceController::getInstance();
  cJSON* root = cJSON_CreateObject();
  cJSON_AddStringToObject(root, "state", deviceController->getCPUFan().isOn() ? "on" : "off");

  char* json = cJSON_Print(root);
  cJSON_Delete(root);

  httpd_resp_set_type(req, "application/json");
  httpd_resp_send(req, json, strlen(json));
  free(json);
  return ESP_OK;
}

// Fan state POST handler /fan/state URI
static esp_err_t fanStatePostHandler(httpd_req_t* req) {
  DeviceController* deviceController = DeviceController::getInstance();
  char buf[1024];
  int ret = httpd_req_recv(req, buf, sizeof(buf));
  if (ret <= 0) {
    if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
      httpd_resp_send_408(req);
    }
    return ESP_FAIL;
  }
  buf[ret] = '\0';

  cJSON* root = cJSON_Parse(buf);
  if (root == nullptr) {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
    return ESP_FAIL;
  }

  cJSON* state = cJSON_GetObjectItem(root, "state");
  if (state == nullptr or !cJSON_IsString(state)) {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid fan state! Must be a string");
    cJSON_Delete(root);
    return ESP_FAIL;
  }

  if (strcmp(state->valuestring, "on") == 0) {
    if (deviceController->getCPUFan().isOn()) {
      httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Fan is already on");
      cJSON_Delete(root);
      return ESP_FAIL;
    }
    deviceController->getCPUFan().turnOn();
  } else if (strcmp(state->valuestring, "off") == 0) {
    if (not deviceController->getCPUFan().isOn()) {
      httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Fan is already off");
      cJSON_Delete(root);
      return ESP_FAIL;
    }
    deviceController->getCPUFan().turnOff();
  } else {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid fan state! Must be 'on' or 'off'");
    cJSON_Delete(root);
    return ESP_FAIL;
  }

  cJSON_Delete(root);
  httpd_resp_send(req, nullptr, 0);

  return ESP_OK;
}

// Fan speed GET handler /fan/speed URI
static esp_err_t fanSpeedGetHandler(httpd_req_t* req) {
  DeviceController* deviceController = DeviceController::getInstance();
  cJSON* root = cJSON_CreateObject();
  cJSON_AddNumberToObject(root, "speed", deviceController->getCPUFan().getFanSpeed());

  char* json = cJSON_Print(root);
  cJSON_Delete(root);

  httpd_resp_set_type(req, "application/json");
  httpd_resp_send(req, json, strlen(json));
  free(json);
  return ESP_OK;
}

// Fan speed POST handler /fan/speed URI
static esp_err_t fanSpeedPostHandler(httpd_req_t* req) {
  DeviceController* deviceController = DeviceController::getInstance();
  char buf[1024];
  int ret = httpd_req_recv(req, buf, sizeof(buf));
  if (ret <= 0) {
    if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
      httpd_resp_send_408(req);
    }
    return ESP_FAIL;
  }
  buf[ret] = '\0';

  cJSON* root = cJSON_Parse(buf);
  if (root == nullptr) {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
    return ESP_FAIL;
  }

  cJSON* speed = cJSON_GetObjectItem(root, "speed");
  if (speed == nullptr or !cJSON_IsNumber(speed)) {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid fan speed! Must be a number");
    cJSON_Delete(root);
    return ESP_FAIL;
  }

  if (speed->valueint < 1 or speed->valueint > 100) {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid fan speed! Must be between 1 and 100");
    cJSON_Delete(root);
    return ESP_FAIL;
  }

  if (not deviceController->getCPUFan().isOn()) {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Fan is off! Turn it on first");
    cJSON_Delete(root);
    return ESP_FAIL;
  }

  deviceController->getCPUFan().setFanSpeed(speed->valueint);
  cJSON_Delete(root);

  httpd_resp_send(req, nullptr, 0);
  return ESP_OK;
}
