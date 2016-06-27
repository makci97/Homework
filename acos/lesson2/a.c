#include<stdlib.h>

int main()
{
	char a;
char *str = malloc(sizeof(char));
int length = 1;
while((a=getchar())!=EOF){
str = realloc(str,length+1);
str[length-1]=a;
++length;
}
str[length-1]='\0';
puts(str);
	return 0;
}
