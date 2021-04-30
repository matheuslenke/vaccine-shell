#include "lib/reader.h"
#include "lib/vaccine_shell.h"
#include "lib/masks.h"


int main(int argc, char const *argv[]) {

    // Seta a mascara de tratamento da VSH
    seta_mascara_vsh();

    /* Inicializa struct de pgids para gerenciamento das sessões */
    vsh_t* vaccine_shell = inicializa_shell();
    while(1) {
        le_comando(vaccine_shell);
    }
    libera_shell(vaccine_shell);
    return 0;
}
