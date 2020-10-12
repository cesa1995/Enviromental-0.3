#include "data.h"

float data::readSensorsBME(String type, Adafruit_BME280 bme, memory memory){

  float humidity=0;
  float temperature=0;
  float pressure=0;
  float height=0;
  
  for(int i=0; i<population;i++){
    humidity+=bme.readHumidity();
    temperature+=bme.readTemperature();
    pressure+=bme.readPressure()/100.0F;
    height+=bme.readAltitude(SEALEVELPRESSURE_HPA);
    delay(50);
    
  }
  if(type=="humidity"){
    return humidity/population;
  }else if(type=="temperature"){
    return temperature/population;
  }else if(type=="pressure"){
    return pressure/population;
  }else if(type=="height"){
    return height/population;
  }
}
    
//0=co, 1=ch4, 2=co2
int data::readSensorsMQ(memory memory, Adafruit_ADS1115 adc){
  float sensor_ADC=0;
  if(memory.getCo_state()==1){
    sensor_ADC=getADC(0,adc);
    return getPPM(memory.getCo_ro(),scaleFactorCO,exponentCO,sensor_ADC);
  }else if(memory.getCo2_state()==1){
    sensor_ADC=getADC(2,adc);
    return getPPM(memory.getCo2_ro(),scaleFactorCO2,exponentCO2,sensor_ADC);
  }else if(memory.getCh4_state()==1){
    sensor_ADC=getADC(1, adc);
    return getPPM(memory.getCh4_ro(),scaleFactorCH4,exponentCH4,sensor_ADC);
  }
}

int data::calibrateSensors(memory memory, Adafruit_ADS1115 adc){
  float sensor_ADC=0;
  if(memory.getCo_state()==1){
    sensor_ADC=getADC(0,adc);
    return getRZero(memory.getCo_Atm(), scaleFactorCO, exponentCO, sensor_ADC);
  }else if(memory.getCo2_state()==1){
    sensor_ADC=getADC(2,adc);
    return getRZero(memory.getCo2_Atm(), scaleFactorCO2, exponentCO2, sensor_ADC);
  }else if(memory.getCh4_state()==1){
    sensor_ADC=getADC(1, adc);
    return getRZero(memory.getCh4_Atm(), scaleFactorCH4, exponentCH4, sensor_ADC);
  }
}

float data::getADC(int pin, Adafruit_ADS1115 ads){
  float plusAdc=0;
  for (int i=0; i<population;i++){
    plusAdc += ads.readADC_SingleEnded(pin);
    delay(200);
  }
  return plusAdc/population;
}

int data::getPPM(float Ro, float scaleFactor, float exponent, float adc){
  return scaleFactor * pow((getResistance(adc)/Ro), -exponent);
}

float data::getVoltage(float adc){
  return adc*mul;
}

float data::getResistance(float adc){
  return ((32767./(float)adc) * 5. - 1.)*RLOAD;
}

int data::getRZero(float atm, float scaleFactor, float exponent, float adc){
  return getResistance(adc) * pow((atm/scaleFactor), (1./exponent));
}
