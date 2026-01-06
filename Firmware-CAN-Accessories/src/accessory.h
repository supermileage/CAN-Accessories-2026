#include "mbed.h"

using namespace std::chrono_literals;
#define BLINK_RATE 500ms
#define TELEM_RATE 1000ms

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
    float get_i_sense();

    private:
    DigitalOut out;
    AnalogIn isenseIn;
    Ticker t;

};

