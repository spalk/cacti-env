/*
  LED.cpp - Library for controlling LEDs on Cacti-Env main device
  Created by Dmitry Natkha, October 20, 2020.
*/

#include "LED.h"
#include <Arduino.h>


LED::LED(){}

void LED::init(){
    pinMode (LEFT_LED_PIN, OUTPUT);
    pinMode (RIGHT_LED_PIN, OUTPUT);
}


// LEFT LED

void LED::switch_left_led_on(){
    digitalWrite (LEFT_LED_PIN, HIGH);
}

void LED::switch_left_led_off(){
    digitalWrite (LEFT_LED_PIN, LOW);
}


// RIGHT LED

void LED::switch_right_led_on(){
    digitalWrite (RIGHT_LED_PIN, HIGH);
}

void LED::switch_right_led_off(){
    digitalWrite (RIGHT_LED_PIN, LOW);
}
