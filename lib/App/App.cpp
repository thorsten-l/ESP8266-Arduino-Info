
#include "App.hpp"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Esp.h>
#include <TelnetStream.h>

#include <WifiHandler.hpp>
#include <OtaHandler.hpp>
#include <WebHandler.hpp>
#include <Util.hpp>

#include <FS.h>
#include <LittleFS.h>

time_t appStartTime = 0l;
struct tm appTimeinfo;
char appUptimeBuffer[64];
char appLocalIpBuffer[32];
char appDateTimeBuffer[32];

char *appBuffer;

const char *appDateTime()
{
  time_t now = time(nullptr);
  localtime_r( &now, &appTimeinfo );
  sprintf( appDateTimeBuffer, "%4d-%02hd-%02hd %02hd:%02hd:%02hd", 
  appTimeinfo.tm_year+1900, appTimeinfo.tm_mon+1, appTimeinfo.tm_mday,
  appTimeinfo.tm_hour, appTimeinfo.tm_min, appTimeinfo.tm_sec);
  return appDateTimeBuffer;
}

const char *appUptime()
{
  time_t uptime = millis() / 1000;
  int uptimeSeconds = uptime % 60;
  int uptimeMinutes = (uptime / 60) % 60;
  int uptimeHours = (uptime / 3600) % 24;
  time_t uptimeDays = (uptime / 86400);

  sprintf( appUptimeBuffer, 
    "%llu days, %d hours, %d minutes, %d seconds",
    uptimeDays, uptimeHours, uptimeMinutes, uptimeSeconds );

  return appUptimeBuffer;
}

void appShowHeader(Stream& out)
{
  out.println( "\n\n" APP_NAME " - " APP_VERSION " - " APP_AUTHOR);
  out.println("BUILD: " __DATE__ " " __TIME__);
  out.println("PIOENV: " PIOENV );
  out.println("PIOPLATFORM: " PIOPLATFORM );
  out.println("PIOFRAMEWORK: " PIOFRAMEWORK );
  out.printf( "ESP SDK Version: %s\n", ESP.getSdkVersion() );
  out.printf( "ESP Core Version: %s\n\n", ESP.getCoreVersion().c_str() );
}

void appSetup()
{
  int bootDevice = getBootDevice();
  appBuffer = NULL;

#ifdef WIFI_LED
  pinMode(WIFI_LED, OUTPUT);
  digitalWrite(WIFI_LED, HIGH);
#endif
#ifdef BUTTON_PIN
  pinMode(BUTTON_PIN, INPUT_PULLUP);
#endif

#ifdef POWER_LED
  pinMode(POWER_LED, OUTPUT);
  digitalWrite(POWER_LED, LOW);
#endif

  Serial.begin(115200); // Same rate as the esp8266 bootloader
  delay(3000);         // wait for PlatformIO to start the serial monitor

  appShowHeader( Serial );
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

  if(LittleFS.begin())
  {    
    if ( !LittleFS.exists( APP_CONFIG_FILE ))
    {
      LOG0( "Config-File <" APP_CONFIG_FILE "> not exists.\n" );
      LOG0( "Formatting LittleFS... " );
      LittleFS.format();
      Serial.println( "done.\n");
      File config = LittleFS.open( APP_CONFIG_FILE, "w" );
      config.write(0);
      config.close();
      WiFi.persistent(false);
      WiFi.disconnect(true);
      WiFi.mode( WIFI_OFF );
      ESP.eraseConfig();
      ESP.reset();
    }
    else
    {
      LOG0( "Config-File <" APP_CONFIG_FILE "> found.\n\n" );
    }
    
    LittleFS.end();
  }

  showChipInfo();

  configTime( TIMEZONE, NTP_SERVER1, NTP_SERVER2, NTP_SERVER3 );

  wifiHandler.wifiInitStationMode();
  strcpy( appLocalIpBuffer, WiFi.localIP().toString().c_str());
  Telnet.begin();

  bool timeNotSet = true;
  struct tm timeinfo;

  for( int i=0; i<15; i++ )
  {
    time_t now = time(nullptr);
    localtime_r( &now, &timeinfo );
    if ( timeinfo.tm_year >= 120 )
    {
      timeNotSet = false;
      appStartTime = now - (millis()/1000);
      break;
    }
    else
    {
      delay( 1000 );
    }
  }

  Serial.printf( "Time              : %s\n", appDateTime());
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
  // static unsigned int counter = 0;
  static time_t timestamp = 0;

  ArduinoOTA.handle();

  if (otaHandler.inProgress == false) 
  {
    time_t currentTimestamp = millis();
    Telnet.handle();

    if ((currentTimestamp - timestamp) >= 2000) 
    {
      // Serial.printf("\r%d ", counter++);
      timestamp = currentTimestamp;
      Serial.printf("free heap %d\n", ESP.getFreeHeap());

#ifdef POWER_LED
      alterPin(POWER_LED);
#endif

#ifdef BUTTON_PIN
  Serial.print( digitalRead(BUTTON_PIN));
#endif

      if (WiFi.status() != WL_CONNECTED) {
        Serial.printf("\n[%s] WiFi connection lost!\n", appDateTime());
        wifiHandler.wifiInitStationMode();
      }

      char currentLocalIp[32];
      strcpy( currentLocalIp, WiFi.localIP().toString().c_str());
      if ( strcmp( appLocalIpBuffer, currentLocalIp ) != 0 )
      {
        Serial.printf( "\n[%s] Local IP-Address has changed to: %s\n", 
          appDateTime(), currentLocalIp );
        strcpy( appLocalIpBuffer, currentLocalIp );
      }
    }

    webHandler.handle();
  }
}