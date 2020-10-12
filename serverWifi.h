#ifndef serverWifi_h
#define serverWifi_h

#include <Arduino.h>
#include <WiFi.h>
#include "memory.h"
#include "timeClock.h"

class serverWifi{
  public:
    void begin(memory memory, timeClock timeClock);
    void AP_connect(memory memory);
    void STA_connect(memory memory, timeClock timeClock);
    int get_state();
  private:
    int state;
};

#endif
