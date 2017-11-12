#include "recursos.h"


void pasar_recursos_de_hash_a_heap(const char* clave, void* recurso, void* heap) {
    heap_encolar((heap_t*)heap, recurso);
}

bool aumenta_cont_solicitudes_recurso(hash_t* temp_recursos_solicitados, char* nombre_recurso) {

    if (!hash_pertenece(temp_recursos_solicitados, nombre_recurso)) {

        recurso_t* recurso = crear_recurso(nombre_recurso);
        if (recurso == NULL) return NULL;
        hash_guardar(temp_recursos_solicitados, nombre_recurso, recurso);
    }
    ((recurso_t *) (hash_obtener(temp_recursos_solicitados, nombre_recurso)))->cant_de_solicitudes++;

    return true;
}

//Funcion de comparacion de recurso_t
int comparar_recursos(recurso_t* recurso1, recurso_t* recurso2){

    //Comparo la cantidad de solicitudes de cada recurso
    int valor_retorno;
    int cantidad_1 = recurso1->cant_de_solicitudes;
    int cantidad_2 = recurso2->cant_de_solicitudes;
    if(cantidad_1 > cantidad_2) valor_retorno = 1;
    else if(cantidad_1 < cantidad_2) valor_retorno = -1;
    else valor_retorno = 0;
    return valor_retorno;
}

recurso_t* crear_recurso(char* nombre_recurso) {
    recurso_t *recurso = malloc(sizeof(recurso_t));
    if (recurso== NULL) return NULL;

    recurso->clave = nombre_recurso;
    recurso->cant_de_solicitudes = 0;

    return recurso;
}