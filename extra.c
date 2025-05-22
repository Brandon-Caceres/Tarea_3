#include "extra.h"


#define MAX_LINE_LENGTH 1024
#define MAX_FIELDS 300

char **leer_linea_csv(FILE *archivo, char separador) {
    static char linea[MAX_LINE_LENGTH];
    static char *campos[MAX_FIELDS];
    char *ptr, *start;
    int idx = 0;

    if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL) {
        return NULL; // No hay más líneas para leer
    }

    // Eliminar salto de línea
    linea[strcspn(linea, "\n")] = '\0';

    ptr = linea;
    while (*ptr && idx < MAX_FIELDS - 1) {
        // Manejar campos vacíos consecutivos
        if (*ptr == separador) {
            campos[idx++] = "";
            ptr++;
            continue;
        }

        // Inicio del campo
        start = ptr;

        // Buscar fin del campo
        while (*ptr && *ptr != separador)
            ptr++;

        // Terminar string y avanzar
        if (*ptr) {
            *ptr = '\0';
            ptr++;
        }

        campos[idx++] = start;
    }

    // Si termina en separador, agregar último campo vacío
    if (*(ptr - 1) == separador && idx < MAX_FIELDS - 1) {
        campos[idx++] = "";
    }

    // Rellenar con campos vacíos si hay menos de MAX_FIELDS
    while (idx < MAX_FIELDS)
        campos[idx++] = "";

    campos[idx] = NULL;
    return campos;
}

List *split_string(const char *str, const char *delim) {
  List *result = list_create();
  char *token = strtok((char *)str, delim);

  while (token != NULL) {
    // Eliminar espacios en blanco al inicio del token
    while (*token == ' ') {
      token++;
    }

    // Eliminar espacios en blanco al final del token
    char *end = token + strlen(token) - 1;
    while (*end == ' ' && end > token) {
      *end = '\0';
      end--;
    }

    // Copiar el token en un nuevo string
    char *new_token = strdup(token);

    // Agregar el nuevo string a la lista
    list_pushBack(result, new_token);

    // Obtener el siguiente token
    token = strtok(NULL, delim);
  }

  return result;
}

// Función para limpiar la pantalla
void limpiarPantalla() { system("cls"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}