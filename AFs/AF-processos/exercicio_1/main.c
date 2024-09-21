#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>


//       (pai)      
//         |        
//    +----+----+
//    |         |   
// filho_1   filho_2


// ~~~ printfs  ~~~
// pai (ao criar filho): "Processo pai criou %d\n"
//    pai (ao terminar): "Processo pai finalizado!\n"
//  filhos (ao iniciar): "Processo filho %d criado\n"

// Obs:
// - pai deve esperar pelos filhos antes de terminar!


int main(int argc, char** argv) {

    // ....

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/
    
    /*if (argc != 2) {
        printf("Somente dois (2) argumentos são aceitos\n");
        return 1;
    }*/

    int numero_forks = 2, numero_processo; // atoi(argv[1]);

    for (size_t i = 0; i < numero_forks; i++) {
        numero_processo = fork();
        if (numero_processo != 0) {
            printf("Processo pai criou %d\n", numero_processo);
            fflush(stdout);
        } else {
            printf("Processo filho %d criado\n", getpid());
            fflush(stdout);
            return 0;
        }
    }
    
    while(wait(NULL) >= 0);
    
    printf("Processo pai finalizado!\n");

    return 0;
}
