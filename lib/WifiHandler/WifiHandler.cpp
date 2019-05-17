#include "WifiHandler.hpp"
#include <App.hpp>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Util.hpp>

WifiHandler wifiHandler;

const char *phyModes[] = {"11B", "11G", "11N"};

void WifiHandler::wifiInitStationMode() {
  LOG0("Starting Wifi in Station Mode\n");
  sprintf( hostname, "%s-%06X", OTA_HOSTNAME, ESP.getChipId() );
  WiFi.persistent(false);
  WiFi.disconnect(true);
  delay(200);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostname);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    alterPin(WIFI_LED);
    Serial.print(".");
  }
  Serial.println("\n");
  connectCounter++;

  Serial.println("WiFi connected to : " WIFI_SSID);
  Serial.printf("WiFi connection # : %d\n", connectCounter );
  Serial.printf("WiFi Channel      : %d\n", WiFi.channel());
  Serial.printf("WiFi phy mode     : %s\n", getPhyMode() );

  WiFi.macAddress(mac);
  sprintf( macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],
                mac[1], mac[2], mac[3], mac[4], mac[5]);

  Serial.printf("WiFi MAC Address  : %s\n", macAddress);

  Serial.printf("WiFi Hostname     : %s\n", WiFi.hostname().c_str());
  Serial.print("WiFi IP-Address   : ");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi Gateway-IP   : ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("WiFi Subnetmask   : ");
  Serial.println(WiFi.subnetMask());
  Serial.print("WiFi DNS Server   : ");
  Serial.println(WiFi.dnsIP());
  Serial.println();

  digitalWrite(WIFI_LED, LOW);
}

int WifiHandler::getConnectCounter() {
  return connectCounter;
}

const char* WifiHandler::getMacAddress()
{
  return macAddress;
}

const char* WifiHandler::getPhyMode()
{
  return phyModes[WiFi.getPhyMode() - 1];
}

const char* WifiHandler::getHostname()
{
  return hostname;
}