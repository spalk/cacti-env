#include "Sensors.h"
#include "Display.h"

// is parameter available from device
bool status_T_out = false;
bool status_T_in = false;
bool status_P = false;
bool status_H = false;
bool status_L_alfa = false;
bool status_L_beta = false;
bool status_S_alfa = false;
bool status_S_beta = false;

float T_out, T_in;
int P, H, L_alfa, L_beta, S_alfa_volt, S_beta_volt, S_alfa_perc, S_beta_perc;

String message;

Sensors sensors;
Display display;


void setup() {
  Serial.begin(115200);

  // sensors
  sensors.init();
  status_T_in = sensors.get_T_in_status();
  status_P = sensors.get_P_status();
  status_H = sensors.get_H_status();
  status_L_alfa = sensors.get_L_alfa_status();
  status_L_beta = sensors.get_L_beta_status();
  status_T_out = sensors.get_T_out_status();

  display.init();
  display.turn_backlight_on();
  display.show_welcome_screen();
  delay(10000);
}

void loop() {
  display.show_title();
  display.show_main_page(12.4, 23.5, 748, 34, 1232, 4231, 23, 35);
  

  if (status_T_in){
    T_in = sensors.get_T_in();
    message = "Temperature is " + String(T_in) + " *C";
  } else {
    message = "Sensor for T inside is not availabe";
  }

  Serial.println(message);
  delay(1000);


  if (status_P){
    P = sensors.get_P();
    message = "Pressure is " + String(P) + " mmHg";
  } else {
    message = "Sensor for P is not availabe";
  }

  Serial.println(message);
  delay(1000);


  if (status_H){
    H = sensors.get_H();
    message = "Humidity is " + String(H) + " %";
  } else {
    message = "Sensor for H is not availabe";
  }

  Serial.println(message);
  delay(1000);


  if (status_L_alfa){
    L_alfa = sensors.get_L_alfa();
    message = "Light alfa is " + String(L_alfa) + " units";
  } else {
    message = "Sensor for L alfa is not availabe";
  }

  Serial.println(message);
  delay(1000);


  if (status_L_beta){
    L_beta = sensors.get_L_beta();
    message = "Light beta is " + String(L_alfa) + " units";
  } else {
    message = "Sensor for L beta is not availabe";
  }

  Serial.println(message);
  delay(1000);


  if (status_T_out){
    T_out = sensors.get_T_out();
    message = "Temp out is " + String(T_out) + "*C";
  } else {
    message = "Sensor for T out is not availabe";
  }
  
  Serial.println(message);
  delay(1000);


  if (status_S_alfa){
    S_alfa_volt = sensors.get_S_alfa_volt();
    S_alfa_perc = sensors.get_S_alfa_perc();
    message = "Soil Moisture alfa is " + String(S_alfa_volt) + "V " + String(S_alfa_perc) + "%";
  } else {
    message = "Sensor for Soil Moisture Alfa is not availabe";
  }

  Serial.println(message);
  delay(1000);


  if (status_S_beta){
    S_beta_volt = sensors.get_S_beta_volt();
    S_beta_perc = sensors.get_S_beta_perc();
    message = "Soil Moisture beta is " + String(S_beta_volt) + "V " + String(S_beta_perc) + "%";
  } else {
    message = "Sensor for Soil Moisture Beta is not availabe";
  }

  Serial.println(message);
  delay(1000);
}
