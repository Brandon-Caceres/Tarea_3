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
    char id[100];
    char nombre[50];
    char descripcion[500];
    List * items_disp;
    Direccion  dir_posibles;
    char esFinal[10];
}Escenarios;

void mostrarMenuPrincipal() {
    limpiarPantalla();
    puts("========================================");
    puts("            MMORPG NO LINEAL          ");
    puts("========================================");
  
    puts("1) Cargar Escenarios");
    puts("2) Jugar en solitario");
    puts("3) Jugar Multijugador");
    puts("4) Salir");
}

void mostrarMenuJuego() {
    limpiarPantalla();
    puts("========================================");
    puts("            MMORPG NO LINEAL          ");
    puts("========================================");
  
    puts("1) Descripcion del escenario actual");
    puts("2) Lista de items disponibles");
    puts("3) Tiempo restante");
    puts("4) Inventario del jugador");
    puts("5) Direcciones");
    puts("6) Volver al menu principal");
}

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
        strncpy(escenario->id, campos[0], sizeof(escenario->id));
        strncpy(escenario->nombre, campos[1], sizeof(escenario->nombre));
        strncpy(escenario->descripcion, campos[2], sizeof(escenario->descripcion));
        
        List * items = split_string(campos[3], ";");
        List * lista_items = list_create();

        for (char * item = list_first(items); item != NULL; item = list_next(items)){
            List * valor = split_string(item, ",");
            Item * nuevo_item = (Item*)malloc(sizeof(Item));

            strncpy(nuevo_item->nombre, list_first(valor), sizeof(nuevo_item->nombre));
            nuevo_item->valor = atoi(list_next(valor));
            nuevo_item->peso = atoi(list_next(valor));

            list_pushBack(lista_items, nuevo_item);

            list_clean(valor);
            free(valor);
        }

        escenario->items_disp = lista_items;


        escenario->dir_posibles.arriba = atoi(campos[4]);
        escenario->dir_posibles.abajo = atoi(campos[5]);
        escenario->dir_posibles.izquierda = atoi(campos[6]);
        escenario->dir_posibles.derecha = atoi(campos[7]);

        strncpy(escenario->esFinal, campos[8], sizeof(escenario->esFinal));

        insertMap(juego, escenario->id, escenario);
    }
    fclose(archivo);
    puts("Escenarios cargados con exito");
}

int main(){
    
    char opcion;
    HashMap * juego = createMap(100);
     do{
        mostrarMenuPrincipal();
        printf("Ingrese su opcion: ");
        scanf(" %c", &opcion);

        switch (opcion)
        {
        case '1':
            leer_escenarios(juego);
            break;
        case '2':
        //Menu para opcines de modo un jugador
            char opcion_s;
            do{
                mostrarMenuJuego();
                printf("Ingrese su opcion: ");
                scanf(" %c", &opcion_s);

                switch (opcion_s)
                {
                case '1':
                    break;
                case '2':
                    break;
                case '3':
                    break;
                case '4':
                    break;
                case '5':
                    break;
                case '6':
                    break;
                default:
                    puts("Opcion no valida. Intente de nuevo.");
                    break;
                }
                presioneTeclaParaContinuar();
            }while(opcion != '5');
            break;
        case '3':
        //Menu para opciones del multijugador
            char opcion_m;
            do{
                mostrarMenuJuego();
                printf("Ingrese su opcion: ");
                scanf(" %c", &opcion_m);

                switch (opcion_m)
                {
                case '1':
                    break;
                case '2':
                    break;
                case '3':
                    break;
                case '4':
                    break;
                case '5':
                    break;
                case '6':
                    break;
                default:
                    puts("Opcion no valida. Intente de nuevo.");
                    break;
                }
                presioneTeclaParaContinuar();
            }while(opcion != '5');
            break;
        case '4':
            break;
        default:
            puts("Opcion no valida. Intente de nuevo.");
            break;
        }
        presioneTeclaParaContinuar();
    }while(opcion != '4');
    return 0;
}