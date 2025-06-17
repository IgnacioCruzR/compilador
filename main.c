// main.c (Versión Final con Argumentos)
#include <stdio.h>
#include <stdlib.h>

// --- DECLARACIONES DE ELEMENTOS EXTERNOS ---
struct NodoAST;
void generar_codigo(struct NodoAST* nodo, FILE* f_salida);
int yyparse(void);
extern struct NodoAST* g_ast_raiz;
extern FILE* yyin; // Variable de Flex/Bison para el archivo de entrada

// --- FUNCIÓN MAIN ---
int main(int argc, char *argv[]) { // <-- Acepta argumentos
    printf("Compilador BitStream v4.0 (con argumentos). Iniciando...\n");

    // 1. Verificar si se pasó un nombre de archivo
    if (argc < 2) {
        fprintf(stderr, "Error: Se requiere un nombre de archivo de entrada.\n");
        fprintf(stderr, "Uso: %s <archivo_fuente.bts>\n", argv[0]);
        return 1;
    }

    // 2. Abrir el archivo de entrada para lectura
    FILE* archivo_entrada = fopen(argv[1], "r");
    if (archivo_entrada == NULL) {
        perror("No se pudo abrir el archivo de entrada");
        return 1;
    }

    // 3. ¡CRUCIAL! Apuntar yyin al archivo de entrada
    yyin = archivo_entrada;

    // 4. Iniciar el análisis (ahora leerá del archivo apuntado por yyin)
    if (yyparse() == 0) {
        printf("Análisis sintáctico completado. Generando código C...\n");

        if (g_ast_raiz == NULL) { /* ... */ }

        FILE* archivo_salida = fopen("output.c", "w");
        if (archivo_salida == NULL) { /* ... */ }

        generar_codigo(g_ast_raiz, archivo_salida);
        
        fclose(archivo_salida);
        printf("Código C generado con éxito en 'output.c'.\n");

        printf("Compilando el código C generado con GCC...\n");
        int ret = system("gcc output.c -o resultado");
        if (ret == 0) {
            printf("Compilación exitosa. Ejecutando el programa final...\n\n");
            printf("------ INICIO DE LA SALIDA DEL PROGRAMA COMPILADO ------\n");
            system("./resultado.exe"); // <-- Ahora sí esperará al teclado real
            printf("------- FIN DE LA SALIDA DEL PROGRAMA COMPILADO -------\n");
        } else {
            printf("Error al compilar el código C generado 'output.c'.\n");
        }

    } else {
        printf("\n--- Fallo en el análisis sintáctico. No se generó código. ---\n");
    }

    fclose(archivo_entrada); // Cerrar el archivo de entrada
    return 0;
}