#include "nu32dip.h"

#define MAX_MESSAGE_LENGTH 200

int main(void){
    char message [MAX_MESSAGE_LENGTH]; 
    NU32DIP_Startup();
    NU32DIP_WriteUART1("Hello!\r\n");
    int i = 0;

    initultra();

    while (1){
        sprintf(message,"%d\r\n",i);
        NU32DIP_WriteUART1(message);
        i++;
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<24000000){} 

        unsigned int time = readultra();
        float d;
        d = (time/2.)/24000000.*340.;
        if (d<4){
        sprintf(message,"%f\r\n",d);
        NU32DIP_WriteUART1(message);
        }
    }
    return 0; 
}