#ifndef MASKS_H
#define MASKS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void restaura_mascara_padrao(void);

void cria_mascara_ignora_sinais_teclado(void);

void cria_mascara_processo_background(void);

void trata_sigurs_vsh(void);

void seta_mascara_vsh(void);

void cria_mascara_trata_sitstp(void);

#endif /* MASKS_H */