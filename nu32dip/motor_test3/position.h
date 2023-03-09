#ifndef position__H__
#define position__H__

#define BUF_SIZE 200

volatile float Kp = 0, Ki = 0, Kd = 0;
volatile int ang = 0;
volatile int eprev = 0, ep = 0, eintp = 0, upos = 0, unewpos = 0, edotp = 0;
volatile int N = 0, ref_pos[100], motor_ang[100];
volatile int i = 0;

void position_init();
void __ISR(_TIMER_4_VECTOR, IPL5SOFT) Position();
void set_position();
void get_position();
void get_angle();
void set_cubic();
void set_step();

#endif //position__H__
