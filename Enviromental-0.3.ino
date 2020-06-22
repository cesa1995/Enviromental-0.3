#include "timeClock.h"
#include "serverWifi.h"
#include "memory.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "things.h"
#include "ArduinoJson.h"

memory memory;
AsyncWebServer server(80);
serverWifi se;
timeClock timeClock;
things things;

const char* conf="/config.txt";
const char* data="/data.txt";

String processor(const String& var){

  if(var=="apn"){
    return memory.getApn();
  }

  if(var=="user"){
    return memory.getApn_user();
  }

  if(var=="passApn"){
    return memory.getApn_pass();
  }

  if(var=="ssidSta"){
    return memory.getSsid_STA();
  }

  if(var=="passSta"){
    return memory.getPasswd_STA();
  }

  if(var=="ssidAp"){
    return memory.getSsid_AP();
  }

  if(var=="passAp"){
    return memory.getPasswd_AP();
  }

  if(var=="hour"){
    return timeClock.getTiempo();
  }

  if(var=="time"){
    return (String)memory.getTime_to_sleep();
  }

  if(var=="co2"){
    return (String)memory.getCo2();
  }

  if(var=="co"){
    return (String)memory.getCo();
  }

  if(var=="ch4"){
    return (String)memory.getCh4();
  }

  if(var=="humedity"){
    return (String)memory.getHumidity();
  }

  if(var=="temperature"){
    return (String)memory.getTemperature();
  }

  if(var=="presion"){
    return (String)memory.getPressure();
  }

  if(var=="higth"){
    return (String)memory.getHeight();
  }
  
  return String();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //montar la memoria interna del micro
  if(!memory.begin()){
    Serial.println("error al montar memoria spiffs");
    return;
  }
  memory.readConfiguration(conf);
  memory.writeConfiguration(conf);
  Serial.println(memory.sizeFiles());
  se.begin(memory, timeClock);  
  // index.html
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // logo.ico
  server.on("/logo.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logo.ico", "image/x-ico");
  });

  // index.css
  server.on("/stylesheet/index.css", HTTP_GET, [](AsyncWebServerRequest *request){    
    
    request->send(SPIFFS, "/stylesheet/index.css","text/css");
  });

  // index.css
  server.on("/stylesheet/ap.css", HTTP_GET, [](AsyncWebServerRequest *request){    
    request->send(SPIFFS, "/stylesheet/ap.css", "text/css");
  });

  //iconos.png
  server.on("/stylesheet/iconos.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/stylesheet/iconos.png", "image/png");  
  });

  //menu
  server.on("/stylesheet/menu.png", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/stylesheet/menu.png", "image/png");  
  });
  
  // ap.html
  server.on("/ap.html", HTTP_GET, [](AsyncWebServerRequest *request){    
    request->send(SPIFFS, "/ap.html", String());
  });

  //apn.html
  server.on("/apn.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/apn.html", String());  
  });

  //calibrar.html
  server.on("/calibrar.html", HTTP_GET, [](AsyncWebServerRequest *request){
    if(request->hasParam("type")){
    
    }
    request->send(SPIFFS, "/calibrar.html", String());  
  });

  //hour.html
  server.on("/hour.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/hour.html", String());  
  });

  //sta.html
  server.on("/sta.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/sta.html", String());  
  });

  //time.html
  server.on("/time.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/time.html", String());  
  });

  //download.html
  server.on("/download.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/download.html", String());  
  });

  //erase_data.html
  server.on("/erase_data.html", HTTP_GET, [](AsyncWebServerRequest *request){
    if(request->hasParam("type")){
      if(request->getParam("type")->value()=="datos"){
        memory.deleteFile(data);
      }
      if(request->getParam("type")->value()=="config"){
        Serial.println("eliminando configuracion.");
        memory.deleteFile(conf);
        memory.readConfiguration(conf);
        memory.writeConfiguration(conf);
      }
    }
    request->send(SPIFFS, "/erase_data.html", String());  
  }); 

  //save.html
  server.on("/save.html", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasParam("type", true)){
      Serial.print("guardando...");
      String argss[3];
      argss[0]=request->getParam("0", true)->value();
      if(request->hasParam("1", true)){
        argss[1]=request->getParam("1", true)->value();
      }
      if(request->hasParam("2", true)){
        argss[2]=request->getParam("2", true)->value();
      }
      int type=request->getParam("type", true)->value().toInt(); 
      Serial.println(type);
      memory.getServerConfig(type, argss, memory ,timeClock, conf);  
    }
    request->send(SPIFFS, "/save.html", String());  
  }); 

  // Start server
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
