/*
  MQTT.cpp - Library for interaction with display of Cacti-Env main device
  Created by Dmitry Natkha, October 18, 2020.
*/

#include "MQTT.h"



MQTT::MQTT(){}

void MQTT::init(){
    client.setClient(espClient);
    client.setServer(mqtt_server, 1883);
    //client.setCallback(callback);
}

void MQTT::check_connection(){
    if (!client.connected()) {
        MQTT::reconnect();
    }
    Serial.println("MQTT Connected");
    client.loop();
}

void MQTT::reconnect(){
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(mqtt_client_name, mqtt_login, mqtt_pass)) {
            Serial.println("connected");
            // Subscribe
            //client.subscribe("esp32/output");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(1000);
        }
    }
}