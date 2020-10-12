String processor(const String& var){

// Ingresar valores de apn
  if(var=="simState"){
      if(gsm.get_state()==0){
        return "No Encendido";
      }
      if(gsm.get_state()==1){
        return "No SimCard";
      }
      if(gsm.get_state()==2){
        return "No Hora";
      }
      if(gsm.get_state()==3){
        return "No Datos";
      }
      if(gsm.get_state()==4){
        return "Ok";
      }
  }
  
  if(var=="apn"){
    return memory.getApn();
  }

  if(var=="user"){
    return memory.getApn_user();
  }

  if(var=="passApn"){
    return memory.getApn_pass();
  }

// Ingresar valores de modo estacion
  if(var=="ssidSta"){
    return memory.getSsid_STA();
  }

  if(var=="passSta"){
    return memory.getPasswd_STA();
  }

  if(var=="ipstation"){
    return WiFi.localIP().toString();
  }

// Ingresar valores de modo punto de acceso
  if(var=="ssidAp"){
    return memory.getSsid_AP();
  }

  if(var=="passAp"){
    return memory.getPasswd_AP();
  }

// Ingresar valores de tiempo
  if(var=="hour"){
    return timeClock.getTiempo();
  }

  if(var=="time"){
    return (String)memory.getTime_to_sleep();
  }

// Ingresar valores de memoria
  if(var=="memory"){
    return ((String)memory.calculateMemorySize())+" kb/7000 kb";
  }

  if(var=="memoryPercent"){
    return memory.memoryPercent(); 
  }

// Igresar valores de sensor BME 
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

//ingresar valores de ppm sensores mq
  if(var=="co2"){
    return (String)memory.getCo2();
  }

  if(var=="co"){
    return (String)memory.getCo();
  }

  if(var=="ch4"){
    return (String)memory.getCh4();
  }

// Ingresar valores R0 de los sensores mq
  if(var=="rco"){
    return (String)memory.getCo_ro();
  }

  if(var=="rco2"){
    return (String)memory.getCo2_ro();
  }

  if(var=="rch4"){
    return (String)memory.getCh4_ro();
  }

// Ingresar valores ATM(atmofericos) de los sensores mq
  if(var=="atmco"){
    return (String)memory.getCo_Atm();
  }

  if(var=="atmco2"){
    return (String)memory.getCo2_Atm();
  }

  if(var=="atmch4"){
    return (String)memory.getCh4_Atm();
  }

// Ingresar valores voltaje de los sensores mq 
  if(var=="volco"){
    return String(memory.getCo_voltage(),8);
  }

  if(var=="volco2"){
    return String(memory.getCo2_voltage(),8);
  }

  if(var=="volch4"){
    return String(memory.getCh4_voltage(),8);
  }

  // Ingresar valores del estado de los valores mq
  if(var=="stateCO2"){
    if(memory.getCo2_state()==1){
      return "Activo";
    }else{
      return "Inactivo";
    }
    
  }

  if(var=="stateCO"){
    if(memory.getCo_state()==1){
      return "Activo";
    }else{
      return "Inactivo";
    }
  }

  if(var=="stateCH4"){
    if(memory.getCh4_state()==1){
      return "Activo";
    }else{
      return "Inactivo";
    }
  }

  if(var=="gas"){
    if(memory.getCo_state()==1){
      return "CO";
    }else if(memory.getCo2_state()==1){
      return "CO2";
    }else if(memory.getCh4_state()==1){
      return "CH4";
    }
  }
  
  return String();
}

void startServers(){
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
    request->send(SPIFFS, "/calibrar.html", String(), false, processor);
  });

  server.on("/making_calibration.html", HTTP_POST, [](AsyncWebServerRequest *request){
    if(request->hasParam("gas", true)){
      String ATM=request->getParam("gas", true)->value();
      memory.set_calibrating(1);
      if(memory.getCo_state()==1){
        Serial.println("calibrando CO");
        memory.setCo_Atm(ATM.toInt());
        memory.setCo_ro(sensors.calibrateSensors(memory, adc));
      }else if(memory.getCo2_state()==1){
        Serial.println("calibrando CO2");
        memory.setCo2_Atm(ATM.toInt());
        memory.setCo2_ro(sensors.calibrateSensors(memory, adc));
      }else if(memory.getCh4_state()==1){
        Serial.println("calibrando CH4");
        memory.setCh4_Atm(ATM.toInt());
        memory.setCh4_ro(sensors.calibrateSensors(memory, adc));
      }
      memory.writeConfiguration(conf);
      memory.set_calibrating(0);
      request->send(SPIFFS, "/making_calibration.html", String(), false, processor);
    }
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
    request->send(SPIFFS, data, String(), true);  
  });

  //erase_data.html
  server.on("/erase_data.html", HTTP_GET, [](AsyncWebServerRequest *request){
    if(request->hasParam("type")){
      if(request->getParam("type")->value()=="datos"){
        Serial.println("eliminando datos");
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
      switch(type){
          //guardar apn
          case 0:{
            Serial.println("configurando APN");
            memory.setApn(argss[0]);
            memory.setApn_user(argss[1]);
            memory.setApn_pass(argss[2]);
          }break;
          //guardar ap
          case 1:{
            Serial.println("configurando AP");
            memory.setSsid_AP(argss[0]);
            memory.setPasswd_AP(argss[1]);
            se.AP_connect(memory);
          }break;
          //guardar sta
          case 2:{
            Serial.println("configurando STA");
            memory.setSsid_STA(argss[0]);
            memory.setPasswd_STA(argss[1]);
            se.STA_connect(memory, timeClock);
          }break;
          //guardar la hora
          case 3:{
            Serial.println("configurando hora");
            int yeard=argss[0].substring(0,4).toInt();
            int mes=argss[0].substring(5,7).toInt()-1;
            int dia=argss[0].substring(8,10).toInt();
            int hora=argss[1].substring(0,2).toInt();
            int minu=argss[1].substring(3,6).toInt();
            timeClock.setTiempo(yeard, mes, dia, hora, minu);
          }break;
          //guardar el tiempo de espera
          case 4:{
            Serial.println("configurando time");
            memory.setTime_to_sleep(argss[0].toInt());
          }break;
          //cambiar el estado de los sensores
          case 5:{
            int sensor=argss[0].toInt();
            switch(sensor){
              case 0:{
                if (memory.getCo2_state()==1){
                  memory.setCo2_state(0);
                }else{
                  memory.setCo2_state(1);
                  memory.setCo_state(0);
                  memory.setCh4_state(0);
                }
              }break;
              case 1:{
                if (memory.getCo_state()==1){
                  memory.setCo_state(0);
                }else{
                  memory.setCo2_state(0);
                  memory.setCo_state(1);
                  memory.setCh4_state(0);
                }
              }break;
              case 2:{
                if (memory.getCh4_state()==1){
                  memory.setCh4_state(0);
                }else{
                  memory.setCo2_state(0);
                  memory.setCo_state(0);
                  memory.setCh4_state(1);
                }
              }break;
            }
            memory.writeConfiguration(conf);
          }
        }
      memory.writeConfiguration(conf);
    }
    request->send(SPIFFS, "/save.html", String());  
  }); 
}
