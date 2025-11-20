#include <mbed.h>
#include <iostream>
#include <CAN.h>
#include <vector>
#include "Accessory.h"

using std::string;

//CAN CONNECTIONS
#define CAN_TX PA_12
#define CAN_RX PA_9
#define CAN_STBY PA_11

//Gates
#define GATE1 PB_4
#define GATE2 PB_5  
#define GATE3 PA_8
#define GATE4 PB_1
#define GATE5 PB_6
#define GATE6 PB_7
#define GATE7 PB_0
#define GATE8 PA_10

//Board Switch
#define SWITCH PB_3

#define BAUD_RATE 50000
#define CAN_FORMAT 0x60

int main(){

//Declare all Accessories
//Not really sure how the two boards play out
                      //PinName_Board_Name_InitialState_Blinks
Accessory headlights  (GATE1, 1, "headlights", 0, 0);
Accessory wiper       (GATE2, 1, "wiper", 0, 0);
Accessory left_indic  (GATE3, 2, "left_indic", 1, 1);
Accessory right_indic (GATE4, 2, "right_indic", 1, 1);
Accessory horn        (GATE5, 1, "horn", 0, 0);
Accessory breaklights (GATE6, 0, "breaklights", 1, 1) ;
Accessory extra1      (GATE7, 2, "extra1", 0, 0);
Accessory extra2      (GATE8, 2, "extra2", 0 ,0);

//Pointer because you cant copy accesory type
std::vector<Accessory*>totalAccList = {&headlights, &wiper, &left_indic, &right_indic, &horn, &breaklights, &extra1, &extra2};
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
CAN can(CAN_RX, CAN_TX, BAUD_RATE);
CANMessage incoming_message;

int mode = incoming_message.data[0];
if(can.read(incoming_message)){
  switch(mode){
  
  //Initial State  
  case 0:
    for(int i = 0; i < sizeBoardAcc; i++){
    }
  case 1:
      //We literally have zero toggles LOL

  //on off implementation
  case 2:

  }
}



}