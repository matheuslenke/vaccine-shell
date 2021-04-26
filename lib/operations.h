#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#include "reader.h"
#include "list.h"

/*
* Roda comandos com até 3 argumentos
*/
void run_single_command(char** commands);

/*
* Função que realiza um fork e roda um comando simples sem pipes
*/
void run_command(char** commands);

/*
* Função que realiza um fork para cada comando e roda o mesmo
* com pipes
*/
void run_command_special(char* commandLine, int QtdPipes, TLista* lista_de_pgids);

/*
* Função interna armageddon
*/
void run_armageddon(TLista *lista_de_pgids);

/*
* Função para ler comandos
*/
void read_command(TLista* lista_de_pgids);

#endif /* OPERATIONS_H */