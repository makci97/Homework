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

int main()
{
    char buf[BUF_SIZE + 1];

    //create connection
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        error("Socket\n");
    }
    //puts("Socket is create\n");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_ADDR);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        error("Connect\n");
    }
    //puts("Conection is create\n");

    //read order
    int read_bytes = 0;

    while(read_bytes < BUF_SIZE && read(0, buf + read_bytes, 1) > 0 && buf[read_bytes] != '\n' && buf[read_bytes] != '\0')
    {
        //printf("%c", buf[read_bytes]);
        ++read_bytes;
    }
    if(buf[read_bytes] != '\n')
    {
        error("Could not read\n");
    }
    buf[read_bytes] = '\0';
    //puts("\nRead order:\n");
    //puts(buf);
    //puts("\nend read\n");

    //send order
    int written_bytes = 0;
    while(written_bytes < read_bytes + 1 && send(sock, buf + written_bytes, 1, 0) > 0)
    {
        if(buf[written_bytes] == '\n' || buf[written_bytes] == '\0')
        {
            //puts("\nBreak\n");
            break;
        }
        //printf("%c", buf[written_bytes]);
        ++written_bytes;
    }
    if(written_bytes != read_bytes)
    {
        error("Could not send\n");
    }
    //puts("\nSend order\n");

    //read answer and write it in stdout
    int output_descriptor = 1;

    while( (read_bytes = recv(sock, buf, BUF_SIZE, 0)) > 0)
    {
        buf[read_bytes] = '\0';
        written_bytes = write(output_descriptor, buf, read_bytes);

        if(written_bytes != read_bytes)
        {
            error("Could not write\n");
        }
    }

    if(read_bytes < 0)
    {
        error("Could not read\n");
    }
    close(sock);
    //puts("Read answer\n");

    return 0;
}
