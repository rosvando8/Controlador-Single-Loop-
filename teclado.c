#include <teclado.h>
#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"





void InicializaBotoes ( void)
{

     BOTAO_PORT ->DIR &= ~(1<<BOTAO_1); // entrada digital
     BOTAO_PORT ->DIR &= ~(1<<BOTAO_2);
     BOTAO_PORT ->DIR &= ~(1<<BOTAO_3);
     BOTAO_PORT ->DIR &= ~(1<<BOTAO_4);
     BOTAO_PORT ->DIR &= ~(1<<BOTAO_5);
}

unsigned int checaBotao(void)
{

    if ( (BOTAO_PORT->DATA &(1<<6 )) ==0 ) // testar se esta em 0
    {
         return BOTAO_1;

      }
      else if  ( ( BOTAO_PORT->DATA  &(1<<7) ) == 0 )
      {
         return BOTAO_2;

      }
      else if  ( ( BOTAO_PORT->DATA & (1<<8)) == 0  )
      {
        return BOTAO_3;

      }
      else if  ( ( BOTAO_PORT->DATA  &(1<<9) )== 0  )
      {
          return BOTAO_4;

      }
      else if  ( ( BOTAO_PORT->DATA   &(1<<10) ) ==0 )
      {

        return BOTAO_5;

      }
}
