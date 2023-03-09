#ifndef CURRENT__H__
#define CURRENT__H__

#include <xc.h> // processor SFR definitions
#include <sys/attribs.h> // __ISR macro
#include "nu32dip.h"   
#include "utilities.h"
#include <xc.h>
#include <stdio.h>

#define BUF_SIZE 200
#define NUMSAMPS 1000

volatile int duty; // pwm duty cycle
volatile float Gp, Gi; // PID parameter
static volatile float ACT_c[NUMSAMPS]; // actual value
static volatile float REF_c[NUMSAMPS]; // reference value
static volatile float u = 0, unew = 0, e = 0, eint = 0; // PID error & values

void current_init();
void set_current();
void get_current();
void makeWaveform();

#endif // CURRENT__H__

// all modes first IDLE and PWM