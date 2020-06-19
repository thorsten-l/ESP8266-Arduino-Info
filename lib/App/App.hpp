#ifndef __APP_H__
#define __APP_H__

#include <Arduino.h>
#include <AppConfig.h>

#define LOG0( format ) Serial.printf( "(%ld) " format, millis())
#define LOG1( format, x) Serial.printf( "(%ld) " format, millis(), x )
#define TLOG0( format ) TelnetStream.printf( "(%ld) " format, millis())
#define TLOG1( format, x) TelnetStream.printf( "(%ld) " format, millis(), x )

#define APP_NAME "ESP8266 Arduino Info"
#define APP_VERSION "0.9.1"
#define APP_AUTHOR "Dr. Thorsten Ludewig <t.ludewig@gmail.com>"
#define APP_CONFIG_FILE "/config.bin"

extern void appSetup();
extern void appLoop();
extern void appShowHeader(Stream& out);
extern const char *appUptime();
extern const char *appDateTime();

#endif
