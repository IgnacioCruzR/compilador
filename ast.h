#ifndef AST_H
#define AST_H

#include <stdbool.h>

// Necesitamos la definición de TipoDato aquí. Idealmente, también estaría en su propio header.
typedef enum {
    TIPO_ENTERO,
    TIPO_DECIMAL,
    TIPO_CADENA,
    TIPO_INDEFINIDO
} TipoDato;

// --- ESTRUCTURAS DEL AST ---

typedef enum {
    NODO_PROGRAMA, NODO_LISTA_SENTENCIAS, NODO_DECLARACION, NODO_ASIGNACION,
    NODO_ENTRADA, NODO_SALIDA, NODO_IF, NODO_EXPRESION_BINARIA, NODO_EXPRESION_ENTERO,
    NODO_EXPRESION_DECIMAL, NODO_EXPRESION_ID, NODO_EXPRESION_CADENA_LITERAL, NODO_CONDICION_BINARIA,
} TipoNodoAST;

typedef enum {
    OP_SUMA, OP_RESTA, OP_MULT, OP_DIV,
    OP_IGUAL_IGUAL, OP_DIFERENTE, OP_MENOR, OP_MAYOR, OP_MENOR_IGUAL, OP_MAYOR_IGUAL
} OperadorAST;

typedef struct NodoAST {
    TipoNodoAST tipo_nodo;
    union {
        struct { struct NodoAST *lista_sentencias_hijo; } programa;
        struct { struct NodoAST *sentencia; struct NodoAST *siguiente; } lista_sentencias;
        struct { TipoDato tipo_dato; char *id_nombre; } declaracion;
        struct { char *id_nombre; struct NodoAST *expresion_derecha; } asignacion;
        struct { char *id_nombre; } entrada;
        struct { char *id_nombre; char *cadena_literal; } salida;
        struct { struct NodoAST *condicion; struct NodoAST *bloque_then; struct NodoAST *bloque_else; } sentencia_if;
        struct { OperadorAST operador; struct NodoAST *izquierda; struct NodoAST *derecha; } binaria; // Para expresiones y condiciones
        struct { int valor_entero; } entero;
        struct { float valor_decimal; } decimal;
        struct { char *id_nombre; } id_expr;
        struct { char *valor_cadena; } cadena_literal;
    } datos;
} NodoAST;


// --- PROTOTIPOS DE FUNCIONES (DECLARACIONES) ---

NodoAST* crear_nodo_programa(NodoAST* lista_sentencias);
NodoAST* crear_nodo_lista_sentencias(NodoAST* sentencia, NodoAST* siguiente);
NodoAST* crear_nodo_declaracion(TipoDato tipo, char* id_nombre);
NodoAST* crear_nodo_asignacion(char* id_nombre, NodoAST* expr_derecha);
NodoAST* crear_nodo_entrada(char* id_nombre);
NodoAST* crear_nodo_salida_id(char* id_nombre);
NodoAST* crear_nodo_salida_literal(char* cadena_literal);
NodoAST* crear_nodo_if(NodoAST* condicion, NodoAST* bloque_then, NodoAST* bloque_else);
NodoAST* crear_nodo_expresion_binaria(OperadorAST op, NodoAST* izq, NodoAST* der);
NodoAST* crear_nodo_condicion_binaria(OperadorAST op, NodoAST* izq, NodoAST* der);
NodoAST* crear_nodo_entero(int valor);
NodoAST* crear_nodo_decimal(float valor);
NodoAST* crear_nodo_id_expresion(char* id_nombre);
NodoAST* crear_nodo_cadena_literal_expresion(char* valor_cadena);


// --- DECLARACIÓN DE VARIABLE GLOBAL ---
// 'extern' le dice al compilador: "esta variable existe en alguna parte,
// pero no la crees aquí". La crearemos en ast.c
extern NodoAST* g_ast_raiz;

#endif // AST_H