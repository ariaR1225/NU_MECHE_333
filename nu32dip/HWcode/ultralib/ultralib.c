#include "nu32dip.h"
#define MAX_MESSAGE_LENGTH 200

int main(void){
    char message [MAX_MESSAGE_LENGTH]; 
    NU32DIP_Startup();
    NU32DIP_WriteUART1("Hello!\r\n");
    int i = 0;
    // B6 input 1
    // B7 output 0
    // B7 init to off
    // TRISB = TRISB | 0b1000000; // or to 1 and to 0
    // TRISBCLR = 0b1000000; // put 0 in certain position (quick)
    // TRISBSET; // put 1 in certain position
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 0;
    LATBbits.LATB7 = 0;

    while (1){
        sprintf(message,"%d\r\n",i);
        NU32DIP_WriteUART1(message);
        i++;
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<24000000){} 

        LATBbits.LATB7 = 1;
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT()<24000000*.00001){} // 10us
        LATBbits.LATB7 = 0;

        while (PORTBbits.RB6 == 0){}
        _CP0_SET_COUNT(0);
        while (PORTBbits.RB6 == 1){} // time4m
        unsigned int time = _CP0_GET_COUNT();
        float d;
        d = (time/2.)/24000000.*340.;
        if (d<4){
        sprintf(message,"%f\r\n",d);
        NU32DIP_WriteUART1(message);
        }
    }
    return 0; 
}