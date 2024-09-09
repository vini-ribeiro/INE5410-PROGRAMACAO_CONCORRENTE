#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ARRAY_SIZE 15
#define RAND_RANGE_MIN 1
#define RAND_RANGE_MAX 100000


typedef struct MinMax {
	int min;
	int max;
} MinMax;


// função para printar o MINMAX
void print_MinMax(const MinMax *const mm) {
	printf("Min: %d; Max: %d\n", mm->min, mm->max);
}


// função para printar um array de int
void print_array(const int * const array, const int SIZE) {
	printf("| ");
	for (int i = 0; i < SIZE; ++i)
	{
		printf("%d | ", array[i]);
	}
	printf("\n");
}


// pega o maior e o menor valor de um array
MinMax* getMinMax(int *array, const int SIZE) {
	MinMax *aux = (MinMax*) malloc(sizeof(MinMax));
	if (!aux) return NULL;
	aux->min = array[0];
	aux->max = array[0];

	for (int i = 0; i < SIZE; ++i)
	{
		if (aux->min > array[i]) aux->min = array[i];
		if (aux->max < array[i]) aux->max = array[i];
	}

	return aux;
}


// monta o array com numeros aleatorios
int* build_array(const int SIZE, const int RANGE_MIN, const int RANGE_MAX) {
	srand(time(NULL));
	int *array = (int*) malloc(sizeof(int) * SIZE);
	
	for (int i = 0; i < SIZE; ++i)
		array[i] = (rand() % RANGE_MAX) + RANGE_MIN;
	
	return array;
}


// retorna a diferença do primeiro pelo segundo
int sortAscending(int *num1, int *num2) {
	return *num1 - *num2;
}


// retorna a diferença do segundo pelo primeiro
int sortDescending(int *num1, int *num2) {
	return *num2 - *num1;
}


// troca dois valores de posição
void swap(int *num1, int *num2) {
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}


// ordena de acordo com uma função compare
void sort(int *array, int SIZE, int (*compare)(int *, int *)) {
	for (int i = 0; i < SIZE - 1; ++i)
		for (int j = i + 1; j < SIZE; ++j)
			if (compare(&array[j], &array[i]) < 0) 
				swap(&array[j], &array[i]);
}

int main(int argc, char const *argv[])
{
	int *array = build_array(ARRAY_SIZE, RAND_RANGE_MIN, RAND_RANGE_MAX);
	print_array(array, ARRAY_SIZE);

	MinMax *mm = getMinMax(array, ARRAY_SIZE);

	print_MinMax(mm);

	sort(array, ARRAY_SIZE, sortAscending);
	print_array(array, ARRAY_SIZE);
	sort(array, ARRAY_SIZE, sortDescending);
	print_array(array, ARRAY_SIZE);

	free(array);
	free(mm);

	return 0;
}