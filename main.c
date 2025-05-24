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
    puts("========================================");
    puts("       MMORPG NO LINEAL SOLITARIO       ");
    puts("========================================");
  
    puts("1) RECOGER ITEM(s)");
    puts("2) DESCARTAR ITEM(s)");
    puts("3) AVANZAR EN UNA DIRECCION");
    puts("4) REINICIAR PARTIDA");
    puts("5) REGRESAR AL MENU PRINCIPAL");
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

        strcpy(escenario->id_arriba, campos[4]);
        strcpy(escenario->id_abajo, campos[5]);
        strcpy(escenario->id_izquierda, campos[6]);
        strcpy(escenario->id_derecha, campos[7]);

        strncpy(escenario->esFinal, campos[8], sizeof(escenario->esFinal));

        insertMap(juego, escenario->id, escenario);
    }
    fclose(archivo);
    puts("Escenarios cargados con exito");

    List * claves = list_create();
    Pair * par = firstMap(juego);
    while (par != NULL) {
        list_pushBack(claves, par->key);
        par = nextMap(juego);
    }

    // Ahora recorremos esa lista para validar enlaces sin afectar el iterador de juego
    for (char * clave = list_first(claves); clave != NULL; clave = list_next(claves)) {
        Pair * pEscenario = searchMap(juego, clave);
        if (!pEscenario) continue;

        Escenarios * escenario = (Escenarios*)pEscenario->value;

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
    }

    list_clean(claves);
    free(claves);    
}

void mostrar_escenario(Jugador * player){
    printf("JUGADOR: %s\n", player->nombre);
    printf("ESCENARIO: %s\n",player->actual->nombre);
    printf("DESCRIPCION: %s\n", player->actual->descripcion);
    
    if (player->actual->items_disp != NULL && list_first(player->actual->items_disp) != NULL) {
        printf("ITEMS DISPONIBLES (nombre,peso,valor): \n");
        for (Item * item = list_first(player->actual->items_disp); item != NULL; item = list_next(player->actual->items_disp)){
            printf("  - %s, %dkg, valor %d\n", item->nombre, item->peso, item->valor);
        }
    }
    else printf("NO SE ENCONTRARON ITEMS EN ESTA SALA\n");

    printf("TIEMPO RESTANTE: %.2f\n", player->tRestante);
    
    
    if (player->inventario != NULL && list_first(player->inventario) != NULL){
        puts("INVENTARIO: ");
        for (Item * objeto = list_first(player->inventario); objeto != NULL; objeto = list_next(player->inventario)){
            printf("  - %s, %dkg, valor %d\n", objeto->nombre, objeto->peso, objeto->valor);
        }
    }
    else printf("EL INVENTARIO ESTA VACIO\n");

    printf("PESO TOTAL: %d\n", player->peso);
    printf("PUNTAJE ACUMULADO: %d\n", player->puntaje);
    
    if(strcmp(player->actual->esFinal, "No") == 0){
        printf("DIRECCIONES POSIBLES: ");
        if (player->actual->dir_posibles.arriba != NULL) printf("ARRIBA ");
        if (player->actual->dir_posibles.abajo != NULL) printf("ABAJO ");
        if (player->actual->dir_posibles.izquierda != NULL) printf("IZQUIERDA ");
        if (player->actual->dir_posibles.derecha != NULL) printf("DERECHA ");
        printf("\n");
    }
    else puts("Felicidades lograste salir del laberinto con un buen botin!!!");
}

void recoger_items(Jugador * player){
    limpiarPantalla();
    if (player->actual->items_disp == NULL || list_first(player->actual->items_disp) == NULL){
        puts("No hay items disponibles en este escenario :(");
        return;
    }

    while (1){
        limpiarPantalla();
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

        player->tRestante -= 1;
    }
}

void descartar_items(Jugador *player){
    limpiarPantalla();
    if (player->inventario == NULL || list_first(player->inventario) == NULL){
        puts("No existen items en el inventario");
        return;
    }

    while (1){
        limpiarPantalla();
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
        printf("ELIGE EL NUMERO DE ITEM A DESCARTAR: ");
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

        player->tRestante -= 1;
    }
}

void avanzarEscenario(Jugador *player) {
    limpiarPantalla();
    Escenarios* actual = player->actual;
    char dir;

    printf("\nSELECCIONA LA DIRECCION PATRA AVANZAR:\n");

    // Mostrar solo las direcciones disponibles
    if (actual->dir_posibles.arriba) printf("W - ARRIBA\n");
    if (actual->dir_posibles.abajo) printf("S - ABAJO\n");
    if (actual->dir_posibles.izquierda) printf("A - IZQUIERDA\n");
    if (actual->dir_posibles.derecha) printf("D - DERECHA\n");

    printf("DIRECCION (W/A/S/D): ");
    scanf(" %c", &dir);

    switch (dir) {
        case 'W':
        case 'w':
            if (actual->dir_posibles.arriba)
                player->actual = actual->dir_posibles.arriba;
            else
                printf("NO UN HAY CAMINO HACIA ARRIBA.\n");
            break;
        case 'S':
        case 's':
            if (actual->dir_posibles.abajo)
                player->actual = actual->dir_posibles.abajo;
            else
                printf("NO HAY UN CAMINO HACI ABAJO.\n");
            break;
        case 'A':
        case 'a':
            if (actual->dir_posibles.izquierda)
                player->actual = actual->dir_posibles.izquierda;
            else
                printf("NO HAY UN CAMINO HACIA LA IZQUIERDA.\n");
            break;
        case 'D':
        case 'd':
            if (actual->dir_posibles.derecha)
                player->actual = actual->dir_posibles.derecha;
            else
                printf("NO HAY UN CAMINO HACIA LA DERECHA.\n");
            break;
        default:
            printf("Direccion no valida. Usa solo W, A, S o D.\n");
            return;
    }

    printf("\nAvanzaste al escenario: %s\n", player->actual->nombre);
    printf("Descripcion: %s\n", player->actual->descripcion);
    
    float tiempo = (player->peso + 1) / 10.0;
    player->tRestante -= tiempo;
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
        mostrarMenuSolo();
        printf("Ingrese su opcion: ");
        scanf(" %c", &op);

        switch (op) {
            case '1':
                recoger_items(player);
                break;
            case '2':
                descartar_items(player);
                break;
            case '3':
                avanzarEscenario(player);
                break;
            case '4':
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
        
        limpiarPantalla();
        mostrar_escenario(player);
        presioneTeclaParaContinuar();
    } while (op != '6');
}

void seleccionOpcionMJ(Jugador *player, HashMap *juego) {

    char op;
    limpiarPantalla(); 
    mostrarMenuSolo();
    printf("Ingrese su opcion (%s): ", player->nombre);
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
            avanzarEscenario(player);
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
            
    limpiarPantalla();
    mostrar_escenario(player);
    presioneTeclaParaContinuar();
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
                
                printf("\nTurno de %s (Tiempo restante: %d)\n", actual->nombre, actual->tRestante);
                seleccionOpcionMJ(actual, juego);
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