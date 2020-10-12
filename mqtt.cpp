#include "mqtt.h"

void mqtt::mqtt_connect(PubSubClient mqtt, char broker[],const char* TOKEN, String FIRMWARE, String NUMERO_SERIAL){
  mqtt.setServer(broker, 1883);
  if(!mqtt.connected()){
    Serial.print("Connecting to ");
    Serial.print(broker);
    mqtt.disconnect();
  
    int trys=0;
    boolean status=false;
    do{
      status = mqtt.connect("Nodemcu_estacion", TOKEN, NULL);
      trys++;
    }while(status==false && trys<=4);
    
    String payload = "{";
    payload += "\"firmware\":"; payload += FIRMWARE; payload += ",";
    payload += "\"Numero Serial\":"; payload += NUMERO_SERIAL;
    payload += "}";
  
    // Send payload
    char attributes[100];
    payload.toCharArray( attributes, 100 );
    mqtt.publish( "v1/devices/me/attributes", attributes );
    Serial.println( attributes );
    delay(300);
  }
}

void mqtt::mqttSend_data(memory memory, PubSubClient mqtt){
  if(mqtt.connected()){
    char attributes[100];
    String Mensaje="{";
    Mensaje+="\"Temperatura\":"; Mensaje+=String(memory.getTemperature()); Mensaje+=",";
    Mensaje+="\"Humedad\":"; Mensaje+=String(memory.getHumidity()); Mensaje+=",";
    Mensaje+="\"Presion\":"; Mensaje+=String(memory.getPressure()); Mensaje+=",";
    Mensaje+="\"Altitud\":"; Mensaje+=String(memory.getHeight());
    Mensaje+="}";
    Mensaje.toCharArray( attributes, 100 );
    Serial.print("Sending BME data...");
    if(mqtt.publish( "v1/devices/me/telemetry", attributes )){
      Serial.println(" OK");
    }else{
      Serial.println(" Fail");
    }
    delay(1000);
    
    //Enviar datos del convertidor
    Mensaje="{";
    Mensaje+="\"CO\":"; Mensaje+=String(memory.getCo()); Mensaje+=",";
    Mensaje+="\"CH4\":"; Mensaje+=String(memory.getCo2()); Mensaje+=",";
    Mensaje+="\"CO2\":"; Mensaje+=String(memory.getCh4());
    Mensaje+="}";
    Mensaje.toCharArray( attributes, 100 );
    Serial.print("Sending Convertidor data...");
    if(mqtt.publish( "v1/devices/me/telemetry", attributes )){
      Serial.println(" OK");
    }else{
      Serial.println(" Fail");
    }
  }else{
    Serial.println("no se pudo conectar al servidor");
  }
}
