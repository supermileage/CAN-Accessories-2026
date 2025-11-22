#include "mbed.h"

using namespace std::chrono_literals;
#define BLINK_RATE 1000ms

using std::string;

class Accessory{
    public:
    
    PinName pin;
    int     board;
    string  name;
    int     initial_state;
    int     blinks_int;
    int     current_state;

    Accessory(PinName pin, int board, string name, int initial_state, int blinks_int);
    void updateState(bool newState);
    void blinks();

    private:
    DigitalOut out;
    Ticker t;

};

