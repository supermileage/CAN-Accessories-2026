#include "mbed.h"

using namespace std::chrono_literals;
#define BLINK_RATE 1000ms
#define TELEM_RATE 1000ms

using std::string;

class Accessory{
    public:
    
    PinName pin;
    PinName isensePin;
    int     board;
    string  name;
    int     initial_state;
    int     blinks_int;
    int     current_state;

    Accessory(PinName pin, PinName isensePin, int board, string name, int initial_state, int blinks_int);
    void updateState(bool newState);
    void blinks();
    void telem_message(std::vector<Accessory*>);
    AnalogIn get_i_sense();

    private:
    DigitalOut out;
    AnalogIn isenseIn;
    Ticker t;
    Ticker telem_ticker;

};

