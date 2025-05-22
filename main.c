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
    Item items_disp;
    Direccion dir_posibles;
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

        for (char * item = list_first(items); item != NULL; item = list_next(items)){
            List * valor = split_string(item, ',');
            char * item_name = list_first(valor);
            int valor_item = atoi(list_next(valor));
            int peso_item = atoi(list_next(valor));

            list_clean(valor);
            free(valor);
        }

        
    }
}

int main(){
    HashMap * juego = createMap(100);
    return 0;
}