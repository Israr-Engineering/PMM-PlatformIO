#include "autotune.h"

int ams_init()
{

    return 0;
}


void ams_tuning()
{
    int option=0, exectime;

    /* Generate menu and input options */
    SerialUSB.println("This is the tuning module for the Microsmooth library.");
    
    SerialUSB.println("Please select a tuning function to use:");
    SerialUSB.println("(S)MA (E)MA (C)MA S(G)A (K)ZA (R)DP");
    if (SerialUSB.available() > 0) 
    {
	option = SerialUSB.read();
    }
	
    SerialUSB.println("Enter the time within which the smoothing function may run:");
    if (SerialUSB.available() > 0) 
    {
	exectime = SerialUSB.parseInt();
	/* TBD: Check for lower bound */
    }
    
	
    switch(option)
    {
    case 'S':ams_sma(exectime);
	break;

    case 'E':ams_ema(exectime);
	break;    
	
    case 'C':;
	break;
	
    case 'G':;
	break;
    
    case 'K':;
	break;    

    case 'R':;
	break;

    default:
	    SerialUSB.println("Invalid option.");
    }
    

}


int ams_sma(int maxtime)
{
    return 0;
}

int ams_ema(int maxtime)
{
    return 0;
}
