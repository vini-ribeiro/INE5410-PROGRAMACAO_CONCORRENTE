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


// Avalia o resultado no vetor c. Assume-se que todos os ponteiros (a, b, e c)
// tenham tamanho size.
void avaliar(double* a, double* b, double* c, int size);


// função da thread
void* thread1(void *arg);


// struct para passagem de parâmetro na funcão da thread
typedef struct {
    int begin;
    int end;
    double *a, *b, *c;
} range_t;


int main(int argc, char* argv[]) {
    // Gera um resultado diferente a cada execução do programa
    // Se **para fins de teste** quiser gerar sempre o mesmo valor
    // descomente o srand(0)
    srand(time(NULL)); //valores diferentes
    //srand(0);        //sempre mesmo valor

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
    //Cria vetor do resultado 
    double* c = malloc(a_size*sizeof(double));

    // Calcula com uma thread só. Programador original só deixou a leitura 
    // do argumento e fugiu pro caribe. É essa computação que você precisa 
    // paralelizar


    //    +-----------------------+
    // ** | MINHA SOLUÇÃO ABAIXO! | **
    //    +-----------------------+

    if (n_threads > a_size) n_threads = a_size;
    
    // cria vetor de threads
    pthread_t *threads = malloc(n_threads * sizeof(pthread_t));
    // cria vetor de intervalos
    range_t *intervalos = malloc(n_threads * sizeof(range_t));

    // configura os ponteiros das instâncias da struct para apontar para o vetor
    for (int i = 0; i < n_threads; i++) {
        intervalos[i].a = a;
        intervalos[i].b = b;
        intervalos[i].c = c;
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
    free(threads);
    free(intervalos);

    // for (int i = 0; i < a_size; ++i) 
    //     c[i] = a[i] + b[i];

    //    +----------------------+
    // ** | MINHA SOLUÇÃO ACIMA! | **
    //    +----------------------+

    //    +---------------------------------+
    // ** | IMPORTANTE: avalia o resultado! | **
    //    +---------------------------------+
    avaliar(a, b, c, a_size);
    

    //Importante: libera memória
    free(a);
    free(b);
    free(c);

    return 0;
}


void* thread1(void *arg) {
    range_t *intervalo = (range_t *) arg;

    for (int i = intervalo->begin; i <= intervalo->end; i++) {
        intervalo->c[i] = intervalo->a[i] + intervalo->b[i];
    }

    return 0;
}