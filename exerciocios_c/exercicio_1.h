#ifndef _EXERCICIO_1
#define _EXERCICIO_1

typedef struct MinMax {
	int min;
	int max;
} MinMax;


// função para printar o MINMAX
void print_MinMax(const MinMax *const mm);

// pega o maior e o menor valor de um array e retorna em um MinMax
MinMax* getMinMax(int *array, const int SIZE);

#endif