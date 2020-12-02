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
#include <math.h>

static pthread_barrier_t barrier1;
static pthread_barrier_t barrier2;
pthread_mutex_t swap_mutex     = PTHREAD_MUTEX_INITIALIZER;

/*typedef struct Attr {
    double* velho;
    double* novo;
} Attr;*/

void *threadCode();

int numThreads;
int iteracoes; //quantidade de iterações
int N = 10000; //elementos totais do vetor(incluindo os de borda)
int swap;   //variavel auxiliar para operação de swap
double velho[10000]; 
double novo[10000];
    
int main(){
    int s;
    
    puts("Digite a quantidade de threads( < 10000");
    scanf("%d", &numThreads);
    puts("Digite a quantidade de iterações");
    scanf("%d", &iteracoes);
    velho[0] = 0;
    velho[N-1] = 1;
    novo[0] = 0;
    novo[N-1] = 1;
    //zerando os elementos do vetor
    for(int i=1; i<N-1; i++){
            novo[i] = 0;
        }
    for(int i=1; i<N-1; i++){
            velho[i] = 0;
        }
    

    int nThread[numThreads];
    for(int i = 0;i<numThreads;i++){
        nThread[i]=i;
    }
    //inicializando as barreiras
    s = pthread_barrier_init(&barrier1, NULL, numThreads);
    s = pthread_barrier_init(&barrier2, NULL, numThreads);


    
    pthread_t thread[numThreads];

    for (int i = 0; i < numThreads; i++)
    {
        pthread_create(&thread[i], NULL, threadCode, &nThread[i]);
    }

    for (int i = 0; i < numThreads; i++)
        pthread_join(thread[i], NULL);
    for(int i=0; i<=N-1; i++){
            printf("%lf\n", velho[i]);
        }
    exit(EXIT_SUCCESS);
}   

void *threadCode(void *data){
    int s;
    int *i = data;
    int t = (int)*i;
    int n = N - 2;
    int d = numThreads;
    int fatia = ceil(n/d);
    int inicio = t*fatia + 1;
    int fim = inicio + fatia - 1;
    if(t==numThreads-1)
        fim = N-2;
    /*int fatia = ceil(N/d);
    int inicio = t * fatia;
    if(inicio = 0)
        inicio = 1;
    //int fim = fatia *(t+1) - 1;
    int fim = inicio + fatia - 1;*/
    
    if(fim > (N-2))
        fim = N-2;

    for(int j=0; j<iteracoes; j++){
        s = pthread_barrier_wait(&barrier1);
        for(int i = inicio; i<=fim; i++){
            novo[i]=(velho[i-1]+velho[i+1])/2;
        }
        swap = 0;

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
            
            swap = 1;
            pthread_mutex_unlock(&swap_mutex);
        }

    }

    return(NULL);
}