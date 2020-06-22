#ifndef things_h
#define things_h

#include "timeClock.h"
#include "memory.h"
#include "Arduino.h"
#include "serverWifi.h"

class things{
  public:
    void getServerConfig(int type, String arg[], memory memory, serverWifi serverWifi, timeClock timeClock);
};

#endif
