#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int const n = 15;

struct section
{
	int s;
	int f;
};


section* getArrayFromRand(int size) // генерация масива размера size c рандомными значениями
{
    int i , s, f;
	section * massiv = (section*) calloc(size, sizeof(section));
	srand(time(NULL));
    for(i=0;i<size;i++)
    {
		do
		{
			s = rand() % 10000;
			f = rand() % 10000;
		}
		while(s >= f);
		massiv[i].s = s;
		massiv[i].f = f;
    }
    return (&massiv[0]);
}


void printArrayToCons(const section* a, int size) // вывод в консоль массива
{
    int i;

    for(i = 0; i < size; i++)
    {
			printf("\n%i) [%i; %i];", i + 1, a[i].s, a[i].f);
    }
	printf("\n");

	return;
}

void swap(section *a, section *b)
{
	section buf;
	buf.s = a -> s;
	buf.f = a -> f;
	a -> s = b -> s;
	a -> f = b -> f;
	b -> s = buf.s;
	b -> f = buf.f;
	return;
}

void insertion_sort(section *Array, int left, int right)
{
	int i, j;

	for(i = left; i < right; i++)
	{
		for(j = i + 1; Array[j - 1].s > Array[j].s && j > left; j--)
		{
			swap(&Array[j - 1], &Array[j]);
		}
	}
}

int partition(section *Array, int left, int right)
{
	int i = left - 1 , j = right, v = Array[right].s;
	while(1)
	{
		while(Array[++i].s < v);
		while(Array[--j].s > v)
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

void q_sort(section *Array, int left, int right)
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
	int i, time = 0 , max;
	section *a = getArrayFromRand(n);
	printArrayToCons(a, n);

	q_sort(a, 0, n - 1);

	printArrayToCons(a, n);

	i = 0;
	printf("*********************");
	while(i < n)
	{
		max = i;
		while(i + 1 < n && a[i + 1].s <= time)
		{
			if(a[i].f > a[max].f)
				max = i;

			i++;
		}
		
		time = a[max].f;
		printf("\n%i) [%i; %i];", max + 1, a[max].s, a[max].f);

		i++;
	}
	return 0;
}