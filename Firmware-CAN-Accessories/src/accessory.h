#include <mbed.h>

using namespace std::chrono_literals;

#define BLINK_RATE 1000ms

using std::string;

class Accessory{
    public:
    
    PinName gatePin;
    PinName isensePin;
    int     board;
    string  name;
    int     initial_state;
    int     blinksInt;
    int     current_state;

    Accessory(PinName gatePin, PinName isensePin, int board, string name, int initial_state, int blinksC);
    //     this->pin = pin;
    //     this->board = board;
    //     this->name = name;
    //     this->initial_state = initial_state;
    //     current_state = initial_state;
    //     out = initial_state;
    //     this->blinksInt = blinksC;
    // }

    void updateState(bool newState);
    void blinks();

    private:
    DigitalOut out;
    AnalogIn isenseIn;
    Ticker t;

};
