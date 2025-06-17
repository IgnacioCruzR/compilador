#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h" // Incluimos nuestro propio header para tener las declaraciones

// --- DEFINICIÓN DE VARIABLE GLOBAL ---
NodoAST* g_ast_raiz = NULL;

// --- IMPLEMENTACIÓN DE LAS FUNCIONES DE CREACIÓN DE NODOS ---

// Función genérica interna
static NodoAST* crear_nodo(TipoNodoAST tipo) {
    NodoAST* nodo = (NodoAST*)malloc(sizeof(NodoAST));
    if (nodo == NULL) {
        // desde este fichero si no se estructura de otra manera.
        fprintf(stderr, "Error: Memoria insuficiente para nodo AST.\n");
        exit(1);
    }
    nodo->tipo_nodo = tipo;
    memset(&nodo->datos, 0, sizeof(nodo->datos));
    return nodo;
}

// El resto de las funciones que tenías, ahora definidas aquí.
NodoAST* crear_nodo_programa(NodoAST* lista_sentencias) {
    NodoAST* nodo = crear_nodo(NODO_PROGRAMA);
    nodo->datos.programa.lista_sentencias_hijo = lista_sentencias;
    return nodo;
}

NodoAST* crear_nodo_lista_sentencias(NodoAST* sentencia, NodoAST* siguiente) {
    NodoAST* nodo = crear_nodo(NODO_LISTA_SENTENCIAS);
    nodo->datos.lista_sentencias.sentencia = sentencia;
    nodo->datos.lista_sentencias.siguiente = siguiente;
    return nodo;
}

NodoAST* crear_nodo_declaracion(TipoDato tipo, char* id_nombre) {
    NodoAST* nodo = crear_nodo(NODO_DECLARACION);
    nodo->datos.declaracion.tipo_dato = tipo;
    nodo->datos.declaracion.id_nombre = strdup(id_nombre);
    return nodo;
}

NodoAST* crear_nodo_asignacion(char* id_nombre, NodoAST* expr_derecha) {
    NodoAST* nodo = crear_nodo(NODO_ASIGNACION);
    nodo->datos.asignacion.id_nombre = strdup(id_nombre);
    nodo->datos.asignacion.expresion_derecha = expr_derecha;
    return nodo;
}

NodoAST* crear_nodo_entrada(char* id_nombre) {
    NodoAST* nodo = crear_nodo(NODO_ENTRADA);
    nodo->datos.entrada.id_nombre = strdup(id_nombre);
    return nodo;
}

NodoAST* crear_nodo_salida_id(char* id_nombre) {
    NodoAST* nodo = crear_nodo(NODO_SALIDA);
    nodo->datos.salida.id_nombre = strdup(id_nombre);
    nodo->datos.salida.cadena_literal = NULL;
    return nodo;
}

NodoAST* crear_nodo_salida_literal(char* cadena_literal) {
    NodoAST* nodo = crear_nodo(NODO_SALIDA);
    nodo->datos.salida.id_nombre = NULL;
    nodo->datos.salida.cadena_literal = strdup(cadena_literal);
    return nodo;
}

NodoAST* crear_nodo_if(NodoAST* condicion, NodoAST* bloque_then, NodoAST* bloque_else) {
    NodoAST* nodo = crear_nodo(NODO_IF);
    nodo->datos.sentencia_if.condicion = condicion;
    nodo->datos.sentencia_if.bloque_then = bloque_then;
    nodo->datos.sentencia_if.bloque_else = bloque_else;
    return nodo;
}

NodoAST* crear_nodo_expresion_binaria(OperadorAST op, NodoAST* izq, NodoAST* der) {
    NodoAST* nodo = crear_nodo(NODO_EXPRESION_BINARIA);
    nodo->datos.binaria.operador = op;
    nodo->datos.binaria.izquierda = izq;
    nodo->datos.binaria.derecha = der;
    return nodo;
}

NodoAST* crear_nodo_condicion_binaria(OperadorAST op, NodoAST* izq, NodoAST* der) {
    NodoAST* nodo = crear_nodo(NODO_CONDICION_BINARIA);
    nodo->datos.binaria.operador = op;
    nodo->datos.binaria.izquierda = izq;
    nodo->datos.binaria.derecha = der;
    return nodo;
}

NodoAST* crear_nodo_entero(int valor) {
    NodoAST* nodo = crear_nodo(NODO_EXPRESION_ENTERO);
    nodo->datos.entero.valor_entero = valor;
    return nodo;
}

NodoAST* crear_nodo_decimal(float valor) {
    NodoAST* nodo = crear_nodo(NODO_EXPRESION_DECIMAL);
    nodo->datos.decimal.valor_decimal = valor;
    return nodo;
}

NodoAST* crear_nodo_id_expresion(char* id_nombre) {
    NodoAST* nodo = crear_nodo(NODO_EXPRESION_ID);
    nodo->datos.id_expr.id_nombre = strdup(id_nombre);
    return nodo;
}

NodoAST* crear_nodo_cadena_literal_expresion(char* valor_cadena) {
    NodoAST* nodo = crear_nodo(NODO_EXPRESION_CADENA_LITERAL);
    nodo->datos.cadena_literal.valor_cadena = strdup(valor_cadena);
    return nodo;
}