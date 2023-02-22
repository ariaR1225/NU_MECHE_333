#include "nu32dip.h"
#define MAX_MESSAGE_LENGTH 200

int main(void){
    char message [MAX_MESSAGE_LENGTH]; 
    NU32DIP_Startup();
    NU32DIP_WriteUART1("Hello!\r\n");
    int i = 0;
    while(1){
        //delay
        _CP0_SET_COUNT(0);
        //print i
        sprintf(message,"%d\r\n",i);
        NU32DIP_WriteUART1(message);
        //update i
        i++; 
        while(_CP0_GET_COUNT()<24000000){} // 1sec
    }
    return 0; 
}