#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

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
    TCelula* p = lista->inicio;
    TCelula* ant = NULL;

    //Faz busca
    while (p!=NULL && p->pgid != pgid)
    {
        ant = p;
        p = p->prox;
    }
    
    // lista vazia ou não encontrou o pgid
    if (p==NULL)
        return NULL;
    
    //se for único
    if (p == lista->inicio && p == lista->fim)
        lista->inicio = lista->fim = NULL;
    else if(p == lista->inicio)
        lista->inicio = p->prox;
    else if (p == lista->fim) {
        lista->fim = ant;
        lista->fim->prox = NULL;
    } else
        ant->prox = p->prox;
    
    free(p);
}

void lista_Imprime (TLista* lista) {
    for (TCelula *p = lista->inicio; p != NULL; p = p->prox) {
        printf("PGID: %d \n", p->pgid);
    }
}

void lista_mata_processos_pgid(TLista* lista) {
    for (TCelula *p = lista->inicio; p != NULL; p = p->prox) {
        killpg(p->pgid, SIGTERM);
    }
    lista_Libera(lista);
    raise(SIGTERM);
}

void lista_filtra_pgids_ativos(TLista* lista) {
    printf("\nEntra na função\n");
    TCelula *ant = NULL;
    for (TCelula *p = lista->inicio; p != NULL; p = p->prox) {
        printf("pid: %d, pgid: %d, pai: %d", getpid(), getpgid(getpid()), getppid());
        int result = kill(p->pgid, 0);
        printf("Resutlado do kill: %d, %d\n", result, errno);
        char* verificaComando = (char*)malloc(100 * sizeof(char));
        strcat(verificaComando, "kill -0 ");
        strcat(verificaComando, p->pgid);
        if (system(verificaComando)) {
            // Verifica se o comando é válido
            printf("Este processo não existe mais %d\n", errno);
            // free(verificaComando);
        } else {
            printf("Esse processo existe!\n");
            // free(verificaComando);
        }
        // if( result == 0) {
        //     printf("Erro: %d", errno);
        //     printf("\nretira 1\n");
        //     ant = p;
        //     // lista_Retira(lista, ant->pgid);
        // }
    }    
}

void lista_Libera (TLista* lista) {
    TCelula* p = lista -> inicio;
    TCelula* t;

    while (p != NULL)
    {
        t = p->prox;
        free(p);
        p = t;
    }

    free(lista);
}