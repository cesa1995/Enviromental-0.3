#include "gsm.h"

void gsm::GSM_begin(HardwareSerial SerialGsm, timeClock timeClock, TinyGsm modem, memory memory){
  Serial.println("Iniciando Modem");
  if(!TinyGsmAutoBaud(SerialGsm)){
    state=0;
    Serial.println(state);
  }else{
    delay(100);
    if(modem.getSimStatus()==1){
      state=4;
      connect_GSM(modem, memory);
      setTime_GSM(timeClock, modem);
      
    }else{
      state=1;
    }
    Serial.println(state);
  }
  Serial.println("finalizando configuracion del Modem");
}

void gsm::connect_GSM(TinyGsm modem, memory memory){
  Serial.println("Initializing modem...");
  modem.restart();
  Serial.print("Modem: ");
  Serial.println(modem.getModemInfo());
  Serial.print("Waiting for network...");
  modem.waitForNetwork();
  Serial.println(" OK");
  Serial.print("Connecting to ");
  Serial.print(memory.getApn());
  int i=0;
  while(!modem.isGprsConnected() && i<10){
    modem.gprsConnect(memory.getApn(),memory.getApn_user(),memory.getApn_pass());
    i++;
    Serial.print(".");
    delay(2000);
  }
}

void gsm::setTime_GSM(timeClock timeClock, TinyGsm modem){
  if (modem.isGprsConnected()) {
    int i=0;
    Serial.println("Getting date...");
    while(!modem.syncTime(-16) && i<10){
      Serial.print(".");
      i++;
    }
    delay(500);
    if(i<10){
      String fecha=modem.getGSMDateTime(DATE_FULL);
      int yy=fecha.substring(0, fecha.indexOf("/")).toInt()+2000;
      fecha=fecha.substring(fecha.indexOf("/")+1, fecha.length());
      int mm=fecha.substring(0, fecha.indexOf("/")).toInt()-1;
      int dd=fecha.substring(fecha.indexOf("/")+1, fecha.indexOf(",")).toInt();
      int hh=fecha.substring(fecha.indexOf(",")+1, fecha.indexOf(":")).toInt();
      fecha=fecha.substring(fecha.indexOf(":")+1, fecha.length());
      int mi=fecha.substring(0, fecha.indexOf(":")).toInt();
      String date=String(yy)+"-"+String(mm)+"-"+String(dd)+"/"+String(hh)+";"+String(mi);
      Serial.println(date);
      timeClock.setTiempo(yy,mm,dd,hh,mi);
      Serial.println(timeClock.getTiempo());
    }else{
      state=2;
    }
  }else{
    state=3;
  }
}

int gsm::get_state(){
  return state;
}
