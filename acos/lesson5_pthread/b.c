#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	int pid;
	int k;
	if((pid=fork()) == 0)
	{
		execlp("ls", "ls", NULL);
		perror("ls");
		exit(1);
	}
	else
	{
		wait(&k);
	}
	return 0;
}
