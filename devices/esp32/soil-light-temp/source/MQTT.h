/*
  MQTT.h - Library for interaction with display of Cacti-Env main device
  Created by Dmitry Natkha, October 18, 2020.
*/

#ifndef MQTT_h
#define MQTT_h

#include <WiFi.h>
#include <PubSubClient.h>
#include "credentials.h"


class MQTT
{
    public:
        MQTT();
        void init();
        void check_connection();

    private:
        WiFiClient espClient;
        PubSubClient client;
        void reconnect();
};

#endif