# Tarea_3

## Descripción de la Tarea
Spotifind es un sistema de base de datos de canciones que permite buscar y explorar canciones. ya sea, por género, artistas o tempo. Las canciones son cargadas desde un archivo CSV y se organizan internamente a traves de hashmaps y listas para lograr ofrecer una búsqueda eficiente. Esto para poder permitir a losa usuarios encontrar fácilmente las canciones según lo que necesiten.

## Funciones implementadas
1. Carga de canciones desde un archivo CSV.
2. Búsqueda de canciones por género.
3. Búsqueda de canciones por artista.
4. Búsqueda de canciones clasificada por tempo (lento, moderado, rápido).
5. Liberación de memoria usada por las estructuras internas.

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
- main.c       # Programa principal
- list.h       # Prototipos de lista enlazada
- list.c       # Implementación de lista enlazada
- hashmap.h    # Prototipos de mapas hash
- hashmap.c    # Implementación de mapas hash
- extra.h      # Prototipos de funciones auxiliares
- extra.c      # Implementación de funbciones auxiliares
- README.md    # Este archivo de texto


### Ejemplo de uso 
1) Cargar Canciones
   > Seleccionar opción: 1
   > "Canciones cargadas con exito"
   
2) Buscar por género
   > Seleccionar opción 2
   > Ingrese género: Pop
   > Se listan canciones del género solicitado.

3) Buscar por artista
   > Seleccionar opción 3
   > Ingrese nombre del artista: Dua Lipa
   > Se listan canciones de ese artista.

4) Buscar por tempo
   > Seleccionar opción 4
   > Elegir entre:

   * 1. Lentas
   * 2. Moderadas
   * 3. Rápidas


## Contribuciones

### [Brandon Cáceres]
- Implementación de lectura CSV y separación de datos.
- Desarrollo de las estructuras de datos: listas y mapas hash.
- Clasificación de canciones por tempo.
- Funcionalidades de búsqueda y visualización.
- Manejo de memoria y liberación estructurada.

## Aspectos a Mejorar
1. Validar formato del CSV al cargar canciones.
2. Permitir guardar canciones nuevas desde la consola.
3. Implementar guardado de playlists personalizadas.
4. Crear menú de ayuda para usuarios nuevos.


# Fin



# SPOTIFIND

## Descripción

Spotifind permite cargar una base de datos de canciones y buscarlas rápida y eficientemente, ya sea por denero, artista o tempo. 

## Cómo compilar y ejecutar

Este sistema ha sido desarrollado en lenguaje C y puede ejecutarse fácilmente utilizando **Visual Studio Code** junto con una extensión para C/C++, como **C/C++ Extension Pack** de Microsoft. Para comenzar a trabajar con el sistema en tu equipo local, sigue estos pasos:

### Requisitos previos:

- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**). Si estás en Windows, se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

### Pasos para compilar y ejecutar:

1. **Descarga y descomprime el** archivo `.zip` en una carpeta de tu elección o **clona el repositorio de Git Hub**.
2. **Abre el proyecto en Visual Studio Code**
    - Inicia Visual Studio Code.
    - FORMA 1: Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde descomprimiste el proyecto.
    - FORMA 2: Selecciona clonar repositorio usando el siguiente link: https://github.com/Joelhu06/tarea2_JosueHuaiquil.git
3. **Compila el código**
    - Abre el archivo principal (por ejemplo, `main.c`).
    - Abre la terminal integrada (`Terminal > Nueva terminal`).
    - En la terminal, compila el programa con el siguiente comando (ajusta el nombre si el archivo principal tiene otro nombre):
        
        ```bash
        gcc main.c tdas/*.c -o test
        ```
        
4. **Ejecuta el programa**
    - Una vez compilado, puedes ejecutar la aplicación con:
        
        ```
        ./test
        ```
        

## Funcionalidades

### Funcionando correctamente:

- Cargar las canciones en sus distintas categorias (Genero, Artista, Tempo) 
- Mostrar un menu interactivo amigable.
- Buscar las canciones según las distintas categorias y mostrarlas por pantalla.
- Finalizar el programa.

### Problemas conocidos:

- El primer dato no se muestra correctamente.

## Ejemplo de uso

Para el uso del programa existen varias opciones para selecionar, las cuales se muestran en el siguiente menú:

```
========================================
       BASE DE DATOS DE CANCIONES
========================================
1) CARGAR CANCIONES
2) BUSCAR POR GENERO
3) BUSCAR POR ARTISTA
4) BUSCAR POR TEMPO
5) SALIR
========================================
INGRESE SU OPCION: 
```

**Opción 1: Cargar canciones**

Se comienza cargando los datos de las canciones.

```
INGRESE SU OPCION: 1) CARGAR CANCIONES
```

Se cargan las canciones y se registran por generos, artistas y tempo.

**Opción 2: Buscar por genero**

Se buscan las canciones por genero, al selecionar un genero se escribe en la consola y luego de una busqueda se imprimen en la pantalla.

Ejemplo:
```
Opción seleccionada: 2) BUSCAR POR GENERO
=======================================
          BUSQUEDA POR GENERO
=======================================
INGRESE EL GENERO DE LA CANCION: acoustic
```

Dentro de las canciones cargadas se buscan todas las que tengan un genero especifico, en el caso del ejemplo se buscan todas aquellas que sean de genero acoustic, y luego en la consola se muestran todas las canciones que sean del mismo genero.

Ejemplo de salida en la consola:
```
CANCION: Waves of Panic
ALBUM: The Shreds
TEMPO: 166
ARTISTAS: Corporate Hearts
------------------------
```

**Opción 3: Buscar por artista**

Se buscan las canciones por artista, al selecionar un artista se escribe en la consola el nombre del artista y luego de una busqueda se muestra en la consola las canciones en las que participo ese artista.

Ejemplo:
```
Opción seleccionada: 3) BUSCAR POR ARTISTA
========================================
          BUSQUEDA POR ARTISTA
========================================
INGRESE EL NOMBRE DEL ARTISTA: SA
```

Dentro de las canciones cargadas se buscan todas las que tengan al artista buscado, en el caso del ejemplo se buscan todas las canciones en las que el artista "SA" haya participado, y luego en la consola se muestran todas sus canciones.

Ejemplo de salida en la consola:
```
CANCION: Oora
ALBUM: Oora
TEMPO: 140
ARTISTA BUSCADO: SA
------------------------
```

**Opción 4: Buscar por tempo**

Se busca una canción según su tempo, ya sean "Lentas", "Moderadas" o "Rapidas", se escribre en la consola el tempo deseado, y luego de una busqueda se muestran en la consola todas las canciones que tengan el tempo buscado.

Ejemplo:
```
Opción seleccionada: 4) BUSCAR POR TEMPO
========================================
           BUSQUEDA POR TEMPO
========================================
INGRESE EL TEMPO DE LA CANCION (Lentas, Moderadas, Rapidas): Lentas
```

Dentro de las canciones cargadas se buscan todas las que tengan el tempo buscado, en el caso del ejemplo se buscan todas las canciones en las que el tempo sea lento ("Lentas"), y luego en la consola se muestran todas las canciones en el que el tempo sea lento.

Ejemplo de salida en la consola:
```
CANCION: Jesus Lover of My Soul - Live
ALBUM: Shout to the Lord (Live)
TEMPO: 72
ARTISTAS: Hillsong Worship, Integrity's Hosanna! Music, Darlene Zschech
------------------------
```

**Opción 5: Salir**

Al seleccionar la opción 5 se finaliza la ejecución del programa.

Ejemplo:
```
Opción seleccionada: 5) SALIR
```