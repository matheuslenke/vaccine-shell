#include "../lib/vaccine_shell.h"
#include "../lib/list.h"
#include "../lib/masks.h"

// Usados para ler e escrever no pipe
#define READ 0
#define WRITE 1
#define MAX_READING 400

struct shell {
    TLista* lista_de_pgids;
};

vsh_t* inicializa_shell() {
    vsh_t *vsh = (vsh_t*) malloc(sizeof(vsh_t));
    vsh->lista_de_pgids = lista_InicializaLista();
    return vsh;
}

void libera_shell(vsh_t* shell){
    free(shell);
}

void roda_comando_unico(char** comandos){
    // Verifica se o comando existe
    char* verificaComando = (char*)malloc(100 * sizeof(char));
    strcat(verificaComando, "which ");
    strcat(verificaComando, comandos[0]);
    strcat(verificaComando, " > /dev/null 2>&1");
    if (system(verificaComando)) {
        // Verifica se o comando é válido
        printf("Este comando [%s] não existe\n", comandos[0]);
        free(verificaComando);
    } else {
        free(verificaComando);
        execvp(comandos[0], comandos);
    }
}

void roda_comando(char** comandos) {
    pid_t p1;
    int wstatus;

    p1 = fork();
    if (p1 == -1) {
        printf("Erro ao realizar fork\n");
    } else if(p1 == 0) {     // Processo filho
        cria_mascara_processo_foreground();
        roda_comando_unico(comandos);
    } else if (p1 > 0) {     // Processo pai
        /* Define as mascaras para a shell ignorar sinais do teclado
        *  enquanto o processo roda em foreground */
        cria_mascara_ignora_sinais_teclado();
        cria_mascara_trata_sigtstp();
        waitpid(p1, &wstatus, WUNTRACED);

        // Retorna a receber sinais do teclado
        restaura_mascara_padrao();
    } 
}

void roda_comando_especial(char* linhaDeComando, int QtdPipes, TLista* lista_de_pgids) {
    // Cria um vetor contendo cada comando em uma posição
    char* linhasDeComando[QtdPipes+1];
    char *tok = strtok(linhaDeComando, "|");
    pid_t session_id, pid_parent;

    for(int i = 0; i < QtdPipes +1; i++, tok = strtok(NULL, "|")){
        //O trim retira os espaços da string no começo e fim
        linhasDeComando[i] = trim(strdup(tok));
    }
    // Cria todos os pids dos filhos e para o pai aguardar
    pid_t pids[QtdPipes + 1], wpid;
    int status = 0;
    int pipes[QtdPipes][2];
    
    // Variáveis envolvendo criação da sessão
    pid_t sid;
    int pipe_pgid[2]; // Pipe para envio do SID criado para registro na lista da shell
    if (pipe(pipe_pgid) < 0) { 
        fprintf(stderr,"Erro no tubo.\n");
        _exit(1);
    }

    if((pid_parent = fork()) == -1) {
        printf("Falha ao fazer o fork.\n");
    } 
    else if (pid_parent == 0) { // Processo filho
        //Bloqueio dos sinais de entrada do teclado
        cria_mascara_processo_background();
        // Inicializa todos os pipes
        for(int i = 0; i < QtdPipes; i++) {
            if (pipe(pipes[i]) < 0) {
                fprintf(stderr,"Erro no tubo.\n");
                _exit(1);
            }
        }

        // Cria uma nova sessão no processo criado
        sid = setsid();

        close(pipe_pgid[READ]);
        write(pipe_pgid[WRITE], &sid, sizeof(pid_t));
        close(pipe_pgid[WRITE]);

        /* Para cada um dos comandos entre os pipes, lê os comandos e seus argumentos
        *  e roda os programas relacionados */
        for(int i = 0; i< QtdPipes + 1; i++) {
            int QtdComandos = strcount(linhasDeComando[i], ' ') + 1;
            char* comandos[QtdComandos + 1];

            // Divide a string em cada espaço
            char *tok = strtok(linhasDeComando[i], " ");
            for(int j = 0; j < QtdComandos; j++, tok = strtok(NULL, " ")){
                comandos[j] = strdup(tok);
            }
            comandos[QtdComandos] = NULL;

            if(QtdComandos > 4){
                printf("Comando não pode ser executado. Limite de argumentos excedido.\n");
                return;
            }

            // Verifica se é liberamoita ou armageddon e retorna caso seja
            if(strcmp(comandos[0], "armageddon") == 0 || strcmp(comandos[0], "liberamoita") == 0) {
                printf("Você não pode rodar o comando %s em background.\n", comandos[0]);
                return;
            }

            // Criando pipes e processos e rodando
            pids[i] = fork();
            if(pids[i] == -1) {
                printf("Erro ao criar filho.\n");
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
                else if(i == QtdPipes+1) { // Ultimo processo
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
                else { // Processos do meio
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
                roda_comando_unico(comandos);
            }
        }
        //Fechando pipes processo pai
        for(int i = 0; i < QtdPipes; i++) {
            close(pipes[i][READ]);
            close(pipes[i][WRITE]);
        }
        
        // Deixa o pai esperando os três filhos, para depois dar exit
        while ((wpid = wait(&status)) > 0) {
            imprime_prompt();
            // Se algum filho morreu por SIGUSR1 ou SIGUSR2, mata todos do grupo
            if(WTERMSIG(status) == SIGUSR1 || WTERMSIG(status) == SIGUSR2) {
                if (WTERMSIG(status) == SIGUSR1)
                    printf("Processo recebeu sinal SIGUSR1. Infelizmente todos do grupo %d morreram.\n", sid);
                else 
                    printf("Processo recebeu sinal SIGUSR2. Infelizmente todos do grupo %d morreram.\n", sid);
                imprime_prompt();

                // Retira o pgid da lista e envia o sinal para matar todos os processos do mesmo grupo
                lista_Retira(lista_de_pgids, sid);
                killpg(sid, SIGTERM);
            }
        }
    
        printf("\n[Background] processos com sid %d terminaram.\n", sid);
        lista_Retira(lista_de_pgids, sid);
        imprime_prompt();

        exit(0);
    }
    // Processo principal
    close(pipe_pgid[WRITE]);
    read(pipe_pgid[READ], &sid, sizeof(pid_t));
    lista_InserePGID(lista_de_pgids, sid);
    close(pipe_pgid[READ]);
    printf("\n");
}

void roda_armageddon(TLista* lista_de_pgids) {
    lista_mata_processos_pgid(lista_de_pgids);
}

void roda_liberamoita(vsh_t* vsh) {
    while(waitpid(-1, NULL, WNOHANG) > 0);
    printf("Liberou todos os zombies.\n");
}

void imprime_prompt() {
    printf("\rvsh 🐊 > ");
    // Limpa o buffer para exibir corretamente no terminal
    fflush(stdout);
}

void le_comando(vsh_t* vsh) {
    // Linha de comando lida, de tamanho máximo MAX_READING
    char* linhaDeComando = (char*) malloc(MAX_READING * sizeof(char));
    imprime_prompt();
    scanf("%[^\n]", linhaDeComando);
    getchar(); // Pega o \n sobrando

    // Verificando se é operacao interna
    char* operacaoInterna = strdup(linhaDeComando);
    trim(operacaoInterna); // Retira espaços sobrando da leitura

    // Verificando se é armageddon
    if(strcmp(operacaoInterna, "armageddon") == 0) {
        roda_armageddon(vsh->lista_de_pgids);
    // Verificando se é liberamoita
    } else if(strcmp(operacaoInterna, "liberamoita") == 0) {
        roda_liberamoita(vsh);
    } else {
        // Conta quantos pipes existem, usando o "|" como contador
        int QtdPipes = strcount(linhaDeComando, '|');
        
        // Background  
        if (QtdPipes > 0) {
            printf("Lista de PGIDs antes de rodar os comandos:\n");
            lista_Imprime(vsh->lista_de_pgids);
            roda_comando_especial(linhaDeComando, QtdPipes, vsh->lista_de_pgids);
            printf("Lista de PGIDs após rodar os comandos:\n");
            lista_Imprime(vsh->lista_de_pgids);
        }
        // Foreground 
        else {
            // Conta a quantidade de espaços, e define quantos argumentos tem
            int QtdComandos = strcount(linhaDeComando, ' ') + 1;
            char* comandos[QtdComandos];

            char *tok = strtok(linhaDeComando, " ");

            for(int i = 0; i < QtdComandos; i++, tok = strtok(NULL, " ")){
                comandos[i] = strdup(tok);
            }

            if(QtdComandos > 4){
                printf("Comando não pode ser executado. Limite de argumentos excedido.\n");
                return;
            }

            comandos[QtdComandos] = NULL; //O ultimo deve ser NULL
            roda_comando(comandos);

            // Libera toda a memoria alocada pros comandos
            for(int i = 0; i < QtdComandos; i++){
                free(comandos[i]);
            }
        }
    }
    free(linhaDeComando);
}