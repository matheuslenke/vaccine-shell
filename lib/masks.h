#ifndef MASKS_H
#define MASKS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/* Restaura a mascara padrão
 * inputs: nenhum
 * output: nenhum
 * pre-condicao: nenhum
 * pos-condicao: restaurado tratadores para o padrão
 */
void restaura_mascara_padrao(void);

/* Cria mascara que ignora os sinais SIGINT, SIGQUIT, SIGTSTP, SIGTERM
 * inputs: nenhum
 * output: nenhum
 * pre-condicao: nenhum
 * pos-condicao: mascara setada para ignorar sinais
 */
void cria_mascara_ignora_sinais_teclado(void);

/* Tratamento sinal SIGTSTP
 * inputs: nenhum
 * output: nenhum
 * pre-condicao: nenhum
 * pos-condicao: nenhum
 */
void trata_sigtstp();

/* Cria mascara com tratamento para sinal SIGSTP
 * inputs: nenhum
 * output: nenhum
 * pre-condicao: nenhum
 * pos-condicao: mascara setada para tratar sinal
 */
void cria_mascara_trata_sigtstp(void);

/* Cria mascara que bloquea os sinais SIGINT, SIGQUIT, SIGTSTP, SIGTERM
 * inputs: nenhum
 * output: nenhum
 * pre-condicao: nenhum
 * pos-condicao: mascara setada para bloquear sinais
 */
void cria_mascara_processo_background(void);

/* Cria mascara que ignora os sinais SIGUSR1 e SIGUSR2
 * inputs: nenhum
 * output: nenhum
 * pre-condicao: nenhum
 * pos-condicao: mascara setada para ignora sinais
 */
void cria_mascara_processo_foreground(void);

/* Tratamento sinais SIGUSR1 e SIGUSR2 para shell
 * inputs: nenhum
 * output: nenhum
 * pre-condicao: nenhum
 * pos-condicao: nenhum
 */
void trata_sigurs_vsh(void);

/* Bloqueia sinais e seta tratamento sinais SIGUSR1 e SIGUSR2
 * inputs: nenhum
 * output: nenhum
 * pre-condicao: nenhum
 * pos-condicao: mascara modificada e tratamento setado 
 */
void seta_mascara_vsh(void);


#endif /* MASKS_H */