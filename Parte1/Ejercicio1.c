#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include "include/auxiliares.h"

#define N_HIJOS 20

int main() {
    srand(time(NULL));

    // Crear memoria compartida para el contador
    int shm_fd = shm_open("/contador_shm", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    ftruncate(shm_fd, sizeof(int)); // tamaño de la memoria compartida

    //Crear un contador en la memoria compartida
    int *contador = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (contador == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // inicializar el contador
    *contador = 0; 

    // Crear semáforo en memoria compartida
    //--------------------- COMPLETAR CODIGO ---------------------------       
    //Asegurarse que no quede uno antiguo
    sem_unlink("/sem_contador");
    sem_t *sem_contador = sem_open("/sem_contador", O_CREAT | O_EXCL, 0644, 1);
    if (sem_contador == SEM_FAILED) {
        perror("sem_open");
        // continuar para no dejar recursos en mal estado
        exit(1);
    }
    //--------------------- COMPLETAR CODIGO ---------------------------  


    // Crear 20 hijos
    for (int i = 0; i < N_HIJOS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        
        
        
        if (pid == 0) { // hijo
 
//--------------------- COMPLETAR CODIGO ---------------------------       
            // Sección crítica: proteger acceso al contador
            sem_wait(sem_contador);
        	incrementar(contador);
            sem_post(sem_contador);
        
//--------------------- COMPLETAR CODIGO ---------------------------          
        
           exit(0); // mata al programa
        }
    }

    // Esperar a todos los hijos
    for (int i = 0; i < N_HIJOS; i++) {
        wait(NULL);
    }

    printf("Valor final del contador: %d (debería ser %d)\n", *contador, N_HIJOS);


 
//--------------------- COMPLETAR CODIGO ---------------------------       
    // Limpiar semaforos creados.
    sem_close(sem_contador);
    sem_unlink("/sem_contador");      
        
//--------------------- COMPLETAR CODIGO ---------------------------     

    munmap(contador, sizeof(int));
    shm_unlink("/contador_shm");

    return 0;
}

