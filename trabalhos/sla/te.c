#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define n 5

// EXISTE UM JEITO MAIS INTELIGENTE DE FAZER ISSO

// ---------------------------------------------------------------
#define NUM_BARREIRAS 5
sem_t *barreira_semaforo;
pthread_mutex_t *barreira_acesso;
int *barreira_contador;

void barreira(int index) {
    if (index >= 5)
        return;
    pthread_mutex_lock(&barreira_acesso[index]);
    barreira_contador[index]++;
    if (barreira_contador[index] == n) {
        for (int i = 0; i < n; ++i) {
            sem_post(&barreira_semaforo[index]);
        }
        barreira_contador[index] = 0;
    }
    pthread_mutex_unlock(&barreira_acesso[index]);
    sem_wait(&barreira_semaforo[index]);
}

void init_barreiras(void) {
    barreira_contador = (int *)malloc(sizeof(int) * NUM_BARREIRAS);
    barreira_acesso = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * NUM_BARREIRAS);
    barreira_semaforo = (sem_t *)malloc(sizeof(sem_t) * NUM_BARREIRAS);
    for (int i = 0; i < NUM_BARREIRAS; ++i) {
        barreira_contador[i] = 0;
        pthread_mutex_init(&barreira_acesso[i], NULL);
        sem_init(&barreira_semaforo[i], 0, 0);
    }
}

void destroi_barreiras(void) {
    for (int i = 0; i < NUM_BARREIRAS; ++i) {
        pthread_mutex_destroy(&barreira_acesso[i]);
        sem_destroy(&barreira_semaforo[i]);
    }
    free(barreira_acesso);
    free(barreira_semaforo);
    free(barreira_contador);
}

// ---------------------------------------------------------------

void *f_t(void *arg) {
    int *t = (int *)arg;
    for (int i = 0; i < NUM_BARREIRAS; ++i) {
        printf("barreira %d thread %d antes\n", i, *t);
        barreira(i);
        sleep(2);
    }
    printf("fim %d\n", *t);
    free(t);
    return NULL;
}

int main(int argc, char const *argv[]) {
    pthread_t threads[n];

    init_barreiras();

    for (int i = 0; i < n; ++i) {
        int *id = (int *)malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&threads[i], NULL, f_t, (void *)id);
    }

    for (int i = 0; i < n; ++i) {
        pthread_join(threads[i], NULL);
    }

    destroi_barreiras();

    return 0;
}