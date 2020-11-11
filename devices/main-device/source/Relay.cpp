/*
  Relay.cpp - Library for controlling Relays with Cacti-Env main device
  Created by Dmitry Natkha, October 20, 2020.
*/

#include "Relay.h"
#include <Arduino.h>


Relay::Relay(){}

void Relay::init(){
    pinMode (RELAY_ALFA_PIN, OUTPUT);
    pinMode (RELAY_BETA_PIN, OUTPUT);
    Relay::switch_alfa_off();
    Relay::switch_beta_off();
}


//  Relay Alfa

void Relay::switch_alfa_on(){
    digitalWrite (RELAY_ALFA_PIN, LOW);
    status_relay_alfa = true;
}

void Relay::switch_alfa_off(){
    digitalWrite (RELAY_ALFA_PIN, HIGH);
    status_relay_alfa = false;
}


//  Relay Beta

void Relay::switch_beta_on(){
    digitalWrite (RELAY_BETA_PIN, LOW);
    status_relay_beta = true;
}

void Relay::switch_beta_off(){
    digitalWrite (RELAY_BETA_PIN, HIGH);
    status_relay_beta = false;
}


