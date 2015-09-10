#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int const zsize = 20;

int* getArrayFromRand(int size) // генерация масива размера size c рандомными значениями
{
    int i, buf, * massiv = (int*) calloc(size, sizeof(int));
	srand(time(NULL));
    for(i = 0; i < size; i++)
    {
        massiv[i] = rand() % 100;
    }

    return massiv;
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


void insertion_sort(int *Array, int first, int last)
{
	int i, j;

	for(i = first; i < last; i++)
	{
		for(j = i + 1; Array[j - 1] > Array[j] && j > first; j--)
		{
			swap(Array[j - 1], Array[j]);
		}
	}
}


void q_sort(int* Array, int first, int last)
{
	if(first >= last)
		return;
	srand(time(NULL));
    int i = first, j = last, x = Array[(rand() % (last - first + 1)) + first];

    do {
        while (Array[i] < x)
			i++;
        while (Array[j] > x)
			j--;

        if(i <= j) {
            if (Array[i] > Array[j])
				swap(Array[i], Array[j]);
            i++;
            j--;
        }
    } while (i <= j);

    if (i < last)
	{
		if(last - i < 8)
			insertion_sort(Array, i, last);
		else
			q_sort(Array, i, last);
	}
    if (first < j)
	{
		if(j - first < 8)
			insertion_sort(Array, first, j);
		else
			q_sort(Array, first, j);
	}
}

int main()
{
	int *a = getArrayFromRand(zsize);
	printArrayToCons(a, zsize);
	q_sort(a, 0, zsize - 1);
	printArrayToCons(a, zsize);
	free(a);
	return 0;
}
