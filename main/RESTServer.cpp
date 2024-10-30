#include <string>

#include "esp_http_server.h"
#include "esp_chip_info.h"
#include "esp_log.h"
#include "cJSON.h"

#include "RESTServer.hpp"

static const char* TAG = "RESTServer";

static const int REST_SERVER_PORT = 80;

/*
* Handler for the /chip/info URI
*/
static esp_err_t chipInfoHandler(httpd_req_t* req);

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
