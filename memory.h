#ifndef memory_h
#define memory_h

#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "timeClock.h"

class memory{
  public:
    bool begin();
    float calculateMemorySize();
    String memoryPercent();
    void deleteFile(const char * path);
    bool readConfiguration(const char * filename);
    bool writeConfiguration(const char * filename);
    bool existFile(const char * path);
    void saveData(timeClock timeClock, const char * path);
    void writeFile(const char * path, const char * message);
    void appendFile(const char * path, const char * message);
 
    char* getSsid_AP();
    void setSsid_AP(String ssid);
    char* getPasswd_AP();
    void setPasswd_AP(String passwd);
    char* getSsid_STA();
    void setSsid_STA(String ssid);
    char* getPasswd_STA();
    void setPasswd_STA(String passwd);
    char* getApn();
    void setApn(String Apn);
    char* getApn_user();
    void setApn_user(String user);
    char* getApn_pass();
    void setApn_pass(String pass);
    
    float getTime_to_sleep();
    void setTime_to_sleep(float time_to_sleep);

    float getHumidity();
    void setHumidity(int humidity);
    float getTemperature();
    void setTemperature(int temperature);
    float getHeight();
    void setHeight(int height);
    float getPressure();
    void setPressure(int pressure);

    int getCo2();
    void setCo2(int co2);
    int getCo2_ro();
    void setCo2_ro(int ro);
    int getCo2_Atm();
    void setCo2_Atm(int Atm);
    float getCo2_voltage();
    void setCo2_voltage(float vol);
    int getCo2_state();
    void setCo2_state(int state);

    int getCo();
    void setCo(int co);
    int getCo_ro();
    void setCo_ro(int ro);
    int getCo_Atm();
    void setCo_Atm(int Atm);
    float getCo_voltage();
    void setCo_voltage(float vol);
    int getCo_state();
    void setCo_state(int state);

    int getCh4();
    void setCh4(int ch4);
    int getCh4_ro();
    void setCh4_ro(int ro);
    int getCh4_Atm();
    void setCh4_Atm(int Atm);
    float getCh4_voltage();
    void setCh4_voltage(float vol);
    int getCh4_state();
    void setCh4_state(int state);
    
    
  private:
    float TIME_TO_SLEEP;

    struct wifiAP {
      char ssid[50];
      char pass[50];
    };

    //Wifi Sta configuracion
    struct wifiSTA {
      char ssid[50];
      char pass[50];
    };

    //Apn Configuracion
    struct SIM {
      char apn[50];
      char user[50];
      char pass[50];
    };
    
    struct BME{
      float humidity;
      float height;
      float temperature;
      float pressure;
    };

    struct CO{
      int co;
      int RO;
      int ATM;
      float voltage;
      int state;
    };

    struct CO2{
      int co2;
      int RO;
      int ATM;
      float voltage;
      int state;
    };

    struct CH4{
      int ch4;
      int RO;
      int ATM;
      float voltage;
      int state;
    };

    wifiSTA STA;
    SIM sim;
    wifiAP AP;
    CO CO;
    CO2 CO2;
    CH4 CH4;
    BME BME;
    
};

#endif
