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

//Estructura para las direcciones posibles
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

//Estructura para cada jugador
typedef struct{
    char nombre[50];
    Escenarios *actual;
    List *inventario;
    int peso;
    int puntaje;
    int tRestante;
}Jugador;

//Funcion encargada de mostrar el menu principal
void mostrarMenuPrincipal() {
    limpiarPantalla();
    /*albion online es un mmorpg no lineal en el que escribes 
    tu propia historia sin limitarte a seguir un camino prefijado, 
    explora un amplio mundo abierto con cinco biomas unicos, todo 
    cuanto hagas tendra su repercusíon en el mundo, con su economia 
    orientada al jugador de albion los jugadores crean...*/
    puts("========================================");
    puts("            MMORPG NO LINEAL          ");
    puts("========================================");
  
    puts("1) JUGAR EN SOLITARIO");
    puts("2) JUGAR MULTIJUGADOR");
    puts("3) SALIR");
}

/*Funcion encargada de mostrar el menu del juego, adaptandose a 
si se juega en solitario o si se decide jugar en multijugador*/
void mostrarMenu(char *texto) {
    limpiarPantalla();
    puts("========================================");
    //Muestra un titulo diferente basado en si el modo de juego es "solitario" o multijugador
    if (strcmp(texto, "SOLO") == 0) puts("       MMORPG NO LINEAL SOLITARIO       ");
    if (strcmp(texto, "MJ") == 0)   puts("      MMORPG NO LINEAL MULTIJUGADOR       ");
    puts("========================================");
  
    puts("1) RECOGER ITEM(s)");
    puts("2) DESCARTAR ITEM(s)");
    puts("3) AVANZAR EN UNA DIRECCION");
    if (strcmp(texto, "SOLO") == 0)
    {
        puts("4) REINICIAR PARTIDA");
        puts("5) REGRESAR AL MENU PRINCIPAL");
    }
    //Muestra opciones adicionales para el modo multijugador
    if (strcmp(texto, "MJ") == 0)
    {
        puts("4) SALTAR TURNO");
        puts("5) REINICIAR PARTIDA");
        puts("6) REGRESAR AL MENU PRINCIPAL");
    }
}

/*Funcion para leer los escenarios desde un archivo CSV y cargarlos en un HashMap
llamado juego que almacenara los escenarios usando su ID como clave*/
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
        //Inicializa los punteros de direccion a NULL para evitar punteros basura
        escenario->dir_posibles.arriba = NULL;
        escenario->dir_posibles.abajo = NULL;
        escenario->dir_posibles.izquierda = NULL;
        escenario->dir_posibles.derecha = NULL;
        
        //Copia los datos del CSV a la estructura del escenario
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

        strcpy(escenario->id_arriba, campos[4]);
        strcpy(escenario->id_abajo, campos[5]);
        strcpy(escenario->id_izquierda, campos[6]);
        strcpy(escenario->id_derecha, campos[7]);

        strncpy(escenario->esFinal, campos[8], sizeof(escenario->esFinal));

        insertMap(juego, escenario->id, escenario);

    }
    fclose(archivo);

    // Segunda pasada: establecer los punteros de dirección entre escenarios
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

//Función para mostrar los escenarios y los datos de cada jugador en un momento determinado
void mostrar_escenario(Jugador * player){
    printf("JUGADOR: %s\n", player->nombre);
    printf("ESCENARIO: %s\n",player->actual->nombre);
    printf("DESCRIPCION: %s\n", player->actual->descripcion);
    
    //Muestra si hay algún item, o no, en el escenario en el que se encuentra el jugador
    if (player->actual->items_disp != NULL && list_first(player->actual->items_disp) != NULL) {
        printf("ITEMS DISPONIBLES (nombre,peso,valor): \n");
        for (Item * item = list_first(player->actual->items_disp); item != NULL; item = list_next(player->actual->items_disp)){
            printf("  - %s, %dKg, %dpts\n", item->nombre, item->peso, item->valor);
        }
    }
    else printf("NO SE ENCONTRARON ITEMS EN ESTA SALA\n");

    //Muestra el tiempo restante
    printf("TIEMPO RESTANTE: %d segundos\n", player->tRestante);
    
    //Muestra si hay algo en el invntario o si esta vacio
    if (player->inventario != NULL && list_first(player->inventario) != NULL){
        puts("INVENTARIO: ");
        for (Item * objeto = list_first(player->inventario); objeto != NULL; objeto = list_next(player->inventario)){
            printf("  - %s, %dkg, valor %dpts\n", objeto->nombre, objeto->peso, objeto->valor);
        }
    }
    else printf("EL INVENTARIO ESTA VACIO\n");

    //Muestra el peso total y el puntaje acumulado
    printf("PESO TOTAL: %dkg\n", player->peso);
    printf("PUNTAJE ACUMULADO: %dpts\n", player->puntaje);
    
    //Mustra los movimientos posibles o si ya salio del laberinto
    if(strcmp(player->actual->esFinal, "No") == 0){
        printf("DIRECCIONES POSIBLES: ");
        if (player->actual->dir_posibles.arriba != NULL) printf("ARRIBA ");
        if (player->actual->dir_posibles.abajo != NULL) printf("ABAJO ");
        if (player->actual->dir_posibles.izquierda != NULL) printf("IZQUIERDA ");
        if (player->actual->dir_posibles.derecha != NULL) printf("DERECHA ");
        printf("\n");
    }
    else puts("FELICIDADES LOGRASTE SALIR DEL LABERINTO!!!");
}

//Función para recoger un item
int recoger_items(Jugador * player){
    //Mensaje si no hay items en el escenario actual
    limpiarPantalla();
    if (player->actual->items_disp == NULL || list_first(player->actual->items_disp) == NULL){
        puts("NO HAY ITEMES DISPONIBLES EN ESTE ESCENARIO :(");
        return 0;
    }

    while (1){
        //Se muestran lo items disponibles para recoger
        limpiarPantalla();
        puts("ITEMS DISPONIBLES PARA RECOGER");
        int i = 1;
        Item * arreglo_items[100];

        for (Item * item = list_first(player->actual->items_disp); item != NULL; item = list_next(player->actual->items_disp)){
            printf("%d) %s, PESO: %dKg, VALOR: %dpts\n", i, item->nombre, item->peso, item->valor);
            arreglo_items[i - 1] = item;
            i++;
        }
        printf("%d) CANCELAR\n", i);

        //Se pide seleccionar el item a recoger
        int opcion;
        printf("ELIGE EL NUMERO DEL ITEM A RECOGER: ");
        scanf(" %d", &opcion);

        //Mensaje para cancelar o si se selecciono una opción invalida
        if (opcion <= 0 || opcion >= i){
            puts("CANCELANDO U OPCION INVALIDA");
            presioneTeclaParaContinuar();
            return 0;
        }

        //Se actualizan los datos del item recogido tanto en el escenario actual como en el jugador
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

        //Mensaje que muestra el item recogido
        printf("RECOGISTE: %s\n", seleccionado->nombre);

        //Se resta un segundo al tiempo restante del jugador
        player->tRestante -= 1;
        return 1;
    }
}

//Función para descartar items
int descartar_items(Jugador *player){
    limpiarPantalla();

    //Se muestra un mensaje en caso de no haber items en el inventario
    if (player->inventario == NULL || list_first(player->inventario) == NULL){
        puts("NO EXISTEN ITEMS EN EL INVENTARIO");
        return 0;
    }

    while (1)
    {
        limpiarPantalla();

        //Se muestran los items que tiene el jugador
        puts("ITEMS DEL INVENTARIO:");
        int i = 1;
        Item * arreglo_items[100];

        for (Item * item = list_first(player->inventario); item != NULL; item = list_next(player->inventario)){
            printf("%d) %s, PESO: %dKg, VALOR: %dpts\n", i, item->nombre, item->peso, item->valor);
            arreglo_items[i - 1] = item;
            i++;
        }
        printf("%d) CANCELAR\n", i);

        //Se pide el item que se va a descartar del inventario
        int opcion;
        printf("ELIGE EL NUMERO DE ITEM A DESCARTAR: ");
        scanf(" %d", &opcion);

        //Mensaje en caso de se cancele el descarte de item o se seleccione una opción invalida
        if (opcion <= 0 || opcion >= i){
            puts("CANCELANDO U OPCION INVALIDA");
            presioneTeclaParaContinuar();
            return 0;
        }

        //Se actualizan los datos del item descartado tanto en el escenario actual como en el jugador
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

        //Se muestra el item descartado
        printf("DESCARTASTE: %s\n", seleccionado->nombre);

        //Se resta un segundo al tiempo restante del jugador
        player->tRestante -= 1;
        return 1;
    }
}

//Función para moverse por los escenarios
int avanzarEscenario(Jugador *player) {    
    limpiarPantalla();
    Escenarios* actual = player->actual;
    char dir;

    //Muestra las direcciones posibles a las que puede ir el jugador
    printf("\nSELECCIONA LA DIRECCION PATRA AVANZAR:\n");

    // Mostrar solo las direcciones disponibles
    if (actual->dir_posibles.arriba) printf("W - ARRIBA\n");
    if (actual->dir_posibles.abajo) printf("S - ABAJO\n");
    if (actual->dir_posibles.izquierda) printf("A - IZQUIERDA\n");
    if (actual->dir_posibles.derecha) printf("D - DERECHA\n");

    //Se pide que se selecione la dirección a la que el jugador se quiera mover
    printf("DIRECCION (W/A/S/D): ");
    scanf(" %c", &dir);

    switch (dir) {
        case 'W':
        case 'w':
            //Moverse hacia arriba
            if (actual->dir_posibles.arriba)
                player->actual = actual->dir_posibles.arriba;
            else{
                //Mensaje en caso de que no se pueda mover hacia arriba
                printf("NO HAY UN CAMINO HACIA ARRIBA.\n");
                return 0;
            }
            break;
        case 'S':
        case 's':
            //Moverse hacia abajo
            if (actual->dir_posibles.abajo)
                player->actual = actual->dir_posibles.abajo;
            else{
                //Mensaje en caso de que no se pueda mover hacia abajo
                printf("NO HAY UN CAMINO HACIA ABAJO.\n");
                return 0;
            }
            break;
        case 'A':
        case 'a':
            //Moverse hacia la izquierda
            if (actual->dir_posibles.izquierda)
                player->actual = actual->dir_posibles.izquierda;
            else{
                //Mensaje en caso de que no se pueda mover hacia la izquierda
                printf("NO HAY UN CAMINO HACIA LA IZQUIERDA.\n");
                return 0;
            }
            break;
        case 'D':
        case 'd':
            //Moverse hacia la derecha
            if (actual->dir_posibles.derecha)
                player->actual = actual->dir_posibles.derecha;
            else{
                //Mensaje en caso de que no se pueda mover hacia la derecha
                printf("NO HAY UN CAMINO HACIA LA DERECHA.\n");
                return 0;
            }
            break;
        default:
            //Mensaje en caso de seleccionar una opcion invalida
            printf("DIRECCION NO VALIDA. USA SOLO W, A, S, D.\n");
            return 0;
    }

    //Se muestra hacia a donde se avanzo
    printf("\nAVANZASTE AL ESCENARIO: %s\n", player->actual->nombre);
    printf("DESCRIPCION: %s\n", player->actual->descripcion);
    
    //Se actualiza el tiempo restante del jugador
    int tiempo = (player->peso + 1 + 9) / 10;
    player->tRestante -= tiempo;
    return 1;
}

//Función para limpiar los datos del juego
void limpiar_juego(HashMap * juego){
    Pair * par = firstMap(juego);
    while (par != NULL){
        Escenarios * escenario = (Escenarios*)par->value;
        if (escenario->items_disp != NULL) {
            for (Item * item = list_first(escenario->items_disp); item != NULL; item = list_next(escenario->items_disp)) {
                free(item);
            }
            list_clean(escenario->items_disp);
            free(escenario->items_disp);
        }
        free(escenario);
        par = nextMap(juego);
    }
    cleanMap(juego);
}

//Funcion para reiniciar al jugador a su estado inicial
void reiniciar_jugador(Jugador * player, HashMap * juego){
    player->actual = firstMap(juego)->value;
    for (Item * item = list_first(player->inventario); item != NULL; item = list_next(player->inventario)) {
        free(item);
    }
    list_clean(player->inventario);
    free(player->inventario);
    player->peso = 0;
    player->puntaje = 0;
    player->tRestante = 10;
}

//Función para reiniciar la partida al estado inicial
void reiniciar_juego(Jugador * player, HashMap * juego){
    limpiar_juego(juego);
    leer_escenarios(juego);
    reiniciar_jugador(player, juego);
}

//Función para seleccionar opciones en el modo solitario
void seleccionOpcion(Jugador *player, HashMap *juego) {
    char op;
    
    //Se inicia un bucle que termina cuando el jugador haya finalizado su partida, ya sea por tiempo o por llegar a la salida
    while ((player->tRestante > 0 && strcmp(player->actual->esFinal, "Si") != 0))
    {
        //Se muestra los datos del jugador despues de cada turno
        limpiarPantalla();
        mostrar_escenario(player);
        presioneTeclaParaContinuar();

        //Se muestra el menú y se pide una opción
        mostrarMenu("SOLO");
        printf("Ingrese su opcion: ");
        scanf(" %c", &op);

        //Se realizan las acciones según la opción seleccionada
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
                reiniciar_juego(player, juego);
                break;
            case '5':
                reiniciar_juego(player, juego);
                return;
            default:
                printf("OPCION NO VALIDA.\n");
                break;
        }
        presioneTeclaParaContinuar();
    }   

    //Mensaje para mostrar como termino la partida el jugador
    limpiarPantalla();
    if (strcmp(player->actual->esFinal, "Si") == 0){
        printf("\nFIN DEL JUEGO\n");
        printf("PUNTAJE DE %s: %d\n", player->nombre, player->puntaje);
        printf("FELICIDADES, LOGRASTE ESCAPAR\n");
    }
    else printf("TE FALTO TIEMPO PARA PODER ESCAPAR\n");

    //El jugador vuelve a su estado inicial
    reiniciar_juego(player, juego);
    return;
}

//Función para seleccionar opciones en el modo multijugador
void seleccionOpcionMJ(Jugador *player1, Jugador *player2, HashMap *juego) 
{
    char op;
    int turno = 0;
    int mov = 0;
    Jugador *jugadores[2] = {player1, player2};

    //Se inicia un bucle que termina cuando ambos jugadores hayan finalizado su partida, ya sea por tiempo o por llegar a la salida
    while ((player1->tRestante > 0 && strcmp(player1->actual->esFinal, "Si") != 0) || 
           (player2->tRestante > 0 && strcmp(player2->actual->esFinal, "Si") != 0)) {

        Jugador *actual = jugadores[turno];
        
        //Mensaje en caso de que un jugador haya llegado al final o se haya quedado sin tiempo
        if (actual->tRestante <= 0 || strcmp(actual->actual->esFinal, "Si") == 0) {
            printf("\n%s YA NO TIENE TIEMPO RESTANTE O LLEGO AL FINAL.\n", actual->nombre);
            turno = 1 - turno;
            mov = 0;
            continue;
        }

        int max_mov = 2;
        int accion_exitosa = 0;

        //Se muestra los datos del jugador despues de cada turno
        limpiarPantalla();
        mostrar_escenario(actual);
        presioneTeclaParaContinuar();

        //Se muestra el menú y se pide una opción
        mostrarMenu("MJ");
        printf("Ingrese su opcion (%s): ", actual->nombre);
        scanf(" %c", &op);
        
        //En caso de haber una opción no valida se muestra un mensaje indicando esto
        if (op != '1' && op != '2' && op != '3' && op != '4' && op != '5' && op != '6') {
            printf("OPCION NO VALIDA.\n");
            presioneTeclaParaContinuar();
            continue;
        }

        //Se realizan las acciones según la opción seleccionada
        switch (op) {
            case '1':
                accion_exitosa = recoger_items(actual);
                break;
            case '2':
                accion_exitosa = descartar_items(actual);
                break;
            case '3':
                accion_exitosa = avanzarEscenario(actual);
                break;
            case '4':
                turno = 1 - turno;
                mov = 0;
                break;
            case '5':
                reiniciar_juego(player1, juego);
                reiniciar_juego(player2, juego);
                break;
            case '6':
                reiniciar_juego(player1, juego);
                reiniciar_juego(player2, juego);
                return;
            default:
                printf("OPCION NO VALIDA.\n");
                break;
        }

        if (accion_exitosa) mov++;

        //Mensaje que muestra de quien es el turno y se actualiza el turno al siguiente jugador si corresponde
        printf("\nTURNO DE %s (TIEMPO RESTANTE: %d SEGUNDOS)\n", actual->nombre, actual->tRestante);
        if (mov == max_mov) { 
            turno = 1 - turno;
            mov = 0;
        }
        presioneTeclaParaContinuar();
    }

    //Mensajes para mostrar quien gano, perdio o si hay un empate
    printf("\nFIN DEL JUEGO\n");
    printf("\nPuntaje de %s: %d\n", player1->nombre, player1->puntaje);
    printf("Puntaje de %s: %d\n", player2->nombre, player2->puntaje);
    if (strcmp(player1->actual->esFinal, "Si") == 0 && strcmp(player2->actual->esFinal, "Si") == 0){
        if (player1->puntaje > player2->puntaje) printf("\n%s HA GANADO!\n", player1->nombre);
        else if (player2->puntaje > player1->puntaje) printf("\n%s HA GANADO\n", player2->nombre);
        else printf("ES UN EMPATE\n");
    }
    else{
        if (strcmp(player1->actual->nombre, "Salida") == 0) printf("\n%s HA GANADO\n", player1->nombre);
        else printf("%s TE FALTO TIEMPO PARA PODER ESCAPAR\n", player1->nombre);

        if (strcmp(player2->actual->nombre, "Salida") == 0) printf("\n%s HA GANADO\n", player2->nombre);
        else printf("%s TE FALTO TIEMPO PARA PODER ESCAPAR\n", player2->nombre);
    }
    
    //Cada jugador vuelve a su estado inicial
    reiniciar_juego(player1, juego);
    reiniciar_juego(player2, juego);
    return;
}

//Función para crear un jugador nuevo
Jugador * crear_jugador(char nombre[], HashMap * juego){
    //Se reserva memoria
    Jugador * player = (Jugador*)malloc(sizeof(Jugador));

    //Se rellena con los datos iniciales de un nuevo jugador
    strcpy(player->nombre, nombre);
    Pair * inicio = firstMap(juego);
    player->actual = inicio->value;
    player->inventario = list_create();
    player->peso = 0;
    player->puntaje = 0;
    player->tRestante = 10;

    return player;
}

//Función para limpiar los datos de un jugador
void liberar_jugador(Jugador *player) {
    for (Item *item = list_first(player->inventario); item != NULL; item = list_next(player->inventario)) {
        free(item);
    }
    list_clean(player->inventario);
    free(player->inventario);
    free(player);
}

//Función principal
int main(){
    char opcion;
    char name[50];
    HashMap * juego = createMap(100);
    Jugador *p = NULL;
    Jugador *p1 = NULL;
    Jugador *p2 = NULL;

    //Se cargan los escenarios
    leer_escenarios(juego);
    do{
        //Se muestra un menú principal y se selecciona una opción
        mostrarMenuPrincipal();
        printf("Ingrese su opcion: ");
        scanf(" %c", &opcion);

        //Se realizan las acciones según la opción seleccionada
        switch (opcion)
        {
        case '1':
            //Modo solitario
            p = NULL;
            if (p == NULL){
                printf("NOMBRE DE JUGADOR: ");
                scanf(" %49s", name);
                getchar();
                p = crear_jugador(name, juego);
            }
            seleccionOpcion(p, juego);
            break;
        case '2':
            //Modo multijugador
            p1 = NULL;
            p2 = NULL;
            if (p1 == NULL) {
                printf("NOMBRE DEL JUGADOR 1: ");
                scanf(" %49s", name);
                getchar();
                p1 = crear_jugador(name, juego);
            }
            if (p2 == NULL) {
                printf("NOMBRE DEL JUGADOR 2: ");
                scanf(" %49s", name);
                p2 = crear_jugador(name, juego);
            }
            seleccionOpcionMJ(p1, p2, juego);
            break;
        case '3':
            //Terminar el juego
            puts("ABANDONANDO EL JUEGO");
            break;
        default:
            //Muestra de mensaje en caso de seleccionar una opción no valida
            puts("OPCION NO VALIDA");
            break;
        }
        presioneTeclaParaContinuar();
    }while(opcion != '3');

    //Se libera la memoria
    limpiar_juego(juego);
    liberar_jugador(p);
    liberar_jugador(p1);
    liberar_jugador(p2);
    return 0;
}