#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

char* read_line(FILE *arq) {
    size_t n = 0, r;
    char *buf = NULL;

    r = getline(&buf, &n, arq);
    if(r < 0)
        return(NULL);

    if(buf && r >= UINT_MAX) {
        free(buf);
        return(NULL);
    }
    
    return(buf);
}