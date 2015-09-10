#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int const size = 10;

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

int *merge_sort(int *a, int *buf, int l, int r)
{
	if (l == r)
    {
        buf[l] = a[l];
        return buf;
    }

	int mid = (int)((l + r) * 0.5);
	int *l_buf = merge_sort(a, buf, l, mid);
    int *r_buf = merge_sort(a, buf, mid + 1, r);

    int *target = (l_buf == a) ? buf : a;
	 printf(" \nWHY? %i r = %i\t l = %i\n", target, r_buf, l_buf);
    int l_cur = l, r_cur = mid + 1;
    for (int i = l; i <= r; i++)
	{
		if (l_cur <= mid && r_cur <= r)
		{
			if (l_buf[l_cur] < r_buf[r_cur])
			{
				target[i] = l_buf[l_cur];
				l_cur++;
			}
			else
			{
				target[i] = r_buf[r_cur];
				r_cur++;
			}
		}
		else if (l_cur <= mid)
		{
			target[i] = l_buf[l_cur];
			l_cur++;
		}
		else
		{
			target[i] = r_buf[r_cur];
			r_cur++;
		}
	}

	return target;
}


int main()
{
	int *buf = (int*) malloc(size * sizeof(int)), *a = getArrayFromRand(size);
	printArrayToCons(a, size);
	a = merge_sort(a, buf, 0, size - 1);
	printArrayToCons(a, size);
	getchar();
	return 0;
}