/*
read core timer
wait core timer to be + 1ms
check pushed (port == 0)? 
 if first time 
    set core timer to 0 
    set flat to 1
 if flag == 1 
    read core timer
    set flag to 0
 else
    nothing
clear int flag
*/

#include "nu32dip.h"          // constants, funcs for startup and UART
#define DTIME 10 //ms

static volatile unsigned int timerState = 0; // first push = 0, second push = 1

void __ISR(_EXTERNAL_0_VECTOR, IPL6SOFT) Ext0ISR(void) { // step 1: the ISR

   char msg[100];
   double time;

   while(_CP0_GET_COUNT() < 10000000/10/25*DTIME) { ; } // Debounce

   if(!NU32DIP_USER){
      if (timerState){
         _CP0_SET_COUNT(0);
         NU32DIP_WriteUART1("Press the USER button again to stop the timer.\r\n");
      }
      else{
         time = _CP0_GET_COUNT();
         time = time/24000000;
         sprintf(msg,"%f seconds elapsed.\r\n",time);
         NU32DIP_WriteUART1(msg);
      }
      timerState = !timerState;
   }

  
   IFS0bits.INT0IF = 0;            // clear interrupt flag IFS0<3>
}

void startTimer(){
   NU32DIP_WriteUART1("Press the USER button to start the timer.\r\n");
}

int main(void) {
  NU32DIP_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
  __builtin_disable_interrupts(); // step 2: disable interrupts
  INTCONbits.INT0EP = 0;          // step 3: INT0 triggers on falling edge
  IPC0bits.INT0IP = 6;            // step 4: interrupt priority 2
  IPC0bits.INT0IS = 0;            // step 4: interrupt priority 1
  IFS0bits.INT0IF = 0;            // step 5: clear the int flag
  IEC0bits.INT0IE = 1;            // step 6: enable INT0 by setting IEC0<3>
  __builtin_enable_interrupts();  // step 7: enable interrupts
                                  // Connect RD7 (USER button) to INT0 (RD0)
  startTimer();
  while(1) {
      ; // do nothing, loop forever
  }

  return 0;
}
