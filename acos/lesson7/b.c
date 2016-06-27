#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void wc(char* str, int* pd)
{
	int pid;
	pid = fork();
	if(pid == 0)
	{
		dup2(pd[0],0);
		close(pd[0]);
		close(pd[1]);
		execlp(str, str, NULL);
		perror(str);
		exit(-1);
	}
}

void ls(char* str, int* pd)
{
	int pid;
	pid = fork();
	if(pid == 0)
	{
		dup2(pd[1],1);
		close(pd[0]);
		close(pd[1]);
		execlp(str, str, NULL);
		perror(str);
		exit(-1);
	}
}

int main(int argc, char** argv)
{
	int pd[2];
	if (pipe(pd) == -1) 
	{
	   perror("pipe");
	   exit(EXIT_FAILURE);
	}

	for(int i = 1; i < argc; ++i)
	{
		if(argv[i] == "ls")
			ls(argv[i], pd);
		else if(argv[i] == "wc")
			wc(argv[i], pd);
	}
	
	int status;
	for(int i = 1; i < argc; ++i)
	{
		wait(&status);
	}
	return 0;
}
