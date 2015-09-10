#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

const int size = 10;


int* getArrayFromRand(int size) // генерация масива размера size c рандомными значениями
{
    int i,* massiv = (int*) calloc(size,sizeof(int));
	srand(time(NULL));
    for(i=0;i<size;i++)
    {
        massiv[i] = rand() % 20;
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

 
void lcs_length(int* a, int* b, int len_a, int len_b, int* result, int &size_res) {
	int i, j;
    int** map = (int**) malloc((len_a + 1) * sizeof(int*));
   
    for (i = 0; i <= len_a; i++) {
        map[i] = (int*) malloc((len_b + 1) * sizeof(int));
    }
   
    for (i = len_a - 1; i >= 0; i--) {
        for (j = len_b - 1; j >= 0; j--) {
           
            if (a[i] == '\0' || b[j] == '\0') 
				map[i][j] = 0;
            else if (a[i] == b[j]) 
				map[i][j] = map[i+1][j+1] + 1;
            else 
				map[i][j] = ((map[i+1][j] > map[i][j+1]) ? map[i+1][j] : map[i][j+1]);
           
        }
    }
   
    int k = 0;
	i = 0;
	j = 0;
    while (i < len_a && j < len_b) {
        if (a[i] == b[j]) {
            result[k++] = a[i];
            i++;
            j++;
        }
        else if (map[i+1][j] >= map[i][j+1]) 
			i++;
        else 
			j++;
    }
	size_res = k;
	
	result = (int*) realloc(result, (k + 1) * sizeof(int));
    for (i = 0; i <= len_a; i++) {
        free(map[i]);
    }
    free(map);
   
}

void copy_array(int *a, int *b, int size)
{
	for(int i = 0; i < size; i++)
	{
		a[i] = b[i];
	}
}


void swap(int &a, int &b)
{
	int buffer = a;
	a = b;
	b = buffer;
	return;
}


void sort(int* Array, int &last)
{
    for(int i = 0; i < last; i++)
	{

		for(int j = last - 1; j > i; j--)
		{

			if(Array[j] < Array[j - 1])
				swap(Array[j], Array[j - 1]);

			if(Array[j] == Array[j - 1])
			{
				swap(Array[j], Array[last - 1]);
				last--;
			}

		}

	}
}

int main()
{
	int size_1 = size, size_2 = size, size_res = size, *a = getArrayFromRand(size), *a_sort = (int*) malloc(size * sizeof(int)), *result = (int*) malloc(size_res * sizeof(int));
	copy_array(a_sort, a, size);
	printArrayToCons(a, size_1);
	sort(a_sort, size_2);
	printArrayToCons(a_sort, size_2);

	printf("\n*****************\n");

	lcs_length(a, a_sort, size_1, size_2, result, size_res);
	printArrayToCons(result, size_res);	

	free(a);
    free(a_sort);
    free(result);
	return 0;
}