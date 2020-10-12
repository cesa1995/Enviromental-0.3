#include "memory.h"


//iniciar la memoria del controlador
bool memory::begin(){
  bool success=true;
  if(!SPIFFS.begin(true)){
    Serial.println("SPIFFS Mount Failed");
    success=false;
  }
  CO.state=0;
  CO2.state=0;
  CH4.state=0;
  return success;
}

bool memory::existFile(const char * path){
  bool success=true;
  if(!SPIFFS.exists(path)){
    success=false;
  }
  return success;
}

void memory::saveData(timeClock timeClock, const char * path){
  //------------Save Data-----------
  String datos=timeClock.getTiempo()+"  "+String(getTemperature())+"  "+String(getHumidity())+" "+String(getPressure())+" "+String(getHeight())+" ";
  datos += String(getCo_voltage(),8)+" "+String(getCo())+" ";
  datos += String(getCh4_voltage(),8)+" "+String(getCh4())+" ";
  datos += String(getCo2_voltage(),8)+" "+String(getCo2())+"\n";
  int sizeStr=datos.length()+1;
  char toSave[sizeStr];
  datos.toCharArray(toSave, sizeStr);
  
  if(existFile(path)){
    appendFile(path, toSave);
  }else{
    writeFile(path, "Fecha  Hora  Temperatura(°C)  Humedad(%) Presion(hPa) Altitud(m) CO/MQ9(V) CO/MQ9(PPM) CH4/MQ4(V) COH4/MQ4(PPM) CO2/MQ135(V) CO2/MQ135(PPM)\n");
    appendFile(path, toSave);
  }
}

void memory::writeFile(const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = SPIFFS.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void memory::appendFile(const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = SPIFFS.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

float memory::calculateMemorySize(){
  float Size=0;
  File root=SPIFFS.open("/");
  File file = root.openNextFile();
  while(file){
      if(!file.isDirectory()){
          Size+=file.size();
      }
      file = root.openNextFile();
  }
  return Size/1024;
}

String memory::memoryPercent(){
  return ((String)((calculateMemorySize()*100)/7000))+" &#37";
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

  CO.RO=doc["co"][0] | 320.0;
  CO.ATM=doc["co"][1] | 1.0;
  CO.state=doc["co"][2] | 0;
  
  CO2.RO=doc["co2"][0] | 18496.15;
  CO2.ATM=doc["co2"][1] | 392.57;
  CO2.state=doc["co2"][2] | 0;
  
  CH4.RO=doc["ch4"][0] | 6765.0;
  CH4.ATM=doc["ch4"][1] | 1845.0;
  CH4.state=doc["ch4"][2] | 0;
  
  
  TIME_TO_SLEEP=doc["sleep"] | 1;

  file.close();

  file=SPIFFS.open(filename);
  
  while(file.available()){
    Serial.print(file.readString());
  }
  Serial.println();
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

  JsonArray co=doc.createNestedArray("co");
  co.add(CO.RO);
  co.add(CO.ATM);
  co.add(CO.state);

  JsonArray co2=doc.createNestedArray("co2");
  co2.add(CO2.RO);
  co2.add(CO2.ATM);
  co2.add(CO2.state);

  JsonArray ch4=doc.createNestedArray("ch4");
  ch4.add(CH4.RO);
  ch4.add(CH4.ATM);
  ch4.add(CH4.state);

  doc["sleep"]=TIME_TO_SLEEP;
  
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

float memory::getTime_to_sleep(){
  return TIME_TO_SLEEP;
}
void memory::setTime_to_sleep(float time_to_sleep){
  TIME_TO_SLEEP=time_to_sleep;
}

//getter a setter de sensores 

//BME sensor
float memory::getHumidity(){
  return BME.humidity;
}
void memory::setHumidity(int humidity){
  BME.humidity=humidity;
}
float memory::getTemperature(){
  return BME.temperature;
}
void memory::setTemperature(int temperature){
  BME.temperature=temperature;
}
float memory::getHeight(){
  return BME.height;
}
void memory::setHeight(int height){
  BME.height=height;
}
float memory::getPressure(){
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

float memory::getCo2_voltage(){
  return CO2.voltage;
}

void memory::setCo2_voltage(float vol){
  CO2.voltage=vol;
}

int memory::getCo2_state(){
  return CO2.state;
}
void memory::setCo2_state(int state){
  CO2.state=state;
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

float memory::getCo_voltage(){
  return CO.voltage;
}

void memory::setCo_voltage(float vol){
  CO.voltage=vol;
}

int memory::getCo_state(){
  return CO.state;
}
void memory::setCo_state(int state){
  CO.state=state;
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

float memory::getCh4_voltage(){
  return CH4.voltage;
}

void memory::setCh4_voltage(float vol){
  CH4.voltage=vol;
}

int memory::getCh4_state(){
  return CH4.state;
}
void memory::setCh4_state(int state){
  CH4.state=state;
}

void memory::set_calibrating(int state){
  calibrating=state;
}

int memory::get_calibrating(){
  return calibrating;
}
