#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int const Mark = 4;

struct student
{
	int bilet;
	char fio[32];
	int bal;
};

void swap(student &a, student &b)
{
	student buf;
	buf.bal = a.bal;
	strcpy (buf.fio, a.fio);
	buf.bilet = a.bilet;
	a.bal = b.bal;
	strcpy (a.fio, b.fio);
	a.bilet = b.bilet;
	b.bal = buf.bal;
	strcpy (b.fio, buf.fio);
	b.bilet = buf.bilet;
	return;
}

void ravn(student &a, student &b)
{
	a.bal = b.bal;
	a.bilet = b.bilet;
	strcpy(a.fio, b.fio);
	return;
}

int main()
{
	int n, i, c[Mark] = {0};
	FILE *f = fopen("input.txt", "r");
	fscanf(f,"%i", &n);
	student* m = (student*) malloc(n * sizeof(student));
	student* buf = (student*) malloc(n * sizeof(student));
	
	for(i = 0; i < n; i++)
	{
		fscanf(f,"%i%s%i", &m[i].bilet, &m[i].fio, &m[i].bal);
		c[ m[i].bal - 2 ]++;
	}
	fclose(f);

	for(i = 1; i < 4; i++)
	{
		c[i] += c[i - 1];
	}
	
	for(i = n - 1; i >= 0; i--)
	{
		ravn(buf[ c[ m[i].bal - 2] - 1], m[i]); 
		c[ m[i].bal - 2]--;
	}


	f = fopen("output.txt", "w");

	for(i = 0; i < n; i++)
	{
		fprintf(f,"%i\t%s\t%i\n", buf[i].bilet, buf[i].fio, buf[i].bal);
	}
	fclose(f);

	free(m);
	return 0;
}