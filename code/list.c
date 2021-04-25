#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "../lib/list.h"

typedef struct celula TCelula;

struct lista {
    TCelula *inicio;
    TCelula *fim;
};

struct celula
{
    int pgid;
    TCelula *prox;
};


TLista* lista_InicializaLista() {
    TLista *lista;
    lista = (TLista *)malloc(sizeof(TLista));
    lista->inicio = NULL;
    lista->fim = NULL;

    return lista;
}

TLista* lista_InserePGID (TLista* lista , int pgid) {
    TCelula* nova = (TCelula*) malloc (sizeof(TCelula));
    nova->pgid = pgid;
    nova->prox = lista->inicio;
    lista->inicio = nova;
    if (lista->fim == NULL)
        lista->fim = nova;

    return lista;
}

void lista_Retira (TLista* lista, int pgid) {
   TCelula *anterior;
    int auxiliar;
    anterior = NULL;

    for (TCelula *p = lista->inicio; p != NULL; p = p->prox)
    {
        // printf("Mat: %d\n", mat);
        // printf("Matricula aluno: %d", p->aluno->matricula);
        if (p->pgid == pgid && p != NULL)
        {
            if(anterior != NULL) {
                anterior->prox = p->prox;
            }
            if (p == lista->fim && anterior != NULL)
            {
                lista->fim = anterior;
                auxiliar = p->pgid;
            }
            if (p == lista->fim && anterior == NULL)
            {   
                lista->inicio = NULL;
                lista->fim = NULL;
                auxiliar = p->pgid;
            }
            if (anterior = NULL)
            {
                lista->inicio = p->prox;
            }
            auxiliar = p->pgid;
            free(p);

            return auxiliar;
        }
        else if (p == lista->fim) {
            auxiliar = NULL;
            return auxiliar;
        }
        anterior = p;
    }
}

void lista_Imprime (TLista* lista) {
  printf("Lista foi impressa\n");
    for (TCelula *p = lista->inicio; p != NULL; p = p->prox) {
        printf("PGID: %d \n", p->pgid);
    }
}

void lista_mata_processos_pgid(TLista* lista) {
    for (TCelula *p = lista->inicio; p != NULL; p = p->prox) {
        kill(-p->pgid, SIGTERM);
        lista_Retira(lista, p->pgid);
    }
}

void lista_Libera (TLista* lista) {
   for (TCelula *p = lista->inicio; p != NULL; p = p->prox) {
        free(p->prox);
        free(p);
    }
}