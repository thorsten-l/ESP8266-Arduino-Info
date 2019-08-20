#include <App.hpp>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Esp.h>
#include <FS.h>
#include <OtaHandler.hpp>
#include <WifiHandler.hpp>
#include "WebHandler.hpp"

WebHandler webHandler;
static ESP8266WebServer server(80);
size_t fsTotalBytes;
size_t fsUsedBytes;
char buffer[3000];

WebHandler::WebHandler() 
{ 
  initialized = false; 
}

void WebHandler::setup()
{
  LOG0("HTTP server setup...\n");

  fsTotalBytes = 0;
  fsUsedBytes = 0;

  if(SPIFFS.begin())
  {
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    fsTotalBytes = fs_info.totalBytes;
    fsUsedBytes = fs_info.usedBytes;
    SPIFFS.end();
  }

  server.on("/", []() {

    server.sendHeader("Access-Control-Allow-Origin", "*");

    time_t now = time(nullptr);

    sprintf(buffer,
          "{"
          "\"millis\":%lu,"
          "\"utc\":%lu,"
          "\"utc_ctime\":\"%s\","
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
          server.client().remoteIP().toString().c_str(),
          server.client().remotePort()
        );

    String message(buffer);

    server.send(200, "application/json", message );
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
