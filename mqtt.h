#ifndef mqtt_h
#define mqtt_h

#include <Arduino.h>
#include <PubSubClient.h>
#include "memory.h"

class mqtt{
  public:
    void mqtt_connect(PubSubClient mqtt, char broker[], const char* TOKEN, String FIRMWARE, String NUMERO_SERIAL);
    void mqttSend_data(memory memory, PubSubClient mqtt);
  private:

};

#endif
