#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *get_string(char *str)
{
	int i;
	char c = 0;
	for(i = 0; (c = getchar()) != '\n'; i++)
	{
		str = (char*) realloc(str, sizeof(char) * (i + 2));
		str[i] = c;
		str[i+1] = '\0';
	}
	return str;
}

int min(int a, int b, int c)
{
	if(a < b)
	{
		if(a < c)
			return a;
		else
			return c;
	}
	else
	{
		if(b < c)
			return b;
		else
			return c;
	}
}

int levinshtein_len(char *s1, char *s2, int l1, int l2)
{
	if(l1 == 0 && l2 == 0)
		return 0;
	else if(l2 == 0 && l1 > 0)
		return l1;
	else if(l1 == 0 && l2 > 0)
		return l2;
	else
	{
		int m = 1;
		if(s1[l1 - 1] == s2[l2 - 1])
			m = 0;

		return min(levinshtein_len(s1,s2, l1, l2 - 1) + 1, levinshtein_len(s1,s2, l1 - 1, l2) + 1, levinshtein_len(s1,s2, l1 - 1, l2 - 1) + m);
	}
}

int levinshtein(char *s1, char *s2)
{
	int l1 = strlen(s1), l2 = strlen(s2);

	return levinshtein_len(s1, s2, l1, l2);
}


int main()
{
	char *s1 = (char*) malloc(sizeof(char)), *s2 = (char*) malloc(sizeof(char));
	s1 = get_string(s1);
	s2 = get_string(s2);
	printf("%i", levinshtein(s1, s2));
	free(s1);
	free(s2);
	return 0;
}