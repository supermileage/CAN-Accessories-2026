#include "mbed.h"

using namespace std::chrono_literals;
#define BLINK_RATE 500ms

using std::string;

class Accessory{
    public:
    
    PinName pin;
    PinName isensePin;
    int     board;
    string  name;
    int     initialState;
    int     blinks;
    int     currentState;

    Accessory(PinName pin, PinName isensePin, int board, string name, int initialState, int blinks);
    void updateState(bool newState);
    void blink();
    void telem_message();

    private:
    DigitalOut out;
    DigitalIn isenseIn;
    Ticker t;

};

