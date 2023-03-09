#include "nu32dip.h"
#include <xc.h>

void initultra(){
    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 0;
    LATBbits.LATB7 = 0;
}
unsigned int readultra(){
    LATBbits.LATB7 = 1;
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT()<24000000*.00001){} // 10us
    LATBbits.LATB7 = 0;

    while (PORTBbits.RB6 == 0){}
    _CP0_SET_COUNT(0);
    while (PORTBbits.RB6 == 1){} // time4m
    return _CP0_GET_COUNT();
};
