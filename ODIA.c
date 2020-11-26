#include <pthread.h>
//#include "tlpi_hdr.h"
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */
#include <stdbool.h> 
#include<math.h>

static pthread_barrier_t barrier1;
static pthread_barrier_t barrier2;
pthread_mutex_t swap_mutex     = PTHREAD_MUTEX_INITIALIZER;



void *threadCode();

int iteracoes = 10;
int N = 10000;
int numThreads = 10;
int swap;

    
int main(){
    int s;
    double velho[N];
    double novo[N];
    velho[0] = 0;
    velho[N-1] = 1;
    novo[0] = 0;
    novo[N-1] = 1;

    s = pthread_barrier_init(&barrier1, NULL, numThreads);
    s = pthread_barrier_init(&barrier2, NULL, numThreads);


    for(int i=1; i<=N; i++){
            novo[i] = 0;
        }
    for(int i=1; i<=N; i++){
            velho[i] = 0;
        }
    
    

    exit(EXIT_SUCCESS);
}

void *threadCode(void *data, double velho[N], double novo[N]){
    int s;
    int *i = data;
    int t = (int)*i;
    int n = N - 2;
    int d = numThreads;
    double fatia = ceil(n/d);
    double inicio = t*fatia + 1;
    double fim = inicio + fatia - 1;
    if(fim > (N-2))
        fim = N-2;

    for(int j=0; j<iteracoes; j++){

        s = pthread_barrier_wait(&barrier1);
        swap = 0;

        for(int i = inicio; i<=fim; i++){
            novo[i]=(velho[i-1]+velho[i+1])/2;
        }

        s = pthread_barrier_wait(&barrier2);
        if(swap == 0 && pthread_mutex_trylock(&swap_mutex)==0){
            double temp[N];
            temp[0] = 0;
            temp[n+1] = 1;
            for(int i=1; i<=n+1; i++){
                temp[i] = novo[i];
            }
            for(int i=1; i<=n+1; i++){
                novo[i] = velho[i];
            }
            for(int i=1; i<=n+1; i++){
                velho[i] = temp[i];
            }
            for(int i=0; i<=N+1; i++){
                printf("%ls\n", velho[i]);
            }
            swap = 1;
            pthread_mutex_unlock(&swap_mutex);
        }

    }

    return(NULL);
}