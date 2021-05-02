#include "../lib/masks.h"

void restaura_mascara_padrao() {
    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa, NULL); 
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void cria_mascara_ignora_sinais_teclado() {
    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL); 
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

void trata_sigtstp() {
    printf("\nProcesso suspenso.\n");
}

void cria_mascara_trata_sigtstp() {
    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &trata_sigtstp;
    sigaction(SIGTSTP, &sa, NULL);
}

void cria_mascara_processo_background() {
    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = SIG_BLOCK;
    sigaction(SIGINT, &sa, NULL); 
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

}

void cria_mascara_processo_foreground() {
    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
}

void trata_sigurs_vsh() {
    // Tratativa para a vacina oriental que o vsh tomou
    printf("\rvsh 🐊 > \n");
    printf("              _  _\n    _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--.\n   (.(.)----'`        ^^'                /^   ^^-._\n    (    `                 \             |    _    ^^-._\n    VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\  (_..,______^^-.\n     `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  )\n              (((`   (((`          (((`  (((`        `'--'^\n");
    printf("I feel weird...\n");
    imprime_prompt();
}

void seta_mascara_vsh() {
    struct sigaction act;   
    // Bloqueio de sinais SIGINT, SIGQUIT, SIGTSTP e SIGTERM
    struct sigaction sa = { 0 };
    sigset_t block_sigs;
    sigemptyset(&block_sigs);  
    sigaddset(&block_sigs, SIGINT);  
    sigaddset(&block_sigs, SIGQUIT);   
    sigaddset(&block_sigs, SIGTSTP); 
    sigaddset(&block_sigs, SIGTERM); 
    sa.sa_mask = block_sigs;

    // Tratramento sinais SIGUSR1 e SIGUSR2
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &trata_sigurs_vsh;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
}
