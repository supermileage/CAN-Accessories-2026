#include <mbed.h>
#include <chrono>

using namespace std::chrono_literals;
#define BLINK_RATE 1000ms
using namespace std::chrono_literals;
#define BLINK_RATE 1000ms

using std::string;

class Accessory{
    public:
    
    PinName pin;
    PinName isensePin;
    int     board;
    string  name;
    int     initial_state;
    int     blinks_int;
    int     blinks_int;
    int     current_state;
    int     id;

    Accessory(PinName pin, PinName isensePin, int board, string name, int initial_state, int blinks_int);
    void updateState(bool newState);
    void blinks();
    void telem_message();

    private:
    DigitalOut out;
    DigitalIn isenseIn;
    Ticker t;

};

