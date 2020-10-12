#include "timeClock.h"
#include "serverWifi.h"
#include "memory.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"
#include "gsm.h"
#include "data.h"
#include "mqtt.h"

//creando mqtt
mqtt mqtt;

//creando memoria
memory memory;

//creando wifi
serverWifi se;
WiFiClient WifiClient;

//creando tiempo
timeClock timeClock;

//creando sensores 
data sensors;
Adafruit_ADS1115 adc;
Adafruit_BME280 bme;

//creando tareas
TaskHandle_t Task1;
TaskHandle_t Task2;

//iniciar libreria del servidor
AsyncWebServer server(80);

//modem GSM
#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
HardwareSerial SerialGsm(2);
TinyGsm modem(SerialGsm);
TinyGsmClient GsmClient(modem);
gsm gsm;

//mqtt
PubSubClient mqttGSM(GsmClient);
PubSubClient mqttWIFI(WifiClient);
char broker[] = "demo.thingsboard.io";
#define TOKEN "chW5YCF2eXJjqeTSquvC"
#define FIRMWARE "0.1"
#define NUMERO_SERIAL  "SN-009"

const char* conf="/config.txt";
const char* data="/data.txt";

#include "index.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //montar la memoria interna del micro
  if(!memory.begin()){
    Serial.println("error al montar memoria ffat");
    return;
  }
  memory.readConfiguration(conf);
  memory.writeConfiguration(conf);
  se.begin(memory, timeClock);

  // Start GSM
  
  // Start convertidor analogico digital
  adc.setGain(GAIN_TWO);
  adc.begin();

  // start sensoor bme
  bool status;
  status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  
  // Start server
  startServers();
  server.begin();
  
//-----------------task1-----------
  xTaskCreate(
    Task1code,   // Task function.
    "Task1",     // name of task.
    10000,       // Stack size of task
    NULL,        // parameter of the task
    1,           // priority of the task
    &Task1);     // Task handle to keep track of created task

//-----------------task2-----------
  xTaskCreate(
    Task2code,   // Task function.
    "Task2",     // name of task.
    10000,       // Stack size of task
    NULL,        // parameter of the task
    2,           // priority of the task
    &Task2);     // Task handle to keep track of created task
}

//-----------Task1code-----------------
void Task1code( void * pvParameters ) {
  //delay(1000);
  //btStop();
  for (;;) {
    if(memory.getCo_state()==1){
      memory.setCo(sensors.readSensorsMQ(memory, adc));
      delay(100);
      memory.setCo_voltage(sensors.getVoltage(sensors.getADC(0,adc)));
    }else if(memory.getCo2_state()==1){
      memory.setCo2(sensors.readSensorsMQ(memory, adc));
      delay(100);
      memory.setCo2_voltage(sensors.getVoltage(sensors.getADC(2,adc)));
    }else if(memory.getCh4_state()==1){
      memory.setCh4(sensors.readSensorsMQ(memory, adc));
      delay(100);
      memory.setCh4_voltage(sensors.getVoltage(sensors.getADC(1,adc)));
    }
    delay(100);
    memory.setHumidity(sensors.readSensorsBME("humidity", bme, memory));
    memory.setPressure(sensors.readSensorsBME("pressure", bme, memory));
    memory.setTemperature(sensors.readSensorsBME("temperature", bme, memory));
    memory.setHeight(sensors.readSensorsBME("height", bme, memory));
    Serial.println("ok.");
    delay(memory.getTime_to_sleep()*60000);
  }
  vTaskDelay(10);
}

//-----------Task2code-----------------
void Task2code( void * pvParameters ) {
  gsm.GSM_begin(SerialGsm, timeClock, modem, memory);
  vTaskDelete(NULL);
}

void loop() {
  delay(3000);
  if(gsm.get_state()==4){
    mqtt.mqtt_connect(mqttGSM, broker, TOKEN, FIRMWARE, NUMERO_SERIAL);
    mqtt.mqttSend_data(memory, mqttGSM);
  }else if(se.get_state()==2){
    mqtt.mqtt_connect(mqttWIFI, broker, TOKEN, FIRMWARE, NUMERO_SERIAL);
    mqtt.mqttSend_data(memory, mqttWIFI);
  }
  memory.saveData(timeClock, data);
  delay(memory.getTime_to_sleep()*60000);
}
