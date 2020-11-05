/*
  Relay.h - Library for controlling Relays wtih Cacti-Env main device
  Created by Dmitry Natkha, October 20, 2020.
*/

#ifndef Relay_h
#define Relay_h

#include <Arduino.h>

class Relay
{
    public:
        Relay();
        void init();
        void switch_alfa_on();
        void switch_alfa_off();
        void switch_beta_on();
        void switch_beta_off();

    private:
        const int RELAY_ALFA_PIN = 13; 
        const int RELAY_BETA_PIN = 27;  
        bool status_relay_alfa;
        bool status_relay_beta;
};

#endif
