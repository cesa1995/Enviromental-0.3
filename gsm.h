#ifndef gsm_h
#define gsm_h

#define TINY_GSM_MODEM_SIM800
#include <Arduino.h>
#include <TinyGsmClient.h>
#include <HardwareSerial.h>
#include "timeClock.h"
#include "memory.h"

class gsm{
  public:
    void GSM_begin(HardwareSerial SerialGsm, timeClock timeClock, TinyGsm modem, memory memory);
    void connect_GSM(TinyGsm modem, memory memory);
    void setTime_GSM(timeClock timeClock, TinyGsm modem);
    int get_state();
  private:
    int state;
};

#endif
