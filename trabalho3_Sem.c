#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>      
#include <fcntl.h> 

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var   = PTHREAD_COND_INITIALIZER;

void *functionCount();
//void *functionCount2();
int count = 0;
int turn = 0;
int nThread[3] = {0, 1, 2};
sem_t thread_turn[3];

#define COUNT_MAX  10
#define COUNT_HALT1  3
#define COUNT_HALT2  6
#define NEXT (id + 1) % 3

void main()
{
   pthread_t thread[3];

   for (int i = 0; i < 3; i++)
    {
        pthread_create(&thread[i], NULL, functionCount, &nThread[i]);
    }

   for (int i = 0; i < 3; i++)
        pthread_join(thread[i], NULL);

   printf("Final count: %d\n",count);

   exit(EXIT_SUCCESS);
}

void turn_check(int id){
    if(id == turn){
        return;
    }else
    {
        sem_wait(&thread_turn[id]);
        return;   
    }
    
}

void count_func(int id){
    if(count >= COUNT_MAX){
        sem_post(&thread_turn[NEXT]);
        return;
    }
    count++;
    printf("turn count: %d", id);
    sem_post(&thread_turn[NEXT]);
    return;
}

void *functionCount(void *data)
{
   for(;;)
   {
        int *id = data;
        // Lock mutex and then wait for signal to relase mutex

        turn_check(*id);
        pthread_mutex_lock( &count_mutex );
        
        count_func(*id);

        printf("Counter value functionCount: %d\n",count);

        pthread_mutex_unlock( &count_mutex );

        if(count >= COUNT_MAX) return(NULL);
    }
}

// Write numbers 4-7

/*void *functionCount2()
{
unsigned long i;
    for(;;)
    {
       pthread_mutex_lock( &count_mutex );

       if( count < COUNT_HALT1 || count > COUNT_HALT2 )
       {
          // Condition of if statement has been met. 
          // Signal to free waiting thread by freeing the mutex.
          // Note: functionCount1() is now permitted to modify "count".
	  pthread_mutex_unlock( &count_mutex );
          pthread_cond_signal( &condition_var );
       }
       else
       {
          count++;
          printf("Counter value functionCount2: %d\n",count);
          pthread_mutex_unlock( &count_mutex );
       }
       
       if(count >= COUNT_MAX) return(NULL);
    }

}*/