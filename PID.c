#include"PID.h"
#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"
#include "habilita_DAC.h"



int erro_atual;
int  erro_anterior;
int  u;
int P;
int integrador;
int derivador;

int C_PID (int Set_Point0, unsigned int valor0,float KP0,float KI0,float  KD0, float ts0)
{



    erro_atual = Set_Point0 - valor0;


    integrador = integrador +((erro_atual + erro_anterior)/2)*ts0;
  
    derivador = (erro_atual - erro_anterior)/ts0;

    u = KP0*erro_atual+KD0*derivador+KI0*integrador;

    if (u> 4095)
    {
        u = 4095;
    }
    if (u<0)
    {
        u = 0;
    }


    erro_anterior = erro_atual;
    return u;
}



