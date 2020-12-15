/* Dica para compilação: 
                         gcc -o test2 MutexESemaforo.c -lpthread */

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

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define N 5 // number of threads
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (i + N - 1) % N //pegar garfo da esquerda
#define RIGHT (i + 1) % N    //pegar garfo da direita

void *mythread(void *data);
void take_forks(int);
void put_forks(int);
void test(int);

int nfilosofo[N] = {0, 1, 2, 3, 4};
int state[N] = {0, 0, 0, 0, 0}; // estados dos philosophers (comendo, pensando e com fome)
sem_t phil_sem[N];              // controlador de cada filosofo

void think(int i)
{ // Só pra consumir processamento
    printf("Filosofo %d esta pensando.\n", i);
    int t = (rand()%5); 
    sleep(t);
    return;
}

void eat(int i)
{ // Só pra consumir processamento
    if(state[i]==EATING){
        printf("Filosofo %d esta comendo.\n", i);
        int t = (rand()%5);
        sleep(t);
        }
    return;
}

void *mythread(void *data)
{
    while (1)
    {
        int *id = data;
        think(*id);
        take_forks(*id);
        eat(*id);
        put_forks(*id);
    }
}

void take_forks(int i)
{
    pthread_mutex_lock(&mutex); // mutex que regula acesso a região crítica(mesa)
    state[i] = HUNGRY;
    test(i);
    pthread_mutex_unlock(&mutex); // libera a região crítica(mesa)
    sem_wait(&phil_sem[i]); // coloca o filosofo em espera para comer ate outro filósofo poder liberá-lo
}

void put_forks(int i)
{
    pthread_mutex_lock(&mutex); // mutex que regula acesso a região crítica(mesa)
    state[i] = THINKING;
    printf("Filosofo %d deixou os garfos %d e %d.\n", i, LEFT, RIGHT);
    test(LEFT);
    test(RIGHT);
    pthread_mutex_unlock(&mutex); // libera a região crítica(mesa)
}

void test(int i) //Verifica se os Filosofos vizinhos já estão comendo
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    { 
        state[i] = EATING;
        printf("Filosofo %d pegou os garfos do filósofo %d e %d.\n", i, LEFT, RIGHT);
        sem_post(&phil_sem[i]); //Se os vizinhoso i não estiverem comendo, libera o filósofo i para pegar os garfos.
    }
}

int main(void)
{
    int i = 0;
    pthread_t filosofo[N];
    for (i = 0; i < N; i++)
        sem_init(&phil_sem[i], 0, 0); // inicializa o semáforo controlador de cada filosofo
    for (i = 0; i < N; i++)
    {   //Criação da thread de cada Filósofo
        int *j = malloc(sizeof(int));
        *j = i;
        pthread_create(&filosofo[i], NULL, mythread, &nfilosofo[i]);
    }
    for (i = 0; i < N; i++)
        pthread_join(filosofo[i], NULL);
    return (1);
}
