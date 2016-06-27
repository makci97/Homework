#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void func(char* str)
{
	int pid;
	pid = fork();
	if(pid == 0)
	{
		execlp(str, str, NULL);
		perror(str);
		exit(-1);
	}
}

int main(int argc, char** argv)
{
	for(int i = 1; i < argc; ++i)
	{
		func(argv[i]);
	}
	
	int status;
	for(int i = 1; i < argc; ++i)
	{
		wait(&status);
	}
	return 0;
}
