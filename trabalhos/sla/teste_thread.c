#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 25
#define NUM_TESTES_POR_THREAD 10000

pthread_mutex_t mutex;

int total_acertos = 0, total_erros = 0;
void *A(void *arg) {
    int acertos = 0, erros = 0;
    int testes = NUM_TESTES_POR_THREAD;

    while (testes-- > 0) {
        int bolas[8] = {1, 1, 1, 1, 1, 1, 1, 1};
        bolas[rand() % 8] = 2;

        int peso_esq = 0, peso_dir = 0;
        for (int i = 0; i < 3; ++i) {
            peso_esq += bolas[i];
            peso_dir += bolas[i + 3];
        }

        if (peso_esq == peso_dir) {
            peso_esq += bolas[6];
            peso_dir += bolas[7];
            if (peso_esq > peso_dir) {
                acertos += bolas[6] == 2;
                erros += bolas[6] != 2;
            } else {
                acertos += bolas[7] == 2;
                erros += bolas[7] != 2;
            }
        } else if (peso_esq > peso_dir) {
            peso_esq = bolas[0];
            peso_dir = bolas[1];
            if (peso_esq > peso_dir) {
                acertos += bolas[0] == 2;
                erros += bolas[0] != 2;
            } else if (peso_dir > peso_esq) {
                acertos += bolas[1] == 2;
                erros += bolas[1] != 2;
            } else {
                acertos += bolas[2] == 2;
                erros += bolas[2] != 2;
            }
        } else {
            peso_dir = bolas[3];
            peso_esq = bolas[4];
            if (peso_esq < peso_dir) {
                acertos += bolas[3] == 2;
                erros += bolas[3] != 2;
            } else if (peso_dir < peso_esq) {
                acertos += bolas[4] == 2;
                erros += bolas[4] != 2;
            } else {
                acertos += bolas[5] == 2;
                erros += bolas[5] != 2;
            }
        }
    }

    pthread_mutex_lock(&mutex);
    total_acertos += acertos;
    total_erros += erros;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void) {
    pthread_t t_id[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&t_id[i], NULL, A, NULL);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(t_id[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("acertos: %d\n", total_acertos);
    printf("erros: %d\n", total_erros);

    for (int i = 1; i < 2; ++i) {
        printf("%d\n", i);
    }

    return 0;
}
