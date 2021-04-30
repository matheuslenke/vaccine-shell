#ifndef READER_H
#define READER_H
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/* Conta a quantidade de caracteres em uma string
 * inputs: ponteiro para string e caracter
 * output: quantidade repetida
 * pre-condicao: ponteiro para string e caracter criados
 * pos-condicao: nenhum
 */
int strcount(char *str, char c);

/* Retira espaços do início da string
 * inputs: ponteiro para string
 * output: string sem espaços no início
 * pre-condicao: ponteiro para string criado
 * pos-condicao: nenhum
 */
char *ltrim(char *s);

/* Retira espaços do fim da string
 * inputs: ponteiro para string
 * output: string sem espaços no fim
 * pre-condicao: ponteiro para string criado
 * pos-condicao: nenhum
 */
char *rtrim(char *s);

/* Retira espaços vazios
 * inputs: ponteiro para string
 * output: string sem espaços
 * pre-condicao: ponteiro para string criado
 * pos-condicao: nenhum
 */
char *trim(char *s);

#endif /* READER_H */