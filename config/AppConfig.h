#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

/*
  Sample application configuration file

  To protect your credetials from accidentally pushing it to github,
  copy this script into the 'private' folder.
   
  You have to create the 'private' folder in the project root by your own.
*/

#define OTA_HOSTNAME "esp8266-info"
#define OTA_PASSWORD "otapass"

#define TIMEZONE "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00"
#define NTP_SERVER1 "ptbtime1.ptb.de"
#define NTP_SERVER2 "ptbtime2.ptb.de"
#define NTP_SERVER3 "de.pool.ntp.org"

#define WIFI_SSID "<your wifi ssid>"
#define WIFI_PASS "<your wifi password>"

#endif
