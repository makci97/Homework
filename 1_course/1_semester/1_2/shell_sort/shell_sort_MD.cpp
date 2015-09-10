#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int const zsize = 10;

int* getArrayFromRand(int size) // генерация масива размера size c рандомными значениями
{
    int i,* massiv = (int*) calloc(size,sizeof(int));
	srand(time(NULL));
    for(i=0;i<size;i++)
    {
        massiv[i] = rand();
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
	int buf = a;
	a = b;
	b = buf;
	return;
}

void sort(int *Array, int size, int d)
{
	int i, ii, jj;

	for(i = 0; i < d; i++)
	{
		for(ii = i; ii < size; ii += d)
		{
			for(jj = i; jj < size - ii - d; jj += d)
			{
				if(Array[jj] > Array[jj + d])
					swap(Array[jj], Array[jj + d]);
			}
		}
	}
}

void shell_sort(int *Array, int size)
{
	int d = size / 2;
	while(d > 0)
	{
		sort(Array, size, d);
		d /= 2;
	}
}

int main()
{
	int *a = getArrayFromRand(zsize);
	printArrayToCons(a, zsize);
	shell_sort(a, zsize);
	printArrayToCons(a, zsize);
	return 0;
}