#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct helper
{
   int **ptr_numbers, *sqrt_N, *N, number_of_thread, amount_of_thread;
   char *work, *end_check;
};

struct helper* copy(struct helper* obj)
{
    struct helper *new_helper = (struct helper*)malloc(sizeof(struct helper));

    if(new_helper == NULL)
        return NULL;

    new_helper->amount_of_thread = obj->amount_of_thread;
    new_helper->N = obj->N;
    new_helper->ptr_numbers = obj->ptr_numbers;
    new_helper->sqrt_N = obj->sqrt_N;
    new_helper->work = obj->work;

    return new_helper;
}

void * thread_func(void *argg)
{
    struct helper* arg = (struct helper*)argg;

    while(*(arg -> work))
    {
        while(!*(arg -> end_check))
        {
            int delta = ceil((float)(*(arg -> sqrt_N))*(*(arg -> sqrt_N) - 1)/arg -> amount_of_thread);
            int first = *(arg -> sqrt_N) + delta * arg -> number_of_thread + 1;
            int second = *(arg -> sqrt_N) + delta * (arg -> number_of_thread + 1);

            if(first > *(arg -> N))
            {
                //printf("\n*I=%i, end_check = %i, first = %i, second = %i, delta = %i\n", arg->number_of_thread, (int)(*(arg->end_check) == 1), first, second, delta);
                *(arg -> end_check) = 1;
                break;
            }
            else if(second > *(arg->sqrt_N)* (*(arg->sqrt_N)))
                second = *(arg->sqrt_N)* (*(arg->sqrt_N));
            else if(second > *(arg -> N))
                second = *(arg -> N);

            //check numbers fewer than min_sqrt^2 in main thread
            for(int p = 2; p <= *(arg -> sqrt_N); ++p)
            {
                if((*(arg->ptr_numbers))[p] == 0)
                {
                    for(int n = first; n <= second; ++n)
                    {
                        if(n % p == 0)
                        {
                            (*(arg->ptr_numbers))[n] = 1;
                        }
                    }
                }
            }

            //printf("\nI=%i, end_check = %i, first = %i, second = %i, delta = %i\n", arg->number_of_thread, (int)(*(arg->end_check) == 1), first, second, delta);
            *(arg -> end_check) = 1;
        }
    }

    free(arg);
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        perror("Few arguments\n");
        exit(-1);
    }
    int amount_of_threads = atoi(argv[1]), N = 64;
    char infinity_check = 0;

    if(argc == 3 && atoi(argv[2]) >= 0)
    {
        N = atoi(argv[2]);
        if(N < 2)
            return 0;
    }
    else
        infinity_check = 1;

    int *numbers = (int*)calloc(N + 1, sizeof(int));
    if(numbers == NULL)
    {
        perror("Failed to allocate memory for numbers\n");
        exit(-1);
    }

    int min_sqrt = (int)fmax(fmin(floor((float)(sqrt(N))), 8), 2);

    //check numbers fewer than min_sqrt in main thread
    for(int p = 2; p <= ceil(sqrt(min_sqrt)); ++p)
    {
        if(numbers[p] == 0)
        {
            for(int n = p * 2; n <= min_sqrt; ++n)
            {
                if(n % p == 0)
                {
                    numbers[n] = 1;
                }
            }
        }
    }

    //print found prime numbers
    for(int n = 2; n <= min_sqrt; ++n)
    {
        if(numbers[n] == 0)
        {
            printf("%i ", n);
        }
    }

    //create threads
    pthread_t *threads = (pthread_t*)malloc((amount_of_threads + 1) * sizeof(pthread_t));
    if(threads == NULL)
    {
        perror("Bad alloc(pthread_t)\n");
        exit(-1);
    }

    char work = 1;  // if =1, that threads working
                    // else if =0, that threads will killed

    char *barier = (char*)calloc(amount_of_threads + 1, sizeof(char));
                    // help in synchronization
    if(barier == NULL)
    {
        perror("Bad alloc(barier)\n");
        exit(-1);
    }

    struct helper *buf = (struct helper*)malloc(sizeof(struct helper));
    if(buf == NULL)
    {
        perror("Bad alloc(helper)\n");
        exit(-1);
    }
    buf->amount_of_thread = amount_of_threads;
    buf->ptr_numbers = &numbers;
    buf->N = &N;
    buf->sqrt_N = &min_sqrt;
    buf->work = &work;

    if(infinity_check)
    {
        N = min_sqrt * min_sqrt;
    }

    for(int i = 0; i < amount_of_threads; ++i)
    {
        int result;

        struct helper *this_helper = copy(buf);
        this_helper->end_check = &barier[i];
        this_helper->number_of_thread = i;

        result = pthread_create(&threads[i], NULL, thread_func, this_helper);

        if(result != 0)
        {
            perror("Create thread\n");
            exit(-1);
        }
    }

    if(min_sqrt < N)
    {
        do
        {
            //printf("\nwork = %i, min_sqrt = %i, N = %i\n",(int)(work == 1), min_sqrt, N);

            for(int i = 0; i < amount_of_threads; ++i)
            {
                while(!barier[i]) { }
            }

            //print found prime numbers
            int last = (infinity_check == 0)?fmin(min_sqrt * min_sqrt, N): min_sqrt * min_sqrt;
            for(int n = min_sqrt + 1; n <= last; ++n)
            {
                if(numbers[n] == 0)
                {
                    printf("%i ", n);
                }
            }

            if(infinity_check || min_sqrt * min_sqrt < N)
            {
                //printf("\ntrue\n");
                min_sqrt *= min_sqrt;

                if(infinity_check)
                {
                    N = min_sqrt * min_sqrt;
                    //printf("\nN = %i\n", N);
                    numbers = (int*)realloc(numbers, (N + 1)*sizeof(int));
                    if(numbers == NULL)
                    {
                        perror("Bad alloc(pthread_t)\n");
                        exit(-1);
                    }

                    for(int i = sqrt(N) + 1; i <= N; ++i)
                    {
                        numbers[i] = 0;
                    }
                }

                for(int i = 0; i < amount_of_threads; ++i)
                {
                    barier[i] = 0;
                }
            }
            else
            {
                work = 0;
            }
        }
        while(infinity_check || min_sqrt < N && work);
    }
    else
    {
        work = 0;
    }

    for(int i = 0; i < amount_of_threads; ++i)
    {
        int result;

        result = pthread_join(threads[i], NULL);
        if (result != 0)
        {
            perror("Wait thread");
            exit(-1);
        }
    }

    printf("\n");

    free(buf);
    free(barier);
    free(numbers);

    return 0;
}
