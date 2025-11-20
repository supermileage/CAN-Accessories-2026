#include <mbed.h>
using std::string;

class Accessory {
  public:
    string name;
    int mosfet_gate;
    int CAN_ID;

    Accessory(string name, int mosfet_gate, int CAN_ID) {
      name = name;
      mosfet_gate = mosfet_gate;
      CAN_ID = CAN_ID;
    }
};

string accessory_list = {"headlights", "wipers", "left_indicator"}

Accessory headlights = Accessory("headlights", 1, 1);
Accessory wipers = Accessory("wipers", 2, 2);
Accessory left_indicator = Accessory("left_indicator", 3, 3);
Accessory right_indicator = Accessory("right_indicator", 4, 4);
Accessory horn = Accessory("horn", 5, 5);
Accessory hazards = Accessory("hazards", 6, 6);

int current_states[6] = {0, 0, 0, 0, 0, 0};
int new_states[6] = {0, 0, 0, 0, 0, 0};

int bits[8] = {0};

CAN can(PA_9, PA_12);

void can_message_received() {
  CANMessage incoming_message;
  can.read(incoming_message);

  unsigned char operation_mode = incoming_message.data[0];

  switch (operation_mode) {
    case (0):
      uint8_t data_byte = incoming_message.data[1];
      for (int i = 0; i<8; i++) {
        bool bit = (data_byte >> i) & 0x01;
        change_state(i, bit);
      }
    
    case (1):
      //ain't nobody use toggle mode
    
    case (2):
      //stuff
  }
}

void change_state(int id, int state) {

}



int main() {
  
  can.attach(&can_message_received, CAN::RxIrq);
  
  while (true) {
    
  }
}



// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}