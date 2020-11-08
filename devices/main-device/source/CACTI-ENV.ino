#include "OTA.h"
#include <TelnetStream.h>
#include "Display.h"
#include "Sensors.h"
#include "MQTT.h"
#include "LED.h"
#include "Relay.h"

#define OTA_DEVICE_ID "Cacti-Env_Main-Device"


Display display;
Sensors sensors;
MQTT mqtt;
LED led;
Relay rel;


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
const long send_data_interval = 60000; // send data to broker every 1 min
unsigned long time_to_switch_bckl = 0;
const long switch_bckl_interval = 500;

// button
const int BUTTON_PIN = 35;


void get_fresh_sensor_values();
bool its_time_to_refresh_display();
bool its_time_to_send_data();
bool its_time_to_switch_bckl();
String IpAddress2String(const IPAddress& ipAddress);


void setup() {
  Serial.begin(115200);

  // display init
  display.init();
  display.turn_backlight_on();
  display.show_msg_in_line("LOADING:", 0);
  delay(500);

  // led init
  led.init();
  display.show_loading_line("LEDs init", "OK", 1);
  led.blink_left(3, 100);
  led.blink_right(3, 100);
  delay(500);

  // OTA (Over The Air update) setup
  display.show_msg_in_line("WiFi", 2);
  setupOTA(OTA_DEVICE_ID);
  display.show_loading_line("WiFi", "OK", 2);
  display.show_msg_in_line(IpAddress2String(WiFi.localIP()), 3);

  TelnetStream.begin();

  // MQTT
  mqtt.init();
  display.show_loading_line("MQTT", "OK", 4);

  // sensors init and get status
  display.clear();
  display.show_msg_in_line("Check sensors:", 0);
  sensors.init();
  status_T_in  = sensors.get_T_in_status();
  status_T_out = sensors.get_T_out_status();
  status_P = sensors.get_P_status();
  status_H = sensors.get_H_status();
  status_L_alfa = sensors.get_L_alfa_status();
  status_L_beta = sensors.get_L_beta_status();
  status_S_alfa = sensors.get_S_alfa_status();
  status_S_beta = sensors.get_S_beta_status();

  display.show_loading_line("Temp out", (status_T_out) ? "OK" : "NOK", 1);
  display.show_loading_line("Temp in", (status_T_in) ? "OK" : "NOK", 2);
  display.show_loading_line("Pressure", (status_P) ? "OK" : "NOK", 3);
  display.show_loading_line("Humidity", (status_H) ? "OK" : "NOK", 4);
  display.clear();

  display.show_msg_in_line("Check sensors:", 0);
  display.show_loading_line("Light 1", (status_L_alfa) ? "OK" : "NOK", 1);
  display.show_loading_line("Light 2", (status_L_beta) ? "OK" : "NOK", 2);
  display.show_loading_line("Soil 1", (status_S_alfa) ? "OK" : "NOK", 3);
  display.show_loading_line("Soil 2", (status_S_beta) ? "OK" : "NOK", 4);
  delay(500);

  // show welcome screen
  display.show_welcome_screen();
  delay(3000);

  // button init
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  ArduinoOTA.handle();

  mqtt.check_connection();

  // Refresh display
  if (its_time_to_refresh_display()){
    TelnetStream.println("*Refresh display*");
    led.switch_left_on();
    get_fresh_sensor_values();
    display.show_title();
    display.show_main_page(T_out, T_in,P, H, L_alfa, L_beta, S_alfa_perc, S_beta_perc);
    led.switch_left_off();
  }

  // Send data via MQTT
  if (its_time_to_send_data()){
    TelnetStream.println("*Publishing data*");
    led.switch_right_on();
    get_fresh_sensor_values();
    mqtt.send_data(T_out, T_in, P, H, L_alfa, L_beta, S_alfa_volt, S_beta_volt);
    led.switch_right_off();
  }

  // Button
  if (digitalRead(BUTTON_PIN)){
    if (its_time_to_switch_bckl()){
      display.switch_backlight();
    }
  }
}


void get_fresh_sensor_values(){
  TelnetStream.println("  Get fresh sensor values:");
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
  unsigned long time_diff;
  time_diff = millis() - time_to_send_data;
  if (time_diff > send_data_interval){
    answer = true;
    time_to_send_data = millis();
  } else {
    answer = false;
  }
  return answer;
}


bool its_time_to_switch_bckl(){
  bool answer;
  unsigned long time_diff;
  time_diff = millis() - time_to_switch_bckl;
  if (time_diff > switch_bckl_interval){
    answer = true;
    time_to_switch_bckl = millis();
  } else {
    answer = false;
  }
  return answer;
}


String IpAddress2String(const IPAddress& ipAddress){
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ;
}