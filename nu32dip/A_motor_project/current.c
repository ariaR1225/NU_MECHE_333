#include "nu32dip.h"     
#include "utilities.h"
#include "current.h"
#include <xc.h>
#include "ina219.h"
#include <stdio.h>
#include "position.h"
#define BUF_SIZE 200

char buffer[BUF_SIZE];
int i_c = 0;

void current_init(void) {
  __builtin_disable_interrupts(); 
  // timer 2 ISR
  T2CONbits.TCKPS = 3; // prescaler = 8
  PR2 = 1999; // 1/5k /12.5 *10^9 /8 - 1 = 1999
  TMR2 = 0; // initial value = 0
  T2CONbits.ON = 1; // turn on Timer2
  IPC2bits.T2IP = 6; //set priority
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
  LATBbits.LATB12 = 0;
}

void __ISR(_TIMER_2_VECTOR, IPL6SOFT) Current(){
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
      OC1RS = 2000.*abs(duty)/100.;
      if(duty < 0){
        LATBbits.LATB12 = 0;
      }
      else{
        LATBbits.LATB12 = 1;
      }
      break;
    }
    case ITEST:
    {
      ACT_c[i_c] = INA219_read_current();
      e = REF_c[i_c] - ACT_c[i_c];
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
      if (i_c == 99){
        e = 0;
        eint = 0;
        i_c = 0;
        set_mode(IDLE);
      }
      i_c++;
      break;
    }
    case HOLD:
    {
      ACT_c[i_c] = INA219_read_current();
      e = upos - ACT_c[i_c];
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
      if (i_c == 99){
        e = 0;
        eint = 0;
        i_c = 0;
        upos = 0;
      }
      i_c++;
      break;
    }
    case TRACK: // same as HOLD(?)
    {
      ACT_c[i_c] = INA219_read_current();
      e = upos - ACT_c[i_c];
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
      if (i_c == 99){
        e = 0;
        eint = 0;
        i_c = 0;
        upos = 0;
      }
      i_c++;
      break;
    }
  }
  IFS0bits.T2IF = 0; // clear interrupt flag
}
void set_current(){
  NU32DIP_ReadUART1(buffer,BUF_SIZE);
  sscanf(buffer, "%f\r\n", &Gp);
  NU32DIP_ReadUART1(buffer,BUF_SIZE);
  sscanf(buffer, "%f\r\n", &Gi);
}

void get_current(){
  sprintf(buffer,"%f\r\n", Gp);
  NU32DIP_WriteUART1(buffer);
  sprintf(buffer,"%f\r\n", Gi);
  NU32DIP_WriteUART1(buffer);
}

void makeWaveform()
{
  int i = 0, center = (PR3 + 1) / 2, A = (PR3 + 1) / 4; // square wave, fill in center value (50%) and amplitude (25%)
  for (i = 0; i < NUMSAMPS; ++i)
  {
    if (i < .25*NUMSAMPS || (i > .5*NUMSAMPS && i < .75*NUMSAMPS)) // 0~25, 50~75
    {
      REF_c[i_c] = center + A;
    }
    else // 25~50, 75~100
    {
      REF_c[i_c] = center - A;
    }
  }
}