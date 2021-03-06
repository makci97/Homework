#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stddef.h>
#include <unistd.h>

const int BUF_SIZE = 1024;
const int PORT_ADDR = 8080;

void error(char *msg)
{
    perror(msg);
    exit(-1);
}

void do_it(char order[], int order_size, int sock)
{
    //parse order
    int amount_of_words = 1;
    for(int i = 0; i < order_size; ++i)
    {
        if(order[i] == ' ' && i != 0 && order[i - 1] != ' ')
        {
            ++amount_of_words;
        }

        if(order[i] == '\n' || order[i] == '\0')
        {
            break;
        }
    }
    if(order[order_size - 1] == ' ')
        --amount_of_words;

    char **str = (char**)malloc((amount_of_words + 1) * sizeof(char*));
    amount_of_words = 0;

    int begin = 0, end;
    for(int i = 0; i < order_size; ++i)
    {
        while(i < order_size && order[i] == ' ')
        {
            ++i;
            if(i >= order_size)
            {
                break;
            }
        }
        begin = i;

        while(i < order_size && order[i] != ' ')
        {
            ++i;
            if(i >= order_size)
            {
                break;
            }
        }
        if(i >= order_size)
        {
            end = order_size;
        }
        else
        {
            end = i;
        }

        char *buf = (char*)malloc( (end - begin + 1) * sizeof(char) );
        for(int j = begin; j < end; ++j)
        {
            buf[j - begin] = order[j];
        }
        buf[end - begin] = '\0';

        str[amount_of_words++] = buf;
    }
    str[amount_of_words] = NULL;
    //puts("Parse order\n");
/*
    printf("\nPrint order:\n");
    for(int i = 0; i < amount_of_words; ++i)
    {
        printf("\n%i) %s\n", i, str[i]);
    }
*/
    //do this order
    int pid;
    pid = fork();
    if(pid == 0)
    {
        //puts("Fork\n");
        dup2(sock, 1);
        //puts("Dup2\n");
        close(sock);
        //puts("Close sock\n");
        execvp(str[0], str);
        error(str[0]);
    }
}

int main()
{
    //create connection
    int sock, listener;
    struct sockaddr_in addr;
    char buf[BUF_SIZE];

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        error("Socket\n");
    }
    //puts("Create socket\n");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_ADDR);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        error("Bind\n");
    }
    //puts("Bind\n");

    listen(listener, 1);

    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            error("Accept\n");
        }
        //puts("Accept\n");

        //read order
        int bytes_read = 0;

        while(BUF_SIZE - bytes_read > 0 && recv(sock, buf + bytes_read, 1, 0) > 0)
        {
            if(buf[bytes_read] == '\n' || buf[bytes_read] == '\0')
            {
                //puts("\nBreak\n");
                break;
            }
            //puts(buf);
            ++bytes_read;
        }
        buf[bytes_read + 1] = '\0';
        //puts("\nRead order:\n");
        //puts(buf);
        //puts("\nend read\n");

        //puts("Now will do it\n");

        do_it(buf, bytes_read + 1, sock);

        //puts("Did it\n");
        close(sock);
    }

    return 0;
}
