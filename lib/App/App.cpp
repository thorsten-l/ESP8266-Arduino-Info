
#include "App.hpp"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Esp.h>
#include <WifiHandler.hpp>
#include <OtaHandler.hpp>
#include <WebHandler.hpp>
#include <Util.hpp>

void appSetup()
{
  int bootDevice = getBootDevice();

#ifdef WIFI_LED
  pinMode(WIFI_LED, OUTPUT);
  digitalWrite(WIFI_LED, HIGH);
#endif
#ifdef BUTTON_PIN
  pinMode(BUTTON_PIN, INPUT_PULLUP);
#endif
  pinMode(POWER_LED, OUTPUT);
  digitalWrite(POWER_LED, LOW);

  Serial.begin(74880); // Same rate as the esp8266 bootloader
  delay(3000);         // wait for PlatformIO to start the serial monitor

  Serial.println();
  Serial.println();
  Serial.println(APP_NAME " - " APP_VERSION " - " APP_AUTHOR);
  Serial.println("BUILD: " __DATE__ " " __TIME__);
  Serial.printf("\nBoot device = %d\n", bootDevice);
  Serial.printf("Boot mode = %d\n", ESP.getBootMode());
  Serial.printf("Boot version = %d\n", ESP.getBootVersion());

  if (bootDevice == 1) {
    Serial.println(F("\nThis sketch has just been uploaded over the UART.\n"
                     "The ESP8266 will freeze on the first restart.\n"
                     "Press the reset button or power cycle the ESP\n"
                     "and operation will be resumed thereafter."));
  }

  Serial.println();

  showChipInfo();

  configTime( TIMEZONE, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3 );

  wifiHandler.wifiInitStationMode();
  bool timeNotSet = true;
  struct tm timeinfo;

  for( int i=0; i<10; i++ )
  {
    time_t now = time(nullptr);
    localtime_r( &now, &timeinfo );
    if ( timeinfo.tm_year >= 120 )
    {
      timeNotSet = false;
      break;
    }
    else
    {
      delay( 1000 );
    }
  }

  Serial.printf( "Time              : %4d-%02d-%02d %02d:%02d:%02d\n",
     timeinfo.tm_year+1900, timeinfo.tm_mon+1, timeinfo.tm_mday,
     timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec );
  Serial.print("Timezone          : ");
  Serial.println(getenv("TZ"));

  if ( timeNotSet )
  {
    Serial.println("Time not set");
  }

  Serial.println();

  otaHandler.setup();
  webHandler.setup();
}

void appLoop()
{
  static unsigned int counter = 0;
  static time_t timestamp = 0;

  ArduinoOTA.handle();

  if (otaHandler.inProgress == false) {
    time_t currentTimestamp = millis();

    if ((currentTimestamp - timestamp) >= 2000) {
      Serial.printf("\r%d ", counter++);
      timestamp = currentTimestamp;

      alterPin(POWER_LED);

#ifdef BUTTON_PIN
  Serial.print( digitalRead(BUTTON_PIN));
#endif

      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection lost");
        wifiHandler.wifiInitStationMode();
      }
    }

    webHandler.handle();
  }
}