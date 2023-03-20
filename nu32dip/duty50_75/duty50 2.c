#include "nu32dip.h"          // constants, functions for startup and UART

int main(void) {
  NU32DIP_Startup();          // cache on, interrupts on, LED/button init, UART init
  float duty = .5;
  RPB15Rbits.RPB15R = 0b0101;
  T3CONbits.TCKPS = 2;     // Timer2 prescaler N=4 (1:4)
  PR3 = 2400 - 1;              // period = (PR2+1) * N * 12.5 ns = 100 us, 10 kHz
  TMR3 = 0;                // initial TMR2 count is 0
  OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
  OC1RS = 0;             // duty cycle = OC1RS/(PR2+1) = 25%
  OC1R = 0;              // initialize before turning OC1 on; afterward it is read-only
  OC1CONbits.OCTSEL = 1; // use timer 3 instead timer 2
  T3CONbits.ON = 1;        // turn on Timer2
  OC1CONbits.ON = 1;       // turn on OC1

  _CP0_SET_COUNT(0);       // delay 4 seconds to see the 25% duty cycle on a 'scope
  while(_CP0_GET_COUNT() < 4 * 40000000) {
    ;
  }
  OC1RS = 1800;            // set duty cycle to 75%
  while(1) {
    ;                      // infinite loop
  }
  return 0;
}

