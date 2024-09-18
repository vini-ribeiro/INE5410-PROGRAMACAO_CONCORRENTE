#include "exercicio_2.h"


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