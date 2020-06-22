#include "serverWifi.h"

serverWifi::serverWifi(){}

void serverWifi::begin(memory memory, timeClock timeClock){
  WiFi.mode(WIFI_AP_STA);
  AP_connect(memory);
  delay(1000);
  STA_connect(memory, timeClock);
}

void serverWifi::AP_connect(memory memory){
  WiFi.softAPdisconnect();
  Serial.print("Configurando AP: ");
  Serial.println(memory.getSsid_AP());
  WiFi.softAP(memory.getSsid_AP(), memory.getPasswd_AP());
}

void serverWifi::STA_connect(memory memory, timeClock timeClock){
  WiFi.disconnect();
  if(memory.existFile("/config.txt") && memory.getSsid_STA()!=""){
    Serial.print("Conectandose a: ");
    Serial.println(memory.getSsid_STA());
    WiFi.begin(memory.getSsid_STA(), memory.getPasswd_STA());

    int trys=0;
    do{
      delay(500);
      Serial.print(".");
      trys++;
    }while(WiFi.status() != WL_CONNECTED && trys<=10);

    if(WiFi.status() == WL_CONNECTED){
      Serial.print("direccion IP: ");
      Serial.println(WiFi.localIP());

    timeClock.begin();
      
    }else{
      Serial.println("Error al conectar con el punto de acceso");
    }
  }else{
    Serial.println("No existe el archivo de configuracion");
  }
}
