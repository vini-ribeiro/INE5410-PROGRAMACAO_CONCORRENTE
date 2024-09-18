#include <stdio.h>
#include <stdlib.h>


#include "exercicio_1.h"


// função para printar o MINMAX
void print_MinMax(const MinMax *const mm) {
	printf("Min: %d; Max: %d\n", mm->min, mm->max);
}


// pega o maior e o menor valor de um array e retorna em um MinMax
MinMax* getMinMax(int *array, const int SIZE) {
	MinMax *aux = (MinMax*) malloc(sizeof(MinMax));
	if (!aux) return NULL;
	aux->min = array[0];
	aux->max = array[0];

	for (int i = 0; i < SIZE; ++i) {
		if (aux->min > array[i]) aux->min = array[i];
		if (aux->max < array[i]) aux->max = array[i];
	}

	return aux;
}