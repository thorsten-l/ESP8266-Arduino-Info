#include <App.hpp>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Esp.h>
#include <FS.h>
#include <LittleFS.h>
#include <OtaHandler.hpp>
#include <WifiHandler.hpp>
#include "WebHandler.hpp"

WebHandler webHandler;
static ESP8266WebServer server(80);
size_t fsTotalBytes;
size_t fsUsedBytes;

const char *getJsonStatus(WiFiClient *client)
{
  time_t now = time(nullptr);

  int remotePort = 0;
  char remoteAddress[32] = { 0 };

  if ( client != NULL )
  {
    remotePort = client->remotePort();
    strncpy( remoteAddress, client->remoteIP().toString().c_str(), 31 );
  }
  else
  {
    remotePort = server.client().remotePort();
    strncpy( remoteAddress, server.client().remoteIP().toString().c_str(), 31 );
  }
  
  if ( appBuffer == NULL )
  {
    appBuffer = (char *)malloc(APP_BUFFER_SIZE);
    memset(appBuffer, 0, APP_BUFFER_SIZE);
  }

  sprintf(appBuffer,
    "{"
    "\"millis\":%lu,"
    "\"utc\":%llu,"
    "\"ctime\":\"%s\","
    "\"timezone\":\"%s\","
    "\"uptime\":\"%s\","
    "\"dateTime\":\"%s\","
    "\"host_name\":\"%s.local\","
    "\"esp_full_version\":\"%s\","
    "\"esp_core_version\":\"%s\","
    "\"esp_sdk_version\":\"%s\","
    "\"platformio_env\":\"%s\","
    "\"platformio_platform\":\"%s\","
    "\"platformio_framework\":\"%s\","
    "\"arduino_board\":\"%s\","
    "\"chip_id\":\"%08X\","
    "\"cpu_freq\":\"%dMhz\","
    "\"flash_size\":%u,"
    "\"flash_speed\":%u,"
    "\"ide_size\":%u,"
    "\"fw_name\":\"%s\","
    "\"fw_version\":\"%s\","
    "\"build_date\":\"%s\","
    "\"build_time\":\"%s\","
     "\"wifi_ssid\":\"%s\","
    "\"wifi_reconnect_counter\":%d,"
    "\"wifi_channel\":%d,"
    "\"wifi_phy_mode\":\"%s\","
    "\"wifi_mac_address\":\"%s\","
    "\"wifi_hostname\":\"%s\","
    "\"wifi_ip_address\":\"%s\","
    "\"wifi_gateway_ip\":\"%s\","
    "\"wifi_subnet_mask\":\"%s\","
    "\"wifi_dns_ip\":\"%s\","
     "\"spiffs_total\":%u,"
    "\"spiffs_used\":%u,"
    "\"free_heap\":%u,"
    "\"sketch_size\":%u,"
    "\"free_sketch_space\":%u,"
    "\"remote_client_ip\":\"%s\","
    "\"remote_client_port\":%u"
    "}",
    millis(),
    now,
    strtok( ctime(&now), "\n" ),
    TIMEZONE,
    appUptime(),
    appDateTime(),
    wifiHandler.getHostname(), 
    ESP.getFullVersion().c_str(), 
    ESP.getCoreVersion().c_str(), 
    ESP.getSdkVersion(),
    PIOENV,
    PIOPLATFORM,
    PIOFRAMEWORK,
    ARDUINO_BOARD,
    ESP.getChipId(),
    ESP.getCpuFreqMHz(), 
    ESP.getFlashChipRealSize(),
    ESP.getFlashChipSpeed(), 
    ESP.getFlashChipSize(), 
    APP_NAME,
    APP_VERSION, 
    __DATE__, 
    __TIME__,
     WIFI_SSID,
    wifiHandler.getConnectCounter(),
    WiFi.channel(),
    wifiHandler.getPhyMode(),
    wifiHandler.getMacAddress(),
    WiFi.hostname().c_str(),
    WiFi.localIP().toString().c_str(),
    WiFi.gatewayIP().toString().c_str(),
    WiFi.subnetMask().toString().c_str(),
    WiFi.dnsIP().toString().c_str(),
    fsTotalBytes, 
    fsUsedBytes,
    ESP.getFreeHeap(), 
    ESP.getSketchSize(), 
    ESP.getFreeSketchSpace(),
    remoteAddress,
    remotePort
  );

  return appBuffer;
}

WebHandler::WebHandler() 
{ 
  initialized = false; 
}

void WebHandler::setup()
{
  LOG0("HTTP server setup...\n");

  fsTotalBytes = 0;
  fsUsedBytes = 0;

  if(LittleFS.begin())
  {
    FSInfo fs_info;
    LittleFS.info(fs_info);
    fsTotalBytes = fs_info.totalBytes;
    fsUsedBytes = fs_info.usedBytes;
    LittleFS.end();
  }

  server.on("/", []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", getJsonStatus(NULL));
  } );

  MDNS.addService("http", "tcp", 80);
  MDNS.addServiceTxt("http", "tcp", "path", "/");
  MDNS.addServiceTxt("http", "tcp", "fw_name", APP_NAME);
  MDNS.addServiceTxt("http", "tcp", "fw_version", APP_VERSION);
  server.begin();

  LOG0("HTTP server started\n");
  initialized = true;
}

void WebHandler::handle()
{
  server.handleClient();
}
