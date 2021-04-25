#include "../lib/operations.h"

#define READ 0
#define WRITE 1

void run_single_command(char** commands, int QtdComandos){
     switch (QtdComandos) {
        case 1: // Caso o comando nao tenha argumentos
            execlp(commands[0], commands[0], (char*) NULL);
            break;
        case 2: // Caso o comando tenha um argumento
             execlp(commands[0], commands[0], commands[1], (char*) NULL);
            break;
        case 3: // Caso o comando tenha dois argumentos
            execlp(commands[0], commands[0], commands[1], commands[2], (char*) NULL);
            break;
        case 4: // Caso o comando tenha quatro argumentos
            execlp(commands[0], commands[0], commands[1], commands[2],  commands[3], (char*) NULL);
            break;
        default: // Se não seguir nenhum dos padrões anteriores
            printf("Erro ao executar comando\n");
            break;
    }
}


void run_command(char** commands, int QtdComandos) {
  pid_t p1;
  int wstatus;

  p1 = fork();
  if (p1 == -1) {
    printf("Erro ao realizar fork\n");
  } else if(p1 == 0) { // Processo filho
    run_single_command(commands, QtdComandos);
  } else if (p1 > 0) { // Processo pai
    waitpid(p1, &wstatus, 0);
    // Libera toda a memoria alocada pros comandos
    for(int i = 0; i < QtdComandos; i++){
        free(commands[i]);
    }
  }
}


void run_command_special(char* commandLine, int QtdPipes, TLista* lista_de_pgids) {
    char* commandLines[QtdPipes+1];
    char *tok = strtok(commandLine, "|");
    pid_t session_id, pid_parent;
    for(int i = 0; i < QtdPipes +1; i++, tok = strtok(NULL, "|")){
        //O trim retira os espaços da string no começo e fim
        commandLines[i] = trim(strdup(tok));
    }
    // Cria todos os pids dos filhos e para o pai aguardar
    pid_t pids[QtdPipes + 1], wpid;
    int status = 0;
    int pipes[QtdPipes][2];
    
    // Variáveis envolvendo criação da sessão
    pid_t sid; 
    int pipe_pgid[2];
    if (pipe(pipe_pgid)<0) { fprintf(stderr,"Erro no tubo\n");_exit(1); }
    // lista_de_pgids = lista_InserePGID(lista_de_pgids, 5); aqui adiciona

    if((pid_parent = fork()) == -1) {
        printf("Failed to fork");
    } else if (pid_parent == 0) { // Child process
        // Inicializa todos os pipes
        for(int i = 0; i < QtdPipes; i++) {
            if (pipe(pipes[i])<0) { fprintf(stderr,"Erro no tubo\n");_exit(1); }
        }
        sid = setsid();
        close(pipe_pgid[READ]);
        write(pipe_pgid[WRITE], &sid, sizeof(pid_t));
        close(pipe_pgid[WRITE]);
        // lista_de_pgids = lista_InserePGID(lista_de_pgids, 5); AQUI NÃO ADICIONA
        // lista_Imprime(lista_de_pgids);
        // printf("[New Parent] session_id: %d\n", getsid(0));
        // printf("[New parent] groud_id: %d\n", getpgid(0));
        for(int i = 0; i< QtdPipes + 1; i++) {
            // printf("%s\n", commandLines[i]);
            int QtdComandos = strcount(commandLines[i], ' ') + 1;
            char* commands[QtdComandos+1];
            // printf("quantidade comandos: %d\n", QtdComandos);
            // Divide a string em cada espaço
            char *tok = strtok(commandLines[i], " ");
            for(int j = 0; j < QtdComandos; j++, tok = strtok(NULL, " ")){
                commands[j] = strdup(tok);
            }
            commands[QtdComandos] = NULL;

            // Criando pipes e processos e rodando
            pids[i] = fork();
            if(pids[i] == -1) {
                printf("Erro ao criar filho");
            } else if (pids[i] == 0) { // Filho
                // if(i == 0) {
                //     sid = setsid();
                //     printf("sid: %d\n", sid);
                // } else {
                //     setpgid(getpid(), sid);
                // }
                // printf("[Child %d] session_id: %d\n", i, getsid(0));
                // printf("[Child %d] groud_id: %d\n", i, getpgid(0));
                if(i == 0) {
                    // Primeiro processo
                    close(pipes[i][READ]);
                    dup2(pipes[i][WRITE], WRITE);
                    close(pipes[i][WRITE]);
                    // Fecha todos os pipes restantes
                    for(int z = 1; z< QtdPipes; z++) {
                        close(pipes[z][READ]);
                        close(pipes[z][WRITE]);
                    }
                } else if(i == QtdPipes+1) {
                    // Ultimo processo
                    close(pipes[i-1][WRITE]);
                    dup2(pipes[i-1][READ], READ);
                    close(pipes[i-1][READ]);
                    // Fecha todos os pipes restantes
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
                    // Fecha todos os pipes restantes
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
        // Deixa o pai esperando os três filhos, para depois dar exit
        while ((wpid = wait(&status)) > 0);
        // printf("Terminou de esperar\n");
        //Fechando pipes processo pai
        for(int i = 0; i < QtdPipes; i++) {
            close(pipes[i][READ]);
            close(pipes[i][WRITE]);
        }
        // fflush();
        printf("\n");
        printf("[Background] processos com sid %d terminaram.\n", sid);
        lista_Retira(lista_de_pgids, sid);
        printf("\rvsh 🐊 > ");

        // printf("Fechou os pipes\n");
        exit(0);
    } 
    // Processo principal
    close(pipe_pgid[WRITE]);
    read(pipe_pgid[READ], &sid, sizeof(pid_t));
    lista_InserePGID(lista_de_pgids, sid);
    close(pipe_pgid[READ]);
    // printf("[Parent] session_id: %d\n", getsid(0));
    // printf("[Parent] group_id: %d\n", getpgid(0));
    // while ((wpid = wait(&status)) > 0);
    // printf("Terminou de esperar\n");
    
    // printf("Depois do wait\n");
}

void run_armageddon(TLista *lista_de_pgids) {
    printf("Rodando Armageddon! Adeus!");
    lista_mata_processos_pgid(lista_de_pgids);
    lista_Imprime(lista_de_pgids);
    raise(SIGTERM);
}


void read_command(TLista* lista_de_pgids) {
    // Linha de comando lida, de tamanho máximo 300 caracteres
    char* commandLine = (char*) malloc(300 * sizeof(char));
    printf("\rvsh 🐊 > ");
    scanf("%[^\n]", commandLine);
    getchar();
    printf("%s\n", commandLine);

    // Verificando se é armageddon
    char* armageddon = strdup(commandLine);
    trim(armageddon);
    if(strcmp(armageddon, "armageddon") == 0) {
        run_armageddon(lista_de_pgids);
    }

    // Conta quantos pipes existem, usando o "|" como contador
    int QtdPipes = strcount(commandLine, '|');
    if (QtdPipes > 0) {
        run_command_special(commandLine, QtdPipes, lista_de_pgids);
    } else {
        int QtdComandos = strcount(commandLine, ' ') + 1;
        char* commands[QtdComandos+1];
        // printf("quantidade comandos: %d\n", QtdComandos);

        char *tok = strtok(commandLine, " ");
        for(int i = 0; i < QtdComandos; i++, tok = strtok(NULL, " ")){
            commands[i] = strdup(tok);
        }
        commands[QtdComandos] = NULL;
        for(int i = 0; i < QtdComandos; i++){
            // printf("%s\n", commands[i]);
        }
        free(commandLine);
        run_command(commands, QtdComandos);
    }
}