#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void close_descs(int file_descs[2][2])
{
    if(file_descs[0][0] != 0 && file_descs[0][0] != -1)
    {
        close(file_descs[0][0]);
    }
    if(file_descs[1][0] != 0 && file_descs[1][0] != -1)
    {
        close(file_descs[1][0]);
    }
    if(file_descs[0][1] != 1 && file_descs[0][1] != -1)
    {
        close(file_descs[0][1]);
    }
    if(file_descs[1][1] != 1 && file_descs[1][1] != -1)
    {
        close(file_descs[1][1]);
    }
}

void child(int input, int output, const char *program, int file_descs[2][2])
{
    if(dup2(input, 0) == -1)
    {
        perror("dup2(input)\n");
        exit(-1);
    }

    if(dup2(output, 1) == -1)
    {
        perror("dup2(output)\n");
        exit(-1);
    }

    close_descs(file_descs);

    //runnig program
    execlp(program, program, NULL);
    perror(program);
    exit(-1);
}

int main(int argc, char **argv)
{
    //amount of arguments fewer than three
    if(argc < 3)
    {
        perror("few arguments\n");
        exit(-1);
    }


    int file_descs[2][2];
    int *stat;

    file_descs[0][0] = 0;
    file_descs[0][1] = -1;
    for(int i = 1; i < argc; ++i)
    {
        //if running last program
        if(i == argc - 1)
        {
            file_descs[1][0] = -1;
            file_descs[1][1] = 1;
        }
        //create pipe
        else if(pipe(file_descs[1]) == -1)
        {
            perror("pipe\n");
            exit(-1);
        }

        //create thread
        int q = fork();
        //if thread didn't create
        if(q < 0)
        {
            perror("fork\n");
            exit(-1);
        }
        //if this is child
        else if(q == 0)
        {
            child(file_descs[0][0], file_descs[1][1], argv[i], file_descs);
        }
        //if this is parent
        else
        {
            if(file_descs[0][0] != -1 && file_descs[0][0] != 0)
            {
                close(file_descs[0][0]);
            }
            if(file_descs[0][1] != -1 && file_descs[0][1] != 1)
            {
                close(file_descs[0][1]);
            }
            file_descs[0][0] = file_descs[1][0];
            file_descs[0][1] = file_descs[1][1];
        }
    }

    //waiting all threads
    for(int i = 1; i < argc; ++i)
    {
        wait(stat);
    }

    close_descs(file_descs);

    return 0;
}
