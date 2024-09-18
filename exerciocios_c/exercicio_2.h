#ifndef _EXERCICIO_2
#define _EXERCICIO_2


// retorna a diferença do primeiro pelo segundo
int sortAscending(int *num1, int *num2);


// retorna a diferença do segundo pelo primeiro
int sortDescending(int *num1, int *num2);


// troca dois valores de posição
void swap(int *num1, int *num2);


// ordena de acordo com uma função compare
void sort(int *array, int SIZE, int (*compare)(int *, int *));

#endif