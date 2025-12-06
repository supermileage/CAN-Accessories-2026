#include "mbed.h"
#include <iostream>
#include <CAN.h>
#include <vector>
#include "accessory.h"
#include <chrono>

using std::string;

//CAN CONNECTIONS
#define CAN_TX PA_12
#define CAN_RX PA_11
#define CAN_STBY PA_9

//Gates
#define GATE1 PB_4
#define GATE2 PB_5  
#define GATE3 PA_8
#define GATE4 PB_1
#define GATE5 PB_6
#define GATE6 PB_7
#define GATE7 PB_0
#define GATE8 PA_10

//Current Sense Op amp output pins
#define OA1 PA_0
#define OA2 PA_1
#define OA3 PA_3
#define OA4 PA_4
#define OA5 PA_5
#define OA6 PA_6
#define OA7 PA_7
#define OA8 PA_2 //not actually an ADC

//Board Switch
#define SWITCH PB_3

//CAN
#define BAUD_RATE 50000
#define CAN_FORMAT 0x60

CAN can(CAN_RX, CAN_TX, BAUD_RATE);

//ADC
#define vref 3.3
#define op_amp_gain 20
#define r_shunt 0.05

//Telemetry
Ticker telem_ticker;
int send_telem(std::vector<Accessory*> totalAccList, CANMessage telem_msg){
  telem_msg.id = 0x61; //edit based on what telemetry wants
  telem_msg.len = 8; //temp
  for(int i = 0; i < totalAccList.size(); i++) {
    int raw_adc = (*totalAccList[i]).get_i_sense();
    int current_draw = (raw_adc*vref)/(op_amp_gain*r_shunt*1000); //in milli-amps
    telem_msg.data[i] = current_draw; //tell telemetry order of accessories based on totalAccList
  }
  can.write(telem_msg);
}

int main(){
//Declare all Accessories
//Not really sure how the two boards play out
//PinName_Board_Name_InitialState_Blinks
Accessory headlights  (GATE1, OA1, 1, "headlights", 0, 0);
Accessory wiper       (GATE2, OA2, 1, "wiper", 0, 0);
Accessory left_indic  (GATE3, OA3, 2, "left_indic", 1, 1);
Accessory right_indic (GATE4, OA4, 2, "right_indic", 1, 1);
Accessory horn        (GATE5, OA5, 1, "horn", 0, 0);
Accessory brakelights (GATE6, OA6, 0, "brakelights", 1, 1) ;
Accessory extra1      (GATE7, OA7, 2, "extra1", 0, 0);
Accessory extra2      (GATE8, OA8, 2, "extra2", 0 ,0);

//Pointer because you cant copy accesory type
std::vector<Accessory*>totalAccList = {&headlights, &wiper, &left_indic, &right_indic, &horn, &brakelights, &extra1, &extra2};
int amount_of_acc = totalAccList.size();

DigitalIn boardSwitch(PB_3);

//Initialize Accessory List Vector
std::vector<Accessory*> boardAccList;
//Fill up Accessory List Vector based on what board your on
for(int i = 0; i < amount_of_acc; i++){
  if(totalAccList[i]->board == boardSwitch.read() || 2){
    boardAccList.push_back(totalAccList[i]);
  }
}
int sizeBoardAcc = boardAccList.size();

//Initialize CAN
CANMessage msg;

bool nextState;

//Telemetry
// Ticker telem_ticker;
CANMessage telem_msg;
// telem_msg.id = 0x61; //temporary, will change based on what telemetry wants
// telem_msg.len = amount_of_acc;
// int send_telem_message() {
//   for(int i = 0; i < amount_of_acc; i++) {
//     int raw_adc = totalAccList[i]->isenseIn;
//     int current_draw = (raw_adc*vref)/(op_amp_gain*r_shunt*1000); //in milli-amps
//     telem_msg.data[i] = current_draw; //tell telemetry order of accessories based on totalAccList
//   }
//   can.write(telem_msg);
// }
telem_ticker.attach(callback([&]() {send_telem(totalAccList, telem_msg);}),TELEM_RATE);

// telem_ticker.attach(&Accessory::telem_message, TELEM_RATE);
// headlights.telem_message();
// right_indic.telem_message();

while(true) {

//Process messages
int mode = msg.data[0];
if(can.read(msg)){
  switch(mode){
  
  //Initial State  
  case 0: {
  for(int j = 0; j < amount_of_acc; j++){
          nextState = msg.data[1] >> j;
          nextState = (nextState & 1);
          if((totalAccList[j])->board == boardSwitch.read() || (totalAccList[j]->board == 2)){
              (*totalAccList[j]).updateState(nextState);
              printf("MODE 1: Object %s has been initialized\n", totalAccList[j]->name.c_str());
            }
        }
    }
//  case 1:
      //We literally have zero toggles LOL

  //on off implementation
  case 2:
    int num_useful_bytes = msg.len;
    for(int j = 0; (j < num_useful_bytes-1); j++) {
      int data_byte = msg.data[j+1];
      bool next_state = (data_byte & 1);
      int acc = data_byte >> 1;
      if(totalAccList[acc]->board == boardSwitch.read() || totalAccList[acc]->board == 2){
        (*totalAccList[acc]).updateState(next_state);
        printf("MODE 2: Object %s has been initialized\n", totalAccList[j]->name.c_str());
      }
    }
  }
}
}
}