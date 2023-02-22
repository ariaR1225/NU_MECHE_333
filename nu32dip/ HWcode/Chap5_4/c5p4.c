#include <xc.h>  
#include "nu32dip.h"
int main(void){
    NU32DIP_Startup(); 
    unsigned int u1=33, u2=17, u3;
    u3 = u1 & u2;
    u3 = u1 | u2;
    u3 = u2 << 4;
    u3 = u1 >> 3;
    return 0;
}