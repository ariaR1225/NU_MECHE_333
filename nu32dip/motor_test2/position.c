#include "nu32dip.h"
#include <stdio.h>
#include "position.h"
#include "utilities.h"
#include "encoder.h"

void position_init();
void __ISR(_TIMER_4_VECTOR, IPL5SOFT) Position();
void set_position();
void get_position();
void get_angle();

void position_init(){
    __builtin_disable_interrupts(); 
    // timer 4 ISR
    T2CONbits.TCKPS = 6; // prescaler = 64
    PR4 = 6249; // 1/200 /12.5 *10^9 /64 - 1 = 6249
    TMR4 = 0; // initial value = 0
    T4CONbits.ON = 1; // turn on Timer2
    IPC4bits.T4IP = 5; //set priority
    IPC4bits.T4IS = 0;
    IFS0bits.T4IF = 0;
    IEC0bits.T4IE = 1;
    __builtin_enable_interrupts(); 
    // digital IO
    TRISBbits.TRISB12 = 0; // set to output
}

void __ISR(_TIMER_4_VECTOR, IPL5SOFT) Position(){
switch(get_mode()){
    case HOLD:
    {
      ep = ref_pos[i] - get_encoder_degree(); // error = ref - actual
      eintp += ep;
      // anti-windup
      if (eintp > 1000){
        eintp = 1000;
      }
      if (eintp < -1000){
        eintp = -1000;
      }
      edotp = ep - eprev;
      u = Kp* ep+ Ki* eintp + Kd* edotp;
      eprev = ep;
      break;
    }
    case TRACK:
    {

      break;
    }
}}


void set_position(){
    NU32DIP_ReadUART1(buffer,BUF_SIZE);
    sscanf(buffer, "%f\n\r", &Kp);
    NU32DIP_ReadUART1(buffer,BUF_SIZE);
    sscanf(buffer, "%f\n\r", &Ki);
    NU32DIP_ReadUART1(buffer,BUF_SIZE);
    sscanf(buffer, "%f\n\r", &Kd);
}

void get_position(){
    sprintf(buffer,"%f\n\r", Kp);
    NU32DIP_WriteUART1(buffer);
    sprintf(buffer,"%f\n\r", Ki);
    NU32DIP_WriteUART1(buffer);
    sprintf(buffer,"%f\n\r", Kd);
    NU32DIP_WriteUART1(buffer);
}

void get_angle(){
    sprintf(buffer,"%f\n\r", ang);
    NU32DIP_WriteUART1(buffer);
}