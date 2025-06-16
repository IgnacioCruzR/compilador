// main.c
#include <stdio.h>

// --- DECLARACIONES DE ELEMENTOS EXTERNOS ---

// Declaramos la estructura para que el compilador sepa qué es 'struct NodoAST'
struct NodoAST; 

// Prototipo de la función que está definida en parser.y
void ejecutar_ast(struct NodoAST* nodo);

// Prototipo de la función principal del parser, definida por Bison
int yyparse(void);

// Declaración de la variable global (la definición está en parser.y)
extern struct NodoAST* g_ast_raiz;


// --- FUNCIÓN MAIN ---

int main(void) {
    printf("Intérprete v1.0. Escribe tu código.\n");
    printf("Presiona Ctrl+D (Linux/macOS) o Ctrl+Z (Windows) para finalizar y ejecutar.\n");
    
    if (yyparse() == 0) {
        printf("\n--- Análisis sintáctico completado con éxito ---\n");
        printf("--- Iniciando ejecución del programa ---\n\n");
        
        // ¡Línea clave DESCOMENTADA para ejecutar el árbol!
        ejecutar_ast(g_ast_raiz);
        
        printf("\n--- Ejecución finalizada ---\n");
    } else {
        printf("\n--- Análisis sintáctico fallido ---\n");
    }
    return 0;
}