#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <time.h>

// ------------------- ESTADÍSTICAS -------------------
typedef struct {
    int total;
    int sospechosos;
    time_t *eventos;
    int capacidad;
    int usados;
} estadisticas_t;

void init_estadisticas(estadisticas_t *est);
void destruir_estadisticas(estadisticas_t *est);
void guardar_estadisticas(estadisticas_t *est, int sospechoso);
void revisar_estadisticas(estadisticas_t *est);

// ------------------- LISTA DE IMÁGENES -------------------
typedef struct nodo {
    int id;
    struct nodo* sig;
} nodo_t;

typedef struct {
    nodo_t* cabeza;
    nodo_t* cola;
} cola_imagenes_t;

void init_cola(cola_imagenes_t* c);
void destruir_cola(cola_imagenes_t* c);
void poner_imagen(cola_imagenes_t* c, int id);
int tomar_imagen(cola_imagenes_t* c);

// ------------------- SIMULACIÓN -------------------
int recibir_imagen();
int procesar_imagen(int id);

#endif

