#ifndef TECLADO_H
        #define TECLADO_H

#define INICIO   0

#define BOTAO_PORT    LPC_GPIO2
#define BOTAO_1  6
#define BOTAO_2  7
#define BOTAO_3  8
#define BOTAO_4  9
#define BOTAO_5  10
void InicializaBotoes ( void);
unsigned int checaBotao(void);


#endif //TECLADO_H
