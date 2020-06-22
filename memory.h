#ifndef memory_h
#define memory_h

#include "Arduino.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"
#include "timeClock.h"

class memory{
  public:
    memory();
    bool begin();
    float sizeFiles();
    void deleteFile(const char * path);
    bool readConfiguration(const char * filename);
    bool writeConfiguration(const char * filename);
    bool existFile(const char * path);

    void getServerConfig(int type, String arg[], memory memory, timeClock timeClock, const char* conf);
    
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
    
    int getTime_to_sleep();
    void setTime_to_sleep(int time_to_sleep);

    int getHumidity();
    void setHumidity(int humidity);
    int getTemperature();
    void setTemperature(int temperature);
    int getHeight();
    void setHeight(int height);
    int getPressure();
    void setPressure(int pressure);

    int getCo2();
    void setCo2(int co2);
    int getCo2_ro();
    void setCo2_ro(int ro);
    int getCo2_Atm();
    void setCo2_Atm(int Atm);

    int getCo();
    void setCo(int co);
    int getCo_ro();
    void setCo_ro(int ro);
    int getCo_Atm();
    void setCo_Atm(int Atm);

    int getCh4();
    void setCh4(int ch4);
    int getCh4_ro();
    void setCh4_ro(int ro);
    int getCh4_Atm();
    void setCh4_Atm(int Atm);
    
    
  private:
    int trys;
    int TIME_TO_SLEEP;
    int Mode;

    struct BME{
      int humidity;
      int height;
      int temperature;
      int pressure;
    };
    
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

    struct CO{
      int co;
      int RO;
      int ATM;
    };

    struct CO2{
      int co2;
      int RO;
      int ATM;
    };

    struct CH4{
      int ch4;
      int RO;
      int ATM;
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
