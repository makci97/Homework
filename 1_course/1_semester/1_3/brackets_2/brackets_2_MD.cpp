#include <stdio.h>
#include <stdlib.h>

int brackets_2(int n)
{
	if(n == 1)
		return 1;
	return ( (2 * (2 * n + 1) * brackets_2(n - 1)) / (n + 2) );
}

int main()
{
	int n;
	scanf("%i", &n);
	printf("%i",brackets_2(n));

	return 0;
}