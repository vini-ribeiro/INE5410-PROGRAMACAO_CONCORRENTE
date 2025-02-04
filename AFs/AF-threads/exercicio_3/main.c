#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>

// Lê o conteúdo do arquivo filename e retorna um vetor E o tamanho dele
// Se filename for da forma "gen:%d", gera um vetor aleatório com %d elementos
//
// +-------> retorno da função, ponteiro para vetor malloc()ado e preenchido
// | 
// |         tamanho do vetor (usado <-----+
// |         como 2o retorno)              |
// v                                       v
double* load_vector(const char* filename, int* out_size);


// Avalia se o prod_escalar é o produto escalar dos vetores a e b. Assume-se
// que ambos a e b sejam vetores de tamanho size.
void avaliar(double* a, double* b, int size, double prod_escalar);


// função da thread
void* thread1(void *arg);


typedef  struct {
    double valor;
} somatorio_t;

// struct para passagem de parâmetro na funcão da thread
typedef struct {
    int begin;
    int end;
    double *a, *b;
    somatorio_t *somatorio_ptr;
} range_t;


int main(int argc, char* argv[]) {
    srand(time(NULL));

    //Temos argumentos suficientes?
    if(argc < 4) {
        printf("Uso: %s n_threads a_file b_file\n"
               "    n_threads    número de threads a serem usadas na computação\n"
               "    *_file       caminho de arquivo ou uma expressão com a forma gen:N,\n"
               "                 representando um vetor aleatório de tamanho N\n", 
               argv[0]);
        return 1;
    }
  
    //Quantas threads?
    int n_threads = atoi(argv[1]);
    if (!n_threads) {
        printf("Número de threads deve ser > 0\n");
        return 1;
    }
    //Lê números de arquivos para vetores alocados com malloc
    int a_size = 0, b_size = 0;
    double* a = load_vector(argv[2], &a_size);
    if (!a) {
        //load_vector não conseguiu abrir o arquivo
        printf("Erro ao ler arquivo %s\n", argv[2]);
        return 1;
    }
    double* b = load_vector(argv[3], &b_size);
    if (!b) {
        printf("Erro ao ler arquivo %s\n", argv[3]);
        return 1;
    }
    
    //Garante que entradas são compatíveis
    if (a_size != b_size) {
        printf("Vetores a e b tem tamanhos diferentes! (%d != %d)\n", a_size, b_size);
        return 1;
    }



    if (n_threads > a_size) n_threads = a_size;
    
    // cria vetor de threads
    pthread_t *threads = malloc(n_threads * sizeof(pthread_t));
    // cria vetor de intervalos
    range_t *intervalos = malloc(n_threads * sizeof(range_t));
    somatorio_t somatorio;
    somatorio.valor = 0;

    // configura os ponteiros das instâncias da struct para apontar para o vetor
    for (int i = 0; i < n_threads; i++) {
        intervalos[i].a = a;
        intervalos[i].b = b;
        intervalos[i].somatorio_ptr = &somatorio;
    }


    // os próximos dois FORs aplicam o princípio da casa dos pombos
    for (int i = 0; i < a_size % n_threads; i++) {
        intervalos[i].begin = i * (a_size / n_threads + 1);
        intervalos[i].end = (i + 1) * (a_size / n_threads + 1) - 1;
    }
    
    for (int i = a_size % n_threads; i < n_threads; i++) {
        intervalos[i].begin = i * (a_size / n_threads) + (a_size % n_threads);
        intervalos[i].end = (i + 1) * (a_size / n_threads) - 1 + (a_size % n_threads);
    }

    for (int i = 0; i < n_threads; ++i) {
        pthread_create(&threads[i], NULL, thread1, &intervalos[i]);
    }

    for (int i = 0; i < n_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    double result = somatorio.valor;

    free(threads);
    free(intervalos);




    //Calcula produto escalar. Paralelize essa parte
    // double result = 0;
    // for (int i = 0; i < a_size; ++i) 
    //     result += a[i] * b[i];
    
    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, a_size, result);

    //Libera memória
    free(a);
    free(b);

    return 0;
}


void* thread1(void *arg) {
    range_t *intervalo = (range_t *) arg;

    for (int i = intervalo->begin; i <= intervalo->end; i++) {
        intervalo->somatorio_ptr->valor += intervalo->a[i] * intervalo->b[i];
    }

    return 0;
}