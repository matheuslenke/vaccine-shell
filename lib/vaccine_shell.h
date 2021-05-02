#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "reader.h"
#include "list.h"

//tipo opaco 
typedef struct shell vsh_t;

/* Roda um unico comando (Foreground)
 * inputs: vetor de string comandos
 * output: nenhum
 * pre-condicao: comandos preenchidos
 * pos-condicao: execução do comando
 */
void roda_comando_unico(char** comandos);

/* Chama as funções que tratam os sinais e chama pra rodar o comando
 * inputs: vetor de string comandos
 * output: nenhum
 * pre-condicao: comandos preenchidos
 * pos-condicao: execução do comando
 */
void roda_comando(char** comandos);

/* Comandos que sao passados com pipe (Background)
 * inputs: string com a linha de comando, inteiro quantidade de pipes e lista com o grupo de processos
 * output: nenhum
 * pre-condicao: lista devidamente inicializada
 * pos-condicao: comando executado e lista inserida
 */
void roda_comando_especial(char* linhaDeComando, int QtdPipes, TLista* lista_de_pgids);

/* Mata todos os processos que estejam rodando, incluindo a shell
 * inputs: lista com o grupo de processos
 * output: nenhum
 * pre-condicao: lista inicializada
 * pos-condicao: terminar a execução de todos os processos
 */
void roda_armageddon(vsh_t* vsh);

/* Mata todos os processos zombies
 * inputs: struct vsh
 * output: nenhum
 * pre-condicao: struct inicializada
 * pos-condicao: todos os processos zombies terminados
 */
void roda_liberamoita(vsh_t* vsh);

/* Inicializa a struct vsh
 * inputs: nenhum
 * output: struct inicializada
 * pre-condicao: nenhuma
 * pos-condicao: struct inicializada
 */
vsh_t* inicializa_shell();

/* Libera a memoria alocada pra strct vsh
 * inputs: struct vsh
 * output: nenhum
 * pre-condicao: struct inicializada
 * pos-condicao: struct desalocada
 */
void libera_shell(vsh_t* shell);

/* Analisa a linha de comando inserida e chama a função especifica pra ele
 * inputs: struct vsh
 * output: nenhum
 * pre-condicao: struct inicializada
 * pos-condicao: comando executado
 */
void le_comando(vsh_t* vsh);

/* Imprime o prompt da shell
 * inputs: nenhum
 * output: String do prompt printada
 * pre-condicao: nenhuma
 * pos-condicao: comando executado
 */
void imprime_prompt();

#endif /* OPERATIONS_H */