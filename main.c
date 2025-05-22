#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "extra.h"
#include "hashmap.h"

//Estructura para cada Item
typedef struct{
    char nombre[50];
    int peso;
    int valor;
}Item;

//Estructura para las direcciones podibles
typedef struct{
    int arriba;
    int abajo;
    int izquierda;
    int derecha;
}Direccion;

//Estructura para cada escenario(nivel)
typedef struct{
    int id;
    char nombre[50];
    char descripcion[500];
    List * items_disp;
    Direccion  dir_posibles[4];
    char esFinal[10];
}Escenarios;

void leer_escenarios(HashMap * juego){
    FILE *archivo = fopen("data/graphquest.csv", "r");
    if (archivo == NULL){
        perror("Error al abrir el archivo");
        return;
    }

    char **campos;
    campos = leer_linea_csv(archivo, ',');

    while ((campos = leer_linea_csv(archivo, ',')) != NULL){
        Escenarios * escenario = (Escenarios*)malloc(sizeof(Escenarios));
        escenario->id = atoi(campos[0]);
        strcnpy(escenario->nombre, campos[1], sizeof(escenario->nombre));
        strcnpy(escenario->descripcion, campos[2], sizeof(escenario->descripcion));
        
        List * items = split_string(campos[3], ';');
        List * lista_items = list_create();

        for (char * item = list_first(items); item != NULL; item = list_next(items)){
            List * valor = split_string(item, ',');
            Item * nuevo_item = (Item*)malloc(sizeof(nuevo_item));

            strcnpy(nuevo_item->nombre, list_first(valor), sizeof(nuevo_item->nombre));
            nuevo_item->valor = atoi(list_next(valor));
            nuevo_item->peso = atoi(list_next(valor));

            list_pushBack(lista_items, nuevo_item);

            list_clean(valor);
            free(valor);
        }

        escenario->items_disp = lista_items;


        escenario->dir_posibles->arriba = atoi(campos[4]);
        escenario->dir_posibles->abajo = atoi(campos[5]);
        escenario->dir_posibles->izquierda = atoi(campos[6]);
        escenario->dir_posibles->derecha = atoi(campos[7]);

        strcpy(escenario->esFinal, campos[8]);
    }
}

int main(){
    HashMap * juego = createMap(100);
    return 0;
}