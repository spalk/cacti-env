/*
  MQTT.h - Library for interaction with display of Cacti-Env main device
  Created by Dmitry Natkha, October 18, 2020.
*/

#ifndef MQTT_h
#define MQTT_h

#define MQTT_MODE_DEBUG 0


#include <WiFi.h>
#include <PubSubClient.h>
#include "credentials.h"
#include "Relay.h"
// #include "LED.h"

//extern LED led;
extern Relay rel;


static void callback(char* topic, byte* payload, unsigned int length);

class MQTT
{
    public:
        MQTT();
        void init();
        void check_connection();
        void send_data(float T_out, float T_in, int P, int H, int L_alfa, int L_beta, int S_alfa, int S_beta);

    private:
        WiFiClient espClient;
        PubSubClient client;
        
        void reconnect();
        //static void callback(char* topic, byte* payload, unsigned int length);
        
        // check value and publish
        void publish(float value, int lengthIncDecimalPoint, int numVarsAfterDecimal, const char* topic_tip);
        
        // root-part of topic 
        const char* topic_root_release =  "izm/south-balcony";
        const char* topic_root_debug   =  "test/test";
        
        // constant end-part of topic for each parameter
        const char* topic_T_out   = "/temperature/outside";
        const char* topic_T_in    = "/temperature/inside";
        const char* topic_P       = "/pressure";
        const char* topic_H       = "/humidity";
        const char* topic_L_alfa  = "/light-intensity/sensor-01";
        const char* topic_L_beta  = "/light-intensity/sensor-02";
        const char* topic_S_alfa  = "/soil-moisture/sensor-01";
        const char* topic_S_beta  = "/soil-moisture/sensor-02";  
};

#endif
