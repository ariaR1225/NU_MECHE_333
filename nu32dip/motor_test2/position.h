#ifndef position__H__
#define position__H__

#define BUF_SIZE 200
#define NUMSAMPS 1000

volatile float Kp, Ki, Kd;
volatile float ang = 0;
volatile float eprev = 0, ep = 0, eintp = 0, upos = 0, unewpos = 0, edotp = 0;
volatile float ACT_p[NUMSAMPS], REF_p[NUMSAMPS];

void position_init();
void set_position();
void get_position();
void get_angle();

#endif //position__H__
