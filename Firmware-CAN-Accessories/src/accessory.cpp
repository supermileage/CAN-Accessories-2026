#include <mbed.h>
#include <chrono>
#include "accessory.h"

Accessory::Accessory(PinName pin, int board, string name, int initial_state, int blinks_int) : out(pin){
    this->pin = pin;
    this->board = board;
    this->name = name;
    this->initial_state = initial_state;
    current_state = initial_state;
    this->blinks_int = blinks_int;
    }

void Accessory::updateState(bool newState){

    if(current_state != newState){

        current_state = newState;
        if(newState && blinks_int){

            out = newState;
            t.attach(callback(this, &Accessory::blinks), BLINK_RATE);
            t.attach(callback(this, &Accessory::blinks), BLINK_RATE);
        }
        else if(!newState && blinks_int){
            t.detach();
            out = newState;
        }

        else{
            out = newState;
        }
    }


};

void Accessory::blinks(){

    out = !out;

}