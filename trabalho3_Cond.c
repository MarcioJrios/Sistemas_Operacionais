#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>      
#include <fcntl.h> 

pthread_mutex_t count_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var0  = PTHREAD_COND_INITIALIZER;
//pthread_cond_t  condition_var1   = PTHREAD_COND_INITIALIZER;
//pthread_cond_t  condition_var2   = PTHREAD_COND_INITIALIZER;



void *functionCount();
//void *functionCount2();
int count = 0; // contador que será incrementado
int turn = 0; // 
int nThread[3] = {0, 1, 2};

#define COUNT_MAX  10 //valor máximo do contador
#define COUNT_HALT1  3
#define COUNT_HALT2  6
#define NEXT (id + 1) % 3 //proxima thread

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

/*void cond_check(int id){
    if(id == 0)
        pthread_cond_wait( &condition_var0, &count_mutex );
    else if(id == 1)
        pthread_cond_wait( &condition_var1, &count_mutex );
        else
        {
            pthread_cond_wait( &condition_var2, &count_mutex );
        }
    return;  
}

void cond_signal(int id){
    if(id == 0)
        pthread_cond_signal( &condition_var1);
    else if(id == 1)
        pthread_cond_signal( &condition_var2);
        else
        {
            pthread_cond_signal( &condition_var0);
        }
    return;  
}*/

void turn_check(int id){
        printf("%d\n", id);
        printf("turn: %d\n", turn);

        if(id == turn){// Se for seu turno de incrementar retorna para a main para seguir para a função count_func
        return;
    }else
    {   // enquanto não for seu turno é colocado para dormir até que seja seu turno
        //cond_check(id);
        pthread_cond_wait( &condition_var0, &count_mutex );
    }
}

void count_func(int id){
    if(count >= COUNT_MAX){ // Se o contador já atingiu seu máximo retorna sem incrementar
        turn = NEXT; // seta o indicador de turno para a proxima thread
        return;
    }
    count++; // Incrementa o contador caso ainda não tenha atingido o máximo
    printf("turn count: %d\n", id);
    turn = NEXT; // seta o indicador de turno para a proxima thread
    return;
}

void *functionCount(void *data)
{
   for(;;)
   {
        int *i = data;
        int id = (int)*i;
        pthread_mutex_lock( &count_mutex ); // Tenta adentrar a região crítica
        turn_check(*i); // Verifica se é o seu turno de incrementar
        if(id == turn)
            count_func(*i); // Incrementa o valor de count
        printf("\tCounter value functionCount: %d\n",count);

        pthread_mutex_unlock( &count_mutex ); // Deixa a região crítica
        //cond_signal(*i);
        pthread_cond_signal( &condition_var0 );
        if(count >= COUNT_MAX) return(NULL);
    }
}