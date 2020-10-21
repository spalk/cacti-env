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


char* MQTT::join_topic(const char* root, const char* tip){
    char* topic;
    strcpy (topic, root);
    strcat (topic, tip);
    return topic;
}


void MQTT::publish(
    float value,
    int lengthIncDecimalPoint,
    int numVarsAfterDecimal,
    const char* topic_tip
){
    if (value != -100){
        char* value_char;
        bool debug = MQTT_MODE_DEBUG;
        const char* topic_root = (debug) ? topic_root_debug : topic_root_release;
        dtostrf (value, lengthIncDecimalPoint, numVarsAfterDecimal, value_char);
        client.publish(
            MQTT::join_topic(topic_root, topic_tip), 
            value_char
        );
    };
}

