#include <mbed.h>
#include <chrono>
#include "accessory.h"

using std::string;

    
    // PinName pin;
    // PinName isense_pin;
    // int     board;
    // string  name;
    // int     initial_state;
    // int     blinksInt;
    // int     current_state;

Accessory::Accessory(PinName gatePin, PinName isensePin, int board, string name, int initial_state, int blinksC) : isenseIn(isensePin), out(gatePin)
{
    this->gatePin = gatePin;
    this->isensePin = isensePin;
    this->board = board;
    this->name = name;
    this->initial_state = initial_state;
    current_state = initial_state;
    out = initial_state;
    this->blinksInt = blinksC;
}


void Accessory::updateState(bool newState){

    if(current_state != newState){

        current_state = newState;

        if(newState && blinksInt){

            out = newState;
            t.attach(callback(this, &Accessory::blinks), BLINK_RATE);
        }

        else if(!newState && blinksInt){
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