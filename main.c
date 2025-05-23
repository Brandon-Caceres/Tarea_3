#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "extra.h"
#include "hashmap.h"

typedef struct Escenarios Escenarios;

//Estructura para cada Item
typedef struct{
    char nombre[50];
    int peso;
    int valor;
}Item;

//Estructura para las direcciones podibles
typedef struct{
    Escenarios * arriba;
    Escenarios * abajo;
    Escenarios * izquierda;
    Escenarios * derecha;
}Direccion;

//Estructura para cada escenario(nivel)
struct Escenarios{
    char id[100];
    char nombre[50];
    char descripcion[500];
    List * items_disp;
    Direccion  dir_posibles;
    char esFinal[10];
    char id_arriba[3];
    char id_abajo[3];
    char id_izquierda[3];
    char id_derecha[3];
};

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
            nuevo_item->peso = atoi(list_next(valor));
            nuevo_item->valor = atoi(list_next(valor));

            list_pushBack(lista_items, nuevo_item);

            list_clean(valor);
            free(valor);
        }

        escenario->items_disp = lista_items;


        strncpy(escenario->id_arriba, campos[4], sizeof(escenario->id_arriba));
        strncpy(escenario->id_abajo, campos[5], sizeof(escenario->id_abajo));
        strncpy(escenario->id_izquierda, campos[6], sizeof(escenario->id_izquierda));
        strncpy(escenario->id_derecha, campos[7], sizeof(escenario->id_derecha));

        strncpy(escenario->esFinal, campos[8], sizeof(escenario->esFinal));

        insertMap(juego, escenario->id, escenario);
    }
    fclose(archivo);
    puts("Escenarios cargados con exito");

    Pair * par = firstMap(juego);
    while ( par!= NULL){
        Escenarios * escenario = (Escenarios*)par->value;
        if (strcmp(escenario->id_arriba, "-1") != 0){
            Pair * aux = searchMap(juego, escenario->id_arriba);
            if (aux != NULL) escenario->dir_posibles.arriba = (Escenarios*)aux->value;
        }
        if (strcmp(escenario->id_abajo, "-1") != 0){
            Pair * aux = searchMap(juego, escenario->id_abajo);
            if (aux != NULL) escenario->dir_posibles.abajo = (Escenarios*)aux->value;
        }
        if (strcmp(escenario->id_izquierda, "-1") != 0){
            Pair * aux = searchMap(juego, escenario->id_izquierda);
            if (aux != NULL) escenario->dir_posibles.izquierda = (Escenarios*)aux->value;
        }
        if (strcmp(escenario->id_derecha, "-1") != 0){
            Pair * aux = searchMap(juego, escenario->id_derecha);
            if (aux != NULL) escenario->dir_posibles.derecha = (Escenarios*)aux->value;
        }
        par = nextMap(juego);
    }
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

    printf("Peso total: %d\n", player->peso);
    printf("Puntaje acumulado: %d\n", player->puntaje);
    
    if(strcmp(player->actual->esFinal, "No") == 0){
        printf("Direcciones posibles: ");
        if (player->actual->dir_posibles.arriba != NULL) printf("Arriba ");
        if (player->actual->dir_posibles.abajo != NULL) printf("Abajo ");
        if (player->actual->dir_posibles.izquierda != NULL) printf("Izquierda ");
        if (player->actual->dir_posibles.derecha != NULL) printf("Derecha ");
        printf("\n");
    }
    else puts("Felicidades lograste salir del laberinto con un buen botin");
}

void recoger_items(Jugador * player){
    if (player->actual->items_disp == NULL || list_first(player->actual->items_disp) == NULL){
        puts("No hay items disponibles en este escenario :(");
        return;
    }

    while (1){
        puts("Items disponibles para recoger");
        int i = 1;
        Item * arreglo_items[100];

        for (Item * item = list_first(player->actual->items_disp); item != NULL; item = list_next(player->actual->items_disp)){
            printf("%d) %s, Peso: %dkg, Valor: $%d\n", i, item->nombre, item->peso, item->valor);
            arreglo_items[i - 1] = item;
            i++;
        }
        printf("%d) Cancelar\n", i);

        int opcion;
        printf("Elige el numero del item a recoger: ");
        scanf(" %d", &opcion);

        if (opcion <= 0 || opcion >= i){
            puts("Cancelando u opcion invalida");
            break;
        }

        Item * seleccionado = arreglo_items[opcion - 1];
        list_pushBack(player->inventario, seleccionado);
        player->peso += seleccionado->peso;
        player->puntaje += seleccionado->valor;
        
        for (Item * item = list_first(player->actual->items_disp); item != NULL; item = list_next(player->actual->items_disp)) {
            if (item == seleccionado) {
                list_popCurrent(player->actual->items_disp);
                break;
            }
        }

        printf("Recogiste: %s\n", seleccionado->nombre);

        float tiempo = (player->peso + 1) / 10.0;
        player->tRestante -= tiempo;
    }
}

void descartar_items(Jugador *player){
    if (player->inventario == NULL || list_first(player->inventario) == NULL){
        puts("No existen items en el inventario");
        return;
    }

    while (1){
        puts("Items del inventario:");
        int i = 1;
        Item * arreglo_items[100];

        for (Item * item = list_first(player->inventario); item != NULL; item = list_next(player->inventario)){
            printf("%d) %s, Peso: %dkg, Valor: $%d\n", i, item->nombre, item->peso, item->valor);
            arreglo_items[i - 1] = item;
            i++;
        }
        printf("%d) Cancelar\n", i);

        int opcion;
        printf("Elige el numero del item a descartar");
        scanf(" %d", &opcion);

        if (opcion <= 0 || opcion >= i){
            puts("Cancelando u opcion invalida");
            break;
        }

        Item * seleccionado = arreglo_items[opcion - 1];
        list_pushBack(player->actual->items_disp, seleccionado);
        player->peso -= seleccionado->peso;
        player->puntaje -= seleccionado->valor;

        for (Item * item = list_first(player->inventario); item != NULL; item = list_next(player->inventario)){
            if (item == seleccionado){
                list_popCurrent(player->inventario);
                break;
            }
        }

        printf("Descartaste: %s\n", seleccionado->nombre);

        float tiempo = (player->peso + 1) / 10.0;
        player->tRestante = tiempo;
    }
}

void avanzarEscenario(Jugador *player, HashMap *juego) {
    printf("\nEscoge la dirección para avanzar:\n");

    // Mostrar solo las direcciones disponibles
    if (strcmp(player->actual->id_arriba, "-1") != 0)
        printf("W - Arriba\n");
    if (strcmp(player->actual->id_abajo, "-1") != 0)
        printf("S - Abajo\n");
    if (strcmp(player->actual->id_izquierda, "-1") != 0)
        printf("A - Izquierda\n");
    if (strcmp(player->actual->id_derecha, "-1") != 0)
        printf("D - Derecha\n");

    char dir;
    printf("Direccion (W/A/S/D): ");
    scanf(" %c", &dir);
    
    char *id_destino = NULL;

    switch (dir) {
        case 'W':
            if (strcmp(player->actual->id_arriba, "-1") == 0) {
                printf("No hay camino hacia arriba.\n");
                return;
            }
            id_destino = player->actual->id_arriba;
            break;
        case 'S':
            if (strcmp(player->actual->id_abajo, "-1") == 0) {
                printf("No hay camino hacia abajo.\n");
                return;
            }
            id_destino = player->actual->id_abajo;
            break;
        case 'A':
            if (strcmp(player->actual->id_izquierda, "-1") == 0) {
                printf("No hay camino hacia la izquierda.\n");
                return;
            }
            id_destino = player->actual->id_izquierda;
            break;
        case 'D':
            if (strcmp(player->actual->id_derecha, "-1") == 0) {
                printf("No hay camino hacia la derecha.\n");
                return;
            }
            id_destino = player->actual->id_derecha;
            break;
        default:
            printf("Direccion no valida. Usa solo W, A, S o D.\n");
            return;
    }
    Pair * aux = searchMap(juego, id_destino);
    Escenarios *nuevo = (Escenarios *)aux->value;
    if (nuevo != NULL) {
        player->actual = nuevo;
        player->tRestante -= 1;

        printf("\nAvanzaste al escenario: %s\n", nuevo->nombre);
        printf("Descripcion: %s\n", nuevo->descripcion);
    } else {
        printf("Error: Escenario con ID %s no encontrado.\n", id_destino);
    }
}

void limpiar_juego(HashMap * juego){
    Pair * par = firstMap(juego);
    while (par != NULL){
        Escenarios * escenario = (Escenarios*)par->value;
        if (escenario->items_disp != NULL) list_clean(escenario->items_disp);
        free(escenario->items_disp);
        free(escenario);
        par = nextMap(juego);
    }
    cleanMap(juego);
}

void reiniciar_jugador(Jugador * player, HashMap * juego){
    player->actual = firstMap(juego)->value;
    list_clean(player->inventario);
    player->peso = 0;
    player->puntaje = 0;
    player->tRestante = 10.0;
}

void seleccionOpcion(Jugador *player, HashMap *juego) {
    char op;

    do {
        limpiarPantalla(); 
        mostrar_escenario(player);
        mostrarMenuSolo();
        printf("Ingrese su opcion: ");
        scanf(" %c", &op);

        switch (op) {
            case '1':
                printf("Recogiendo Item\n");
                recoger_items(player);
                break;
            case '2':
                printf("Descartar Item\n");
                descartar_items(player);
                break;
            case '3':
                printf("Avanzar en una direccion\n");
                avanzarEscenario(player, juego);
                break;
            case '4':
                printf("Reiniciar Partida\n");
                limpiar_juego(juego);
                leer_escenarios(juego);
                reiniciar_jugador(player, juego);
                break;
            case '5':
                return;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
                break;
        }

        presioneTeclaParaContinuar();
    } while (op != '6');
}

Jugador * crear_jugador(char nombre[], HashMap * juego){
    Jugador * player = (Jugador*)malloc(sizeof(Jugador));
    strcpy(player->nombre, nombre);
    Pair * inicio = firstMap(juego);
    player->actual = inicio->value;
    player->inventario = list_create();
    player->peso = 0;
    player->puntaje = 0;
    player->tRestante = 10.0;

    return player;
}

int main(){
    
    char opcion;
    char name[50];
    HashMap * juego = createMap(100);
    Jugador *p1 = NULL;
    Jugador *p2 = NULL;
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
            if (p1 == NULL){
                printf("Escribe nombre de usuario: ");
                scanf(" %49s", name);
                getchar();
                p1 = crear_jugador(name, juego);
            }
            seleccionOpcion(p1, juego);
            free(p1);
            break;
        case '3':
            if (p1 == NULL) {
                printf("Escribe nombre de jugador 1: ");
                scanf(" %49s", name);
                getchar();
                p1 = crear_jugador(name, juego);
            }
            if (p2 == NULL) {
                printf("Escribe nombre de jugador 2: ");
                scanf(" %49s", name);
                p2 = crear_jugador(name, juego);
            }
            int turno = 0;
            Jugador *jugadores[2] = {p1, p2};

            while (p1->tRestante > 0 || p2->tRestante > 0) {
                Jugador *actual = jugadores[turno];

                if (actual->tRestante <= 0) {
                    printf("\n%s ya no tiene tiempo restante.\n", actual->nombre);
                    turno = 1 - turno;
                    continue;
                }
                seleccionOpcion(actual, juego);
                printf("\nTurno de %s (Tiempo restante: %d)\n", actual->nombre, actual->tRestante);
                
                turno = 1 - turno;
            }
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