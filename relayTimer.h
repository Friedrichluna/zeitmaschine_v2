#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

#define MAX_RELAY_TIMERS 32
#define MAX_RELAY_AMOUNT 16


void print_something() {
    printf("test");
}

class RelayTimer {
private:
    int gpio_pin_switch;
    int frequency;
    int length_sec;
    int relay_arr[MAX_RELAY_AMOUNT];
    bool toggleable;

    int counter = 0; 

public:
    RelayTimer(int gpio_pin_switch_ = 0, int frequency_ = 0, int length_sec_ = 0,int relay_arr_[MAX_RELAY_AMOUNT] = {}, bool toggleable_ = false) {
        gpio_pin_switch = gpio_pin_switch_;
        frequency = frequency_;
        length_sec = length_sec_;
        relay_arr[MAX_RELAY_AMOUNT] = {};
        for (int i = 0; i < MAX_RELAY_AMOUNT; i++) {
            relay_arr[i] = relay_arr_[i];
        }
        toggleable = toggleable_;
    }

    void routine(){
        counter++;
        printf("amount = %d\n",counter);
    }

private:
    void toggle_relays() {
        
    }
    bool execute_condition(){
        return true;
    }
};