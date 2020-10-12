#ifndef data_h
#define data_h

#define SEALEVELPRESSURE_HPA (1013.25)
#define RLOAD 10.0

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "memory.h"


class data{
  public:
    float readSensorsBME(String type, Adafruit_BME280 bme, memory memory);
    int readSensorsMQ(memory memory, Adafruit_ADS1115 adc);
    int calibrateSensors(memory memory, Adafruit_ADS1115 adc);
    
    float getADC(int pin, Adafruit_ADS1115 adc);
    //0=co, 1=ch4, 2=co2
    void readSensorsMQ(int sensors);
    int getPPM(float Ro, float scaleFactor, float exponent, float adc);
    float getVoltage(float adc);
    float getResistance(float adc);
    int getRZero(float atm, float scaleFactor, float exponent, float adc);
  private:
    int population=10;
    const float mul = 0.0625 / 1000;
    //constantes de CO2
    float scaleFactorCO2 = 116.6020682;
    float exponentCO2 = 2.769034857;
    //constantes de CH4
    float scaleFactorCH4 = 1051.932;
    float exponentCH4 = 2.773532;
    //constantes de CO
    float scaleFactorCO = 580.074;
    float exponentCO = 2.398271;
};

#endif
