/*
  LED.h - Library for controlling LEDs on Cacti-Env main device
  Created by Dmitry Natkha, October 20, 2020.
*/

#ifndef LED_h
#define LED_h

#include <Arduino.h>

class LED
{
    public:
        LED();
        void init();
        void switch_left_led_on();
        void switch_left_led_off();
        void switch_right_led_on();
        void switch_right_led_off();


    private:
        const int LEFT_LED_PIN = 33;  // yellow
        const int RIGHT_LED_PIN = 26;  // green
};

#endif