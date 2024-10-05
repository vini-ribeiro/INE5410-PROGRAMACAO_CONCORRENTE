#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

//                 (main)      
//                    |
//    +----------+----+------------+
//    |          |                 |   
// worker_1   worker_2   ....   worker_n


// ~~~ argumentos (argc, argv) ~~~
// ./program n_threads

// ~~~ printfs  ~~~
// main thread (após término das threads filhas): "Contador: %d\n"
// main thread (após término das threads filhas): "Esperado: %d\n"

// Obs:
// - pai deve criar n_threds (argv[1]) worker threads 
// - cada thread deve incrementar contador_global (operador ++) n_loops vezes
// - pai deve esperar pelas worker threads  antes de imprimir!


int contador_global = 0;

void* funcao_thread(void *arg);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("n_threads é obrigatório!\n");
        printf("Uso: %s n_threads n_loops\n", argv[0]);
        return 1;
    }

    int n_threads = atoi(argv[1]);
    int n_loops = atoi(argv[2]);
    //...
    
    pthread_t *thread = (pthread_t *) malloc(sizeof(pthread_t) * ((size_t) n_threads));
    for (int i = 0; i < n_threads; i++) {
        pthread_create(&thread[i], NULL, funcao_thread, (void *) &n_loops);
    }

    for (int i = 0; i < n_threads; ++i)
    {
        pthread_join(thread[i], NULL);
    }

    free(thread);

    printf("Contador: %d\n", contador_global);
    printf("Esperado: %d\n", n_threads*n_loops);
    return 0;
}


void* funcao_thread(void *arg) {
    int *n_loops = (int *) arg;

    for (int i = 0; i < *n_loops; i++) {
        contador_global++;
    }
    
    return(NULL);
}