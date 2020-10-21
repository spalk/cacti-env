#include "OTA.h"
#include "Display.h"
#include "Sensors.h"
#include "MQTT.h"
#include "LED.h"

#define OTA_DEVICE_ID "ESP32_Cacti-Env_DEV"
#define MQTT_MODE_DEBUG 1

Display display;
Sensors sensors;
MQTT mqtt;
LED led;


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
String IpAddress2String(const IPAddress& ipAddress);



void setup() {
  Serial.begin(115200);

  // led init
  led.init();

  // led welcome
  led.switch_right_led_on();
  led.switch_right_led_off();
  led.switch_left_led_on();
  led.switch_left_led_off();

  // display init
  display.init();
  display.turn_backlight_on();

  display.show_msg_in_title("WiFi init...");

  // OTA (Over The Air update) setup
  setupOTA(OTA_DEVICE_ID);

  display.show_msg_in_title(IpAddress2String(WiFi.localIP()));
  delay(2000);



  display.show_msg_in_title("Seinsors init...");

  // sensors init and get status
  sensors.init();
  status_T_in  = sensors.get_T_in_status();
  status_T_out = sensors.get_T_out_status();
  status_P = sensors.get_P_status();
  status_H = sensors.get_H_status();
  status_L_alfa = sensors.get_L_alfa_status();
  status_L_beta = sensors.get_L_beta_status();
  status_S_alfa = sensors.get_S_alfa_status();
  status_S_beta = sensors.get_S_beta_status();

  display.show_msg_in_title("MQTT init...");

  // MQTT
  mqtt.init();

  // show welcome screen
  display.show_welcome_screen();
  delay(1000);
  //display.turn_backlight_off();

}

void loop() {
  // Over the air update handler
  ArduinoOTA.handle();

  // Refresh display
  if (its_time_to_refresh_display()){
    led.switch_left_led_on();
    get_fresh_sensor_values();
    display.show_title();
    display.show_main_page(T_out, T_in,P, H, L_alfa, L_beta, S_alfa_perc, S_beta_perc);
    led.switch_left_led_off();
  }

/*
  if (its_time_to_send_data){
    led.switch_right_led_on();
    mqtt.check_connection();
    mqtt.send_data(T_out, T_in, P, H, L_alfa, L_beta, S_alfa_volt, S_beta_volt);
    led.switch_right_led_off();
  }
*/

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
  S_beta_perc = (status_S_beta) ? sensors.get_S_beta_perc() : -100;
}


bool its_time_to_refresh_display(){
  bool answer;
  unsigned long time_diff;
  time_diff = millis() - time_to_refresh_display;
  if (time_diff > refresh_display_interval){
    answer = true;
    time_to_refresh_display = millis();
  } else {
    answer = false;
  }
  return answer;
}


bool its_time_to_send_data(){
  bool answer;
  answer = (millis() - time_to_send_data > send_data_interval) ? true : false;
  return answer;
}


String IpAddress2String(const IPAddress& ipAddress){
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}
