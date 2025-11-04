#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define ITER 10

//---------------- COMPLETAR CODIGO ----------------------- 
// Declarar semaforos.
sem_t sem_ping;
sem_t sem_pong;
  
//---------------- COMPLETAR CODIGO -----------------------   

// Función del hilo que imprime "Ping"

void* ping(void* arg) {
    for (int i = 0; i < ITER; i++) {
    
//---------------- COMPLETAR CODIGO -----------------------     
        sem_wait(&sem_ping);
        printf("Ping\n");
        fflush(stdout);
        sem_post(&sem_pong);
//---------------- COMPLETAR CODIGO ----------------------- 
    }
    return NULL;
}

// Función del hilo que imprime "Pong"

void* pong(void* arg) {
    for (int i = 0; i < ITER; i++) {
    

//---------------- COMPLETAR CODIGO -----------------------    
        sem_wait(&sem_pong);
        printf("Pong\n");
        fflush(stdout);
        sem_post(&sem_ping);

//---------------- COMPLETAR CODIGO ----------------------- 
    }
    return NULL;
}

int main() {
    pthread_t hilo1, hilo2;

    // Inicializar semáforos en memoria compartida entre hilos (pshared = 0)
    //---------------- COMPLETAR CODIGO ----------------------- 
    // sem_init(&nombre_semaforo, 0, valor_inicial);  
    sem_init(&sem_ping, 0, 0); // ping inicia
    sem_init(&sem_pong, 0, 1);
    
    //---------------- COMPLETAR CODIGO ----------------------- 

    // Crear hilos
    pthread_create(&hilo1, NULL, ping, NULL);
    pthread_create(&hilo2, NULL, pong, NULL);

    // Esperar a que terminen
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    
    //---------------- COMPLETAR CODIGO -----------------------
    // Destruir semáforos, Ej:
    sem_destroy(&sem_ping);
    sem_destroy(&sem_pong);
    
    //---------------- COMPLETAR CODIGO ----------------------- 

    printf("Fin de la ejecución\n");

    return 0;
}

