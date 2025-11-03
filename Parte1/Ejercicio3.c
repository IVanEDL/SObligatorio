#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "include/auxiliares.h"

#define N_PROD 10
#define N_CONS 3
#define NUM_GEN 30
#define ITER_PROD NUM_GEN/N_PROD
#define ITER_CONS NUM_GEN/N_CONS

//---------------- COMPLETAR CODIGO ----------------------- 
// Declarar semaforos.
sem_t sem_empty;
sem_t sem_full;
sem_t sem_mutex;
//---------------- COMPLETAR CODIGO ----------------------- 

void* productor(void* arg) {
    int item;
    for (int i = 0; i < ITER_PROD; i++) 
//---------------- COMPLETAR CODIGO -----------------------{
    {
        item = producir();
        sem_wait(&sem_empty);
        sem_wait(&sem_mutex);
        
        guardar(item);
        
        sem_post(&sem_mutex);
        sem_post(&sem_full);
//---------------- COMPLETAR CODIGO -----------------------
    }
    return NULL;
}

void* consumidor(void* arg) {
    int item;
    for (int i = 0; i < ITER_CONS; i++) {
//---------------- COMPLETAR CODIGO -----------------------

        sem_wait(&sem_full);
        sem_wait(&sem_mutex);
        
        item = tomar();
        
        sem_post(&sem_mutex);
        sem_post(&sem_empty);
        
        consumir(item);

//---------------- COMPLETAR CODIGO -----------------------
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    pthread_t hilos_prod[N_PROD], hilos_cons[N_CONS];

//---------------- COMPLETAR CODIGO ----------------------- 
// Inicializar semaforos.
    // Asumimos tamaño de buffer de 10 celdas (ajustar si su auxiliar define otro tamaño)
    sem_init(&sem_mutex, 0, 1);
    sem_init(&sem_full, 0, 0);
    sem_init(&sem_empty, 0, 10);
//---------------- COMPLETAR CODIGO ----------------------- 


    inicializar_buffer();

    for (int i = 0; i < N_PROD; i++) {
        pthread_create(&hilos_prod[i], NULL, productor, NULL);
    }
    for (int i = 0; i < N_CONS; i++) {
        pthread_create(&hilos_cons[i], NULL, consumidor, NULL);
    }

    for (int i = 0; i < N_PROD; i++) pthread_join(hilos_prod[i], NULL);
    for (int i = 0; i < N_CONS; i++) pthread_join(hilos_cons[i], NULL);

//---------------- COMPLETAR CODIGO ----------------------- 
// Eliminar semaforos.
    sem_destroy(&sem_mutex);
    sem_destroy(&sem_full);
    sem_destroy(&sem_empty);
//---------------- COMPLETAR CODIGO -----------------------

    printf("Fin de la ejecución\n");
    return 0;
}


