#ifndef position__H__
#define position__H__

#define BUF_SIZE 200
#define NUMSAMPS 1000

volatile float Kp, Ki, Kd; // PID parameter
static volatile float ang = 0; // encoder angle
static volatile float eprev = 0, ep = 0, eintp = 0, upos = 0, edotp = 0; // PID error & value
static volatile float ACT_p[NUMSAMPS], REF_p[NUMSAMPS]; // actual & reference values

void position_init();
void set_position();
void get_position();
void get_angle();
void set_step();
void set_cubic();

#endif //position__H__
