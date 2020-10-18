#include "Sensors.h"


Sensors::Sensors(){}

void Sensors::init(){
    // bme280
    bme280_status = bme.begin(0x76);

    // Initialize the I2C bus and light sensors init
    Wire.begin(21, 22);
    bh1750_a_status = bh1750_a.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x23, &Wire); //addr to gnd
    bh1750_b_status = bh1750_b.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, 0x5C, &Wire); //addr to 3v3

}


/// Status getters ///

bool Sensors::get_T_in_status(){
    return bme280_status;
}

bool Sensors::get_P_status(){
    return bme280_status;
}

bool Sensors::get_H_status(){
    return bme280_status;
}

bool Sensors::get_L_alfa_status(){
    return bh1750_a_status;
}

bool Sensors::get_L_beta_status(){
    return bh1750_b_status;
}

bool Sensors::get_T_out_status(){
    int iters = 5;
    float summ = 0;
    for (int i = 0; i < iters; i++){
        summ = summ + Sensors::get_T_out();
    }
    if (summ == 0){
        return false;
    } else {
        return true;
    }
}

bool Sensors::get_S_alfa_status(){
    int iters = 5;
    float summ = 0;
    for (int i = 0; i < iters; i++){
        summ = summ + Sensors::get_S_alfa_volt();
    }
    if (summ / iters < 1000 ){
        return false;
    } else {
        return true;
    }
}

bool Sensors::get_S_beta_status(){
    int iters = 5;
    float summ = 0;
    for (int i = 0; i < iters; i++){
        summ = summ + Sensors::get_S_beta_volt();
    }
    if (summ / iters < 1000 ){
        return false;
    } else {
        return true;
    }
}

/// Value getters ///

float Sensors::get_T_in(){
    float result = bme.readTemperature();
    return result;
}

int Sensors::get_P(){
    float result = bme.readPressure() / 100.0F * 0.75;
    return int(result);
}

int Sensors::get_H(){
    float result = bme.readHumidity();
    return int(result);
}

int Sensors::get_L_alfa(){
    float result;
    result = bh1750_a.readLightLevel();
    return int(result);
}

int Sensors::get_L_beta(){
    float result;
    result = bh1750_b.readLightLevel();
    return int(result);
}

int Sensors::get_S_alfa_volt(){
    int result;
    result = analogRead(SM_PIN_ALFA);
    return result;
}

int Sensors::get_S_alfa_perc(){
    int value_volt = Sensors::get_S_alfa_volt();
    int result;
    result = map(result, AIR_VALUE_ALFA, WATER_VALUE_ALFA, 0, 100);
    return result;
}

int Sensors::get_S_beta_volt(){
    int result;
    result = analogRead(SM_PIN_BETA);
    return result;
}

int Sensors::get_S_beta_perc(){
    int value_volt = Sensors::get_S_beta_volt();
    int result;
    result = map(result, AIR_VALUE_BETA, WATER_VALUE_BETA, 0, 100);
    return result;
}

float Sensors::get_T_out(){
    OneWire ds(ONE_WIRE_BUS);
    byte i;
    byte present = 0;
    byte data[12];
    byte addr[8];
    float celsius;

    if ( !ds.search(addr)){
        ds.reset_search();
        delay(500);
        for(byte i = 0; i < 10; i++) addr[i] = 0;
        ds.search(addr);
        //return;
    }

    ds.reset();
    ds.select(addr);
    ds.write(0x44, 1); // start conversion, with parasite power on at the end
    delay(750);
    present = ds.reset();
    ds.select(addr);
    ds.write(0xBE); // Read Scratchpad

    for ( i = 0; i < 9; i++){
        data[i] = ds.read();
    }

    // Convert the data to actual temperature
    int16_t raw = (data[1] << 8) | data[0];
    byte cfg = (data[4] & 0x60);

    if (cfg == 0x00) raw = raw & ~7; // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    celsius = (float)raw / 16.0;
    return celsius;
}
