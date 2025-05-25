# Tarea 3: GraphQuest

## Descripción de la Tarea
GraphQuest es un juego de aventura implementado en lenguaje C, donde el jugador recorre un laberinto representado como un grafo. Cada nodo del grafo es un escenario único que puede contener ítems coleccionables con un valor en puntos y peso. El objetivo principal es tomar decisiones estratégicas para recolectar el mayor puntaje posible y llegar a la salida del laberinto antes que se acabe el tiempo. Este juego además cuenta con un modo en solitario o multijugador

## Funciones implementadas
1-. Carga de escenarios desde un archivo CSV: Se importan los nodos del grafo (escenarios) junto con sus conexiones, ítems y tiempos asociados.
2-. Navegación entre escenarios: El jugador puede desplazarse entre escenarios conectados, gestionando el tiempo disponible.
3-. Recolección y descarte de ítems: El jugador puede recoger ítems disponibles en un escenario y descartarlos desde su inventario.
4-. Gestión de inventario del jugador: Se almacena información de los ítems recogidos, su valor y peso, permitiendo optimizar el puntaje.
5-. Cálculo del puntaje total: Se calcula el puntaje del jugador en función de los ítems recolectados.
6-. Control del tiempo restante: Se actualiza el tiempo disponible según las decisiones del jugador y los tiempos de transición entre escenarios.
7-. Liberación de memoria: Al finalizar el juego, se libera correctamente toda la memoria utilizada por las estructuras dinámicas.

## Compilar el código 

1. Tener Visual Studio Code descargado junto con la extensión de C/C++.
2. Descarga y descomprime el archivo ".zip" en una carpeta a eleccion.
3. Abre el proyecto en Visual Studio Code: 
    - Inicia Visual Studio Code. 
    - Selecciona (Archivos > abrir carpeta..) y elige la carpeta en la que descomprimiste el proyecto.
4. Compila el codigo: 
    - Abre el archivo principal (main.c). 
    - Abre la terminal integrada (Ctrl+Mayus+ñ o Terminal > Nueva Terminal). 
    - En la terminal, compila el codigo con el siguiente comando:
```bash
gcc *.c -o main.exe
```

5. finalmente para ejecutar el codigo hace falta escribir el comando 
```bash
./main.exe
```

## Problemas conocidos 
1. No se puede volver a cargar el archivo CSV más de una vez por ejecución (evita duplicados).
2. Posible pérdida de memoria si se cierra abruptamente sin liberar memoria.
3. No hay validación de campos vacíos o malformateados en el CSV.
4. Sensibilidad a mayúsculas y minúsculas a la hora de realizar una búsqueda.
    * Ejemplo:  Si el artista en el CSV está como "Joseph Sullinger", buscar "joseph sullinger" no arrojará resultados.

## Estructura del código
- extra.c      # Implementación de funbciones auxiliares
- extra.h      # Prototipos de funciones auxiliares
- hashmap.c    # Implementación de mapas hash
- hashmap.h    # Prototipos de mapas hash
- list.c       # Implementación de lista enlazada
- list.h       # Prototipos de lista enlazada
- main.c       # Programa principal
- README.md    # Este archivo de texto


### Ejemplo de uso 
1) Jugar en solitario: Al seleccionar esta opcion primero se pedira el nombre de usuario, luego se mostrara los datos del jugador, al igual que después de cada turno y por último el menú de opciones para jugar
    
    OPCION SELECCIONADA:
    > Ingrese su opcion: 1
    
    INGRESAR EL NOMBRE DE JUGADOR:
    > NOMBRE DE JUGADOR: JOSUE

    DATOS DEL JUGADOR:
    ```
    JUGADOR: BRANDON
    ESCENARIO: Entrada principal
    DESCRIPCION: Una puerta rechinante abre paso a esta mansion olvidada por los dioses y los conserjes. El aire huele a humedad y a misterios sin resolver.
    NO SE ENCONTRARON ITEMS EN ESTA SALA
    TIEMPO RESTANTE: 10.00
    EL INVENTARIO ESTA VACIO
    PESO TOTAL: 0
    PUNTAJE ACUMULADO: 0
    DIRECCIONES POSIBLES: ABAJO 
    ```
    ```
    JUGADOR: JOSUE
    ESCENARIO: Entrada principal
    DESCRIPCION: Una puerta rechinante abre paso a esta mansion olvidada por los dioses y los conserjes. El aire huele a humedad y a misterios sin resolver.
    NO SE ENCONTRARON ITEMS EN ESTA SALA
    TIEMPO RESTANTE: 10.00
    EL INVENTARIO ESTA VACIO
    PESO TOTAL: 0
    PUNTAJE ACUMULADO: 0
    DIRECCIONES POSIBLES: ABAJO 
    ```

    MENÚ DE OPCIONES:
    ```
    ========================================
        MMORPG NO LINEAL SOLITARIO       
    ========================================
    1) RECOGER ITEM(s)
    2) DESCARTAR ITEM(s)
    3) AVANZAR EN UNA DIRECCION
    4) REINICIAR PARTIDA
    5) REGRESAR AL MENU PRINCIPAL
    ``` 
- 1) RECOGER ITEM(s): Se recoge un item, si hay disponibles.
- 2) DESCARTAR ITEM(s): Se deja un item del inventario en el escenario en el que se encuentra el jugador.
- 3) AVANZAR EN UNA DIRECCION: Se avanza en una dirreción (ARRIBA - ABAJO - DERECHA - IZQUIERDA).
- 4) REINICIAR PARTIDA: Se vuelve al estado inicial de la partida.
- 5) REGRESAR AL MENU PRINCIPAL: Se vuelveal menú principal.

2) Jugar Multijugador: Al seleccionar esta opcion primero se pedira el nombre de usuario, luego se mostrara los datos del jugador y por último el menú de opciones para jugar
    
    OPCION SELECCIONADA:
    > Ingrese su opcion: 2
    
    INGRESAR EL NOMBRE DE JUGADOR:
    > NOMBRE DEL JUGADOR 1: BRANDON
    > NOMBRE DEL JUGADOR 2: JOSUE

    DATOS DEL JUGADOR:
    ```
    JUGADOR: JOSUE
    ESCENARIO: Entrada principal
    DESCRIPCION: Una puerta rechinante abre paso a esta mansion olvidada por los dioses y los conserjes. El aire huele a humedad y a misterios sin resolver.
    NO SE ENCONTRARON ITEMS EN ESTA SALA
    TIEMPO RESTANTE: 10.00
    EL INVENTARIO ESTA VACIO
    PESO TOTAL: 0
    PUNTAJE ACUMULADO: 0
    DIRECCIONES POSIBLES: ABAJO 
    ```

    MENÚ DE OPCIONES:
    ```
    ========================================
          MMORPG NO LINEAL MULTIJUGADOR       
    ========================================
    1) RECOGER ITEM(s)
    2) DESCARTAR ITEM(s)
    3) AVANZAR EN UNA DIRECCION
    4) SALTAR TURNO
    5) REINICIAR PARTIDA
    6) REGRESAR AL MENU PRINCIPAL
    ``` 
- 1) RECOGER ITEM(s): Se recoge un item, si hay disponibles.
- 2) DESCARTAR ITEM(s): Se deja un item del inventario en el escenario en el que se encuentra el jugador.
- 3) AVANZAR EN UNA DIRECCION: Se avanza en una dirreción (ARRIBA - ABAJO - DERECHA - IZQUIERDA).
- 4) SALTAR TURNO: Se termina el turno del jugador que esta jugando y se pasa al siguiente.
- 5) REINICIAR PARTIDA: Se vuelve al estado inicial de la partida.
- 6) REGRESAR AL MENU PRINCIPAL: Se vuelveal menú principal.

3) Salir: Se finaliza el juego.

    OPCION SELECCIONADA:
    > Ingrese su opcion: 3

## Contribuciones

### [Brandon Cáceres]
- Implementación de lectura CSV y separación de datos.
- Desarrollo de las estructuras de datos: listas y mapas hash.
- Implementación de la mecánica de recolección y descarte de ítems en los escenarios.
- Manejo de memoria y liberación estructurada.
- Revisión de errores en el código (validación, lógica y funcionamiento general)

### [Josué Huaiquil]
- Implementación del menú principal y flujo del programa (main.c).
- Implementación de la selección de opciones en los modos Solo y Multijugador.
- Implementación del sistema de avance entre escenarios.
- Diseño de la interfaz estética del juego en consola.
- Redacción del README.md

## Aspectos a Mejorar
1. Validar formato del CSV al cargar canciones.
2. Permitir guardar canciones nuevas desde la consola.
3. Implementar guardado de playlists personalizadas.
4. Crear menú de ayuda para usuarios nuevos.

# Fin :)