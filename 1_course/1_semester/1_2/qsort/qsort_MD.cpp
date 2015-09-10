#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cassert>

int const zsize = 40;

int* getArrayFromRand(int size) // генерация масива размера size c рандомными значениями
{
    int i,* massiv = (int*) calloc(size,sizeof(int));
	srand(0);
    for(i=0;i<size;i++)
    {
        massiv[i] = rand() % 20;
    }
    return (&massiv[0]);
}


void printArrayToCons(const int* array, int size) // вывод в консоль массива
{
    int i;
    for(i = 0; i < size; i++)
    {
        printf("%i\t", array[i]);
    }
}

void swap(int &a, int &b)
{
	int buffer = a;
	a = b;
	b = buffer;
}

int partition(int *Array, int left, int right)
{
	srand(time(NULL));
	int i = left, j = right, v = (right + left) / 2;
	
  while(1)
	{
		while (Array[i] < Array[v] && i < v)
      ++i;

		while (Array[j] >= Array[v] && j > v)
      --j;

		if (i == j)
			break;
    
    if (i == v)
      v = j;
    else if (j == v)
      v = i;
    
		swap(Array[i], Array[j]);
	}

	return i;
}

void q_sort(int *Array, int left, int right)
{
  if (left >= right)
    return;

	int i = partition(Array, left, right);
  
	q_sort(Array, left, i-1);
	q_sort(Array, i+1, right);
}


int main()
{
	int *a = getArrayFromRand(zsize);
  // int a[] = {1, 2};
	printArrayToCons(a, zsize);
	q_sort(a, 0, zsize - 1);
  printf("\n");
	printArrayToCons(a, zsize);
	return 0;
}