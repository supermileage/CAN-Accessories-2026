#include <mbed.h>
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
Accessory brakelights (GATE6, 0, "brakelights", 1, 1) ;
//Can remove extra 1 and 2

//Pointer because you cant copy accesory type
std::vector<Accessory*>totalAccList = {&headlights, &wiper, &left_indic, &right_indic, &horn, &brakelights};
int amount_of_acc = totalAccList.size();

DigitalIn boardSwitch(SWITCH);

//Initialize Accessory List Vector
std::vector<Accessory*> boardAccList;
//Fill up Accessory List Vector based on what board your on

for(int i = 0; i < amount_of_acc; i++){
  if(totalAccList[i]->board == boardSwitch.read() || totalAccList[i]->board == 2){
    boardAccList.push_back(totalAccList[i]);
  }
}

int sizeBoardAcc = boardAccList.size();

//Initialize CAN
DigitalOut canstby(CAN_STBY);
canstby = 0;

CAN can(CAN_RX, CAN_TX, BAUD_RATE);
CANMessage msg;


bool nextState;

DigitalOut ctx(CAN_TX);
DigitalOut crx(CAN_RX);

while(true)
{


    // for (int i = 0; i < amount_of_acc; i++)
    // {
    //     (* (totalAccList[i])).updateState(1);        
    // }
    if(can.read(msg))
    {
        
        //int mode = msg.data[0];
        switch(msg.data[0])
        {
    
  //Initial State  
            case 0:
                for(int j = 0; j < 6; j++){
                    nextState = (msg.data[1] >> j) & 1;
                    
                    //if((totalAccList[j])->board == boardSwitch.read() || (totalAccList[j]->board == 2)){
                        (*(totalAccList[j])).updateState(nextState);
                    //}
                }
            break;
  //case 1:
      //We literally have zero toggles LOL 



  //on off implementation
            case 2:
                int num_useful_bytes = msg.len;
                
                for(int j = 1; (j < num_useful_bytes); j++)
                {
                    //int data_byte = msg.data[j+1];
                    bool next_state = (msg.data[j] & 1);
                    int acc = msg.data[j] >> 1;
                    
                    //if(totalAccList[acc]->board == boardSwitch.read() || totalAccList[acc]->board == 2){
                        (*(totalAccList[acc])).updateState(next_state);
                    //}
                }
        }
    }

    
} 

}