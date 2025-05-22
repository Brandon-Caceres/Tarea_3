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

typedef struct{
    char nombre[50];
    Escenarios *actual;
    List *inventario;
    int peso;
    int puntaje;
    float tRestante;
}Jugador;

void mostrarMenuPrincipal() {
    limpiarPantalla();
    //encontre el meme jajaja
    /*albion online es un mmorpg no lineal en el que escribes 
    tu propia historia sin limitarte a seguir un camino prefijado, 
    explora un amplio mundo abierto con cinco biomas unicos, todo 
    cuanto hagas tendra su repercusíon en el mundo, con su economia 
    orientada al jugador de albion los jugadores crean...*/
    puts("========================================");
    puts("            MMORPG NO LINEAL          ");
    puts("========================================");
  
    puts("1) Cargar Escenarios");
    puts("2) Jugar en solitario");
    puts("3) Jugar Multijugador");
    puts("4) Salir");
}

void mostrarMenuSolo() {
    limpiarPantalla();
  
    puts("1) Recoger Item(s)");
    puts("2) Descartar Item(s)");
    puts("3) Avanzar en una Dirección");
    puts("4) Reiniciar Partida");
    puts("5) Salir del Juego");
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

void mostrar_escenario(Jugador * player){
    printf("ID: %s, Nombre: %s\n", player->actual->id, player->actual->nombre);
    printf("Descripcion: %s\n", player->actual->descripcion);
    
    if (player->actual->items_disp != NULL && list_first(player->actual->items_disp) != NULL) {
        printf("Items Disponibles(nombre,peso,valor): \n");
        for (Item * item = list_first(player->actual->items_disp); item != NULL; item = list_next(player->actual->items_disp)){
            printf("  - %s, %dkg, valor %d\n", item->nombre, item->peso, item->valor);
        }
    }
    else printf("No se encuentran items en esta sala\n");

    printf("Tiempo restante: %.2f\n", player->tRestante);
    
    puts("Inventario: ");
    if (player->inventario != NULL && list_first(player->inventario) != NULL){
        for (Item * objeto = list_first(player->inventario); objeto != NULL; objeto = list_next(player->inventario)){
            printf("  - %s, %dkg, valor %d\n", objeto->nombre, objeto->peso, objeto->valor);
        }
    }
    else printf("   Vacio\n");

    printf("Peso total: %d", player->peso);
    printf("Puntaje acumulado: %d", player->puntaje);
    
    if(strcmp(player->actual->esFinal, "No") == 0){
        printf("Direcciones posibles: ");
        if (player->actual->dir_posibles.arriba != -1) printf("Arriba ");
        if(player->actual->dir_posibles.abajo != -1) printf("Abajo ");
        if(player->actual->dir_posibles.izquierda != -1) printf("Izquierda ");
        if(player->actual->dir_posibles.derecha != -1) printf("Derecha ");
        printf("\n");
    }
    else puts("Felicidades lograste salir del laberinto con un buen botin");
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
            mostrarMenuSolo();
            char op;
            printf("Ingrese su opcion: ");
            switch (op)
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
            default:
                break;
            }
            scanf("%s", &op);
            break;
        case '3':
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