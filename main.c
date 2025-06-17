// main.c (Versión Final con Solución 1)
#include <stdio.h>
#include <stdlib.h>

// --- DECLARACIONES DE ELEMENTOS EXTERNOS ---

// Estructura del AST (declaración adelantada)
struct NodoAST;

// Prototipo principal del generador.
// Las funciones auxiliares (generar_funciones, generar_main_body)
// ya no necesitan ser conocidas por main.c.
void generar_codigo(struct NodoAST* nodo, FILE* f_salida);

// Función del parser generada por Bison
int yyparse(void);

// Variable global que contendrá la raíz del AST, definida en parser.y
extern struct NodoAST* g_ast_raiz;


// --- FUNCIÓN MAIN ---
int main(void) {
    printf("Compilador de Ignacio v3.0 (con funciones). Iniciando...\n");

    if (yyparse() == 0) {
        printf("Análisis sintáctico completado. Generando código C...\n");

        // Asegurarnos de que el AST se creó correctamente
        if (g_ast_raiz == NULL) {
            printf("Error: El análisis fue exitoso pero no se generó el AST.\n");
            return 1;
        }

        FILE* archivo_salida = fopen("output.c", "w");
        if (archivo_salida == NULL) {
            perror("No se pudo crear el archivo de salida 'output.c'");
            return 1;
        }

        // --- LÓGICA DE GENERACIÓN SIMPLIFICADA ---
        // Una sola llamada a la función principal que se encarga de todo.
        generar_codigo(g_ast_raiz, archivo_salida);
        // --- FIN DE LA LÓGICA ---
        
        fclose(archivo_salida);

        printf("Código C generado con éxito en 'output.c'.\n");

        // Compilar y ejecutar el código generado
        printf("Compilando el código C generado con GCC...\n");
        int ret = system("gcc output.c -o resultado");
        if (ret == 0) {
            printf("Compilación exitosa. Ejecutando el programa final...\n\n");
            printf("------ INICIO DE LA SALIDA DEL PROGRAMA COMPILADO ------\n");
            system("./resultado.exe");
            printf("------- FIN DE LA SALIDA DEL PROGRAMA COMPILADO -------\n");
        } else {
            printf("Error al compilar el código C generado 'output.c'.\n");
        }

    } else {
        printf("\n--- Fallo en el análisis sintáctico. No se generó código. ---\n");
    }
    return 0;
}