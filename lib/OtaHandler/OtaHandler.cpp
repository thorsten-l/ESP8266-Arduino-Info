#include <App.hpp>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "OtaHandler.hpp"
#include <Util.hpp>
#include <WifiHandler.hpp>

OtaHandler otaHandler;

void OtaHandler::setup()
{
  LOG0("OTA Setup started...\n");

  ArduinoOTA.setHostname(wifiHandler.getHostname());
  ArduinoOTA.setPassword(OTA_PASSWORD);
  inProgress = false;

  ArduinoOTA.onStart([]()
  {
    Serial.println("\nOTA Start");
    otaHandler.inProgress = true;
  });

  ArduinoOTA.onEnd([]()
  {
#ifdef WIFI_LED
    digitalWrite(WIFI_LED,HIGH);
#endif
    Serial.println("\nOTA End\n");
    otaHandler.inProgress = false;
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    Serial.printf("\rOTA Progress: %u%%", (progress / (total / 100)));
#ifdef WIFI_LED
    alterPin(WIFI_LED);
#endif
  });

  ArduinoOTA.onError([](ota_error_t error)
  {
#ifdef WIFI_LED
    digitalWrite(WIFI_LED,LOW);
#endif
    otaHandler.inProgress = false;
    Serial.printf("OTA Error[%u]: ", error);

    if (error == OTA_AUTH_ERROR)
      Serial.println("OTA Auth Failed");
    else
    if (error == OTA_BEGIN_ERROR)
      Serial.println("OTA Begin Failed");
    else
    if (error == OTA_CONNECT_ERROR)
      Serial.println("OTA Connect Failed");
    else
    if (error == OTA_RECEIVE_ERROR)
      Serial.println("OTA Receive Failed");
    else
    if (error == OTA_END_ERROR)
      Serial.println("OTA End Failed");
  });

  ArduinoOTA.begin();
  MDNS.addServiceTxt("arduino", "tcp", "fw_name", APP_NAME );
  MDNS.addServiceTxt("arduino", "tcp", "fw_version", APP_VERSION );
}
