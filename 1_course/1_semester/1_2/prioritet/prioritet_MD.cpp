#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int const zsize = 10;

struct pri
{
	int value;
	int key;
};

void prisvoit(pri &a, const pri &b)
{
	a.key = b.key;
	a.value = b.value;
}

void swap_pri(pri &a, pri &b)
{
	pri buf;
	prisvoit(buf, a);
	prisvoit(a, b);
	prisvoit(b, buf);
	return;
}

pri* getArrayFromRand(int size) // генерация масива размера size c рандомными значениями
{
    int i;
	pri *massiv = (pri*) calloc(size + 1, sizeof(pri));

	srand(time(NULL));
    for(i = 0; i <= size; i++)
    {
		massiv[i].key = rand() % 100;
		massiv[i].value = rand() % 100;
    }
    return massiv;
}


void printHeapToCons(const pri* Array, int size) // вывод в консоль массива
{
    int i;
    for(i = 1; i <= size; i++)
    {
		printf("%i(%i)\t", Array[i].value, Array[i].key);
    }
	printf("\n");
	return;
}

void printArrayToCons(const pri* Array, int size) // вывод в консоль массива
{
    int i;
    for(i = 0; i < size; i++)
    {
		printf("%i(%i)\t", Array[i].value, Array[i].key);
    }
	printf("\n");
	return;
}


void heapify(pri *Array, int el, int size)
{
	if((2 * el + 1 > size && 2 * el == size && Array[el].key >= Array[2 * el].key) || (Array[el].key >= Array[2 * el].key && Array[el].key >= Array[2 * el + 1].key && 2 * el + 1 <= size) || 2 * el > size)
		return;

	if(el * 2 + 1 <= size)
	{
		if(Array[2 * el].key > Array[2 * el + 1].key)
		{
			swap_pri(Array[el], Array[2 * el]);
			heapify(Array, 2 * el, size);
		}
		else
		{
			swap_pri(Array[el], Array[2 * el + 1]);
			heapify(Array, 2 * el + 1, size);
		}
	}
	else
		swap_pri(Array[el], Array[2 * el]);
}

void create_binary_heap(pri *Array, int size)
{
	int i = (size + 1) / 2;
	while(i > 0)
		heapify(Array, i--, size);
}

pri max_elem_binary_heap(pri *Array, int &size)
{
	if(size < 1)
	{
		pri buf;
		buf.key = -1;
		buf.value = -1;
		return buf;
	}

	pri max;

	prisvoit(max, Array[1]);
	prisvoit(Array[1], Array[size--]);

	heapify(Array, 1, size);

	return max;
}


void heapify_up(pri *Array, int el, int size)
{
	if(el == 1 || Array[el / 2].key >= Array[el].key)
		return;

	int sw;
	if(el % 2 == 1)
		if(Array[el].key < Array[el - 1].key)
			sw = el - 1;
		else
			sw = el;
	else
		if(el == size)
			sw = el;
		else if(Array[el].key < Array[el - 1].key)
			sw = el - 1;
		else
			sw = el;

	swap_pri(Array[sw], Array[el / 2]);
	heapify_up(Array, el / 2, size);
	heapify(Array, sw, size);
}


void put_new_elem_binary_heap(pri *Array, int &size, pri new_elem)
{
	Array = (pri*) realloc(Array, (++size + 1) * sizeof(pri));
	prisvoit(Array[size], new_elem);

	heapify_up(Array, size, size);
	return;
}

pri *priority_sort(int size)
{
	int i = 0;
	pri buf, *Array = getArrayFromRand(size + 1), *a = (pri*) malloc(size * sizeof(pri));

	
	printf("Initial array:\n");
	printHeapToCons(Array, size);
	
	create_binary_heap(Array, size);
	printf("Heap:\n");
	printHeapToCons(Array, size);

	while( (buf = max_elem_binary_heap(Array, size)).key != -1)
		prisvoit(a[i++], buf);

	free(Array);
	return a;
}

int main()
{
	pri *a = priority_sort(zsize);
	
	printf("Final array:\n");
	printArrayToCons(a, zsize);

	return 0;
}
