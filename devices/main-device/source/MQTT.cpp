/*
  MQTT.cpp - Library for interaction with display of Cacti-Env main device
  Created by Dmitry Natkha, October 18, 2020.
*/

#include <TelnetStream.h>
#include "MQTT.h"
#include "Relay.h"


MQTT::MQTT(){}

void MQTT::init(){
    client.setClient(espClient);
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
    rel.init();
}

void MQTT::check_connection(){
    if (!client.connected()) {
        Serial.println("MQTT Reconnection");
        MQTT::reconnect();
    }
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
            client.subscribe("izm/south-balcony/main-device");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(1000);
        }
    }
}

void MQTT::send_data(
    float T_out, 
    float T_in, 
    int P, 
    int H, 
    int L_alfa, 
    int L_beta, 
    int S_alfa, 
    int S_beta
){
    MQTT::publish(T_out,  5, 1, topic_T_out);
    MQTT::publish(T_in,   5, 1, topic_T_in);
    MQTT::publish(P,      3, 0, topic_P);
    MQTT::publish(H,      3, 0, topic_H);
    MQTT::publish(L_alfa, 6, 0, topic_L_alfa);
    MQTT::publish(L_beta, 6, 0, topic_L_beta);
    MQTT::publish(S_alfa, 4, 0, topic_S_alfa);
    MQTT::publish(S_beta, 4, 0, topic_S_beta);
}


void MQTT::publish(
    float value,
    int lengthIncDecimalPoint,
    int numVarsAfterDecimal,
    const char* topic_tip
){
    if (value != -100){
        char value_char[6];
        bool debug = MQTT_MODE_DEBUG;

        // get root part of topic
        const char* topic_root = (debug) ? topic_root_debug : topic_root_release;

        // get full topic (concatenate root and tip parts of topic)
        char full_topic[100];
        sprintf(full_topic, "%s%s", topic_root, topic_tip);

        // convert type of value from float to char
        dtostrf (value, lengthIncDecimalPoint, numVarsAfterDecimal, value_char);

        // publish
        client.publish(full_topic, value_char);
        TelnetStream.print("  ");
        TelnetStream.print(full_topic);
        TelnetStream.print(" => ");
        TelnetStream.println(value_char);
    };
    
}


static void callback(char* topic, byte* payload, unsigned int length) {
    // Conver the incoming byte array to a string
    payload[length] = '\0'; // Null terminator used to terminate the char array
    String message = (char*)payload;

    Serial.print("Message arrived on topic: [");
    Serial.print(topic);
    Serial.print("], ");
    Serial.println(message);

    TelnetStream.print("! Message arrived on topic: [");
    TelnetStream.print(topic);
    TelnetStream.print("], ");
    TelnetStream.println(message);

    if (message == "reboot"){
        TelnetStream.print("  REBOOTING...");
        ESP.restart();
    } 

    // relay 1
    else if (message == "relay_1_on"){
        Serial.print("Relay 1 switching on...");
        TelnetStream.print("  Relay 1 switching ON...");
        rel.switch_alfa_on();
    } else if (message == "relay_1_off"){
        Serial.print("Relay 1 switching off...");
        TelnetStream.print("  Relay 1 switching OFF...");
        rel.switch_alfa_off();
    } 

    // relay 2
    else if (message == "relay_2_on"){
        Serial.print("Relay 2 switching on...");
        TelnetStream.print("  Relay 2 switching ON...");
        rel.switch_beta_on();
    } else if (message == "relay_2_off"){
        Serial.print("Relay 2 switching off...");
        TelnetStream.print("  Relay 2 switching OFF...");
        rel.switch_beta_off();
    } 
    
    /*
    // alert 
    else if (message == "alert"){
        Serial.print("Achtung!");
        led.switch_right_on();
    }else if (message == "no_alert"){
        Serial.print("It's ok");
        led.switch_right_off();
    } 
    */



}
