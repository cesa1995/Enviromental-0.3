#include "timeClock.h"

timeClock::timeClock(){}

bool timeClock::begin(){
  bool success=true;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  int trys=0;
  Serial.print("Actualizando hora:");
  while(!saveWifiTime() && trys<5){
    trys++;
    Serial.print(".");
    delay(200);
  }
  if(trys>4){
    Serial.println("No se pudo actualizar la hora.");
    success=false;
  }else{
    Serial.println("Hora actualizada exitozamente.");
  }
  return success;
}

String timeClock::getTiempo(){
  String tiempo;
  time_t now = time(0);
  // Convert now to tm struct for local timezone
  tm* localtm = localtime(&now);
  tiempo=String(localtm->tm_year+1900)+"/"+String(localtm->tm_mday)+"/"+String(localtm->tm_mon+1)+" "+String(localtm->tm_hour)+":"+String(localtm->tm_min);
  //Serial.println(tiempo);
  return tiempo;
}

void timeClock::setTiempo(int set_year, int set_mon, int set_mday, int set_hour, int set_min){
  struct tm tm;
  tm.tm_year = set_year - 1900;//2018 - 1900;
  tm.tm_mon = set_mon;//10;
  tm.tm_mday = set_mday;//15;
  tm.tm_hour = set_hour;//14;
  tm.tm_min = set_min;//10;
  tm.tm_sec = 00;//10;
  time_t t = mktime(&tm);
  printf("Setting time: %s", asctime(&tm));
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}

bool timeClock::saveWifiTime(){
  boolean statust;
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    statust=false;
  }else{
    statust=true;
  }
  return statust;
}
