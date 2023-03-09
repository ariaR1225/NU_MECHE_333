// an enum of possible states, public in the utilities h file
enum mode_t {IDLE, PWM, ITEST, HOLD, TRACK}; 


// a variable of type mode_t, private in the utilities c file
// volatile because it is shared between the while(1) in main and the ISRs
volatile enum mode_t mode; 

// public function prototypes in the utilities h file
// function content in the utilities c file:
volatile enum mode_t get_mode(void){
	return mode;
}
void set_mode(enum mode_t s){
	mode = s;
}

Here is an example using the get function:

// inside an ISR
switch(get_mode()){
    case IDLE:
    {
      // set pwm duty to 0 and direction to 0
      break;
    }
}
