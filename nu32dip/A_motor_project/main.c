// include header files here
#include <xc.h> // processor SFR definitions
#include <sys/attribs.h> // __ISR macro
#include <stdio.h>

#include "nu32dip.h"
#include "i2c_master_noint.h"
#include "ina219.h"
#include "encoder.h"
#include "utilities.h"
#include "current.h"
#include "position.h"

#define BUF_SIZE 200
char buffer[BUF_SIZE]; // initialize communication message array
    

int main() {
    NU32DIP_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
    NU32DIP_YELLOW = 1;  // turn off the LEDs
    NU32DIP_GREEN = 1;
    __builtin_disable_interrupts(); // initialize modules or peripherals here
    UART2_Startup(); // encoder
    i2c_master_setup(); // i2c
    INA219_Startup();  //ina
    current_init(); // current
    position_init(); // position
    __builtin_enable_interrupts();
    while(1) {
        NU32DIP_ReadUART1(buffer,BUF_SIZE); // we expect the next character to be a menu command
        NU32DIP_GREEN = 1;                   // clear the error LED
        switch (buffer[0]) {

        case 'a':
        {
            int counts = INA219_read_current()*3;
            sprintf(buffer,"%d\r\n", counts);
            NU32DIP_WriteUART1(buffer);
            break;
        }

        case 'b':
        {
            float current = INA219_read_current();
            sprintf(buffer,"%f\r\n", current);
            NU32DIP_WriteUART1(buffer);
            break;
        }

        case 'c': 
        {
            WriteUART2("a"); // request the encoder count
            while(!get_encoder_flag()){} // wait for the Pico to respond
            set_encoder_flag(0); // clear the flag so you can read again later
            int p = get_encoder_count(); // get the encoder value
            sprintf(buffer,"%d\r\n", p);
            NU32DIP_WriteUART1(buffer);  // send encoder count to client
            break;
        }

        case 'd':                     
        {
            WriteUART2("a"); // request the encoder count
            while(!get_encoder_flag()){} // wait for the Pico to respond
            set_encoder_flag(0); // clear the flag so you can read again later
            int p = get_encoder_count(); // get the encoder value
            float d = 360.*p/3360; // 360*p/resolution
            sprintf(buffer,"%f\r\n", d);
            NU32DIP_WriteUART1(buffer);  // send encoder count to client
            break;
        }

        case 'e':
        {   
            WriteUART2("b"); // request the encoder count
            break;
        }

        case 'f':
        {
            // set to PWM
            set_mode(PWM);
            // read from user input
            NU32DIP_ReadUART1(buffer,BUF_SIZE);
            sscanf(buffer, "%d", &duty);
            break;
        }

        case 'g':
        {
            set_current();
            break;
        }

        case 'h':
        {
            get_current();
            break;
        }

        case 'i':
        {
            set_position();
            break;
        }

        case 'j':
        {
            get_position();
            break;
        }

        case 'k':
        {
            int i = 0;
            set_mode(ITEST);
            makeWaveform();
            sprintf(buffer,"%d\r\n", NUMSAMPS);
            NU32DIP_WriteUART1(buffer); 
            for(i = 0; i < NUMSAMPS; i++) {
                sprintf(buffer,"%f %f\n\r", REF_c[i], ACT_c[i]);
                NU32DIP_WriteUART1(buffer);    // send current array to client for plotting
            break;
            }
        }

        case 'l':
        {
            WriteUART2("b");
            set_mode(HOLD);
            break;
        }

        case 'm':
        {
            set_step();
            break;
        }

        case 'n':
        {
            set_cubic();
            break;
        }

        case 'o':
        {
            int i = 0;
            int N = 0;
            WriteUART2("b");
            set_mode(TRACK);
            sprintf(buffer,"%d\r\n", N);
            NU32DIP_WriteUART1(buffer); 
            for(i = 0; i < N; i++) {
                sprintf(buffer,"%f %f\n\r", REF_p[i], ACT_p[i]);
                NU32DIP_WriteUART1(buffer);    // send current array to client for plotting
            }
            break;
        }

        case 'p':
        {
            set_mode(IDLE);
            break;
        }

        case 'q': 
        {
            set_mode(IDLE);
            break; 
        }

        case 'r':
        {
            sprintf(buffer,"%d\r\n",get_mode());
            NU32DIP_WriteUART1(buffer);
            break;
        }

        default: 
        {
            NU32DIP_GREEN = 0;  // turn on LED2 to indicate an error
            break; 
        }
         } }
return 0; }