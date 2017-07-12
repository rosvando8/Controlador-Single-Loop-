

#include <LPC11xx.h>			/* LPC11xx Peripheral Registers */
#include "system_LPC11xx.h"
#include "teclado.h"
#include "lcd.h"
#include "adc.h"
#include "interrupt.h"
#include "PID.h"
#include "habilita_DAC.h"


#define PORTA_LED   LPC_GPIO2
#define PINO_LED    5
// Variáveis Globais

float ts=0.1;
float tw=0;
float KD1=0 ;
int Set_Point1=0;
float KI1=6.084;
float  KP1=2.424;
int j;
int flag =0;
unsigned int valor;

void SysTick_Handler(void) 		 		//SysTick handler
{
    //valor = LeAD();
    j = C_PID(Set_Point1,valor,KP1,KI1,KD1,ts);

    if ( flag == 1)
    {
        DAC_Transmitir(j);
    }
}




// Protótipos locais
void bit_set( volatile uint32_t *p_iValor, unsigned char v_ucBit );
void bit_clr( volatile uint32_t *p_iValor, unsigned char v_ucBit );
void delay_ms( unsigned int v_uiTempo );
void LCD_iniciar();
void LCD_comando( char v_cComando );
void LCD_caractere( char v_cCaractere );
void InicializaBotoes ( void);
unsigned int checaBotao(void);
void InicializaAD();





int main(void)
{

    unsigned  char T1[2];
    unsigned char  V [4];
    unsigned char  KP[3];
    unsigned char  KI[3];
    unsigned char KD[3];
    unsigned char  Set_Point[4];
    unsigned int aux;   //auxiliar do menu
    unsigned int botao;
    unsigned int bt_ant;


    aux = 0 ;



    // Inicializações
    LCD_iniciar();
    InicializaBotoes();
    InicializaAD();
    configura_DAC();
    SysTick_Config(SystemCoreClock*ts);





    // Laço de repetição da aplicação
    //

    while(1)
    {
        //debounceTeclas();
        valor= LeAD();
        flag=0;
        int T2 = ts*10;
        int KP2=KP1*10;
        int KI2=KI1*10;
        int KD2=KD1*10;

        KP[0] = (KP2/100)%10;
        KP[1] = (KP2/10)%10;
        KP[2] = KP2% 10;
        KP[0] += '0';
        KP[1] += '0';
        KP[2] += '0';

        KI[0] = (KI2/100)%10;
        KI[1] = (KI2/10)%10;
        KI[2] = KI2 % 10;
        KI[0] += '0';
        KI[1] += '0';
        KI[2] += '0';


        KD[0] = (KD2/100)%10;
        KD[1] = (KD2/10)%10;
        KD[2] = KD2 % 10;
        KD[0] += '0';
        KD[1] += '0';
        KD[2] += '0';

        Set_Point[0] = (Set_Point1/1000)%10;
        Set_Point[1] = (Set_Point1/100)%10;
        Set_Point[2] = (Set_Point1/10)%10;
        Set_Point[3] = Set_Point1 % 10;
        Set_Point[0] += '0';
        Set_Point[1] += '0';
        Set_Point[2] += '0';
        Set_Point[3] += '0';


        V[0] = (valor/1000)%10;
        V[1] = (valor/100)%10;
        V[2] = (valor/10)%10;
        V[3] =  valor% 10;
        V[0] += '0';
        V[1] += '0';
        V[2] += '0';
        V[3] += '0';


        T1[0] = (T2/10)%10;
        T1[1] = T2 % 10;
        T1[0] += '0';
        T1[1] += '0';



        botao = checaBotao();

        switch (aux)
        {
            //maquina de estados do menu
        case 0:                     //tela de inicio

            //LCD_comando(0x01);
            lcdString("  SINGLE LOOP");
            LCD_comando(0xC0);
            lcdString("  PRESS  [>>] ");
            LCD_comando(0x0C);

            if ((botao == BOTAO_4)&&(bt_ant != botao))
            {
                aux=1;
            }



            break;

        case 1:
            //tela seta setpoint

            LCD_comando(0x80);
            lcdString("  SET POINT:");
            LCD_caractere(Set_Point[0]);
            LCD_caractere(Set_Point[1]);
            LCD_caractere(Set_Point[2]);
            LCD_caractere(Set_Point[3]);
            LCD_comando(0xC0);
            lcdString("Config SP   [>>]");
            LCD_comando(0x0C);


            if (botao!= bt_ant)
            {
                if(botao == BOTAO_4)
                {
                    aux = 2;
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9; // tela que confirma e roda o sistema
                }
            }

            break;

        case 2:                         //tela configura set point

            LCD_comando(0x80);
            lcdString("    CONFIG SP      ");
            LCD_comando(0xC0);
            lcdString("Set Point:  ");
            LCD_comando(0x0C);

            if(botao != bt_ant)
            {
                if(botao == BOTAO_1)
                {
                    Set_Point1= Set_Point1+ 100 ;
                    if(Set_Point1 >= 2000)
                    {
                        Set_Point1 = 2000;
                    }
                }
                else if(botao == BOTAO_3)
                {
                    Set_Point1= Set_Point1 - 100;   //diminui set point
                    if(Set_Point1 < 0 )
                    {
                        Set_Point1 = 0;
                    }

                }
                else if(botao == BOTAO_4)
                {
                    aux = 3;

                }
                else if(botao == BOTAO_2)   //tempo
                {
                    aux = 1;

                }
                else if (botao == BOTAO_5)
                {
                    aux =9;
                }
            }

            LCD_caractere(Set_Point[0]);
            LCD_caractere(Set_Point[1]);
            LCD_caractere(Set_Point[2]);
            LCD_caractere(Set_Point[3]);

            break;

        case 3:
            //tela seta setpoint

            LCD_comando(0x80);
            lcdString("Cte KP:");
            LCD_caractere(KP[0]);
            LCD_caractere(KP[1]);
            lcdString(",");
            LCD_caractere(KP[2]);
            lcdString("       ");
            LCD_comando(0xC0);
            lcdString("Config KP   [>>]");
            LCD_comando(0x0C);

            if (botao != bt_ant)
            {
                if(botao == BOTAO_4)
                {
                    aux = 4;
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9; // tela que confirma e roda o sistema
                }
                else if(botao == BOTAO_2)
                {
                    aux = 2;
                }

            }

            break;

        case 4:

            LCD_comando(0x80);
            lcdString("   CONFIG KP    ");
            LCD_comando(0xC0);
            lcdString("PROPOR:     ");
            LCD_comando(0x0C);

            if(botao != bt_ant)
            {
                if(botao == BOTAO_1)
                {
                    KP1 = KP1 +0.1;            //aumenta KP
                }
                else if(botao == BOTAO_3)
                {
                    KP1= KP1 - 0.1;            //diminui KP

                }
                else if(botao == BOTAO_4)
                {
                    aux = 5;

                }
                else if(botao == BOTAO_2)   //tempo
                {
                    aux = 3;

                }
                else if (botao == BOTAO_5)
                {
                    aux =9;
                }
            }

            LCD_caractere(KP[0]);  //printa valor de KP
            LCD_caractere(KP[1]);  //printa valor de KP
            lcdString(",");
            LCD_caractere(KP[2]);
            break;

        case 5:                         //MOSTRA KI
            LCD_comando(0x80);
            lcdString("Cte KI:");
            LCD_caractere(KI[0]);
            LCD_caractere(KI[1]);
            lcdString(",");
            LCD_caractere(KI[2]);
            lcdString("          ");
            LCD_comando(0xC0);  //PULA LINHA
            lcdString("Config KI   [>>]");
            LCD_comando(0x0C);
            checaBotao();
            if (botao != bt_ant)
            {
                if(botao == BOTAO_4)
                {
                    aux = 6;            //configurar ldr
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9; // tela que confirma e roda o sistema
                }
                else if(botao == BOTAO_2)
                {
                    aux = 4;
                }

            }


            break;

        case 6:
            //config KI
            LCD_comando(0x80);
            lcdString("   CONFIG KI    ");
            LCD_comando(0xC0);
            lcdString("INTEGRATIVO:");
            LCD_comando(0x0C);
            checaBotao();
            if ( botao != bt_ant)
            {
                if(botao == BOTAO_1)
                {
                    KI1= KI1 + 0.1;            //aumenta KI
                }
                else if(botao == BOTAO_3)
                {
                    KI1= KI1 - 0.1;            //diminui KI

                }
                else if(botao == BOTAO_4)
                {
                    aux = 7;

                }
                else if(botao == BOTAO_2)
                {
                    aux = 5;

                }
                else if (botao == BOTAO_5)
                {
                    aux =9;
                }

            }

            LCD_caractere(KI[0]);
            LCD_caractere(KI[1]);
            lcdString(",");
            LCD_caractere(KI[2]);

            break;

        case 7:
            //Mostra KD
            LCD_comando(0x80);
            lcdString("Cte KD:");
            LCD_caractere(KD[0]);
            LCD_caractere(KD[1]);
            lcdString(",");
            LCD_caractere(KD[2]);
            lcdString("         ");
            LCD_comando(0xC0);  //PULA LINHA
            lcdString("Config KD   [>>]");
            LCD_comando(0x0C);

            if (botao != bt_ant)
            {
                if(botao == BOTAO_4)
                {
                    aux = 8;
                }
                else if(botao == BOTAO_5)
                {
                    aux = 9; // tela que confirma e roda o sistema
                }
                else if(botao == BOTAO_2)
                {
                    aux = 6;
                }

            }


            break;

        case 8:
            //config KD
            LCD_comando(0x80);
            lcdString("   CONFIG KD      ");
            LCD_comando(0xC0);
            lcdString("DERIVATIVO: ");
            LCD_comando(0x0C); // lembrar o que faz
            checaBotao();
            if( botao != bt_ant)
            {
                if(botao == BOTAO_1)
                {
                    KD1= KD1 + 0.1;            //aumenta KI
                }
                else if(botao == BOTAO_3)
                {
                    KD1= KD1- 0.1;            //diminui KI

                }
                else if(botao == BOTAO_2)
                {
                    aux = 7;

                }
                else if (botao == BOTAO_5)
                {
                    aux =9;
                }
                else if (botao == BOTAO_4)
                {
                    aux =10;
                }

            }

            LCD_caractere(KD[0]);
            LCD_caractere(KD[1]);
            lcdString(",");
            LCD_caractere(KD[2]);



            break;

        case 9:

            flag =1;

            LCD_comando(0x80);
            lcdString("SET POINT:");
            LCD_caractere(Set_Point[0]);
            LCD_caractere(Set_Point[1]);
            LCD_caractere(Set_Point[2]);
            LCD_caractere(Set_Point[3]);
            lcdString("            ");
            LCD_comando(0xC0);
            lcdString( "ADC: ");
            LCD_caractere(V[0]);
            LCD_caractere(V[1]);
            LCD_caractere(V[2]);
            LCD_caractere(V[3]);
            lcdString("            ");

            if (botao != bt_ant)
            {

                if(botao == BOTAO_2)
                {
                    aux = 8;
                }

            }


            break;

        case 10:

            LCD_comando(0x80);
            lcdString("VALOR DO ADC             ");
            LCD_comando(0xC0);
            LCD_caractere(V[0]);
            LCD_caractere(V[1]);
            LCD_caractere(V[2]);
            LCD_caractere(V[3]);
            lcdString("             ");



            if( botao != bt_ant)
            {

                if(botao == BOTAO_2)
                {
                    aux = 8;

                }
                else if (botao == BOTAO_5)
                {
                    aux =9;
                }
                else if (botao == BOTAO_4)
                {
                    aux =11;
                }
            }
            break;

        case 11:
            //config KD
            LCD_comando(0x80);
            lcdString("TAXA DE AMOSTRAGEM");
            LCD_comando(0xC0);
            lcdString("TS: ");
            LCD_comando(0x0C);
            checaBotao();
            if( botao != bt_ant)
            {
                if(botao == BOTAO_1)
                {
                    ts= ts + 0.1;            //aumenta ts
                }
                else if(botao == BOTAO_3)
                {
                    ts= ts- 0.1;            //diminui ts
                    if ( ts <0)
                    {

                        ts=0;
                    }

                }
                else if(botao == BOTAO_2)
                {
                    aux = 10;

                }
                else if (botao == BOTAO_5)
                {
                    aux =9;
                }
            }

            LCD_caractere(T1[0]);
            lcdString(",");
            LCD_caractere(T1[1]);

            break;
        }
        bt_ant = botao;
    }

    return 0;

}





























