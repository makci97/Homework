#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Athlete
{
	int weight;
	int strength;
};



void printArrayToCons(const Athlete* a, int size) // вывод в консоль массива
{
    int i;

    for(i = 0; i < size; i++)
    {
			printf("\n%i) weight = %i; strength = %i;", i + 1, a[i].weight, a[i].strength);
    }
	printf("\n");
}

void mean(Athlete *a, Athlete *b)
{
	a -> weight = b -> weight;
	a -> strength = b -> strength;
}

void swap(Athlete *a, Athlete *b)
{
	Athlete buf;
	mean(&buf, a);
	mean(a, b);
	mean(b, &buf);
}

void insertion_sort(Athlete *Array, int left, int right)
{
	int i, j;

	for(i = left; i < right; i++)
	{
		for(j = i + 1; Array[j - 1].strength > Array[j].strength && j > left; j--)
		{
			swap(&Array[j - 1], &Array[j]);
		}
	}
}


void q_sort(Athlete* Array, int first, int last)
{
	if(first >= last)
		return;
	srand(time(NULL));
    int i = first, j = last, x = Array[(rand() % (last - first + 1)) + first].strength;

    do {
        while (Array[i].strength < x)
			i++;
        while (Array[j].strength > x)
			j--;

        if(i <= j) {
            if (Array[i].strength > Array[j].strength)
				swap(&Array[i], &Array[j]);
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


Athlete *Athlete_tower(Athlete *a, int &size)
{
	Athlete *tower = (Athlete*) malloc(size * sizeof(Athlete));
	int i, sum_weight = 0, amount_athlete = 0;

	mean(tower, a);

	for(i = 0; i < size; i++)
	{
		if(a[i].strength >= sum_weight)
		{
			sum_weight += a[i].weight;
			mean(&tower[amount_athlete], &a[i]);
			amount_athlete++;
		}
	}

	tower = (Athlete*) realloc(tower, amount_athlete * sizeof(Athlete));

	size = amount_athlete;
	return tower;
}


int main()
{
	int i, size;
	FILE *f = fopen("input.txt", "r");
	fscanf(f,"%i", &size);
	Athlete *tower, *a = (Athlete*) malloc(size * sizeof(Athlete));

	for(i = 0; i < size; i++)
	{
		fscanf(f, "%i%i", &a[i].weight, &a[i].strength);
	}

	printArrayToCons(a, size);

	q_sort(a, 0, size - 1);

	printArrayToCons(a, size);

	tower = Athlete_tower(a, size);
	
	printf("\nTower:\n");
	printArrayToCons(tower, size);
	return 0;
}