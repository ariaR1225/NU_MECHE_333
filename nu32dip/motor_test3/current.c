#include "nu32dip.h"     
#include "utilities.h"
#include "current.h"
#include <xc.h>
#include <stdio.h>
#define BUF_SIZE 200
              
void current_init();
void __ISR(_TIMER_2_VECTOR, IPL7SOFT) Current();
void set_current();
void get_current();
void makeWaveform();

void current_init(void) {
  __builtin_disable_interrupts(); 
  // timer 2 ISR
  T2CONbits.TCKPS = 3; // prescaler = 8
  PR2 = 1999; // 1/5k /12.5 *10^9 /8 - 1 = 1999
  TMR2 = 0; // initial value = 0
  T2CONbits.ON = 1; // turn on Timer2
  IPC2bits.T2IP = 7; //set priority
  IPC2bits.T2IS = 0;
  IFS0bits.T2IF = 0;
  IEC0bits.T2IE = 1;
  __builtin_enable_interrupts(); 
  // timer 3 & oc1 PWM
  // timer 3
  T3CONbits.TCKPS = 1; // prescaler = 2
  PR3 = 1999; // 1/20k /12.5 *10^9 /2 - 1 = 1999
  TMR3 = 0; // initial value = 0
  OC1CONbits.OCTSEL = 1; // use t3 instead of t2
  T3CONbits.ON = 1; // turn on Timer3
  // oc1
  RPB15Rbits.RPB15R = 0b0101;
  OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
  OC1RS = 500; // duty cycle = OC1RS/(PR2+1) = 25%
  OC1R = 0; // initialize before turning OC1 on; afterward it is read-only
  OC1CONbits.ON = 1; // turn on OC1
  
  // digital IO
  TRISBbits.TRISB12 = 0; // set to output
}

void __ISR(_TIMER_2_VECTOR, IPL7SOFT) Current(void){
  switch(get_mode()){
    case IDLE:
    {
      // set duty cycle to 0
      duty = 0;
      OC1RS = 0;
      break;
    }
    case PWM:
    {
      if(duty < 0){
        LATBbits.LATB12 = 1;
        OC1RS = 2000.*(-duty)/100.;
      }
      else{
        LATBbits.LATB12 = 0;
        OC1RS = 2000.*(duty)/100.;
      }
      break;
    }
    case ITEST:
    {
      ADCarray[i] = INA219_read_current();
      e = REFarray[i] - ADCarray[i];
      eint += e;
      // anti-windup
      if (eint > 1000){
        eint = 1000;
      }
      if (eint < -1000){
        eint = -1000;
      }
      u = Gp * e + Gi * eint;
      // direction
      if (u < 0){
        LATBbits.LATB12 = 1; 
      }
      else{
        LATBbits.LATB12 = 0;
      }
      OC1RS = ((abs(u) / 100.0) * PR3);
      // reset
      if (i == 99){
        e = 0;
        eint = 0;
        i = 0;
        set_mode(IDLE);
      }
      i++;
      break;
    }
    case HOLD:
    {
      break;
    }
    case TRACK:
    {
      break;
    }
  }
  IFS0bits.T2IF = 0; // clear interrupt flag
}
void set_current(){
  NUDIP32_ReadUART1(buffer,BUF_SIZE);
  sscanf(buffer, "%f\n\r", &Gp);
  NUDIP32_ReadUART1(buffer,BUF_SIZE);
  sscanf(buffer, "%f\n\r", &Gi);
}

void get_current(){
  sprintf(buffer,"%f\n\r", Gp);
  NUDIP32_WriteUART1(buffer);
  sprintf(buffer,"%f\n\r", Gi);
  NUDIP32_WriteUART1(buffer);
}

void makeWaveform()
{
  int i = 0, center = (PR3 + 1) / 2, A = (PR3 + 1)  / 4; // square wave, fill in center value (50%) and amplitude (25%)
  for (i = 0; i < NUMSAMPS; ++i)
  {
    if (i < .25*NUMSAMPS || (i > .5*NUMSAMPS && i < .75*NUMSAMPS))
    {
      REFarray[i] = center + A;
    }
    else
    {
      REFarray[i] = center - A;
    }
  }
}