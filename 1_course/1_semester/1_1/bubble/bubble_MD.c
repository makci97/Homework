#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* getArrayFromRand(int size) // генерация масива размера size c рандомными значениями
{
    int i,* massiv = (int*) calloc(size,sizeof(int));
    for(i=0;i<size;i++)
    {
        massiv[i] = rand();
    }
    return (&massiv[0]);
}


int* getArrayFromCons(int *size) // считывание массива с клавиатуры
{
    char c, *str = (char*) malloc(0);
    int i, sp = 0, j, k = 0, *arr;
    for(i = 0; (c = getchar()) != '\n'; i++)
    {
        str = (char*) realloc(str, i+1);
        str[i] = c;
    }
    while(str[i] == ' ')
    {
        i--;
    }
    str = (char*) realloc(str, i+2);
    str[i+1] = '\0';
    for(j = 1; str[j] != '\0'; j++)
    {
        if(str[j] == ' ' && str[j-1] != ' ')
        {
            sp++;
        }
    }

    arr = (int*) calloc(sp+1,sizeof(int));
    for(j = 0; str[j] != '\0'; j++)
    {
        if(str[j] >= '0' && str[j] <= '9')
        {
            arr[k] *= 10;
            arr[k] += (str[j] - '0');
        }
        if(str[j] == ' ' && str[j-1] != ' ' && j != 0)
        {
            k++;
        }
    }
    *size = sp + 1;
    return arr;
}

int* getArrayFromFile(int* size, const char* filePath) // чтение массива из файла
{
    FILE *f = fopen(filePath, "r");
    char c, *str = (char*) malloc(0);
    int i, sp = 0, j, k = 0, *arr;
    for(i = 0; (c = fgetc(f)) != '\n'; i++)
    {
        str = (char*) realloc(str, i+1);
        str[i] = c;
    }
    while(str[i] == ' ')
    {
        i--;
    }
    str = (char*) realloc(str, i+2);
    str[i+1] = '\0';
    for(j = 1; str[j] != '\0'; j++)
    {
        if(str[j] == ' ' && str[j-1] != ' ')
        {
            sp++;
        }
    }

    arr = (int*) calloc(sp+1,sizeof(int));
    for(j = 0; str[j] != '\0'; j++)
    {
        if(str[j] >= '0' && str[j] <= '9')
        {
            arr[k] *= 10;
            arr[k] += (str[j] - '0');
        }
        if(str[j] == ' ' && str[j-1] != ' ' && j != 0)
        {
            k++;
        }
    }
    *size = sp + 1;
    fclose(f);
    return arr;
}

void printArrayToCons(const int* array, int size) // вывод в консоль массива
{
    int i;
    for(i = 0; i < size; i++)
    {
        printf("%i\t", array[i]);
    }
}


void printArrayToFile(const int* array, int size, const char* filePath) // запись в файл массива
{
    int i;
    FILE *f = fopen(filePath, "w");
    for(i = 0; i < size; i++)
    {
        fprintf(f,"%i\t", array[i]);
    }
    fprintf(f,"\n");
    fclose(f);
}


void printResultToCons(const int* array, int size) // вывод результатов в консоль
{
   printf("%s", array);
}


void printResultToFile(const int* array, int size, const char* filePath) // запись результатов в файл
{
    FILE *f = fopen(filePath, "w");
    fprintf(f,"%s", array);
    fclose(f);
}



int main(int argc, char *argv[])
{
    int size, i, jj, chislo, j = 0, * m;
    char in[20], out[20];
    for (i = 0; i < argc; ++i)
    {
        if (strncmp(argv[i], "-in=", 4) == 0)
        {
            i += 4;
            j = 0;
            while(strncmp(argv[i], "-out=", 5) != 0 && argv[i] != 0 && argv[i] != 13)
            {
                in[j] = argv[i];
                j++;
            }
            if(strncmp(argv[i], "console", 7) == 0)
                m = getArrayFromCons(&size);
            else
            {
                if(j != 0)
                    chislo = 1;
                for(jj = 0; jj <= j; jj++)
                {
                    if(in[jj] < '0' && in[jj] > '9')
                    {
                        chislo = 0;
                        break;
                    }
                }
                if(chislo == 1)
                {
                    size = atoi(in);
                    m = getArrayFromRand(size);
                }
                else
                {
                    m = getArrayFromFile(&size, &in);
                }

            }
        }
        else if (strncmp(argv[i], "-out=", 5) == 0)
        {
            i += 5;
            j = 0;
            while(argv[i] != 0 && argv[i] != 13)
            {
                out[j] = argv[i];
                j++;
            }
            if(strncmp(argv[i], "console", 7) == 0)
                printResultToCons(m,size);
            else
            {
                printResultToFile(m,size,&out);
            }
        }
    }

    free(m);
    return 0;
}
