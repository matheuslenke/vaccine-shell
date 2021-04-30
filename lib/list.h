#ifndef LISTA_H_
#define LISTA_H_

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

#endif /* LISTA_H_ */