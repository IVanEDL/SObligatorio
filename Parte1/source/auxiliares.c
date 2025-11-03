#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "../include/auxiliares.h"

// Función que incrementa el contador con una pequeña posibilidad de sleep
void incrementar(int *contador) {
    int aux = *contador;
    aux++;
    // Probabilidad de hacer sleep: 30%
    if (rand() % 10 < 3) {
        usleep(rand() % 100); //
    }
    *contador = aux;
}

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

void inicializar_buffer() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = -1;  // -1 significa "vacío"
    }
}

void imprimir_buffer() {
    printf("[ ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        //if (buffer[i] == -1)
          //  printf("· ");   // un puntito para los vacíos
        //else
            printf("%d ", buffer[i]);
    }
    printf("]\n");
}

int producir() {
    int item = rand() % 100;
    sleep(rand() % 10);
    //printf("Producido: %d\n", item);
    //fflush(stdout);
    return item;
}

void consumir(int item) {
    sleep(rand() % 10);
    //printf("Consumido: %d\n", item);
    //fflush(stdout);
}

void guardar(int item) {
    buffer[in] = item;
    
    if (rand() % 10 < 3) {
        usleep(rand() % 1000); //
    }
    
    printf("Guardado %d en posición %d\n", item, in);
    imprimir_buffer();

    in = (in + 1) % BUFFER_SIZE;
}

int tomar() {
    int item = buffer[out];
    
    if (rand() % 10 < 3) {
        usleep(rand() % 1000); //
    }
    
    printf("Tomado %d de posición %d\n", item, out);
    buffer[out] = -1;   // marcar como vacío
    imprimir_buffer();

    out = (out + 1) % BUFFER_SIZE;
    return item;
}
