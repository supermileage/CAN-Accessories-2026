#include "mbed.h"
#include <chrono>
#include "accessory.h"
#include <vector>


using std::string;

Accessory::Accessory(PinName pin, PinName isensePin, int board, string name, int initialState, int blinks) : out(pin), isenseIn(isensePin)
{
        this->pin = pin;
        this->isensePin = isensePin;
        this->board = board;
        this->name = name;
        this->initialState = initialState;
        this->currentState = initialState;
        //out = initial_state;
        this->blinks = blinks;
}


void Accessory::updateState(bool newState){

    //if(current_state != newState){

        currentState = newState;

        if(newState && blinks){

            out = newState;
            t.attach(callback(this, &Accessory::blink), BLINK_RATE);
        }

        else if((!newState && blinks)){
            t.detach();
            out = newState;
        }

        else{
            out = newState;
        }
    //}


};

void Accessory::blink(){

    out = !out;

}

AnalogIn Accessory::get_i_sense(){
    return isenseIn;
}
