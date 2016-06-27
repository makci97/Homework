#include <pthread.h>
#include <stdio.h>
int a = 0;
int threadsNum=10;
pthread_mutex_t mutex;

void * f( void *p ) {
    int k = *(int *)p;
    int temp = 0;
    int s=0;
    for( int i = k ; i < 200000000; i += threadsNum )
    {
		++s;
    }
		pthread_mutex_lock(&mutex);
        a += s; 
		pthread_mutex_unlock(&mutex);
    
    return NULL;
}

int main(int argc, const char * argv[]) {
	pthread_mutex_init(&mutex, NULL);
	
	if(argc >= 1)
	{
		threadsNum = atoi(argv[1]);
	}
    int *k = (int*)malloc(threadsNum * sizeof(int));
    for( int i = 0; i < threadsNum; ++i ) {
        k[i] = i;
    }
    
    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * threadsNum);
    for( int i = 0; i < threadsNum; ++i ) {

        pthread_create(threads + i, NULL, f, k + i);
    }
        
    void *p;
    for( int i = 0; i < threadsNum; ++i ) {
        pthread_join(threads[i], &p);
    }
    printf("%d\n", a);
    return 0;
}
