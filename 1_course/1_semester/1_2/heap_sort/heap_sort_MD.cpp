#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int const zsize = 20;

int* getArrayFromRand(int size) // генерация масива размера size c рандомными значениями
{
    int i,* massiv = (int*) calloc(size + 1, sizeof(int));
	srand(time(NULL));
    for(i = 0; i <= size; i++)
    {
        massiv[i] = rand() % 100;
    }
    return massiv;
}


void printHeapToCons(const int* array, int size) // вывод в консоль массива
{
    int i;
    for(i = 1; i <= size; i++)
    {
        printf("%i\t", array[i]);
    }
	printf("\n");
}

void printArrayToCons(const int* array, int size) // вывод в консоль массива
{
    int i;
    for(i = 0; i < size; i++)
    {
        printf("%i\t", array[i]);
    }
	printf("\n");
}

void swap(int &a, int &b)
{
	int buffer = a;
	a = b;
	b = buffer;
	return;
}

void heapify(int *Array, int el, int size)
{
	if((2 * el + 1 > size && 2 * el == size && Array[el] <= Array[2 * el]) || (Array[el] <= Array[2 * el] && Array[el] <= Array[2 * el + 1] && 2 * el + 1 <= size) || 2 * el > size)
		return;

	if(el * 2 + 1 <= size)
	{
		if(Array[2 * el] < Array[2 * el + 1])
		{
			swap(Array[el], Array[2 * el]);
			heapify(Array, 2 * el, size);
		}
		else
		{
			swap(Array[el], Array[2 * el + 1]);
			heapify(Array, 2 * el + 1, size);
		}
	}
	else
		swap(Array[el], Array[2 * el]);
}

void create_binary_heap(int *Array, int size)
{
	int i = (size + 1) / 2;
	while(i > 0)
		heapify(Array, i--, size);
}

int min_elem_binary_heap(int *Array, int &size)
{
	if(size < 1)
		return -1;

	int min = Array[1];

	Array[1] = Array[size--];
	heapify(Array, 1, size);

	return min;
}


void heapify_up(int *Array, int el, int size)
{
	if(el == 1 || Array[el / 2] <= Array[el])
		return;

	int sw;
	if(el % 2 == 1)
		if(Array[el] > Array[el - 1])
			sw = el - 1;
		else
			sw = el;
	else
		if(el == size)
			sw = el;
		else if(Array[el] > Array[el - 1])
			sw = el - 1;
		else
			sw = el;

	swap(Array[sw], Array[el / 2]);
	heapify_up(Array, el / 2, size);
	heapify(Array, sw, size);
}


void put_new_elem_binary_heap(int *Array, int &size, int new_elem)
{
	Array = (int*) realloc(Array, (++size + 1) * sizeof(int));
	Array[size] = new_elem;
	heapify_up(Array, size, size);
	return;
}



int *heap_sort(int size)
{
	int buf, i = 0, *Array = getArrayFromRand(size + 1), *a = (int*) malloc(size * sizeof(int));

	
	printf("Initial array:\n");
	printHeapToCons(Array, size);
	
	create_binary_heap(Array, size);
	printf("Heap:\n");
	printHeapToCons(Array, size);

	while( (buf = min_elem_binary_heap(Array, size)) != -1)
		a[i++] = buf;

	free(Array);
	return a;
}

int main()
{
	int *a = heap_sort(zsize);
	
	printf("Final array:\n");
	printArrayToCons(a, zsize);

	return 0;
}