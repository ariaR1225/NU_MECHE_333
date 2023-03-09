#include "helper.h"

void calculateGrowth(Investment *invp) {

  int i;

 
  for (i = 1; i <= invp->years; i= i + 1) {   
    invp->invarray[i] = invp->growth * invp->invarray[i-1]; 
  }
} 


int getUserInput(Investment *invp) {

  char str1[100],str2[100],str3[100],str4[100];
  int valid;       

  sprintf(str1,"Enter investment, growth rate, number of yrs (up to %d): ",MAX_YEARS);
  NU32DIP_WriteUART1(str1);
  NU32DIP_ReadUART1(str4,100);
  sscanf(str4,"%lf %lf %d", &(invp->inv0), &(invp->growth), &(invp->years));


  valid = (invp->inv0 > 0) && (invp->growth > 0) &&
    (invp->years > 0) && (invp->years <= MAX_YEARS);
  sprintf(str2,"Valid input?  %d\r\n",valid);
  NU32DIP_WriteUART1(str2);
  
  if (!valid) { 
    sprintf(str3,"Invalid input; exiting.\r\n");
    NU32DIP_WriteUART1(str3);
  }
  return(valid);
} 

void sendOutput(double *arr, int yrs) {

  int i;
  char outstring[100], str1[100], str2[100];      

  sprintf(str1,"\r\nRESULTS:\r\n\n");
  NU32DIP_WriteUART1(str1);
  for (i=0; i<=yrs; i++) {  
    sprintf(outstring,"Year %3d:  %10.2f\r\n",i,arr[i]); 
    NU32DIP_WriteUART1(outstring);
  }
  sprintf(str2,"\n");
  NU32DIP_WriteUART1(str2);
} 
