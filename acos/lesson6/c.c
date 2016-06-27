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
	
	int pid, status;
			
	int pid2;
	pid2 = fork();
	if(pid2 == 0)
	{
		dup2(pd[0],0);
		close(pd[0]);
		close(pd[1]);
		
		pid = fork();
		if(pid == 0)
		{
			dup2(pd[1],1);
			close(pd[0]);
			close(pd[1]);
			execlp("ls", "ls", NULL);
			perror("ls");
			exit(-1);
		}
		wait(&status);
		wait(&status);
		execlp("wc", "wc", NULL);
		perror("wc");
		exit(-1);
	}
	
	
	for(int i = 1; i < argc; ++i)
	{
		wait(&status);
	}
	return 0;
}
