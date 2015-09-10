#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int const n = 5;

struct application
{
	int s;
	int f;
};


application* getArrayFromRand(int size) // генерация масива размера size c рандомными значениями
{
    int i , s, f;
	application * massiv = (application*) calloc(size, sizeof(application));
	srand(time(NULL));
    for(i=0;i<size;i++)
    {
		do
		{
			s = rand() % 100;
			f = rand() % 100;
		}
		while(s >= f);
		massiv[i].s = s;
		massiv[i].f = f;
    }
    return (&massiv[0]);
}


void printArrayToCons(const application* a, int size) // вывод в консоль массива
{
    int i;

    for(i = 0; i < size; i++)
    {
			printf("\n%i) [%i; %i);", i + 1, a[i].s, a[i].f);
    }
	printf("\n");

	return;
}

void swap(application *a, application *b)
{
	application buf;
	buf.s = a -> s;
	buf.f = a -> f;
	a -> s = b -> s;
	a -> f = b -> f;
	b -> s = buf.s;
	b -> f = buf.f;
	return;
}

void insertion_sort(application *Array, int left, int right)
{
	int i, j;

	for(i = left; i < right; i++)
	{
		for(j = i + 1; Array[j - 1].f > Array[j].f && j > left; j--)
		{
			swap(&Array[j - 1], &Array[j]);
		}
	}
}

int partition(application *Array, int left, int right)
{
	int i = left - 1 , j = right, v = Array[right].f;
	while(1)
	{
		while(Array[++i].f < v);
		while(Array[--j].f > v)
		{
			if(j == left)
				break;
		}
		if(i >= j)
			break;
		swap(&Array[i], &Array[j]);
	}
	swap(&Array[right], &Array[i]);
	return i;
}

void q_sort(application *Array, int left, int right)
{
	if(left >= right)
		return;
	if(right - left <= 8)
		insertion_sort(Array, left, right);

	int i = partition(Array, left, right);
	q_sort(Array, left, i - 1);
	q_sort(Array, i, right);
}

int main()
{
	int i, time = 0;
	application *a = getArrayFromRand(n);
	printArrayToCons(a, n);

	q_sort(a, 0, n - 1);

	printArrayToCons(a, n);

	i = 0;
	printf("*********************");
	while(i < n)
	{
		if(a[i].s >= time)
		{
			printf("\n%i) [%i; %i);", i + 1, a[i].s, a[i].f);
			time = a[i].f;
		}
		i++;
	}
	return 0;
}