
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

// Definición de la estructura Libro
typedef struct {
    char titulo[100];
    char autor[50];
    int anio;
    float precio;
} Libro;

// Constantes
#define MAX_LIBROS 10
#define CARPETA_BIBLIOTECA "biblioteca"

// Prototipos de funciones
void crear_carpeta();
void guardar_libro(Libro *l);
void cargar_libro(Libro biblioteca[], int *cantidad);
void ingresar_libro(Libro *l);
void imprimir_libro(Libro l);

int main() {
    Libro biblioteca[MAX_LIBROS];
    int cantidad = 0;
    char opcion;

    crear_carpeta();
    cargar_libro(biblioteca, &cantidad);

    // Menú principal
    do {
        printf("\n--- Gestion de Biblioteca ---\n");
        printf("1. Agregar libro\n");
        printf("2. Mostrar todos los libros\n");
        printf("3. Guardar y salir\n");
        printf("Opcion: ");
        scanf(" %c", &opcion);
        while(getchar() != '\n'); // Limpiar buffer

        switch(opcion) {
        case '1':
            if(cantidad < MAX_LIBROS) {
                ingresar_libro(&biblioteca[cantidad]);
                guardar_libro(&biblioteca[cantidad]);
                cantidad++;
            } else {
                printf("La biblioteca esta llena!\n");
            }
            break;
        case '2':
            for(int i = 0; i < cantidad; i++) {
                imprimir_libro(biblioteca[i]);
            }
            break;
        case '3':
            printf("Saliendo...\n");
            break;
        default:
            printf("Opcion no valida!\n");
        }
    } while(opcion != '3');

    return 0;
}

// Crear carpeta si no existe
void crear_carpeta() {
#ifdef _WIN32
    system("if not exist biblioteca mkdir biblioteca");
#else
    system("mkdir -p biblioteca");
#endif
}

// Guardar libro en archivo individual
void guardar_libro(Libro *l) {
    char nombre_archivo[200];
    // Reemplazar espacios y caracteres no válidos en autor y título
    char autor[50], titulo[100];
    strcpy(autor, l->autor);
    strcpy(titulo, l->titulo);
    for(int i = 0; autor[i]; i++) {
        if(autor[i] == ' ' || autor[i] == '/' || autor[i] == '\\') autor[i] = '_';
    }
    for(int i = 0; titulo[i]; i++) {
        if(titulo[i] == ' ' || titulo[i] == '/' || titulo[i] == '\\') titulo[i] = '_';
    }
#ifdef _WIN32
    sprintf(nombre_archivo, "%s\\%s_%s.txt", CARPETA_BIBLIOTECA, autor, titulo);
#else
    sprintf(nombre_archivo, "%s/%s_%s.txt", CARPETA_BIBLIOTECA, autor, titulo);
#endif

    FILE *archivo = fopen(nombre_archivo, "w");
    if(archivo == NULL) {
        printf("Error al crear el archivo para el libro.\n");
        return;
    }

    fprintf(archivo, "Titulo: %s\n", l->titulo);
    fprintf(archivo, "Autor: %s\n", l->autor);
    fprintf(archivo, "Anio: %d\n", l->anio);
    fprintf(archivo, "Precio: %.2f\n", l->precio);

    fclose(archivo);
    printf("Libro guardado en: %s\n", nombre_archivo);
}

// Cargar libros desde archivos al iniciar
void cargar_libro(Libro biblioteca[], int *cantidad) {
    DIR *dir;
    struct dirent *ent;

    if((dir = opendir(CARPETA_BIBLIOTECA)) != NULL) {
        while((ent = readdir(dir)) != NULL && *cantidad < MAX_LIBROS) {
            if(strstr(ent->d_name, ".txt")) {
                char ruta_archivo[256];
#ifdef _WIN32
                sprintf(ruta_archivo, "%s\\%s", CARPETA_BIBLIOTECA, ent->d_name);
#else
                sprintf(ruta_archivo, "%s/%s", CARPETA_BIBLIOTECA, ent->d_name);
#endif
                FILE *archivo = fopen(ruta_archivo, "r");
                if(archivo) {
                    Libro l = {"", "", 0, 0.0f};
                    char linea[150];
                    while(fgets(linea, sizeof(linea), archivo)) {
                        if(strncmp(linea, "Titulo: ", 8) == 0) {
                            sscanf(linea + 8, "%99[^\n]", l.titulo);
                        } else if(strncmp(linea, "Autor: ", 7) == 0) {
                            sscanf(linea + 7, "%49[^\n]", l.autor);
                        } else if(strncmp(linea, "Anio: ", 6) == 0) {
                            sscanf(linea + 6, "%d", &l.anio);
                        } else if(strncmp(linea, "Precio: ", 8) == 0) {
                            sscanf(linea + 8, "%f", &l.precio);
                        }
                    }
                    biblioteca[*cantidad] = l;
                    (*cantidad)++;
                    fclose(archivo);
                }
            }
        }
        closedir(dir);
    }
}

// Ingresar libro por teclado
void ingresar_libro(Libro *l) {
    printf("Ingrese titulo: ");
    fgets(l->titulo, sizeof(l->titulo), stdin);
    l->titulo[strcspn(l->titulo, "\n")] = '\0';

    printf("Ingrese autor: ");
    fgets(l->autor, sizeof(l->autor), stdin);
    l->autor[strcspn(l->autor, "\n")] = '\0';

    printf("Ingrese anio: ");
    scanf("%d", &l->anio);
    printf("Ingrese precio: ");
    scanf("%f", &l->precio);
    while(getchar() != '\n'); // Limpiar buffer
}

// Imprimir libro
void imprimir_libro(Libro l) {
    printf("\nDetalles del libro:\n");
    printf("Titulo: %s\n", l.titulo);
    printf("Autor: %s\n", l.autor);
    printf("Anio: %d\n", l.anio);
    printf("Precio: $%.2f\n", l.precio);
}
