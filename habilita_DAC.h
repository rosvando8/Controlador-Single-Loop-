#ifndef HABILITA_DAC_H
#define HABILITA_DAC_H

#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"


#define K_ENDERECO_MCP4725		0x62	// Endereço em 7 bits, podendo ser 0x60, 0x62, 0x64 ou 0x66
                                        // Vide partnumber completo do DAC para identificar o endereço (MCP4725, 10.0)

void DAC_Transmitir ( int v_uAmostra );
void configura_DAC(void);
#endif
