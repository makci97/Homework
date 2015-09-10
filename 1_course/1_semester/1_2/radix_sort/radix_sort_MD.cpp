#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
	printf("\n");
}

int getmax(int *Array, int right)
{
	int max = Array[0], i;

	for(i = 1; i <= right; i++)
	{
		if(max < Array[i])
			max = Array[i];
	}

	return max;
}

void sort(int *Array, int right, int razr)
{
	int i, *c = (int*) calloc(10 , sizeof(int)), *buffer = (int*) malloc((right + 1) * sizeof(int));
	
	for(i = 0; i <= right; i++)
	{
		c[(Array[i] / razr) % 10]++;
	}

	for(i = 1; i < 10; i++)
	{
		c[i] += c[i - 1];
	}
	
	for(i = right; i >= 0; i--)
	{
		buffer[c[(Array[i] / razr) % 10] - 1] = Array[i];
		c[(Array[i] / razr) % 10]--;
	}
	
	for(i = 0; i <= right; i++)
	{
		Array[i] = buffer[i];
	}
	free(c);
	free(buffer);
	return;
}

void radix_sort(int *Array, int right)
{
	if(0 >= right)
		return;

	int max = getmax(Array, right), razr;
	for(razr = 1; max / razr > 0; razr *= 10)
	{
		sort(Array, right, razr);
	}
}


int main()
{
	int  *a = getArrayFromRand(zsize);
	
	printArrayToCons(a, zsize);
	radix_sort(a, zsize - 1);
	printArrayToCons(a, zsize);
	return 0;
}