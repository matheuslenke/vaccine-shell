#include "lib/reader.h"
#include "lib/operations.h"

void trata_sigurs_vsh() {
  // Tratativa para a vacina oriental que o vsh tomou
  printf("\rvsh 🐊 > \n");
  printf("              _  _\n    _ _      (0)(0)-._  _.-'^^'^^'^^'^^'^^'--.\n   (.(.)----'`        ^^'                /^   ^^-._\n    (    `                 \             |    _    ^^-._\n    VvvvvvvVv~~`__,/.._>  /:/:/:/:/:/:/:/\  (_..,______^^-.\n     `^^^^^^^^`/  /   /  /`^^^^^^^^^>^^>^`>  >        _`)  )\n              (((`   (((`          (((`  (((`        `'--'^\n");
  printf("I feel weird...\n");
}

int main(int argc, char const *argv[]) {
  struct sigaction act;   
  // Bloqueio de sinais SIGINT, SIGQUIT e SIGTSTP durante tratamento
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

  /* Inicializa lista de pgids para gerenciamento das sessões 
  *  relacionadas à essa sessão da shell
  */
  TLista* lista_de_pgids = lista_InicializaLista();
  while(1) {
    read_command(lista_de_pgids);
  }
  return 0;
}
