#include "habilita_DAC.h"


void configura_DAC(void) {

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);	// habilitar o clock para o bloco IOCON (UM10398, 3.5.14)

	LPC_IOCON->PIO0_4         = 0x01;		// configurar o pino como SCL em modo I2C padrão (I2CMODE=00) (UM10398, 7.4.11)
	LPC_IOCON->PIO0_5         = 0x01;		// configurar o pino como SDA em modo I2C padrão (I2CMODE=00) (UM10398, 7.4.12)



	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<5);	// habilitar o clock para o bloco I2C (UM10398, 3.5.14)
	LPC_SYSCON->PRESETCTRL    |= (1<<1);	// garantir que o periférico I2C não esteja em estado de reset (UM10398, 3.5.2)


	LPC_I2C->SCLH 			   = 240;		// Estipular o clock I2C para 100kHz (UM10398, 15.7.5.1)
	LPC_I2C->SCLL 			   = 240;

	LPC_I2C->CONSET			   |= (1<<6);	// habilitar o periférico I2C como mestre (UM10398, 15.7.1 / 15.8.1)


}

//
// Transmitir os valores para o DAC
//
// Utilizar o modo de atualização especificado em (MCP4725, 6.1.1)
//

void DAC_Transmitir ( int v_uAmostra )
{

	char vcDado;

	LPC_I2C->CONSET = (1<<5);					// requisitar o evento de START (UM10398, 15.7.1)

	while ( LPC_I2C->STAT != 0x08 );			// esperar pelo pelo fim do evento de START (bit SI, UM10398, 15.7.1)

	LPC_I2C->DAT	= K_ENDERECO_MCP4725 << 1;	// enviar o endereço do dispositivo escravo e espeficicação de escrita (R/W=0) (UM10398, 15.7.3)
	LPC_I2C->CONCLR = (5<<3);					// limpar a sinalizações SI e START (UM10398, 15.7.6)
	while( LPC_I2C->STAT != 0x18 );				// esperar por um estado 0x18 (ACK recebido) (UM10398, 15.10.1, tabela 236)

	//
	// Enviar os dados requisitados
	//

	// Transmitir byte com os valores  <C2, C1, PD1, PD0> = <0,0,0,0>
	// O formato é apresentado em  (MCP4725, figura 6-1)

	vcDado =  ( v_uAmostra >> 8 );
	LPC_I2C->DAT	= vcDado;				// transmitir o dado (UM10398, 15.7.3)
	LPC_I2C->CONCLR = (1<<3);				// limpar o bit SI para transmitir um byte e esperar por um ACK (UM10398, 15.7.6 e tabela 236)

	while( LPC_I2C->STAT != 0x28 );			// esperar por um estado 0x28 (ACK recebido) (UM10398, 15.10.1, tabela 236)

	//
	// Transmitir o segundo byte com a parte menos significativa da amostra
	// Vide (MCP4725, figura 6-1)
	//
	vcDado =  ( v_uAmostra & 0xFF);
	LPC_I2C->DAT	= vcDado;				// transmitir o dado (UM10398, 15.7.3)
	LPC_I2C->CONCLR = (1<<3);				// limpar o bit SI para transmitir um byte e esperar por um ACK (UM10398, 15.7.6 e tabela 236)

	while( LPC_I2C->STAT != 0x28 );			// esperar por um estado 0x28 (ACK recebido) (UM10398, 15.10.1, tabela 236)


	LPC_I2C->CONSET = (1<<4);				// requisitar o evento de STOP (UM10398, 15.7.1)
	LPC_I2C->CONCLR = (1<<3);				// limpar o bit SI (UM10398, 15.7.6)

}


