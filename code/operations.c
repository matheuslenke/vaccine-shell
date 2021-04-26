#include "../lib/operations.h"
#include "../lib/list.h"

#define READ 0
#define WRITE 1

void run_single_command(char** commands){
    execvp(commands[0], commands);
}

void run_command(char** commands) {
    pid_t p1;
    int wstatus;

    p1 = fork();
    if (p1 == -1) {
        printf("Erro ao realizar fork\n");
    } else if(p1 == 0) {     // Processo filho
        create_mask_foreground_process();

        run_single_command(commands);
    } else if (p1 > 0) {     // Processo pai
        waitpid(p1, &wstatus, 0);
    } 
}

void create_mask_background_process() {
    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = SIG_BLOCK;
    sigaction(SIGINT, &sa, NULL); 
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void create_mask_foreground_process() {
    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
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
    if (pipe(pipe_pgid) < 0) { 
        fprintf(stderr,"Erro no tubo\n");
        _exit(1);
    }

    if((pid_parent = fork()) == -1) {
        printf("Failed to fork");
    } 
    else if (pid_parent == 0) { // Child process
        //Bloqueio dos sinais de entrada do teclado
        create_mask_background_process();
        // Inicializa todos os pipes
        for(int i = 0; i < QtdPipes; i++) {
            if (pipe(pipes[i]) < 0) {
                fprintf(stderr,"Erro no tubo\n");
                _exit(1);
            }
        }

        // Cria uma nova sessão no processo criado
        sid = setsid();

        close(pipe_pgid[READ]);
        write(pipe_pgid[WRITE], &sid, sizeof(pid_t));
        close(pipe_pgid[WRITE]);

        for(int i = 0; i< QtdPipes + 1; i++) {
            int QtdComandos = strcount(commandLines[i], ' ') + 1;
            char* commands[QtdComandos + 1];

            // Divide a string em cada espaço
            char *tok = strtok(commandLines[i], " ");
            for(int j = 0; j < QtdComandos; j++, tok = strtok(NULL, " ")){
                commands[j] = strdup(tok);
            }
            commands[QtdComandos] = NULL;

            if(QtdComandos > 4){
                printf("Comando não pode ser executado. Limite de argumentos excedido.\n");
                return;
            }

            // Criando pipes e processos e rodando
            pids[i] = fork();
            if(pids[i] == -1) {
                printf("Erro ao criar filho");
            } else if (pids[i] == 0) { // Filho
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
                } 
                else if(i == QtdPipes+1) {
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
                } 
                else {
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
                run_single_command(commands);
            }
        }
        //Fechando pipes processo pai
        for(int i = 0; i < QtdPipes; i++) {
            close(pipes[i][READ]);
            close(pipes[i][WRITE]);
        }

        sleep(500); //para criar zoombies
        
        // Deixa o pai esperando os três filhos, para depois dar exit
        while ((wpid = wait(&status)) > 0) {
            print_prompt();
            // Se algum filho morreu por SIGUSR1 ou SIGUSR2, mata todos do grupo
            if(WTERMSIG(status) == SIGUSR1 || WTERMSIG(status) == SIGUSR2) {
                if (WTERMSIG(status) == SIGUSR1)
                    printf("Processo recebeu sinal SIGUSR1. Infelizmente todos do grupo %d morreram.\n", sid);
                else 
                    printf("Processo recebeu sinal SIGUSR2. Infelizmente todos do grupo %d morreram.\n", sid);
                print_prompt();

                // Retira o pgid da lista e envia o sinal para matar todos os processos do mesmo grupo
                lista_Retira(lista_de_pgids, sid);
                killpg(sid, SIGTERM);
            }
        }

        // for(int i = 0; i < QtdComandos; i++){
        //     free(commands[i]);
        // }
    
        printf("\n");
        printf("[Background] processos com sid %d terminaram.\n", sid);
        lista_Retira(lista_de_pgids, sid);
        print_prompt();

        exit(0);
    }
    // Processo principal
    close(pipe_pgid[WRITE]);
    read(pipe_pgid[READ], &sid, sizeof(pid_t));
    lista_InserePGID(lista_de_pgids, sid);
    close(pipe_pgid[READ]);
    printf("\n");
}

void run_armageddon(TLista *lista_de_pgids) {
    lista_mata_processos_pgid(lista_de_pgids);
}

void run_liberamoita(TLista *lista_de_pgids) {
    lista_mata_processos_zombies(lista_de_pgids);
}

void print_prompt() {
    printf("\rvsh 🐊 > ");
    // Limpa o buffer para exibir corretamente no terminal
    fflush(stdout);
}

void read_command(TLista* lista_de_pgids) {
    // Linha de comando lida, de tamanho máximo 300 caracteres
    char* commandLine = (char*) malloc(400 * sizeof(char));
    print_prompt();
    // commandLine = getline(&commandLine, 300, stdin);
    scanf("%[^\n]", commandLine);
    getchar();
    printf("%s\n", commandLine);

    // Verificando se é operacao interna
    char* operacaoInterna = strdup(commandLine);
    trim(operacaoInterna);
    // Verificando se é armageddon
    if(strcmp(operacaoInterna, "armageddon") == 0) {
        run_armageddon(lista_de_pgids);
    // Verificando se é liberamoita
    } else if(strcmp(operacaoInterna, "liberamoita") == 0) {
        run_liberamoita(lista_de_pgids);
    } else {
        // Conta quantos pipes existem, usando o "|" como contador
        int QtdPipes = strcount(commandLine, '|');

        // Background
        if (QtdPipes > 0) {
            run_command_special(commandLine, QtdPipes, lista_de_pgids);
        }
        // Foreground 
        else {
            int QtdComandos = strcount(commandLine, ' ') + 1;
            char* commands[QtdComandos];

            char *tok = strtok(commandLine, " ");

            for(int i = 0; i < QtdComandos; i++, tok = strtok(NULL, " ")){
                commands[i] = strdup(tok);
            }
            printf("Quantidade comandos: %d\n", QtdComandos);

            if(QtdComandos > 4){
                printf("Comando não pode ser executado. Limite de argumentos excedido.\n");
                return;
            }

            commands[QtdComandos] = NULL;
            run_command(commands);
            // Libera toda a memoria alocada pros comandos
           
            for(int i = 0; i < QtdComandos; i++){
                free(commands[i]);
            }
        }
    }
    free(commandLine);
}