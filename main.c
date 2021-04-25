#include "lib/reader.h"
#include "lib/operations.h"

int main(int argc, char const *argv[]) {
  TLista* lista_de_pgids = lista_InicializaLista();
  while(1) {
    read_command(lista_de_pgids);
    lista_Imprime(lista_de_pgids);
  }
  lista_Libera(lista_de_pgids);
  return 0;
}
