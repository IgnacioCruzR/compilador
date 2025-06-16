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
    TIPO_INDEFINIDO // Para valores de retorno
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

// --- Prototipos de las funciones del Ejecutor AST ---
struct NodoAST;
void ejecutar_ast(struct NodoAST* nodo);
bool evaluar_condicion(struct NodoAST* nodo);

typedef struct {
    TipoDato tipo;
    union {
        int   val_entero;
        float val_decimal;
        char* val_cadena;
    } valor;
} ValorEvaluado;

ValorEvaluado evaluar_expresion(struct NodoAST* nodo);

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
NodoAST* crear_nodo_while(NodoAST* condicion, NodoAST* bloque, int linenum) {
    NodoAST* nodo = crear_nodo(NODO_WHILE, linenum);
    nodo->datos.sentencia_while.condicion = condicion;
    nodo->datos.sentencia_while.bloque = bloque;
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

/* --- Código C Adicional --- */

void ejecutar_ast(NodoAST* nodo) {
    if (!nodo) return;
    switch (nodo->tipo_nodo) {
        case NODO_PROGRAMA:
            ejecutar_ast(nodo->datos.programa.lista_sentencias_hijo);
            break;
        case NODO_LISTA_SENTENCIAS:
            ejecutar_ast(nodo->datos.lista_sentencias.sentencia);
            ejecutar_ast(nodo->datos.lista_sentencias.siguiente);
            break;
        case NODO_DECLARACION:
            break;
        case NODO_ASIGNACION: {
            ValorEvaluado resultado = evaluar_expresion(nodo->datos.asignacion.expresion_derecha);
            int idx = buscar_variable(nodo->datos.asignacion.id_nombre);
            if (idx != -1 && tabla[idx].tipo == resultado.tipo) {
                switch(resultado.tipo) {
                    case TIPO_ENTERO: tabla[idx].valor.val_entero = resultado.valor.val_entero; break;
                    case TIPO_DECIMAL: tabla[idx].valor.val_decimal = resultado.valor.val_decimal; break;
                    case TIPO_CADENA:
                        if (tabla[idx].valor.val_cadena) free(tabla[idx].valor.val_cadena);
                        tabla[idx].valor.val_cadena = strdup(resultado.valor.val_cadena);
                        break;
                    default: break;
                }
            } else {
                 fprintf(stderr, "Línea %d: Error semántico: Incompatibilidad de tipos en la asignación para la variable '%s'.\n", nodo->linenum, nodo->datos.asignacion.id_nombre);
                 exit(1);
            }
            break;
        }
        case NODO_SALIDA:
            if (nodo->datos.salida.cadena_literal) {
                printf("%s\n", nodo->datos.salida.cadena_literal);
            } else {
                int idx = buscar_variable(nodo->datos.salida.id_nombre);
                if (idx != -1) {
                    switch(tabla[idx].tipo) {
                        case TIPO_ENTERO: printf("%d\n", tabla[idx].valor.val_entero); break;
                        case TIPO_DECIMAL: printf("%f\n", tabla[idx].valor.val_decimal); break;
                        case TIPO_CADENA: printf("%s\n", tabla[idx].valor.val_cadena); break;
                        default: break;
                    }
                }
            }
            break;
        case NODO_ENTRADA: {
            int idx = buscar_variable(nodo->datos.entrada.id_nombre);
            if (idx != -1) {
                printf("Ingrese valor para %s (%s): ", tabla[idx].nombre,
                    tabla[idx].tipo == TIPO_ENTERO ? "entero" : (tabla[idx].tipo == TIPO_DECIMAL ? "decimal" : "cadena"));
                fgets(buffer_fgets, sizeof(buffer_fgets), stdin);
                buffer_fgets[strcspn(buffer_fgets, "\n")] = 0;
                switch(tabla[idx].tipo) {
                    case TIPO_ENTERO: tabla[idx].valor.val_entero = atoi(buffer_fgets); break;
                    case TIPO_DECIMAL: tabla[idx].valor.val_decimal = atof(buffer_fgets); break;
                    case TIPO_CADENA:
                        if(tabla[idx].valor.val_cadena) free(tabla[idx].valor.val_cadena);
                        tabla[idx].valor.val_cadena = strdup(buffer_fgets);
                        break;
                    default: break;
                }
            }
            break;
        }
        case NODO_IF:
            if (evaluar_condicion(nodo->datos.sentencia_if.condicion)) {
                ejecutar_ast(nodo->datos.sentencia_if.bloque_then);
            } else if (nodo->datos.sentencia_if.bloque_else) {
                ejecutar_ast(nodo->datos.sentencia_if.bloque_else);
            }
            break;
        case NODO_WHILE:
            // Mientras la condición sea verdadera, ejecuta el bloque de código repetidamente.
            while (evaluar_condicion(nodo->datos.sentencia_while.condicion)) {
                ejecutar_ast(nodo->datos.sentencia_while.bloque);
            }
            break;
                
        default:
             fprintf(stderr, "Línea %d: Error interno del compilador: Nodo de sentencia desconocido.\n", nodo->linenum);
             exit(1);
             
    }
}

ValorEvaluado evaluar_expresion(NodoAST* nodo) {
    ValorEvaluado resultado;
    resultado.tipo = TIPO_INDEFINIDO;
    if (!nodo) {
         fprintf(stderr, "Error semántico: Expresión nula.\n");
         exit(1);
    }
    switch (nodo->tipo_nodo) {
        case NODO_EXPRESION_ENTERO:
            resultado.tipo = TIPO_ENTERO;
            resultado.valor.val_entero = nodo->datos.entero.valor_entero;
            break;
        case NODO_EXPRESION_DECIMAL:
            resultado.tipo = TIPO_DECIMAL;
            resultado.valor.val_decimal = nodo->datos.decimal.valor_decimal;
            break;
        case NODO_EXPRESION_CADENA_LITERAL:
            resultado.tipo = TIPO_CADENA;
            resultado.valor.val_cadena = nodo->datos.cadena_literal.valor_cadena;
            break;
        case NODO_EXPRESION_ID: {
            int idx = buscar_variable(nodo->datos.id_expr.id_nombre);
            if (idx != -1) {
                resultado.tipo = tabla[idx].tipo;
                 switch(resultado.tipo) {
                    case TIPO_ENTERO: resultado.valor.val_entero = tabla[idx].valor.val_entero; break;
                    case TIPO_DECIMAL: resultado.valor.val_decimal = tabla[idx].valor.val_decimal; break;
                    case TIPO_CADENA: resultado.valor.val_cadena = tabla[idx].valor.val_cadena; break;
                    default: break;
                }
            } else {
                fprintf(stderr, "Línea %d: Error semántico: Variable '%s' no definida.\n",
                        nodo->linenum, nodo->datos.id_expr.id_nombre);
                exit(1);
            }
            break;
        }
        case NODO_EXPRESION_BINARIA: {
            ValorEvaluado izq = evaluar_expresion(nodo->datos.expresion_binaria.izquierda);
            ValorEvaluado der = evaluar_expresion(nodo->datos.expresion_binaria.derecha);
            if (izq.tipo == TIPO_ENTERO && der.tipo == TIPO_ENTERO) {
                resultado.tipo = TIPO_ENTERO;
                switch (nodo->datos.expresion_binaria.operador) {
                    case OP_SUMA: resultado.valor.val_entero = izq.valor.val_entero + der.valor.val_entero; break;
                    case OP_RESTA: resultado.valor.val_entero = izq.valor.val_entero - der.valor.val_entero; break;
                    case OP_MULT: resultado.valor.val_entero = izq.valor.val_entero * der.valor.val_entero; break;
                    case OP_DIV:
                        if (der.valor.val_entero == 0) {
                            fprintf(stderr, "Línea %d: Error en ejecución: División por cero.\n", nodo->linenum);
                            exit(1);
                        }
                        resultado.valor.val_entero = izq.valor.val_entero / der.valor.val_entero;
                        break;
                    default:
                        fprintf(stderr, "Línea %d: Error interno: Operador binario desconocido.\n", nodo->linenum);
                        exit(1);
                }
            } else {
                fprintf(stderr, "Línea %d: Error de tipos en expresión. Solo se soportan operaciones entre enteros.\n", nodo->linenum);
                exit(1);
            }
            break;
        }
        default:
             fprintf(stderr, "Línea %d: Error interno: Nodo de expresión desconocido.\n", nodo->linenum);
             exit(1);
    }
    return resultado;
}

bool evaluar_condicion(NodoAST* nodo) {
    ValorEvaluado izq = evaluar_expresion(nodo->datos.condicion_binaria.izquierda);
    ValorEvaluado der = evaluar_expresion(nodo->datos.condicion_binaria.derecha);
    if (izq.tipo == TIPO_ENTERO && der.tipo == TIPO_ENTERO) {
        switch (nodo->datos.condicion_binaria.operador) {
            case OP_IGUAL_IGUAL: return izq.valor.val_entero == der.valor.val_entero;
            case OP_DIFERENTE:   return izq.valor.val_entero != der.valor.val_entero;
            case OP_MENOR:       return izq.valor.val_entero <  der.valor.val_entero;
            case OP_MAYOR:       return izq.valor.val_entero >  der.valor.val_entero;
            case OP_MENOR_IGUAL: return izq.valor.val_entero <= der.valor.val_entero;
            case OP_MAYOR_IGUAL: return izq.valor.val_entero >= der.valor.val_entero;
            default: 
                fprintf(stderr, "Línea %d: Error interno: Operador de condición desconocido.\n", nodo->linenum);
                exit(1);
        }
    } else {
        fprintf(stderr, "Línea %d: Error de tipos en condición. Solo se soportan comparaciones de enteros.\n", nodo->linenum);
        exit(1);
    }
    return false;
}

void yyerror(const char* s) {
    fprintf(stderr, "Error de sintaxis en la línea %d: Cerca del texto '%s'. Mensaje: %s\n",
            yylineno, yytext, s);
}

int yywrap() {
    return 1;
}