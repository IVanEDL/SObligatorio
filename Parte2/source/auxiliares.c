#include "../include/auxiliares.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_IMAGENES 30   // límite para simular
static int contador_imagenes = 0;

// ------------------- COLA DE IMÁGENES -------------------
void init_cola(cola_imagenes_t* c) {
    c->cabeza = NULL;
    c->cola = NULL;
}

void destruir_cola(cola_imagenes_t* c) {
    nodo_t* act = c->cabeza;
    while (act) {
        nodo_t* tmp = act;
        act = act->sig;
        free(tmp);
    }
    c->cabeza = c->cola = NULL;
}

void poner_imagen(cola_imagenes_t* c, int id) {
    nodo_t* nuevo = malloc(sizeof(nodo_t));
    nuevo->id = id;
    nuevo->sig = NULL;
    
    if (c->cola == NULL) {
        c->cabeza = nuevo;
        
        
    	if (rand() % 10 < 3) {
        	usleep(rand() % 1000); //
    	}	
        
        c->cola = nuevo;
    } else {
        c->cola->sig = nuevo;
        
        if (rand() % 10 < 3) {
            usleep(rand() % 1000); //
        }

        c->cola = nuevo;
    }
}

int tomar_imagen(cola_imagenes_t* c) {
    if (c->cabeza == NULL) return -2; // vacío
    nodo_t* tmp = c->cabeza;
    int id = tmp->id;

    if (rand() % 10 < 3) {
        usleep(rand() % 1000); //
    }    
    
    c->cabeza = c->cabeza->sig;
    if (c->cabeza == NULL) c->cola = NULL;
    free(tmp);
    return id;
}

// ------------------- ESTADÍSTICAS -------------------
void init_estadisticas(estadisticas_t *est) {
    est->total = 0;
    est->sospechosos = 0;
    est->capacidad = 16;
    est->usados = 0;
    est->eventos = malloc(est->capacidad * sizeof(time_t));
}

void destruir_estadisticas(estadisticas_t *est) {
    free(est->eventos);
}

void guardar_estadisticas(estadisticas_t *est, int sospechoso) {
    est->total++;
    if (sospechoso) {
        est->sospechosos++;
        if (est->usados == est->capacidad) {
            est->capacidad *= 2;
            est->eventos = realloc(est->eventos, est->capacidad * sizeof(time_t));
        }
        est->eventos[est->usados++] = time(NULL);
    }
}

void revisar_estadisticas(estadisticas_t *est) {
    int ventana_seg = 5;
    int umbral = 4;
    time_t ahora = time(NULL);
    int recientes = 0;
    for (int i = 0; i < est->usados; i++) {
        if (difftime(ahora, est->eventos[i]) <= ventana_seg) {
            recientes++;
        }
    }
    printf("[Gestor] Últimos %d segundos: %d sospechosos\n", ventana_seg, recientes);
    if (recientes >= umbral) {
        printf(">>> ALARMA: más de %d casos sospechosos detectados! <<<\n", umbral);
    }
}

// ------------------- SIMULACIÓN -------------------
int recibir_imagen() {
    if (contador_imagenes >= MAX_IMAGENES) return -1; // fin
    usleep(200000); // 200ms entre imágenes
    return ++contador_imagenes;
}

int procesar_imagen(int id) {
    sleep(2); // 100ms de procesamiento
    return (rand() % 100) < 25; // 20% sospechosa
}

