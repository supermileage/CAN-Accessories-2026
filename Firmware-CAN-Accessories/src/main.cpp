#include "mbed.h"
#include <iostream>
//#include <CAN.h>
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
#define OA8 PA_2 //this disables serial communication.

//Board Switch
#define SWITCH PB_3

//Front and Back boards
#define FRONT 0
#define BACK 1
#define BOTH 2

//CAN
#define BAUD_RATE 500000
#define CAN_FORMAT 0x60

//Telemetry
Ticker t;
int send_can_message(int message) {

}

int main(){
//Declare all Accessories
                      //Gate,Opamp,Board,Name,InitialState,Blinks
Accessory headlights  (GATE1, OA1, FRONT, "headlights" , 0, 0);// id0
Accessory wiper       (GATE2, OA2, FRONT, "wiper"      , 0, 0);// id1
Accessory leftIndic   (GATE3, OA3, BOTH , "leftIndic"  , 1, 1);// id2
Accessory rightIndic  (GATE4, OA4, BOTH , "rightIndic" , 1, 1);// id3
Accessory horn        (GATE5, OA5, FRONT, "horn"       , 0, 0);// id4
Accessory brakelights (GATE6, OA6, BACK , "brakelights", 1, 1);// id5

//Pointer because you cant copy accesory type
std::vector<Accessory*>totalAccList = {&headlights, &wiper, &leftIndic, &rightIndic, &horn, &brakelights};
int accAmount = totalAccList.size();

//Declare board switch pin
DigitalIn boardSwitch(SWITCH);

//Initialize CAN
CAN can(CAN_RX, CAN_TX, BAUD_RATE);
DigitalOut canStby(CAN_STBY);
//disable CAN standby
canStby=0;

CANMessage msg;

while(true)
{
    if(can.read(msg) && msg.id == CAN_FORMAT)
    {
        switch(msg.data[0])
        {
    
            //IOPERATION MODE 0: Set status of all accessories at once
            case 0: 
                for(int j = 0; j < accAmount; j++)
                {
                    bool nextState = (msg.data[1] >> j) & 1;
                    
                    if((totalAccList[j])->board == boardSwitch.read() || (totalAccList[j]->board == BOTH)){
                        (*(totalAccList[j])).updateState(nextState);
                    }
                }
                
            break;

            //OPERATION MODE 2: turn individual accessories on and off.
            case 2:
                
                for(int j = 1; j < msg.len; j++)
                {
                    bool nextState = (msg.data[j] & 1);
                    int acc = msg.data[j] >> 1;
                    
                    if(totalAccList[acc]->board == boardSwitch.read() || totalAccList[acc]->board == BOTH){
                        (*(totalAccList[acc])).updateState(nextState);
                    }
                }
        }
    }

} 

}
