//// Autor: Marcio Junior Rios             ////
//// github: MarcioJrios                   ////
////                                       ////
//// Universidade Federal da Fronteira Sul ////

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
int count = 0; // contador que será incrementado
int turn = 0; // 
int nThread[3] = {0, 1, 2};
sem_t thread_turn[3];

#define COUNT_MAX  10 //valor máximo do contador
#define COUNT_HALT1  3
#define COUNT_HALT2  6
#define NEXT (id + 1) % 3 //proxima thread

void main()
{
   pthread_t thread[3];
   for (int i = 0; i < 5; i++)
        sem_init(&thread_turn[i], 0, 0); // inicializa o semáforo controlador de cada filosofo
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
    while(1){
        if(id == turn){// Se for seu turno de incrementar retorna para a main para seguir para a função count_func
        return;
    }else
    {   // enquanto não for seu turno é colocado para dormir até que seja seu turno
        sem_wait(&thread_turn[id]);      
        return;   
    }
    }
}

void count_func(int id){
    if(count >= COUNT_MAX){ // Se o contador já atingiu seu máximo retorna sem incrementar
        turn = NEXT; // seta o indicador de turno para a proxima thread
        sem_post(&thread_turn[NEXT]); // acorda a proxima thread que deve incrementar
        return;
    }
    count++; // Incrementa o contador caso ainda não tenha atingido o máximo
    printf("turn count: %d", id);
    turn = NEXT; // seta o indicador de turno para a proxima thread
    sem_post(&thread_turn[NEXT]); // acorda a proxima thread que deve incrementar
    return;
}

void *functionCount(void *data)
{
   for(;;)
   {
        int *id = data;

        turn_check(*id); // Verifica se é o seu turno de incrementar
        pthread_mutex_lock( &count_mutex ); // Tenta adentrar a região crítica
        
        count_func(*id); // Incrementa o valor de count
        
        printf("\tCounter value functionCount: %d\n",count);

        pthread_mutex_unlock( &count_mutex ); // Deixa a região crítica

        if(count >= COUNT_MAX) return(NULL);
    }
}