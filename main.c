#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define READ 0
#define WRITE 1

int strcount(char *str, char c) {
    int r = 0;
    for(int i = 0; str[i]; i++){
        r += str[i] == c; 
    }
    return r;
}

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}


void run_single_command(char** commands, int QtdComandos){
     switch (QtdComandos) {
        case 1:
            execlp(commands[0], commands[0], (char*) NULL);
            break;
        case 2:
             execlp(commands[0], commands[0], commands[1], (char*) NULL);
            break;
        case 3:
            execlp(commands[0], commands[0], commands[1], commands[2], (char*) NULL);
            break;
        case 4:
            execlp(commands[0], commands[0], commands[1], commands[2],  commands[3], (char*) NULL);
            break;
        default:
            printf("Erro ao executar comando\n");
            break;
    }
}

void run_command(char** commands, int QtdComandos) {
 pid_t p1;

  p1 = fork();
  if (p1 == -1) {
    printf("Erro ao realizar fork\n");
  } else if(p1 == 0) { // Child process
    run_single_command(commands, QtdComandos);
  } else if (p1 > 0) { // Parent process
    wait(p1);
    // printf("Acabou o processo filho\n");
    for(int i = 0; i < QtdComandos; i++){
        free(commands[i]);
    }
  }
}

void run_command_special(char* commandLine, int QtdPipes) {
    char* commandLines[QtdPipes+1];
    char *tok = strtok(commandLine, "|");
    for(int i = 0; i < QtdPipes +1; i++, tok = strtok(NULL, "|")){
        //O trim retira os espaços da string no começo e fim
        commandLines[i] = trim(strdup(tok));
    }
    // Cria todos os pids dos filhos e para o pai aguardar
    pid_t pids[QtdPipes + 1], wpid;
    int status = 0;
    int pipes[QtdPipes][2];

    // Criando id do grupo
    int groupId = 1234;

    // Inicializa todos os pipes
    for(int i = 0; i < QtdPipes; i++) {
        if (pipe(pipes[i])<0) { fprintf(stderr,"Erro no tubo\n");_exit(1); }
    }

    for(int i = 0; i< QtdPipes + 1; i++) {
        // printf("%s\n", commandLines[i]);
        int QtdComandos = strcount(commandLines[i], ' ') + 1;
        char* commands[QtdComandos+1];
        // printf("quantidade comandos: %d\n", QtdComandos);
        char *tok = strtok(commandLines[i], " ");
        for(int j = 0; j < QtdComandos; j++, tok = strtok(NULL, " ")){
            commands[j] = strdup(tok);
        }
        commands[QtdComandos] = NULL;

        // Criando pipes e processos e rodando
        pids[i] = fork();
        if(pids[i] == -1) {
            printf("Erro ao criar filho");
        } else if (pids[i] == 0) { // Child
            if(i == 0) {
                close(pipes[i][READ]);
                // Primeiro processo
                dup2(pipes[i][WRITE], WRITE);
                close(pipes[i][WRITE]);
                for(int z = 1; z< QtdPipes; z++) {
                    close(pipes[z][READ]);
                    close(pipes[z][WRITE]);
                }
            } else if(i == QtdPipes+1) {
                // Ultimo processo
                close(pipes[i-1][WRITE]);
                dup2(pipes[i-1][READ], READ);
                close(pipes[i-1][READ]);
                for(int z = 0; z< QtdPipes; z++) {
                    if(z!= i - 1) {
                        close(pipes[z][READ]);
                        close(pipes[z][WRITE]);
                    }
                }
            } else {
                // Processos do meio
                close(pipes[i-1][WRITE]);
                close(pipes[i][READ]);
                dup2(pipes[i-1][READ], READ);
                close(pipes[i-1][READ]);
                dup2(pipes[i][WRITE], WRITE);
                close(pipes[i][WRITE]);
                for(int z = 0; z< QtdPipes; z++) {
                    if(z != i-1 && z != i) {
                        close(pipes[z][READ]);
                        close(pipes[z][WRITE]);
                    }
                }
            }
            run_single_command(commands, QtdComandos);

        }
    }
    for(int i = 0; i < QtdPipes; i++) {
        close(pipes[i][READ]);
        close(pipes[i][WRITE]);
    }
    // while ((wpid = wait(&status)) > 0);
    // printf("Terminou de esperar\n");
    
    // printf("Depois do wait\n");
}

void read_command() {
    char* commandLine = (char*) malloc(100 * sizeof(char));
    printf("vsh > ");
    scanf("%[^\n]", commandLine);
    getchar();
    printf("%s\n", commandLine);

    int QtdPipes = strcount(commandLine, '|');
    if (QtdPipes > 0) {
        run_command_special(commandLine, QtdPipes);
    } else {
        int QtdComandos = strcount(commandLine, ' ') + 1;
        char* commands[QtdComandos+1];
        printf("quantidade comandos: %d\n", QtdComandos);

        char *tok = strtok(commandLine, " ");
        for(int i = 0; i < QtdComandos; i++, tok = strtok(NULL, " ")){
            commands[i] = strdup(tok);
        }
        commands[QtdComandos] = NULL;
        for(int i = 0; i < QtdComandos; i++){
            printf("%s\n", commands[i]);
        }
        free(commandLine);
        run_command(commands, QtdComandos);
    }

}


int main(int argc, char const *argv[]) {
  while(1) {
    read_command();
  }
  return 0;
}
