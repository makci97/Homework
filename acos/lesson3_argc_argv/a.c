#include<stdio.h>

main(int argc, char **argv)
{
    printf("%i\n",argc);
    int i;
    for(i = 0; i < argc; ++i)
    {
        puts(argv[i]);
    }
    return;
}
