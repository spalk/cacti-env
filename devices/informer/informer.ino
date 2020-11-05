#include "OTA.h"
#include "credentials.h"
#include <PubSubClient.h>
#include <Wire.h>
#include "SH1106.h"
#include "font_lato_light_48.h"
#include <ESP8266WiFi.h>

unsigned long entry;

// WiFi & MQTT
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];


// Display
SH1106 display(0x3c, D2, D1);     // ADDRESS, SDA, SCL
int showTimeout = 10000; // time before off display

// PIR
int pirPin = D7;
int pirVal; //low = no motion, high = motion

// delays
unsigned long timeing;
unsigned long timedelta = 60000;
unsigned long data_time;
unsigned long show_time;

// Data from mqtt
String currentT;
String currentP;
String diffP;
int yesterdayP;


void mainView() {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont((uint8_t *) Lato_Light_48);
    display.drawString(64, -5, currentT);
    display.drawHorizontalLine(0, 48, 128);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 50, "Pressure: " + currentP + " [" + diffP + "] mmHg");
    display.display();
}

void displayOff() {
    display.clear();
    display.display();
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Informer", mqtt_login, mqtt_pass)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("izm/south-balcony/temperature/outside");
      client.subscribe("izm/south-balcony/pressure");
      client.subscribe("izm/south-balcony/pressure-yesterday");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
    // Conver the incoming byte array to a string
    payload[length] = '\0'; // Null terminator used to terminate the char array
    String message = (char*)payload;

    Serial.print("Message arrived on topic: [");
    Serial.print(topic);
    Serial.print("], ");
    Serial.println(message);

    String MSG;
    String FirstSym;

    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        MSG += (char)message[i];
    }
    Serial.println();

    if (String(topic) == "izm/south-balcony/temperature/outside") {
        if(MSG.toFloat() > 0){  // to remove spaces
            currentT = "+";
            currentT += String(MSG.toFloat(), 1); // 1 - decimal places
        } else {
            currentT = MSG;
        }
        data_time = millis();
    }

    if (String(topic) == "izm/south-balcony/pressure") {
      currentP = MSG;
    }

    if (String(topic) == "izm/south-balcony/pressure-yesterday") {
      yesterdayP = MSG.toInt();
      diffP = String(currentP.toInt()-yesterdayP);
      if (diffP.toInt() > 0){
        diffP = "+" + diffP;
      }
    }

}



void setup() {
    Serial.begin(115200);
    Serial.println("Booting");

    setupOTA("Wemos_Informer_01");

    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    // Initialising display
    display.init();
    //display.flipScreenVertically();

    currentT = "???";
    currentP = "???";

    mainView();

    timeing = millis();
    show_time = millis();


}


void loop() {
    ArduinoOTA.handle();
    delay(20);

    // connection
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // check, how fresh data is
    // if data older than 10 min, then show nothing
    if (millis() - data_time > 600000){
        currentT = "???";
    }


    // Motion detection
    pirVal = digitalRead(pirPin);
    if (pirVal == LOW){
        //Serial.println("No motion");
        if (millis() - show_time > showTimeout){
            displayOff();
        }
    } else {
        //Serial.println("Motion detected");
        mainView();
        show_time = millis();
    }

    if (millis() - timeing > timedelta){
        timeing = millis();
        show_time = millis();
    }
}
