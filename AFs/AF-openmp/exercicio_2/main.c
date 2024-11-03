#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_matrix(double *m, int rows, int columns) {

#pragma omp parallel for schedule(guided)
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            m[i * columns + j] = i + j;
}

/*
Como as variáveis i, j e k foram declaradas fora da região paralela, temos que torna-las privadas para cada thread, tendo em vista que seu valor
será modificado no decorrer da execução de cada thread. A segunda região paralela foi removida pois seu uso não aumentou a performance dos cálculos
*/
void mult_matrix(double *out, double *left, double *right, int rows_left, int cols_left, int cols_right) {

    int i, j, k;
#pragma omp parallel for schedule(dynamic, 1) private(i, j, k)
    for (i = 0; i < rows_left; ++i) {
        for (j = 0; j < cols_right; ++j) {
            out[i * cols_right + j] = 0;
            for (k = 0; k < cols_left; ++k)
                out[i * cols_right + j] += left[i * cols_left + k] * right[k * cols_right + j];
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s tam_matriz\n", argv[0]);
        return 1;
    }
    int sz = atoi(argv[1]);
    double *a = malloc(sz * sz * sizeof(double));
    double *b = malloc(sz * sz * sizeof(double));
    double *c = calloc(sz * sz, sizeof(double));

    // double start = omp_get_wtime();
    init_matrix(a, sz, sz);
    init_matrix(b, sz, sz);

    //          c = a * b
    mult_matrix(c, a, b, sz, sz, sz);
    // double duration = omp_get_wtime() - start;

    /* ~~~ imprime matriz ~~~ */
    char tmp[32];
    int max_len = 1;
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j) {
            int len = sprintf(tmp, "%ld", (unsigned long) c[i * sz + j]);
            max_len = max_len > len ? max_len : len;
        }
    }
    char fmt[16];
    if (snprintf(fmt, 16, "%%s%%%dld", max_len) < 0)
        abort();
    for (int i = 0; i < sz; ++i) {
        for (int j = 0; j < sz; ++j)
            printf(fmt, j == 0 ? "" : " ", (unsigned long) c[i * sz + j]);
        printf("\n");
    }

    // printf("duracao: %.4f\n", duration);

    free(a);
    free(b);
    free(c);

    return 0;
}
