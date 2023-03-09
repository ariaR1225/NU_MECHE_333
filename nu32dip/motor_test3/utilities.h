#ifndef UTILITIES__H__
#define UTILITIES__H__

#include <xc.h> // processor SFR definitions
#include <sys/attribs.h> // __ISR macro

#include "nu32dip.h"

enum mode_t {IDLE, PWM, ITEST, HOLD, TRACK}; 

volatile enum mode_t get_mode(void);
void set_mode(enum mode_t s);

#endif // UTILITIES__H__
