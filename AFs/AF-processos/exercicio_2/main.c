#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

// funcão dedicada ao filho
void filho_funcao(void);
void neto_funcao(void);

int main(int argc, char** argv) {

    // ....

    /*************************************************
     * Dicas:                                        *
     * 1. Leia as intruções antes do main().         *
     * 2. Faça os prints exatamente como solicitado. *
     * 3. Espere o término dos filhos                *
     *************************************************/
    
    
    int filho1 = fork();
    if (filho1 == 0) filho_funcao();

    int filho2 = fork();
    if (filho2 == 0) filho_funcao();

    while(wait(NULL) >= 0);

    printf("Processo principal %d finalizado\n", getpid());
    
    return 0;
}

void filho_funcao(void) {

    printf("Processo %d, filho de %d\n", getpid(), getppid());
    fflush(stdout);

    int filho11 = fork();
    if (filho11 == 0) neto_funcao();

    int filho12 = fork();
    if (filho12 == 0) neto_funcao();

    int filho13 = fork();
    if (filho13 == 0) neto_funcao();
    
    while(wait(NULL) >= 0);
    
    printf("Processo %d finalizado\n", getpid());
    fflush(stdout);

    exit(0); // encerra o processo filho
}

void neto_funcao(void) {
    
    printf("Processo %d, filho de %d\n", getpid(), getppid());
    fflush(stdout);
    sleep(5);
    printf("Processo %d finalizado\n", getpid());
    fflush(stdout);
    exit(0); // encerra o processo neto
}