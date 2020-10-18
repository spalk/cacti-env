/*
  Sensors.h - Library for interaction with sensors of Cacti-Env main device
  Created by Dmitry Natkha, October 14, 2020.
*/

#ifndef Sensors_h
#define Sensors_h

#include <Adafruit_BME280.h>
#include <Wire.h>
#include <BH1750.h>
#include <OneWire.h>


class Sensors
{
  public:
    Sensors();
    void init();

    // get status
    bool get_T_in_status();
    bool get_T_out_status();
    bool get_P_status();
    bool get_H_status();
    bool get_L_alfa_status();
    bool get_L_beta_status();
    bool get_S_alfa_status();
    bool get_S_beta_status();

    // get values
    float get_T_in();
    float get_T_out();
    int get_P();
    int get_H();
    int get_L_alfa();
    int get_L_beta();
    int get_S_alfa_volt();
    int get_S_alfa_perc();
    int get_S_beta_volt();
    int get_S_beta_perc();


  private:
    // BME280: Temperature (inside), Atm. Pressure, Humidity
    Adafruit_BME280 bme;
    bool bme280_status;

    // BH1750: Light Sensors
    BH1750 bh1750_a;
    BH1750 bh1750_b;
    bool bh1750_a_status;
    bool bh1750_b_status;

    // DS18B20: Temperature (outside)
    int ONE_WIRE_BUS = 25;
    bool ds18b20_status;
    
    // Ananlog Capacitive Soli Moisture Sensors
    int SM_PIN_ALFA = 32;
    int SM_PIN_BETA = 34;
    // Calibration of exact sensors
    int AIR_VALUE_ALFA = 2550;
    int AIR_VALUE_BETA = 2400;
    int WATER_VALUE_ALFA = 2100;
    int WATER_VALUE_BETA = 1850;
};
 
#endif
