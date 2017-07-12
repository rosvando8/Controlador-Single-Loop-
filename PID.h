#ifndef PID_H
#define PID_H
#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"
#include "adc.h"

int C_PID (int Set_Point0,unsigned int valor0,float KP0,float KI0,float KD0,float ts0);


#endif
