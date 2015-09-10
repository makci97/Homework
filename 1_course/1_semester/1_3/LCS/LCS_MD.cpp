#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

 
void lcs_length(char* a, char* b, char* result) {
	int len_a = strlen(a), len_b = strlen(b), i, j;
    char** map = (char**) malloc((len_a + 1) * sizeof(char*));
   
    for (i = 0; i <= len_a; i++) {
        map[i] = (char*) malloc((len_b + 1) * sizeof(char));
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
	result[k] = '\0';
	
	result = (char*) realloc(result, (k + 1) * sizeof(char));
    for (i = 0; i <= len_a; i++) {
        free(map[i]);
    }
    free(map);
   
}
 
int main() {	
	char *a = (char*) malloc(sizeof(char)), *b = (char*) malloc(sizeof(char));
	a = get_string(a);
	b = get_string(b);
    char* res = (char*) malloc(((strlen(a) > strlen(b)) ? strlen(a) : strlen(b)) * sizeof(char));
   
    lcs_length(a, b, res);
   
    puts(res);
   
    free(a);
    free(b);
    free(res);
   
    return 0;
}