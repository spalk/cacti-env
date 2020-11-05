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

void LED::switch_left_on(){
    digitalWrite (LEFT_LED_PIN, HIGH);
}

void LED::switch_left_off(){
    digitalWrite (LEFT_LED_PIN, LOW);
}


// RIGHT LED

void LED::switch_right_on(){
    digitalWrite (RIGHT_LED_PIN, HIGH);
}

void LED::switch_right_off(){
    digitalWrite (RIGHT_LED_PIN, LOW);
}


void LED::blink_left(int times, int delay_time){
    for (int i = 0; i < times; i++){
        LED::switch_left_on();
        delay(delay_time);
        LED::switch_left_off();
        delay(delay_time);
    }
}

void LED::blink_right(int times, int delay_time){
    for (int i = 0; i < times; i++){
        LED::switch_right_on();
        delay(delay_time);
        LED::switch_right_off();
        delay(delay_time);
    }

}

