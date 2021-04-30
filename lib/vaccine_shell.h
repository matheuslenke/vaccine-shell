#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "reader.h"
#include "list.h"


typedef struct shell vsh_t;
/*
* Roda comandos com até 3 argumentos
*/
void roda_comando_unico(char** comandos);

/*
* Função que realiza um fork e roda um comando simples sem pipes
*/
void roda_comando(char** comandos);

/*
* Função que realiza um fork para cada comando e roda o mesmo
* com pipes
*/
void roda_comando_especial(char* commandLine, int QtdPipes, TLista* lista_de_pgids);

/*
* Função interna armageddon
*/
void roda_armageddon(TLista* lista_de_pgids);

/*
* Função interna liberamoita
*/
void roda_liberamoita(vsh_t* vsh);

/*
* Função para inicializar shell
*/
vsh_t* inicializa_shell();

/*
* Função para ler comandos
*/
void le_comando(vsh_t* vsh);

#endif /* OPERATIONS_H */