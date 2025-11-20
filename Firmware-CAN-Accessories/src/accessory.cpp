#include "mbed.h"

#define BLINK_RATE 1

using std::string;

class Accessory{
    public:
    
    PinName pin;
    int     board;
    string  name;
    int     initial_state;
    int     blinks;
    int     current_state;

    Accessory(PinName pin, int board, string name, int initial_state, int blinks) : out(pin){
        this->pin = pin;
        this->board = board;
        this->name = name;
        this->initial_state = initial_state;
        current_state = initial_state;
        out = initial_state;
        this->blinks = blinks;
    }

    void updateState(bool newState);
    void blinks();

    private:
    DigitalOut out;
    Ticker t;

};


void Accessory::updateState(bool newState){

    if(current_state != newState){

        current_state = newState;

        if(newState && blinks){

            out = newState;
            t.attach(&blinks, BLINK_RATE);
        }

        else if(!newState && blinks){
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