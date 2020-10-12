#ifndef timeClock_h
#define timeClock_h

#include "Arduino.h"
#include <time.h>
#include <sys/time.h>

class timeClock{
  public:
    bool begin();
    String getTiempo();
    void setTiempo(int set_year, int set_mon, int set_mday, int set_hour, int set_min);
    bool saveWifiTime();
  private:
    const char* ntpServer = "0.south-america.pool.ntp.org";
    const long  gmtOffset_sec = -14400;
    const int   daylightOffset_sec = 0;
};

#endif
