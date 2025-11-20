#include "mbed.h"
using std::string;

class Accessory {
  public:
    string Name;
    PinName Mosfet_gate;
    int CAN_ID;
    bool Do_it_blink_question_mark;

    Accessory(string name, PinName mosfet_gate, int can_id, bool do_it_blink_question_mark) { //mosfet gate doubles as index
      Name = name;
      Mosfet_gate = mosfet_gate;
      CAN_ID = can_id;
      Do_it_blink_question_mark = do_it_blink_question_mark;
    }
};

// CREATING OBJECTS
Accessory headlights = Accessory("headlights", PB_4, 0, 0);
Accessory wipers = Accessory("wipers", PB_5, 1, 0);
Accessory left_indicator = Accessory("left_indicator", PA_8, 2, 1);
Accessory right_indicator = Accessory("right_indicator", PB_1, 3, 1);
Accessory horn = Accessory("horn", PB_6, 4, 0);
Accessory hazards = Accessory("hazards", PB_7, 5, 1); 
Accessory extra1 = Accessory("extra1", PB_0, 6, 0);
Accessory extra2 = Accessory("extra2", PA_10, 7, 0);

//GATE PINS
PinName gate_list[8] = {PB_4, PB_5, PA_8, PB_1, PB_6, PB_7, PB_0, PA_10}; //consistent with indices

//INITIALIZATION
int current_states[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int new_states[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int bits[8] = {0};

//CAN INITIALIZATION
CAN can(PA_9, PA_12);

//FUNCTIONS
void can_message_received() {
  CANMessage incoming_message;
  can.read(incoming_message);

  unsigned char operation_mode = incoming_message.data[0];

  switch (operation_mode) {
    case (0): {
      uint8_t data_byte = incoming_message.data[1];
      for (int i = 0; i<8; i++) {
        bool bit = (data_byte >> i) & 0x01;
        change_state(i, bit); //i=0 is accessory index 0 (headlights), i=7 is accessory index 7 (extra), going from left to right
      }
      break;
    }
    case (1): {
      //ain't nobody use toggle mode
      break;
    }
    case (2): {
      uint8_t data_byte = incoming_message.data[1];
      uint8_t id = data_byte >> 1;
      uint8_t set = data_byte & 0x01;
      break;
    }
  }
}

void change_state(int id, int state) {
  DigitalOut(gate_list[id], state);
}



int main() {
  
  can.attach(&can_message_received, CAN::RxIrq);
  
  while (true) {
    
  }
}