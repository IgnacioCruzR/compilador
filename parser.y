%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VARS 100

// --- Tabla de Símbolos y Tipos de Datos ---
typedef enum {
    TIPO_ENTERO,
    TIPO_DECIMAL,
    TIPO_CADENA,
    TIPO_INDEFINIDO
} TipoDato;

typedef struct {
    char      nombre[32];
    TipoDato  tipo;
    union {
        int   val_entero;
        float val_decimal;
        char* val_cadena;
    } valor;
} Variable;

Variable tabla[MAX_VARS];
int num_vars = 0;

// --- Funciones de la Tabla de Símbolos ---
int buscar_variable(const char* nombre) {
    for (int i = 0; i < num_vars; ++i)
        if (strcmp(tabla[i].nombre, nombre) == 0)
            return i;
    return -1;
}

void yyerror(const char* s);

void declarar_variable(const char* nombre, TipoDato tipo) {
    if (buscar_variable(nombre) != -1) {
        char err_msg[100];
        sprintf(err_msg, "Variable '%s' ya declarada.", nombre);
        yyerror(err_msg);
        exit(1);
    }
    if (num_vars >= MAX_VARS) {
        yyerror("Tabla de símbolos llena.");
        exit(1);
    }
    strcpy(tabla[num_vars].nombre, nombre);
    tabla[num_vars].tipo = tipo;
    if (tipo == TIPO_ENTERO) tabla[num_vars].valor.val_entero = 0;
    else if (tipo == TIPO_DECIMAL) tabla[num_vars].valor.val_decimal = 0.0f;
    else if (tipo == TIPO_CADENA) tabla[num_vars].valor.val_cadena = NULL;
    num_vars++;
}

// --- Estructuras AST ---
typedef enum {
    NODO_PROGRAMA,
    NODO_LISTA_SENTENCIAS,
    NODO_DECLARACION,
    NODO_ASIGNACION,
    NODO_ENTRADA,
    NODO_SALIDA,
    NODO_IF,
    NODO_WHILE,
    NODO_EXPRESION_BINARIA,
    NODO_EXPRESION_ENTERO,
    NODO_EXPRESION_DECIMAL,
    NODO_EXPRESION_ID,
    NODO_EXPRESION_CADENA_LITERAL,
    NODO_CONDICION_BINARIA
} TipoNodoAST;

typedef enum {
    OP_SUMA, OP_RESTA, OP_MULT, OP_DIV,
    OP_IGUAL_IGUAL, OP_DIFERENTE, OP_MENOR, OP_MAYOR, OP_MENOR_IGUAL, OP_MAYOR_IGUAL
} OperadorAST;

typedef struct NodoAST {
    int linenum;
    TipoNodoAST tipo_nodo;
    union {
        struct { struct NodoAST *lista_sentencias_hijo; } programa;
        struct { struct NodoAST *sentencia; struct NodoAST *siguiente; } lista_sentencias;
        struct { TipoDato tipo_dato; char *id_nombre; } declaracion;
        struct { char *id_nombre; struct NodoAST *expresion_derecha; TipoDato tipo_asignacion_esperado; } asignacion;
        struct { char *id_nombre; } entrada;
        struct { char *id_nombre; char *cadena_literal; } salida;
        struct { struct NodoAST *condicion; struct NodoAST *bloque_then; struct NodoAST *bloque_else; } sentencia_if;
        struct { OperadorAST operador; struct NodoAST *izquierda; struct NodoAST *derecha; } expresion_binaria;
        struct { OperadorAST operador; struct NodoAST *izquierda; struct NodoAST *derecha; } condicion_binaria;
        struct { int valor_entero; } entero;
        struct { float valor_decimal; } decimal;
        struct { char *id_nombre; } id_expr;
        struct { char *valor_cadena; } cadena_literal;
        struct { struct NodoAST *condicion; struct NodoAST *bloque; } sentencia_while;
    } datos;
} NodoAST;

// Declaraciones de variables globales que Flex/Bison usan
extern int yylineno;
extern char* yytext;

// --- Funciones para Crear Nodos AST ---
NodoAST* crear_nodo(TipoNodoAST tipo, int linenum) {
    NodoAST* nodo = (NodoAST*)malloc(sizeof(NodoAST));
    if (nodo == NULL) { yyerror("Memoria insuficiente para nodo AST."); exit(1); }
    nodo->tipo_nodo = tipo;
    nodo->linenum = linenum;
    memset(&nodo->datos, 0, sizeof(nodo->datos));
    return nodo;
}
NodoAST* crear_nodo_programa(NodoAST* lista_sentencias, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_PROGRAMA, linenum);
    nodo->datos.programa.lista_sentencias_hijo = lista_sentencias;
    return nodo;
}
NodoAST* crear_nodo_lista_sentencias(NodoAST* sentencia, NodoAST* siguiente, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_LISTA_SENTENCIAS, linenum);
    nodo->datos.lista_sentencias.sentencia = sentencia;
    nodo->datos.lista_sentencias.siguiente = siguiente;
    return nodo;
}
NodoAST* crear_nodo_declaracion(TipoDato tipo, char* id_nombre, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_DECLARACION, linenum);
    nodo->datos.declaracion.tipo_dato = tipo;
    nodo->datos.declaracion.id_nombre = strdup(id_nombre);
    declarar_variable(id_nombre, tipo);
    return nodo;
}
NodoAST* crear_nodo_asignacion(char* id_nombre, NodoAST* expr_derecha, TipoDato tipo_asignacion_esperado, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_ASIGNACION, linenum);
    nodo->datos.asignacion.id_nombre = strdup(id_nombre);
    nodo->datos.asignacion.expresion_derecha = expr_derecha;
    nodo->datos.asignacion.tipo_asignacion_esperado = tipo_asignacion_esperado;
    return nodo;
}
NodoAST* crear_nodo_entrada(char* id_nombre, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_ENTRADA, linenum);
    nodo->datos.entrada.id_nombre = strdup(id_nombre);
    return nodo;
}
NodoAST* crear_nodo_salida_id(char* id_nombre, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_SALIDA, linenum);
    nodo->datos.salida.id_nombre = strdup(id_nombre);
    return nodo;
}
NodoAST* crear_nodo_salida_literal(char* cadena_literal, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_SALIDA, linenum);
    nodo->datos.salida.cadena_literal = strdup(cadena_literal);
    return nodo;
}
NodoAST* crear_nodo_if(NodoAST* condicion, NodoAST* bloque_then, NodoAST* bloque_else, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_IF, linenum);
    nodo->datos.sentencia_if.condicion = condicion;
    nodo->datos.sentencia_if.bloque_then = bloque_then;
    nodo->datos.sentencia_if.bloque_else = bloque_else;
    return nodo;
}
NodoAST* crear_nodo_while(NodoAST* condicion, NodoAST* bloque, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_WHILE, linenum);
    nodo->datos.sentencia_while.condicion = condicion;
    nodo->datos.sentencia_while.bloque = bloque;
    return nodo;
}
NodoAST* crear_nodo_expresion_binaria(OperadorAST op, NodoAST* izq, NodoAST* der, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_EXPRESION_BINARIA, linenum);
    nodo->datos.expresion_binaria.operador = op;
    nodo->datos.expresion_binaria.izquierda = izq;
    nodo->datos.expresion_binaria.derecha = der;
    return nodo;
}
NodoAST* crear_nodo_entero(int valor, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_EXPRESION_ENTERO, linenum);
    nodo->datos.entero.valor_entero = valor;
    return nodo;
}
NodoAST* crear_nodo_decimal(float valor, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_EXPRESION_DECIMAL, linenum);
    nodo->datos.decimal.valor_decimal = valor;
    return nodo;
}
NodoAST* crear_nodo_id_expresion(char* id_nombre, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_EXPRESION_ID, linenum);
    nodo->datos.id_expr.id_nombre = strdup(id_nombre);
    return nodo;
}
NodoAST* crear_nodo_cadena_literal_expresion(char* valor_cadena, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_EXPRESION_CADENA_LITERAL, linenum);
    nodo->datos.cadena_literal.valor_cadena = strdup(valor_cadena);
    return nodo;
}
NodoAST* crear_nodo_condicion_binaria(OperadorAST op, NodoAST* izq, NodoAST* der, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_CONDICION_BINARIA, linenum);
    nodo->datos.condicion_binaria.operador = op;
    nodo->datos.condicion_binaria.izquierda = izq;
    nodo->datos.condicion_binaria.derecha = der;
    return nodo;
}


// --- Variables Globales ---
NodoAST* g_ast_raiz = NULL;
char buffer_fgets[256];
int yylex(void);

%}

/* --- Declaraciones de Bison --- */
%union {
    char* str;
    int   num_int;
    float num_float;
    struct NodoAST *nodo_ast;
}
%token ENTERO DECIMAL CADENA PUNTOYCOMA IGUAL
%token <str> NENTERO NDECIMAL CADENA_LITERAL ID
%token SUMA RESTA MULT DIV LPAREN RPAREN
%token LEER IMPRIMIR
%token IF ELSE WHILE
%token LLAVE_IZQ LLAVE_DER
%token IGUAL_IGUAL DIFERENTE MENOR MAYOR MENOR_IGUAL MAYOR_IGUAL

%precedence ELSE
%type <nodo_ast> programa lista_sentencias sentencia declaracion asignacion entrada salida sentencia_if expresion condicion sentencia_while
%start programa
%left SUMA RESTA
%left MULT DIV

%%

/* --- Reglas de Gramática --- */

programa:
    lista_sentencias {
        $$ = crear_nodo_programa($1, yylineno);
        g_ast_raiz = $$;
    }
    ;

lista_sentencias:
    sentencia { $$ = crear_nodo_lista_sentencias($1, NULL, yylineno); }
    | lista_sentencias sentencia {
        NodoAST* actual = $1;
        while(actual->datos.lista_sentencias.siguiente != NULL) {
            actual = actual->datos.lista_sentencias.siguiente;
        }
        actual->datos.lista_sentencias.siguiente = crear_nodo_lista_sentencias($2, NULL, yylineno);
        $$ = $1;
    }
    | %empty { $$ = NULL; }
    ;

sentencia:
    declaracion { $$ = $1; }
    | asignacion  { $$ = $1; }
    | entrada     { $$ = $1; }
    | salida      { $$ = $1; }
    | sentencia_if{ $$ = $1; }
    | sentencia_while { $$ = $1; }
    ;

declaracion:
    ENTERO ID PUNTOYCOMA  { $$ = crear_nodo_declaracion(TIPO_ENTERO, $2, yylineno); free($2); }
    | DECIMAL ID PUNTOYCOMA { $$ = crear_nodo_declaracion(TIPO_DECIMAL, $2, yylineno); free($2); }
    | CADENA ID PUNTOYCOMA  { $$ = crear_nodo_declaracion(TIPO_CADENA, $2, yylineno); free($2); }
    ;

asignacion:
    ID IGUAL expresion PUNTOYCOMA {
        int idx = buscar_variable($1);
        if (idx == -1) {
            char err_msg[100]; sprintf(err_msg, "Línea %d: Variable no declarada '%s'", yylineno, $1);
            yyerror(err_msg); YYABORT;
        }
        $$ = crear_nodo_asignacion($1, $3, tabla[idx].tipo, yylineno);
        free($1);
    }
    | ID IGUAL CADENA_LITERAL PUNTOYCOMA {
        int idx = buscar_variable($1);
        if (idx == -1) {
            char err_msg[100]; sprintf(err_msg, "Línea %d: Variable no declarada '%s'", yylineno, $1);
            yyerror(err_msg); YYABORT;
        }
        if (tabla[idx].tipo != TIPO_CADENA) {
            char err_msg[100]; sprintf(err_msg, "Línea %d: Incompatibilidad de tipos en asignación para '%s'", yylineno, $1);
            yyerror(err_msg); YYABORT;
        }
        $$ = crear_nodo_asignacion($1, crear_nodo_cadena_literal_expresion($3, yylineno), TIPO_CADENA, yylineno);
        free($1); free($3);
    }
    ;

entrada:
    LEER LPAREN ID RPAREN PUNTOYCOMA { $$ = crear_nodo_entrada($3, yylineno); free($3); }
    ;

salida:
    IMPRIMIR LPAREN ID RPAREN PUNTOYCOMA { $$ = crear_nodo_salida_id($3, yylineno); free($3); }
    | IMPRIMIR LPAREN CADENA_LITERAL RPAREN PUNTOYCOMA { $$ = crear_nodo_salida_literal($3, yylineno); free($3); }
    ;

condicion:
    expresion IGUAL_IGUAL expresion   { $$ = crear_nodo_condicion_binaria(OP_IGUAL_IGUAL, $1, $3, yylineno); }
    | expresion DIFERENTE expresion     { $$ = crear_nodo_condicion_binaria(OP_DIFERENTE, $1, $3, yylineno); }
    | expresion MENOR expresion         { $$ = crear_nodo_condicion_binaria(OP_MENOR, $1, $3, yylineno); }
    | expresion MAYOR expresion         { $$ = crear_nodo_condicion_binaria(OP_MAYOR, $1, $3, yylineno); }
    | expresion MENOR_IGUAL expresion   { $$ = crear_nodo_condicion_binaria(OP_MENOR_IGUAL, $1, $3, yylineno); }
    | expresion MAYOR_IGUAL expresion   { $$ = crear_nodo_condicion_binaria(OP_MAYOR_IGUAL, $1, $3, yylineno); }
    ;

sentencia_if:
    IF LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER
    ELSE LLAVE_IZQ lista_sentencias LLAVE_DER {
        $$ = crear_nodo_if($3, $6, $10, yylineno);
    }
    | IF LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER %prec ELSE {
        $$ = crear_nodo_if($3, $6, NULL, yylineno);
    }
    ;

sentencia_while:
    WHILE LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER {
        $$ = crear_nodo_while($3, $6, yylineno);
    }
    ;

expresion:
    NENTERO               { $$ = crear_nodo_entero(atoi($1), yylineno); free($1); }
    | NDECIMAL            { $$ = crear_nodo_decimal(atof($1), yylineno); free($1); }
    | ID                  { $$ = crear_nodo_id_expresion($1, yylineno); free($1); }
    | expresion SUMA expresion  { $$ = crear_nodo_expresion_binaria(OP_SUMA, $1, $3, yylineno); }
    | expresion RESTA expresion { $$ = crear_nodo_expresion_binaria(OP_RESTA, $1, $3, yylineno); }
    | expresion MULT expresion  { $$ = crear_nodo_expresion_binaria(OP_MULT, $1, $3, yylineno); }
    | expresion DIV expresion   { $$ = crear_nodo_expresion_binaria(OP_DIV, $1, $3, yylineno); }
    | LPAREN expresion RPAREN   { $$ = $2; }
    ;
%%

/* --- SECCIÓN DE GENERACIÓN DE CÓDIGO --- */

// Prototipo de la función auxiliar para expresiones
void generar_codigo_expresion(NodoAST* nodo, FILE* f_salida);

// Función principal que recorre el árbol para generar código
void generar_codigo(NodoAST* nodo, FILE* f_salida) {
    if (!nodo) return;

    switch (nodo->tipo_nodo) {
        case NODO_PROGRAMA:
            generar_codigo(nodo->datos.programa.lista_sentencias_hijo, f_salida);
            break;

        case NODO_LISTA_SENTENCIAS:
            generar_codigo(nodo->datos.lista_sentencias.sentencia, f_salida);
            generar_codigo(nodo->datos.lista_sentencias.siguiente, f_salida);
            break;

        case NODO_DECLARACION:
            switch (nodo->datos.declaracion.tipo_dato) {
                case TIPO_ENTERO:
                    fprintf(f_salida, "    int %s = 0;\n", nodo->datos.declaracion.id_nombre);
                    break;
                case TIPO_CADENA:
                    fprintf(f_salida, "    char* %s = NULL;\n", nodo->datos.declaracion.id_nombre);
                    break;
                default: break;
            }
            break;

        case NODO_ASIGNACION:
            // Para la asignación de cadenas, necesitamos usar strcpy
            if (nodo->datos.asignacion.tipo_asignacion_esperado == TIPO_CADENA) {
                fprintf(f_salida, "    %s = ", nodo->datos.asignacion.id_nombre);
                generar_codigo_expresion(nodo->datos.asignacion.expresion_derecha, f_salida);
                fprintf(f_salida, ";\n");
            } else {
                fprintf(f_salida, "    %s = ", nodo->datos.asignacion.id_nombre);
                generar_codigo_expresion(nodo->datos.asignacion.expresion_derecha, f_salida);
                fprintf(f_salida, ";\n");
            }
            break;

        case NODO_SALIDA: // 'imprimir'
            if (nodo->datos.salida.cadena_literal) {
                fprintf(f_salida, "    printf(\"%s\\n\");\n", nodo->datos.salida.cadena_literal);
            } else {
                int idx = buscar_variable(nodo->datos.salida.id_nombre);
                if (idx != -1) {
                    if (tabla[idx].tipo == TIPO_ENTERO) {
                        fprintf(f_salida, "    printf(\"%%d\\n\", %s);\n", nodo->datos.salida.id_nombre);
                    } else if (tabla[idx].tipo == TIPO_CADENA) {
                        fprintf(f_salida, "    printf(\"%%s\\n\", %s);\n", nodo->datos.salida.id_nombre);
                    }
                }
            }
            break;
        
        case NODO_ENTRADA: // 'leer' - Esto es complejo de traducir directamente a C simple
            fprintf(f_salida, "    {\n");
            fprintf(f_salida, "        char buffer_lectura[256];\n");
            fprintf(f_salida, "        fgets(buffer_lectura, 256, stdin);\n");
            int idx = buscar_variable(nodo->datos.entrada.id_nombre);
             if (idx != -1) {
                    if (tabla[idx].tipo == TIPO_ENTERO) {
                        fprintf(f_salida, "        sscanf(buffer_lectura, \"%%d\", &%s);\n", nodo->datos.entrada.id_nombre);
                    } else if (tabla[idx].tipo == TIPO_CADENA) {
                        fprintf(f_salida, "        buffer_lectura[strcspn(buffer_lectura, \"\\n\")] = 0;\n");
                        fprintf(f_salida, "        %s = strdup(buffer_lectura);\n", nodo->datos.entrada.id_nombre);
                    }
                }
            fprintf(f_salida, "    }\n");
            break;

        case NODO_IF:
            fprintf(f_salida, "    if (");
            generar_codigo_expresion(nodo->datos.sentencia_if.condicion, f_salida);
            fprintf(f_salida, ") {\n");
            generar_codigo(nodo->datos.sentencia_if.bloque_then, f_salida);
            fprintf(f_salida, "    }\n");
            if (nodo->datos.sentencia_if.bloque_else) {
                fprintf(f_salida, "    else {\n");
                generar_codigo(nodo->datos.sentencia_if.bloque_else, f_salida);
                fprintf(f_salida, "    }\n");
            }
            break;

        case NODO_WHILE:
             fprintf(f_salida, "    while (");
             generar_codigo_expresion(nodo->datos.sentencia_while.condicion, f_salida);
             fprintf(f_salida, ") {\n");
             generar_codigo(nodo->datos.sentencia_while.bloque, f_salida);
             fprintf(f_salida, "    }\n");
             break;
        
        default:
            break;
    }
}

// Función auxiliar para generar el código de las expresiones y condiciones
void generar_codigo_expresion(NodoAST* nodo, FILE* f_salida) {
    if (!nodo) return;

    switch (nodo->tipo_nodo) {
        case NODO_EXPRESION_ENTERO:
            fprintf(f_salida, "%d", nodo->datos.entero.valor_entero);
            break;
        case NODO_EXPRESION_ID:
            fprintf(f_salida, "%s", nodo->datos.id_expr.id_nombre);
            break;
        case NODO_EXPRESION_CADENA_LITERAL:
            fprintf(f_salida, "\"%s\"", nodo->datos.cadena_literal.valor_cadena);
            break;

        case NODO_EXPRESION_BINARIA: {
            // --- CORRECCIÓN: Usamos 'expresion_binaria' ---
            char op_char = ' ';
            switch (nodo->datos.expresion_binaria.operador) {
                case OP_SUMA: op_char = '+'; break;
                case OP_RESTA: op_char = '-'; break;
                case OP_MULT: op_char = '*'; break;
                case OP_DIV: op_char = '/'; break;
                default: break; // Los operadores de comparación no deberían estar aquí
            }
            fprintf(f_salida, "(");
            generar_codigo_expresion(nodo->datos.expresion_binaria.izquierda, f_salida);
            fprintf(f_salida, " %c ", op_char);
            generar_codigo_expresion(nodo->datos.expresion_binaria.derecha, f_salida);
            fprintf(f_salida, ")");
            break;
        }

        case NODO_CONDICION_BINARIA: {
            // --- CORRECCIÓN: Usamos 'condicion_binaria' ---
            char* op_str = "";
            switch (nodo->datos.condicion_binaria.operador) {
                case OP_IGUAL_IGUAL: op_str = "=="; break;
                case OP_DIFERENTE:   op_str = "!="; break;
                case OP_MENOR:       op_str = "<";  break;
                case OP_MAYOR:       op_str = ">";  break;
                case OP_MENOR_IGUAL: op_str = "<="; break;
                case OP_MAYOR_IGUAL: op_str = ">="; break;
                default: break; // Los operadores aritméticos no deberían estar aquí
            }
            fprintf(f_salida, "(");
            generar_codigo_expresion(nodo->datos.condicion_binaria.izquierda, f_salida);
            fprintf(f_salida, " %s ", op_str);
            generar_codigo_expresion(nodo->datos.condicion_binaria.derecha, f_salida);
            fprintf(f_salida, ")");
            break;
        }
        default:
            break;
    }
}

void yyerror(const char* s) {
    fprintf(stderr, "Error de sintaxis en la línea %d: Cerca del texto '%s'. Mensaje: %s\n",
            yylineno, yytext, s);
}

int yywrap() {
    return 1;
}