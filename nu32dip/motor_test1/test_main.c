// include header files here
#include "nu32dip.h" 
#include "encoder.h"
#include "ina219.h"
#include "utilities.h"
#include "i2c_master_noint.h"
#include "current.h"
#include <stdio.h>

#define BUF_SIZE 200
char buffer[BUF_SIZE];

int main() {
    NU32DIP_Startup(); // cache on, min flash wait, interrupts on, LED/button init, UART init
    NU32DIP_YELLOW = 1;  // turn off the LEDs
    NU32DIP_GREEN = 1;
    __builtin_disable_interrupts(); // initialize modules or peripherals here
    UART2_Startup();
    i2c_master_setup();
    INA219_Startup(); 
    current_init();
    __builtin_enable_interrupts();
    set_mode(IDLE);

    while(1) {
        NU32DIP_ReadUART1(buffer,BUF_SIZE); // we expect the next character to be a menu command
        NU32DIP_GREEN = 1;                   // clear the error LED
        switch (buffer[0]) {

        case 'a':
        {
            int counts = INA219_read_current()*3.;
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
            break;
        }

        case 'j':
        {
            break;
        }

        case 'k':
        {
            set_mode(ITEST);
            makeWaveform();
            sprintf(buffer,"%d\r\n", NUMSAMPS);
            NU32DIP_WriteUART1(buffer); 
            int i = 0;
            for(i = 0; i < NUMSAMPS; i++) {
                sprintf(buffer,"%d %d\n\r", REFarray[i], ADCarray[i]);
                NU32DIP_WriteUART1(buffer);    // send current array to client for plotting
        }
            break;
        }

        case 'l':
        {
            break;
        }

        case 'm':
        {
            break;
        }

        case 'n':
        {
            break;
        }

        case 'o':
        {
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
// case r return get mode
        case 'r':
        {
            sprintf(buffer,"%d\r\n", get_mode());
            NU32DIP_WriteUART1(buffer);  // send encoder count to client
            break;
        }
        default: 
        {
            NU32DIP_GREEN = 0;  // turn on LED2 to indicate an error
            break; 
        }
} }
return 0; }