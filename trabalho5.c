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

pthread_mutex_t acess_seats    = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  custReady  = PTHREAD_COND_INITIALIZER; 

int N; //quantidade de cadeiras da sala de espera
int C; //quantidade de clientes
int freeSeats; //quantidade de assentos livres
sem_t barber_state; //semaforo que controla se o barbeiro esta acordado
//sem_t custReady;
void *barber();
void *customer();


void main(){
    puts("Digite a quantidade de cadeiras:");
    scanf("%d", &N);
    puts("Digite a quantidade de clientes ( clientes > cadeiras + 1)");
    scanf("%d", &C);
    freeSeats = N;
    int nThread[C+1];
    for(int i = 0; i<C;i++){
        nThread[i] = i;
    }
    pthread_t thread[C+1];
    sem_init(&barber_state, 0, 0);
    //sem_init(&custReady, 0, 0);

    for (int i = 0; i < C; i++)
    {
        pthread_create(&thread[i], NULL, customer, &nThread[i]);
    }
    pthread_create(&thread[C], NULL, barber, &nThread[C]);

    for (int i = 0; i <= C; i++)
        pthread_join(thread[i], NULL);

   exit(EXIT_SUCCESS);
}

void cutHair(){ 
    // Só pra consumir processamento
    printf("Cortando o cabelo de um cliente.\n");
    int t = (rand()%5); 
    sleep(t);
    return;
}

void leaveBarber(){
    //Só pra consumir processamento
    int t = (rand()%5); 
    sleep(t);
    return;
}

void *barber(void *data){
    while(1){
        if(freeSeats == N){ //verifica se há clientes esperando
            sem_wait(&barber_state); //se não houver, dorme até um cliente acordá-lo
        }
        pthread_mutex_lock( &acess_seats );
        freeSeats += 1; //libera uma cadeira na sala de espera já que vai cortar o cabelo de um cliente
        pthread_mutex_unlock( &acess_seats );
        cutHair();
        puts("cortou o cabelo de um cliente");
        pthread_cond_signal( &custReady );//libera o cliente
        //sem_post(&custReady);
    }
}

void *customer(void *data){
    int *i = data;
    int id = (int)*i;
    while(1){
        pthread_mutex_lock( &acess_seats );
        if (freeSeats > 0){// verifica se há cadeiras livres
            freeSeats -= 1; // ocupa uma cadeira livre
            printf("Cliente %d Esperando\n", id);
            pthread_mutex_unlock( &acess_seats );
            sem_post(&barber_state); //acorda o barbeiro caso ele esteja dormindo
            //sem_wait(&custReady);
            pthread_cond_wait( &custReady, &acess_seats ); //espera pela sua vez
            pthread_mutex_unlock( &acess_seats );
            //quando passar por aqui significa que ja cortou o cabelo
            printf("Cliente %d cortou o cabelo\n", id);
            leaveBarber();
        }else{
            pthread_mutex_unlock( &acess_seats ); // se não tem cadeiras deixa o estabelecimento
            leaveBarber();
        }
    }
}