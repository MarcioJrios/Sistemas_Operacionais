//// Autor: Marcio Junior Rios             ////
//// github: MarcioJrios                   ////
////                                       ////
//// Universidade Federal da Fronteira Sul ////

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thread1();
void *thread2();



pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER, mutex2 = PTHREAD_MUTEX_INITIALIZER, interest = PTHREAD_MUTEX_INITIALIZER;
pthread_t Thread1, Thread2;

int main(void) {
    pthread_create(&Thread1, NULL, thread1, NULL);
    pthread_create(&Thread2, NULL, thread2, NULL);

    //now join them
    pthread_join(Thread1, NULL);
    printf("Thread id %ld returned\n", Thread1);
    pthread_join(Thread2, NULL);
    printf("Thread id %ld returned\n", Thread2);


    return 1;
    

}

void *thread1(){
     unsigned long i,j;
     while(1){
     if(pthread_mutex_trylock(&mutex1)==0){
         printf("Thread ID%ld got mutex1.\n", pthread_self());
         for(i=0; i< 10000000; ++i); // just for wasting some time
         if(pthread_mutex_trylock(&mutex2)==0){
            printf("Thread ID%ld got mutex2.\n", pthread_self());          
            for(i=0; i< 10000000; ++i); // just for wasting some time
            pthread_mutex_unlock(&mutex2);
            pthread_mutex_unlock(&mutex1);
            pthread_mutex_unlock(&interest);
            break;
         }
         else  {
             printf("\nThread ID%ld did not get mutex2.\n", pthread_self());
             pthread_mutex_unlock(&mutex1);
             while(1){// enquanto houver outra thread interessada nos mutexes continuará esperando ela perder o interesse.
                    if(pthread_mutex_trylock(&interest)==0){//quando não conseguir o mutex1 libera seus mutexes e sinaliza seu interesse pelos mesmos
                        break;
                    }
                }  
             
             }
     }
     else { printf("\nThread ID%ld did not get mutex1. Trying again\n", pthread_self());}
     }
   //pthread_mutex_unlock(&mutex1);
    pthread_exit(NULL);
}

void *thread2(){
     unsigned long i,j;
     while(1){
     if(pthread_mutex_trylock(&mutex2)==0) {
         printf("Thread ID%ld got mutex2.\n", pthread_self());
         for(i=0; i< 10000000; ++i); // just for wasting some time
         if(pthread_mutex_trylock(&mutex1)==0){
            printf("Thread ID%ld got mutex1.\n", pthread_self());          
            for(i=0; i< 10000000; ++i); // just for wasting some time
            pthread_mutex_unlock(&mutex1);
            pthread_mutex_unlock(&mutex2);
            pthread_mutex_unlock(&interest);
            break;
         }
         else { printf("\nThread ID%ld did not get mutex1.\n", pthread_self());
                pthread_mutex_unlock(&mutex2);
                while(1){// enquanto houver outra thread interessada nos mutexes continuará esperando ela perder o interesse.
                    if(pthread_mutex_trylock(&interest)==0){ //quando não conseguir o mutex1 libera seus mutexes e sinaliza seu interesse pelos mesmos
                        break;
                    }
                }     
                }
     }
     else { printf("\nThread ID%ld did not get mutex2.\n", pthread_self());}
     }
     //pthread_mutex_unlock(&mutex2);
     pthread_exit(NULL);
}






