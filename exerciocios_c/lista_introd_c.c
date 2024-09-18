#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "exercicio_1.h"
#include "exercicio_2.h"


#define ARRAY_SIZE 15
#define RAND_RANGE_MIN 1
#define RAND_RANGE_MAX 100


// função para printar um array de int
void print_array(const int * const array, const int SIZE) {
	printf("| ");
	for (int i = 0; i < SIZE; ++i) {
		printf("%d | ", array[i]);
	}
	printf("\n");
}


// monta o array com numeros aleatorios
int* build_array(const int SIZE, const int RANGE_MIN, const int RANGE_MAX) {
	srand(time(NULL));
	int *array = (int*) malloc(sizeof(int) * SIZE);
	
	for (int i = 0; i < SIZE; ++i)
		array[i] = (rand() % RANGE_MAX) + RANGE_MIN;
	
	return array;
}


int main()
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