#include <stdio.h>
#include <stdlib.h>


void print_bytes(const void *mem, int size)
{
    unsigned char *number = (unsigned char*) mem, **bytes = (unsigned char**) calloc(size , sizeof(unsigned char*));;

    
    for (int i = 0; i < size; i++, number++)
    {
		bytes[size - i - 1] = (unsigned char*) calloc(8, sizeof(char)); 
        unsigned char mask = ~0; // маска вида 10..000000
        mask = mask >> 1;
        mask = ~mask;
        
		int j = 0;
        while (mask)
        {
            if (*number & mask)
                bytes[size - i - 1][j++] = '1';
            else
                bytes[size - i - 1][j++] = '0';
            
            mask >>= 1;
        }
    }
    
	for(int i = 0; i < size; i++)
	{
		for(int j = 0; j < 8; j++)
			printf("%c", bytes[i][j]);
		printf(" ");
	}
    printf("\n");
}

int main()
{
    int a;

	scanf("%d", &a);
    
    print_bytes(&a, sizeof (a));
    
    return 0;
}