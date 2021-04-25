#ifndef READER_H
#define READER_H
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/*
* Função STRCOUNT: Conta a quantidade de caracteres
*/
int strcount(char *str, char c);

/*
* Função auxiliar TRIM
*/
char *ltrim(char *s);

/*
* Função auxiliar TRIM
*/
char *rtrim(char *s);

/*
* Função TRIM: retira espaços vazios
*/
char *trim(char *s);

#endif /* READER_H */