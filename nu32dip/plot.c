#include "nu32dip.h"                    // constants, functions for startup and UART
#define NUMSAMPS 1000                   // num of poibts in waveform
static volatile int Waceform[NUMSAMPS]; // waveform not cached (volatile) only in this C file (static)

#define NUMSAMPS 1000
#define PLOTPTS 200
#define DECIMATION 10
// number of points in waveform
// number of data points to plot
// plot every 10th point
static volatile int Waveform[NUMSAMPS];
static volatile int ADCarray[PLOTPTS];
static volatile int REFarray[PLOTPTS];
static volatile int StoringData = 0;
static volatile float Kp = 0, Ki = 0;

void makeWaveform();
unsigned int adc_sample_convert(int pin);

void __ISR(_TIMER_2_VECTOR, IPL5SOFT) Controller(void)
{
  static int counter = 0;
  static int adcval = adc_sample_convert(1);
  int error = adcval - Waceform[counter] 
  static int eint = 0;
  eint = eint + error;
  float u = Kp * ei + ki * eint;
  float unew = u + 50.;
  // safety

  if (unew > 100)
  {
    unew = 100;
  }

  if (unew < 0)
  {
    unew = 0;
  }

  // insert line(s) to set OC1RS
  OC1RS = (unsigned int)((unew / 100.0) * PR3);
  counter++;
  if (StoringData)
  {
    decctr++;
    if (decctr == DECIMATION)
    {
      // initialize counter once
      // index for data arrays; counts up to PLOTPTS
      // counts to store data one every DECIMATION
      //
      // after DECIMATION control loops,
      // reset decimation counter
      // store data in global arrays
      decctr = 0;
      ADCarray[plotind] = adcval;
      REFarray[plotind] = Waveform[counter];
      plotind++;
    }
    if (plotind == PLOTPTS)
    {
      plotind = 0;
      StoringData = 0;
    }
  }
  counter++;
  if (counter == NUMSAMPS)
  {
    // increment plot data index
    // if max number of plot points plot is reached,
    // reset the plot index
    // tell main data is ready to be sent to MATLAB
    // add one to counter every time ISR is entered
    counter = 0; // rollover counter over when end of waveform reached
  }
  // insert line to clear interrupt flag
}

int main(void)
{
  NU32DIP_Startup(); // cache on, interrupts on, LED/button init, UART init
  makeWaveform();

  // make pin
  RPB15bits.RPB15R = 0b0101;
  T3CONbits.TCKPS = 0;
  PR3 = 2400 - 1;
  TMR3 = 0;
  OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
  OC1RS = 0;              // duty cycle = OC1RS/(PR2+1) = 25%
  OC1R = 0;               // initialize before turning OC1 on; afterward it is read-only
  OC1CONbits.OCTSEL = 1;  // use t3 instead of t2
  T3CONbits.ON = 1;       // turn on Timer2
  OC1CONbits.ON = 1;      // turn on OC1

  // analog
  // AD1PCFGbits.PCFG14 = 0;                 // AN14 is an adc pin
  ANSELAbits.ANSA1 = 0; // use A1 as AN1
  AD1CON3bits.ADCS = 1; // ADC clock period is Tad = 2*(ADCS+1)*Tpb =
                        //                           2*3*12.5ns = 75ns < 83ns
  AD1CON1bits.ADON = 1;

  __builtin_disable_interrupts(); // INT step 2: disable interrupts at CPU
                                  // INT step 3: setup peripheral
  PR2 = 48000 - 1;
  TMR2 = 0;
  T2CONbits.TCKPS = 0;
  T2CONbits.ON = 1;
  IPC2bits.T2IP = 5;
  IPC2bits.T2IS = 0;
  IFS0bits.T2IF = 0;
  IEC0bits.T2IE = 1;
  __builtin_enable_interrupts();

  OC1RS = 1200; // set duty cycle to 50%

  char message[100];            // message to and from MATLAB
  float kptemp = 0, kitemp = 0; // temporary local gains
  int i = 0;
  while (1)
  {
    NU32DIP_ReadUART1(message, sizeof(message)); // wait for a message from MATLAB
    sscanf(message, "%f %f", &kptemp, &kitemp);
    __builtin_disable_interrupts();
    Kp = kptemp;
    Ki = kitemp;
    __builtin_enable_interrupts();
    StoringData = 1;
    while (StoringData)
    {
      ; // do nothing
    }
    for (i = 0; i < PLOTPTS; i++)
    {
      sprintf(message, "%d %d %d\r\n", PLOTPTS - i, ADCarray[i], REFarray[i]);
      NU32DIP_WriteUART1(message);
    }
  }
  return 0;
}

void makeWaveform()
{
  int i = 0, center = 1023 / 2, A = 1023 / 4; // square wave, fill in center value (50%) and amplitude (25%)
  for (i = 0; i < NUMSAMPS; ++i)
  {
    if (i < NUMSAMPS / 2)
    {
      Waveform[i] = center + A;
    }
    else
    {
      Waveform[i] = center - A;
    }
  }
}

unsigned int adc_sample_convert(int pin)
{ // sample & convert the value on the given
  // adc pin the pin should be configured as an
  // analog input in AD1PCFG
  unsigned int elapsed = 0, finish_time = 0;
  AD1CHSbits.CH0SA = pin; // connect chosen pin to MUXA for sampling
  AD1CON1bits.SAMP = 1;   // start sampling
  elapsed = _CP0_GET_COUNT();
  finish_time = elapsed + 10;
  while (_CP0_GET_COUNT() < finish_time)
  {
    ; // sample for more than 250 ns
  }
  AD1CON1bits.SAMP = 0; // stop sampling and start converting
  while (!AD1CON1bits.DONE)
  {
    ; // wait for the conversion process to finish
  }
  return ADC1BUF0; // read the buffer with the result
}