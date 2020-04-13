#include <App.hpp>
#include <Arduino.h>

void setup() {
  appSetup();
}

void loop() {
  appLoop();
  delay(20); // time for IP stack
}
