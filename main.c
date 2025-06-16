// main.c (Versión para Compilador)
#include <stdio.h>
#include <stdlib.h> // Necesario para la función system()

// --- DECLARACIONES DE ELEMENTOS EXTERNOS ---
struct NodoAST;
void generar_codigo(struct NodoAST* nodo, FILE* f_salida);
int yyparse(void);
extern struct NodoAST* g_ast_raiz;

// --- FUNCIÓN MAIN ---
int main(void) {
    printf("Compilador de Ignacio v2.0. Iniciando...\n");

    if (yyparse() == 0) {
        printf("Análisis sintáctico completado. Generando código C...\n");

        FILE* archivo_salida = fopen("output.c", "w");
        if (archivo_salida == NULL) {
            perror("No se pudo crear el archivo de salida 'output.c'");
            return 1;
        }

        fprintf(archivo_salida, "#include <stdio.h>\n");
        fprintf(archivo_salida, "#include <string.h>\n\n"); // Para strcpy, etc.
        fprintf(archivo_salida, "int main() {\n");

        generar_codigo(g_ast_raiz, archivo_salida);

        fprintf(archivo_salida, "    return 0;\n");
        fprintf(archivo_salida, "}\n");
        fclose(archivo_salida);

        printf("Código C generado con éxito en 'output.c'.\n");

        printf("Compilando el código C generado con GCC...\n");
        int ret = system("gcc output.c -o resultado");
        if (ret == 0) {
            printf("Compilación exitosa. Ejecutando el programa final...\n\n");
            printf("------ INICIO DE LA SALIDA DEL PROGRAMA COMPILADO ------\n");
            system("./resultado.exe"); // En MSYS/Windows. En Linux sería "./resultado"
            printf("------- FIN DE LA SALIDA DEL PROGRAMA COMPILADO -------\n");
        } else {
            printf("Error al compilar el código C generado 'output.c'.\n");
        }

    } else {
        printf("\n--- Fallo en el análisis sintáctico. No se generó código. ---\n");
    }
    return 0;
}