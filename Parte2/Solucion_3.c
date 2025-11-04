#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "include/auxiliares.h"

#define N_PROCESADORES 4

// --------- ESPACIOS COMPARTIDOS ---------
cola_imagenes_t cola;
estadisticas_t global_stats;

int esperando = 0; 
int cant_norm = 0;
int cant_prio = 0;
int fin_receptor = 0; // bandera de fin

// --------- SEMÁFOROS ---------
sem_t sem_cola;     // protege acceso a la cola
sem_t sem_items;    // cuenta de imágenes disponibles
sem_t sem_stats;    // protege estadísticas
sem_t sem_stats_prio;    // da prioridad al gestor
sem_t sem_mutex_cant;    //mutex de cant
sem_t sem_mutex_fin_receptor;    //mutex de cant

// --------- HILOS ---------
void* receptor(void* arg) {
    while (fin_receptor == 0) {
        int img = recibir_imagen();
        if (img == -1) {
            // fin -> marcar bandera y liberar a procesadores
            sem_wait(&sem_mutex_fin_receptor);
            fin_receptor = 1;
            sem_post(&sem_mutex_fin_receptor);
            
            for (int i = 0; i < N_PROCESADORES; i++)
                sem_post(&sem_items); // desbloquearlos
        }
        else{
            printf("[Receptor] Imagen %d recibida\n", img);

            sem_wait(&sem_cola);
            poner_imagen(&cola, img);
            sem_post(&sem_cola);

            sem_post(&sem_items); // nuevo ítem disponible
        }
        
    }
    return NULL;
}

void* procesador(void* arg) {
    int id = (int)(intptr_t)arg;
    int img;
    int sospechosos[3];
    int contador = 0;
    
    while (1) {
        sem_wait(&sem_items);
        
        sem_wait(&sem_mutex_fin_receptor);
        if (fin_receptor) {
            sem_wait(&sem_cola);
            sem_post(&sem_mutex_fin_receptor);
            if (cola.cabeza == NULL) {
                // Si hay imágenes pendientes en el buffer, procesarlas
                if (contador > 0) {
                    sem_wait(&sem_mutex_cant);
                    cant_norm++;
                    sem_post(&sem_mutex_cant);
                    
                    sem_wait(&sem_stats);
                    for (int i = 0; i < contador; i++) {
                        guardar_estadisticas(&global_stats, sospechosos[i]);
                    }
                    sem_wait(&sem_mutex_cant);
                    cant_norm--;
                    if (cant_prio>0) {
                        sem_post(&sem_stats_prio);
                    } else {
                        sem_post(&sem_stats);
                    }
                    sem_post(&sem_mutex_cant);
                }
                sem_post(&sem_cola);
                break;
            }
            img = tomar_imagen(&cola);
            sem_post(&sem_cola);
        } else {
            sem_wait(&sem_cola);
            sem_post(&sem_mutex_fin_receptor);
            img = tomar_imagen(&cola);
            sem_post(&sem_cola);
        }
        
        printf("[Procesador %d] Imagen %d en proceso\n", id, img);
        sospechosos[contador] = procesar_imagen(img);
        printf("[Procesador %d] Imagen %d procesada (%s)\n",
               id, img, sospechosos[contador] ? "SOSPECHOSA" : "ok");
        
        contador++;
        
        if (contador == 3) {
            sem_wait(&sem_mutex_cant);
            cant_norm++;
            sem_post(&sem_mutex_cant);
            
            sem_wait(&sem_stats);
            for (int i = 0; i < 3; i++) {
                guardar_estadisticas(&global_stats, sospechosos[i]);
            }
            
            sem_wait(&sem_mutex_cant);
            cant_norm--;
            if (cant_prio>0) {
                sem_post(&sem_stats_prio);
            } else {
                sem_post(&sem_stats);
            }
            sem_post(&sem_mutex_cant);
            
            contador = 0;
        }
    }
    return NULL;
}

void* gestor(void* arg) {
    while (1) {
        sleep(5); // cada 5 segundos
        printf("Gestor quiere mirar estadistica\n");
        
        sem_wait(&sem_mutex_cant);
        if (cant_norm>0){ 
        	cant_prio = 1;
        	sem_post(&sem_mutex_cant);
        	sem_wait(&sem_stats_prio);       // pide entrar a estadísticas
        }
        else{
        	sem_wait(&sem_stats);
        	sem_post(&sem_mutex_cant); 
        }	
        revisar_estadisticas(&global_stats);
        sem_wait(&sem_mutex_cant);
        cant_prio = 0;
        sem_post(&sem_mutex_cant);
	sem_post(&sem_stats);              // libera exclusión
	
	
        sem_wait(&sem_mutex_fin_receptor);
        if (fin_receptor) {
            sem_wait(&sem_cola);
            
            sem_post(&sem_mutex_fin_receptor);
            int vacia = (cola.cabeza == NULL);
            sem_post(&sem_cola);
            if (vacia) break; // terminar
        }
        else{
            sem_post(&sem_mutex_fin_receptor);
        }
    }
    return NULL;
}

// --------- MAIN ---------
int main() {
    srand(time(NULL));
    init_cola(&cola);
    init_estadisticas(&global_stats);

    pthread_t h_receptor, h_gestor, h_procs[N_PROCESADORES];

    sem_init(&sem_cola, 0, 1);
    sem_init(&sem_items, 0, 0);
    sem_init(&sem_stats, 0, 1);
    sem_init(&sem_stats_prio, 0, 0);
    sem_init(&sem_mutex_cant, 0, 1);
    sem_init(&sem_mutex_fin_receptor, 0, 1);

    for (int i = 0; i < N_PROCESADORES; i++) {
        pthread_create(&h_procs[i], NULL, procesador, (void*)(intptr_t)i);
    }
    
    pthread_create(&h_receptor, NULL, receptor, NULL);
    pthread_create(&h_gestor, NULL, gestor, NULL);

    pthread_join(h_receptor, NULL);
    for (int i = 0; i < N_PROCESADORES; i++) pthread_join(h_procs[i], NULL);
    pthread_join(h_gestor, NULL);

    sem_destroy(&sem_cola);
    sem_destroy(&sem_items);
    sem_destroy(&sem_stats);
    sem_destroy(&sem_stats_prio);
    sem_destroy(&sem_mutex_cant);
    sem_destroy(&sem_mutex_fin_receptor);

    destruir_cola(&cola);
    destruir_estadisticas(&global_stats);

    printf("Fin de la ejecución\n");
    return 0;
}

