#include "OTA.h"
#include "Display.h"
#include "Sensors.h"
#include "MQTT.h"

#define OTA_DEVICE_ID "ESP32_Cacti-Env_DEV"
#define MQTT_MODE_DEBUG 1

Display display;
Sensors sensors;
MQTT mqtt;


// parameter status (is sensor available and returns value)
bool status_T_out = false;
bool status_T_in = false;
bool status_P = false;
bool status_H = false;
bool status_L_alfa = false;
bool status_L_beta = false;
bool status_S_alfa = false;
bool status_S_beta = false;

// parameter values (from sensors)
float T_out, T_in;
int P, H, L_alfa, L_beta, S_alfa_volt, S_beta_volt, S_alfa_perc, S_beta_perc;

// timers
unsigned long time_to_refresh_display = 0;
const long refresh_display_interval = 1000;
unsigned long time_to_send_data = 0;
const long send_data_interval = 60000;


void get_fresh_sensor_values();
bool its_time_to_refresh_display();
bool its_time_to_send_data();



void setup() {
  Serial.begin(115200);

  // OTA
  setupOTA(OTA_DEVICE_ID);

  // sensors
  sensors.init();
  status_T_in = sensors.get_T_in_status();
  status_P = sensors.get_P_status();
  status_H = sensors.get_H_status();
  status_L_alfa = sensors.get_L_alfa_status();
  status_L_beta = sensors.get_L_beta_status();
  status_T_out = sensors.get_T_out_status();

  // display
  display.init();
  display.turn_backlight_on();
  display.show_welcome_screen();
  delay(3000);
  display.turn_backlight_off();

  // MQTT
  mqtt.init();
}

void loop() {
  // Over the air update handler
  ArduinoOTA.handle();

  if (its_time_to_refresh_display){
    get_fresh_sensor_values();
    display.show_title();
    display.show_main_page(T_out, T_in,P, H, L_alfa, L_beta, S_alfa_perc, S_beta_perc);
  }

  if (its_time_to_send_data){
    mqtt.check_connection();
    mqtt.send_data(T_out, T_in, P, H, L_alfa, L_beta, S_alfa_volt, S_beta_volt);
  }

}


void get_fresh_sensor_values(){
  T_in  = (status_T_in)  ? sensors.get_T_in()  : -100;
  T_out = (status_T_out) ? sensors.get_T_out() : -100;
  P = (status_P) ? sensors.get_P() : -100;
  H = (status_H) ? sensors.get_H() : -100;
  L_alfa = (status_L_alfa) ? sensors.get_L_alfa() : -100;
  L_beta = (status_L_beta) ? sensors.get_L_beta() : -100;
  S_alfa_volt = (status_S_alfa) ? sensors.get_S_alfa_volt() : -100;
  S_alfa_perc = (status_S_alfa) ? sensors.get_S_alfa_perc() : -100;
  S_beta_volt = (status_S_beta) ? sensors.get_S_beta_volt() : -100;
  S_alfa_perc = (status_S_beta) ? sensors.get_S_beta_perc() : -100;
}


bool its_time_to_refresh_display(){
  bool answer;
  answer = (millis() - time_to_refresh_display > refresh_display_interval) ? true : false;
  return answer;
}

bool its_time_to_send_data(){
  bool answer;
  answer = (millis() - time_to_send_data > send_data_interval) ? true : false;
  return answer;
}