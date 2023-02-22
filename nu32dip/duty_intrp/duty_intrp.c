#include "nu32dip.h"                    // constants, functions for startup and UART
#define NUMSAMPS 1000                   // number of points in waveform
static volatile int Waveform[NUMSAMPS]; // waveform
void makeWaveform();

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void)
{
  static int counter = 0;
  OC1RS = Waveform[counter];
  counter++;
  if (counter == NUMSAMPS)
  {
    counter = 0;
  }
  IFS0bits.T2IF = 0;
}

int main(void)
{
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  makeWaveform();
  float duty = .5;
  RPB15Rbits.RPB15R = 0b0101;
  T3CONbits.TCKPS = 2;    // Timer2 prescaler N=4 (1:4)
  PR3 = 2400 - 1;         // period = (PR2+1) * N * 12.5 ns = 100 us, 10 kHz
  TMR3 = 0;               // initial TMR2 count is 0
  OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
  OC1RS = 0;              // duty cycle = OC1RS/(PR2+1) = 25%
  OC1R = 0;               // initialize before turning OC1 on; afterward it is read-only
  OC1CONbits.OCTSEL = 1;  // use timer 3 instead timer 2
  T3CONbits.ON = 1;       // turn on Timer2
  OC1CONbits.ON = 1;      // turn on OC1

__builtin_disable_interrupts(); // INT step 2: disable interrupts at CPU
  PR2 = 48000-1;
  TMR2 = 0;
  T2CONbits.TCKPS = 0;
  T2CONbits.ON = 1;
  IPC2bits.T2IP = 5;
  IPC2bits.T2IS = 0;
  IFS0bits.T2IF = 0;
  IEC0bits.T2IE = 1;
  __builtin_enable_interrupts();

  //OC1RS = 1200; // set duty cycle to 50%
  while (1)
  {
    ; // infinite loop
  }
  return 0;
}

void makeWaveform()
{
  int i = 0, center = 1200, A = 600; // square wave, fill in center value and amplitude
  for (i = 0; i < NUMSAMPS; ++i)
  {
    if (i < NUMSAMPS / 2)
    {
      Waveform[i] = center + A;
    }
    else
    {
      Waveform[i] = center - A;
    }
  }
}
