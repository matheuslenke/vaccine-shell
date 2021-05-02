#ifndef LISTA_H_
#define LISTA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

//tipo opaco lista
typedef struct lista TLista;

/*Inicializa uma lista
 * inputs: nenhum
 * output: lista inicializada
 * pre-condicao: nenhum
 * pos-condicao: lista inicializada pronta para a inserção de pgids
 */
TLista* lista_InicializaLista(void);

/*Insere pgid na lista
 * inputs: lista e pgid
 * output: lista com pgid inserido
 * pre-condicao: lista inicializada
 * pos-condicao: lista com pgid inserido
 */
TLista* lista_InserePGID (TLista* lista , int pgid);

/*Retira pgid da lista
 * inputs: lista e pgid 
 * output: lista com elemento retirado
 * pre-condicao: lista inicializada
 * pos-condicao: lista sem pgid especificado
 */
void lista_Retira (TLista* lista, int pgid);

/*Imprime a lista completa
 * inputs: lista
 * output: nenhum
 * pre-condicao: lista inicializada
 * pos-condicao: nenhuma
 */
void lista_Imprime (TLista* lista);

/*Libera memória alocada para a lista
 * inputs: lista
 * output: nenhum
 * pre-condicao: lista inicializada
 * pos-condicao: toda a memória alocada para a lista liberada
 */
void lista_Libera (TLista* lista);

/*Termina todos os processos rodando na shell (Armageddon)
 * inputs: lista
 * output: nenhum
 * pre-condicao: lista inicializada
 * pos-condicao: todos os processos finalizados, inclusive a shell
 */
void lista_mata_processos_pgid(TLista* lista);

/* Percorre a lista de pgids e remove todos os pgids referente aos processos que não existem mais
 * inputs: lista 
 * output: nenhum
 * pre-condicao: lista inicializada
 * pos-condicao: lista atualizada
 */
void lista_filtra_pgids_ativos(TLista* lista);

#endif /* LISTA_H_ */