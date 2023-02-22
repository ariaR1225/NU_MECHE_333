#include <xc.h>          // Load the proper header for the processor
#define MAXCYCLES 1000000
#define DELTACYCLES 100000

int cycles;
void delay(void);


int main(void) {
  TRISBCLR = 0x30; // Make TRIS for B4 and B5 0, 
  LATBbits.LATB4 = 1;    // Turn GREEN on and YELLOW off.  These pins sink current
  LATBbits.LATB5 = 1;    // on the NU32DIP, so "high" (1) = "off" and "low" (0) = "on"

  while(1) {
    delay();
    LATBINV = 0b110000;    // toggle GREEN and YELLOW; 
  }
  return 0;
}

void delay() {
  for (cycles = 0; cycles < MAXCYCLES; cycles = cycles + DELTACYCLES) { // number is 1 million
    while(!PORTAbits.RA4) {
        ;   // Pin A4 is the USER switch, low (FALSE) if pressed.
    }
  }
}
