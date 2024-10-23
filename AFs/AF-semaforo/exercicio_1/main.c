#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *out;
sem_t qt_A, qt_B;

void *thread_a(void *args) {
    for (int i = 0; i < *(int *) args; ++i) {
        sem_wait(&qt_A);
        //      +---> arquivo (FILE*) destino
        //      |    +---> string a ser impressa
        //      v    v
        fprintf(out, "A");
        // Importante para que vocês vejam o progresso do programa
        // mesmo que o programa trave em um sem_wait().
        fflush(stdout);
        sem_post(&qt_B);
    }
    return NULL;
}

void *thread_b(void *args) {
    for (int i = 0; i < *(int *) args; ++i) {
        sem_wait(&qt_B);
        fprintf(out, "B");
        fflush(stdout);
        sem_post(&qt_A);
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s [ITERAÇÕES]\n", argv[0]);
        return 1;
    }
    int iters = atoi(argv[1]);
    srand(time(NULL));
    out = fopen("result.txt", "w");

    sem_init(&qt_A, 0, 1);
    sem_init(&qt_B, 0, (size_t) rand() % 2);

    pthread_t ta, tb;

    // Cria threads
    pthread_create(&ta, NULL, thread_a, &iters);
    pthread_create(&tb, NULL, thread_b, &iters);

    // Espera pelas threads
    pthread_join(ta, NULL);
    pthread_join(tb, NULL);

    sem_destroy(&qt_A);
    sem_destroy(&qt_B);

    // Imprime quebra de linha e fecha arquivo
    fprintf(out, "\n");
    fclose(out);

    return 0;
}
