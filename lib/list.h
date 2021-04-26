#ifndef LISTA_H_
#define LISTA_H_

typedef struct lista TLista;

TLista* lista_InicializaLista();

TLista* lista_InserePGID (TLista* lista , int pgid);

void lista_Retira (TLista* lista, int pgid);

void lista_Imprime (TLista* lista);

void lista_Libera (TLista* lista);

void lista_mata_processos_pgid(TLista* lista);

void lista_mata_processos_zombies(TLista* lista);

#endif /* LISTA_H_ */