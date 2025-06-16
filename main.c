// main.c (VERSIÓN LIMPIA)
#include <stdio.h>

// --- DECLARACIONES DE ELEMENTOS EXTERNOS ---
struct NodoAST;
void ejecutar_ast(struct NodoAST* nodo);
int yyparse(void);
extern struct NodoAST* g_ast_raiz;

// --- FUNCIÓN MAIN ---
int main(void) {
    printf("Intérprete v1.0. Escribe tu código.\n");
    printf("Presiona Ctrl+D (Linux/macOS) o Ctrl+Z (Windows) para finalizar y ejecutar.\n");

    if (yyparse() == 0) {
        printf("\n--- Análisis sintáctico completado con éxito ---\n");
        printf("--- Iniciando ejecución del programa ---\n\n");
        ejecutar_ast(g_ast_raiz);
        printf("\n--- Ejecución finalizada ---\n");
    } else {
        printf("\n--- Análisis sintáctico fallido ---\n");
    }
    return 0;
}