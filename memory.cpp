#include "memory.h"

memory::memory(){}


//iniciar la memoria del controlador
bool memory::begin(){
  bool success=true;
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Failed");
    success=false;
  }
  return success;
}

void memory::getServerConfig(int type, String arg[], memory memory, timeClock timeClock, const char* conf){
    switch(type){
      //guardar apn
      case 0:{
        Serial.println("configurando APN");
        memory.setApn(arg[0]);
        memory.setApn_user(arg[1]);
        memory.setApn_pass(arg[2]);
        Serial.print(memory.getApn());
      }break;
      //guardar ap
      case 1:{
        Serial.println("configurando AP");
        memory.setSsid_AP(arg[0]);
        memory.setPasswd_AP(arg[1]);
//        serverWifi.AP_connect(memory);
      }break;
      //guardar sta
      case 2:{
        Serial.println("configurando STA");
        memory.setSsid_STA(arg[0]);
        memory.setPasswd_STA(arg[1]);
        //serverWifi.STA_connect(memory, timeClock);
      }break;
      //guardar la hora
      case 3:{
        Serial.println("configurando hora");
        int yeard=arg[0].substring(0,4).toInt();
        int mes=arg[0].substring(5,7).toInt()-1;
        int dia=arg[0].substring(8,10).toInt();
        int hora=arg[1].substring(0,2).toInt();
        int minu=arg[1].substring(3,6).toInt();
        timeClock.setTiempo(yeard, mes, dia, hora, minu);
      }break;
      //guardar el tiempo de espera
      case 4:{
        Serial.println("configurando time");
        memory.setTime_to_sleep(arg[0].toInt());
      }break;
    }
  memory.writeConfiguration(conf);
}

bool memory::existFile(const char * path){
  bool success=true;
  if(!SPIFFS.exists(path)){
    success=false;
  }
  return success;
}

float memory::sizeFiles(){
  float Size=0;
  File root=SPIFFS.open("/");
  File file = root.openNextFile();
  while(file){
      if(!file.isDirectory()){
          Size+=file.size();
      }
      file = root.openNextFile();
  }
  Serial.print("Memoria usada:");
  Serial.print(Size/1024);
  Serial.println(" KB/4096 KB");
  return Size;
}

void memory::deleteFile(const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(SPIFFS.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}

// leer la configuacion si existe en la memoria del controlador
// en caso contrario colocar valores por defecto 
bool memory::readConfiguration(const char * filename){
  DynamicJsonDocument doc(1500);
  
  bool success=true;
  File file = SPIFFS.open(filename);

  if(!file){
    Serial.println("No existe el archivo. Configurando por defecto!");
    success=false;
  }
  
  DeserializationError errorD = deserializeJson(doc, file);
  
  if (errorD){
    Serial.println("Error al deserializar el archivo.");
    Serial.println(errorD.c_str());
    success=false;
  }

  strlcpy(sim.apn, doc["sim"][0] | "", sizeof(sim.apn)); 
  strlcpy(sim.user, doc["sim"][1] | "", sizeof(sim.user)); 
  strlcpy(sim.pass, doc["sim"][2] | "", sizeof(sim.pass)); 
  strlcpy(STA.ssid, doc["sta"][0] | "", sizeof(STA.ssid)); 
  strlcpy(STA.pass, doc["sta"][1] | "", sizeof(STA.pass));
  strlcpy(AP.ssid, doc["ap"][0] | "9-COCO2CH4", sizeof(AP.ssid)); 
  strlcpy(AP.pass, doc["ap"][1] | "12345678", sizeof(AP.pass));
  
  CO2.RO=doc["RoCO2"] | 18496.15;
  CH4.RO=doc["RoCH4"] | 6765.0;
  CO.RO=doc["RoCO"] | 320.0;
  CO2.ATM=doc["atmCO2"] | 392.57;
  CH4.ATM=doc["atmCH4"] | 1845.0;
  CO.ATM=doc["atmCO"] | 1.0;
  TIME_TO_SLEEP=doc["sleep"] | 1;
  Mode=doc["mode"] | 3;

  file.close();
  delay(1000);
  return success;
}

// Guardar los datos del programa en la memoria interna del controlador
bool memory::writeConfiguration(const char * filename){
  DynamicJsonDocument doc(1500);
  SPIFFS.remove(filename);
  delay(100);
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("no se pudo abrir el archivo.");
    return false;
  }
  
  JsonArray SIM=doc.createNestedArray("sim");
  SIM.add(sim.apn);
  SIM.add(sim.user);
  SIM.add(sim.pass);

  JsonArray sta=doc.createNestedArray("sta");
  sta.add(STA.ssid);
  sta.add(STA.pass);

  JsonArray ap=doc.createNestedArray("ap");
  ap.add(AP.ssid);
  ap.add(AP.pass);

  doc["RoCO2"]=(String)CO2.RO;
  doc["RoCH4"]=(String)CH4.RO;
  doc["RoCO"]=(String)CO.RO;
  doc["atmCO2"]=(String)CO2.ATM;
  doc["atmNH4"]=(String)CH4.ATM;
  doc["atmCO"]=(String)CO.ATM;
  doc["sleep"]=(String)TIME_TO_SLEEP;
  doc["mode"]=(String)Mode;
  
  if (serializeJson(doc, file) == 0) {
    Serial.println("no se pudo guardar configuracion.");
    return false;
  }
  
  file.close();
  delay(1000);
  return true;
}

char* memory::getSsid_AP(){
  return AP.ssid;
}
void memory::setSsid_AP(String ssid){
  ssid.toCharArray(AP.ssid,sizeof(AP.ssid));
}
char* memory::getPasswd_AP(){
  return AP.pass;
}
void memory::setPasswd_AP(String passwd){
  passwd.toCharArray(AP.pass,sizeof(AP.pass));
}
char* memory::getSsid_STA(){
  return STA.ssid;
}
void memory::setSsid_STA(String ssid){
  ssid.toCharArray(STA.ssid, sizeof(STA.ssid));
}
char* memory::getPasswd_STA(){
  return STA.pass;
}
void memory::setPasswd_STA(String passwd){
  passwd.toCharArray(STA.pass,sizeof(STA.pass));
}

char* memory::getApn(){
  return sim.apn;
}
void memory::setApn(String Apn){
  Apn.toCharArray(sim.apn, sizeof(sim.apn));
}
char* memory::getApn_user(){
  return sim.user;
}
void memory::setApn_user(String user){
  user.toCharArray(sim.user, sizeof(sim.user));
}
char* memory::getApn_pass(){
  return sim.pass;
}
void memory::setApn_pass(String pass){
  pass.toCharArray(sim.pass, sizeof(sim.pass));
}

int memory::getTime_to_sleep(){
  return TIME_TO_SLEEP;
}
void memory::setTime_to_sleep(int time_to_sleep){
  TIME_TO_SLEEP=time_to_sleep;
}

//getter a setter de sensores 

//BME sensor
int memory::getHumidity(){
  return BME.humidity;
}
void memory::setHumidity(int humidity){
  BME.humidity=humidity;
}
int memory::getTemperature(){
  return BME.temperature;
}
void memory::setTemperature(int temperature){
  BME.temperature=temperature;
}
int memory::getHeight(){
  return BME.height;
}
void memory::setHeight(int height){
  BME.height=height;
}
int memory::getPressure(){
  return BME.pressure;
}
void memory::setPressure(int pressure){
  BME.pressure=pressure;
}

//co2 sensor

int memory::getCo2(){
  return CO2.co2;
}
void memory::setCo2(int co2){
  CO2.co2=co2;
}
int memory::getCo2_ro(){
  return CO2.RO;
}
void memory::setCo2_ro(int ro){
  CO2.RO=ro;
}
int memory::getCo2_Atm(){
  return CO2.ATM;
}
void memory::setCo2_Atm(int Atm){
  CO2.ATM=Atm;
}

//co sensor

int memory::getCo(){
  return CO.co;
}
void memory::setCo(int co){
  CO.co=co;
}
int memory::getCo_ro(){
  return CO.RO;
}
void memory::setCo_ro(int ro){
  CO.RO=ro;
}
int memory::getCo_Atm(){
  return CO.ATM;
}
void memory::setCo_Atm(int Atm){
  CO.ATM=Atm;
}

//ch4 sensor 

int memory::getCh4(){
  return CH4.ch4;
}
void memory::setCh4(int ch4){
  CH4.ch4=ch4;
}
int memory::getCh4_ro(){
  return CH4.RO;
}
void memory::setCh4_ro(int ro){
  CH4.RO=ro;
}
int memory::getCh4_Atm(){
  return CH4.ATM;
}
void memory::setCh4_Atm(int Atm){
  CH4.ATM=Atm;
}
