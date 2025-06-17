/* ================================================================== */
/* --- SECCIÓN 1: DEFINICIONES C Y PROTOTIPOS --- */
/* ================================================================== */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VARS 100

/* --- Definiciones de Tipos y Estructuras --- */
typedef enum {
    TIPO_ENTERO, TIPO_CADENA, TIPO_INDEFINIDO, TIPO_VOID
} TipoDato;

typedef struct {
    char      nombre[32];
    TipoDato  tipo;
} Variable;



typedef enum {
    NODO_PROGRAMA, NODO_LISTA_SENTENCIAS, NODO_DECLARACION, NODO_ASIGNACION,
    NODO_SALIDA, NODO_IF, NODO_WHILE, NODO_EXPRESION_BINARIA,
    NODO_EXPRESION_ENTERO, NODO_EXPRESION_ID, NODO_EXPRESION_CADENA_LITERAL,
    NODO_CONDICION_BINARIA, NODO_DECLARACION_FUNCION, NODO_LLAMADA_FUNCION,
    NODO_SENTENCIA_RETORNO, NODO_LISTA_ARGS,
    NODO_EXPRESION_LEER
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
        struct { char *id_nombre; struct NodoAST *expresion_derecha; } asignacion;
        struct { struct NodoAST* expresion_salida; } salida;
        struct { struct NodoAST *condicion; struct NodoAST *bloque_then; struct NodoAST *bloque_else; } sentencia_if;
        struct { struct NodoAST *condicion; struct NodoAST *bloque; } sentencia_while;
        struct { OperadorAST operador; struct NodoAST *izquierda; struct NodoAST *derecha; } expresion_binaria;
        struct { OperadorAST operador; struct NodoAST *izquierda; struct NodoAST *derecha; } condicion_binaria;
        struct { int valor_entero; } entero;
        struct { char *id_nombre; } id_expr;
        struct { char *valor_cadena; } cadena_literal;
        struct { char* nombre; struct NodoAST* args; } llamada_funcion;
        struct { struct NodoAST* valor_retorno; } retorno;
        struct { char* nombre_param; struct NodoAST* expresion_arg; struct NodoAST* siguiente; } lista_args;
        struct { char* nombre; struct NodoAST* params; struct NodoAST* cuerpo; TipoDato tipo_retorno; } declaracion_funcion;
    } datos;
} NodoAST;

Variable tabla[MAX_VARS];
int num_vars = 0;
NodoAST* g_ast_raiz = NULL;

/* --- Prototipos de Funciones C --- */
void yyerror(const char* s);
int yylex(void);
int buscar_variable(const char* nombre);
NodoAST* crear_nodo(TipoNodoAST tipo, int linenum);
NodoAST* crear_nodo_programa(NodoAST* lista, int linenum);
NodoAST* crear_nodo_lista_sentencias(NodoAST* sentencia, NodoAST* siguiente, int linenum);
NodoAST* crear_nodo_declaracion(TipoDato tipo, char* id, int linenum);
NodoAST* crear_nodo_asignacion(char* id, NodoAST* expr, int linenum);
NodoAST* crear_nodo_salida(NodoAST* expr, int linenum);
NodoAST* crear_nodo_if(NodoAST* cond, NodoAST* then_b, NodoAST* else_b, int linenum);
NodoAST* crear_nodo_while(NodoAST* cond, NodoAST* block, int linenum);
NodoAST* crear_nodo_expresion_binaria(OperadorAST op, NodoAST* izq, NodoAST* der, int linenum);
NodoAST* crear_nodo_entero(int val, int linenum);
NodoAST* crear_nodo_id_expresion(char* id, int linenum);
NodoAST* crear_nodo_cadena_literal_expresion(char* val, int linenum);
NodoAST* crear_nodo_condicion_binaria(OperadorAST op, NodoAST* izq, NodoAST* der, int linenum);
NodoAST* crear_nodo_declaracion_funcion(TipoDato tipo_retorno, char* nombre, NodoAST* params, NodoAST* cuerpo, int linenum);
NodoAST* crear_nodo_llamada_funcion(char* nombre, NodoAST* args, int linenum);
NodoAST* crear_nodo_sentencia_retorno(NodoAST* valor, int linenum);
NodoAST* crear_nodo_lista_args(char* nombre_param, NodoAST* expresion_arg, NodoAST* siguiente, int linenum);
NodoAST* crear_nodo_leer(int linenum);
void generar_codigo(NodoAST* nodo, FILE* f_salida);

// Declaración de variables externas que vienen del lexer
extern int yylineno;
extern char* yytext;
extern FILE* yyin;

%}

/* ================================================================== */
/* --- SECCIÓN 2: DECLARACIONES BISON --- */
/* ================================================================== */
%union {
    char* str;
    int   num_int;
    struct NodoAST *nodo_ast;
}

%token ENTERO CADENA PUNTOYCOMA IGUAL COMA
%token <str> NENTERO CADENA_LITERAL ID NDECIMAL
%token SUMA RESTA MULT DIV LPAREN RPAREN
%token IMPRIMIR
%token IF ELSE WHILE FUNCION RETORNAR VOID
%token LLAVE_IZQ LLAVE_DER
%token IGUAL_IGUAL DIFERENTE MENOR MAYOR MENOR_IGUAL MAYOR_IGUAL
%token DECIMAL LEER

%precedence ELSE
%type <nodo_ast> programa lista_sentencias sentencia declaracion asignacion salida sentencia_if expresion condicion sentencia_while declaracion_funcion llamada_funcion sentencia_retorno lista_parametros lista_argumentos

%start programa
%type <num_int> tipo_retorno
%left SUMA RESTA
%left MULT DIV

/* ================================================================== */
/* --- SECCIÓN 3: REGLAS DE GRAMÁTICA --- */
/* ================================================================== */
%%

programa:
    lista_sentencias {
        $$ = crear_nodo_programa($1, yylineno);
        g_ast_raiz = $$; // <-- ¡ESTA ES LA LÍNEA CLAVE QUE FALTA!
    }
    ;

lista_sentencias:
    %empty { $$ = NULL; }
    | lista_sentencias sentencia { $$ = crear_nodo_lista_sentencias($2, $1, yylineno); }
    ;

sentencia:
    declaracion { $$ = $1; }
    | asignacion  { $$ = $1; }
    | salida      { $$ = $1; }
    | sentencia_if{ $$ = $1; }
    | sentencia_while { $$ = $1; }
    | sentencia_retorno { $$ = $1; }
    | declaracion_funcion { $$ = $1; }
    | expresion PUNTOYCOMA { $$ = $1; }
    ;
tipo_retorno:
    ENTERO { $$ = TIPO_ENTERO; }
    | VOID   { $$ = TIPO_VOID; }
    ;    

declaracion_funcion:
    FUNCION tipo_retorno ID LPAREN lista_parametros RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER {
        $$ = crear_nodo_declaracion_funcion($2, $3, $5, $8, yylineno);
        free($3);
    }
    ;

lista_parametros:
    %empty { $$ = NULL; }
    // Caso base: un solo parámetro
    | ENTERO ID { $$ = crear_nodo_lista_args($2, NULL, NULL, yylineno); free($2); }
    // Caso recursivo por la derecha
    | ENTERO ID COMA lista_parametros { $$ = crear_nodo_lista_args($2, NULL, $4, yylineno); free($2); }

sentencia_retorno:
    RETORNAR expresion PUNTOYCOMA { $$ = crear_nodo_sentencia_retorno($2, yylineno); }
    ;

expresion:
    NENTERO { $$ = crear_nodo_entero(atoi($1), yylineno); free($1); }
    | ID { $$ = crear_nodo_id_expresion($1, yylineno); free($1); }
    | CADENA_LITERAL { $$ = crear_nodo_cadena_literal_expresion($1, yylineno); free($1); }
    | expresion SUMA expresion { $$ = crear_nodo_expresion_binaria(OP_SUMA, $1, $3, yylineno); }
    | expresion RESTA expresion { $$ = crear_nodo_expresion_binaria(OP_RESTA, $1, $3, yylineno); }
    | expresion MULT expresion  { $$ = crear_nodo_expresion_binaria(OP_MULT, $1, $3, yylineno); }
    | expresion DIV expresion   { $$ = crear_nodo_expresion_binaria(OP_DIV, $1, $3, yylineno); }
    | LPAREN expresion RPAREN   { $$ = $2; }
    | llamada_funcion { $$ = $1; }
    | LEER LPAREN RPAREN { $$ = crear_nodo_leer(yylineno); }
    ;

llamada_funcion:
    ID LPAREN lista_argumentos RPAREN { $$ = crear_nodo_llamada_funcion($1, $3, yylineno); free($1); }
    ;

lista_argumentos:
    %empty { $$ = NULL; }
    | expresion { $$ = crear_nodo_lista_args(NULL, $1, NULL, yylineno); }
    | expresion COMA lista_argumentos { $$ = crear_nodo_lista_args(NULL, $1, $3, yylineno); }
    ;

declaracion:
    ENTERO ID PUNTOYCOMA  { $$ = crear_nodo_declaracion(TIPO_ENTERO, $2, yylineno); free($2); }
    | CADENA ID PUNTOYCOMA  { $$ = crear_nodo_declaracion(TIPO_CADENA, $2, yylineno); free($2); }
    ;

asignacion:
    ID IGUAL expresion PUNTOYCOMA {
        $$ = crear_nodo_asignacion($1, $3, yylineno);
        free($1);
    }
    ;

salida:
    IMPRIMIR LPAREN expresion RPAREN PUNTOYCOMA { $$ = crear_nodo_salida($3, yylineno); }
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
    IF LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER { $$ = crear_nodo_if($3, $6, NULL, yylineno); }
  | IF LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER ELSE LLAVE_IZQ lista_sentencias LLAVE_DER { $$ = crear_nodo_if($3, $6, $10, yylineno); }
    ;

sentencia_while:
    WHILE LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER { $$ = crear_nodo_while($3, $6, yylineno); }
    ;

%%
/* ================================================================== */
/* --- SECCIÓN 4: CÓDIGO C ADICIONAL (DEFINICIONES DE FUNCIONES) --- */
/* ================================================================== */


/* --- Implementaciones de Funciones --- */
void yyerror(const char* s) { fprintf(stderr, "Error de sintaxis en línea %d: Cerca del texto '%s'. Mensaje: %s\n", yylineno, yytext, s); }
int yywrap() { return 1; }
int buscar_variable(const char* nombre) { for (int i = 0; i < num_vars; ++i) if (strcmp(tabla[i].nombre, nombre) == 0) return i; return -1; }
void declarar_variable(const char* nombre, TipoDato tipo) { if (buscar_variable(nombre) != -1) { char msg[100]; sprintf(msg, "Variable '%s' ya declarada.", nombre); yyerror(msg); exit(1); } if (num_vars >= MAX_VARS) { yyerror("Tabla de símbolos llena."); exit(1); } strcpy(tabla[num_vars].nombre, nombre); tabla[num_vars].tipo = tipo; num_vars++; }
TipoDato deducir_tipo_expr(NodoAST* nodo) { if (!nodo) return TIPO_INDEFINIDO; switch(nodo->tipo_nodo) { case NODO_EXPRESION_ENTERO: return TIPO_ENTERO; case NODO_EXPRESION_CADENA_LITERAL: return TIPO_CADENA; case NODO_EXPRESION_ID: { int idx = buscar_variable(nodo->datos.id_expr.id_nombre); return (idx != -1) ? tabla[idx].tipo : TIPO_INDEFINIDO; } case NODO_LLAMADA_FUNCION: return TIPO_ENTERO; case NODO_EXPRESION_BINARIA: return TIPO_ENTERO; default: return TIPO_INDEFINIDO; } }

/* --- Implementaciones de crear_nodo_* --- */
NodoAST* crear_nodo(TipoNodoAST tipo, int l) { NodoAST* n=(NodoAST*)malloc(sizeof(NodoAST)); if(!n){yyerror("malloc failed"); exit(1);} n->tipo_nodo=tipo; n->linenum=l; memset(&n->datos,0,sizeof(n->datos)); return n; }
NodoAST* crear_nodo_programa(NodoAST* l, int lin) { NodoAST* n=crear_nodo(NODO_PROGRAMA,lin); n->datos.programa.lista_sentencias_hijo=l; return n; }
NodoAST* crear_nodo_lista_sentencias(NodoAST* s, NodoAST* sig, int l) { NodoAST* n=crear_nodo(NODO_LISTA_SENTENCIAS,l); n->datos.lista_sentencias.sentencia=s; n->datos.lista_sentencias.siguiente=sig; return n; }
NodoAST* crear_nodo_declaracion(TipoDato t, char* id, int l) { NodoAST* n=crear_nodo(NODO_DECLARACION,l); n->datos.declaracion.tipo_dato=t; n->datos.declaracion.id_nombre=strdup(id); declarar_variable(id,t); return n; }
NodoAST* crear_nodo_asignacion(char* id, NodoAST* e, int l) { NodoAST* n=crear_nodo(NODO_ASIGNACION,l); n->datos.asignacion.id_nombre=strdup(id); n->datos.asignacion.expresion_derecha=e; return n; }
NodoAST* crear_nodo_salida(NodoAST* expr, int l) { NodoAST* n=crear_nodo(NODO_SALIDA,l); n->datos.salida.expresion_salida=expr; return n; }
NodoAST* crear_nodo_if(NodoAST* c, NodoAST* t, NodoAST* e, int l) { NodoAST* n=crear_nodo(NODO_IF,l); n->datos.sentencia_if.condicion=c; n->datos.sentencia_if.bloque_then=t; n->datos.sentencia_if.bloque_else=e; return n; }
NodoAST* crear_nodo_while(NodoAST* c, NodoAST* b, int l) { NodoAST* n=crear_nodo(NODO_WHILE,l); n->datos.sentencia_while.condicion=c; n->datos.sentencia_while.bloque=b; return n; }
NodoAST* crear_nodo_expresion_binaria(OperadorAST o, NodoAST* i, NodoAST* d, int l) { NodoAST* n=crear_nodo(NODO_EXPRESION_BINARIA,l); n->datos.expresion_binaria.operador=o; n->datos.expresion_binaria.izquierda=i; n->datos.expresion_binaria.derecha=d; return n; }
NodoAST* crear_nodo_entero(int v, int l) { NodoAST* n=crear_nodo(NODO_EXPRESION_ENTERO,l); n->datos.entero.valor_entero=v; return n; }
NodoAST* crear_nodo_id_expresion(char* id, int l) { NodoAST* n=crear_nodo(NODO_EXPRESION_ID,l); n->datos.id_expr.id_nombre=strdup(id); return n; }
NodoAST* crear_nodo_cadena_literal_expresion(char* v, int l) { NodoAST* n=crear_nodo(NODO_EXPRESION_CADENA_LITERAL,l); n->datos.cadena_literal.valor_cadena=strdup(v); return n; }
NodoAST* crear_nodo_condicion_binaria(OperadorAST o, NodoAST* i, NodoAST* d, int l) { NodoAST* n=crear_nodo(NODO_CONDICION_BINARIA,l); n->datos.condicion_binaria.operador=o; n->datos.condicion_binaria.izquierda=i; n->datos.condicion_binaria.derecha=d; return n; }
NodoAST* crear_nodo_llamada_funcion(char* nom, NodoAST* a, int l) { NodoAST* n=crear_nodo(NODO_LLAMADA_FUNCION,l); n->datos.llamada_funcion.nombre=strdup(nom); n->datos.llamada_funcion.args=a; return n; }
NodoAST* crear_nodo_sentencia_retorno(NodoAST* v, int l) { NodoAST* n=crear_nodo(NODO_SENTENCIA_RETORNO,l); n->datos.retorno.valor_retorno=v; return n; }
NodoAST* crear_nodo_lista_args(char* nom, NodoAST* expr, NodoAST* sig, int l) { NodoAST* n=crear_nodo(NODO_LISTA_ARGS,l); if(nom) n->datos.lista_args.nombre_param=strdup(nom); n->datos.lista_args.expresion_arg=expr; n->datos.lista_args.siguiente=sig; return n; }
NodoAST* crear_nodo_leer(int l) {
    return crear_nodo(NODO_EXPRESION_LEER, l);
}
NodoAST* crear_nodo_declaracion_funcion(TipoDato t, char* nom, NodoAST* p, NodoAST* c, int l) {
    NodoAST* n = crear_nodo(NODO_DECLARACION_FUNCION, l);
    n->datos.declaracion_funcion.tipo_retorno = t;
    n->datos.declaracion_funcion.nombre = strdup(nom);
    n->datos.declaracion_funcion.params = p;
    n->datos.declaracion_funcion.cuerpo = c;
    return n;
}
/* --- Implementaciones del Generador de Código --- */
void generar_codigo_expresion(NodoAST* nodo, FILE* f_salida);

void generar_funciones(NodoAST* nodo, FILE* f_salida) {
    if (!nodo) return;
    if (nodo->tipo_nodo == NODO_LISTA_SENTENCIAS) {
        generar_funciones(nodo->datos.lista_sentencias.siguiente, f_salida);
        if (nodo->datos.lista_sentencias.sentencia && nodo->datos.lista_sentencias.sentencia->tipo_nodo == NODO_DECLARACION_FUNCION) {
            generar_codigo(nodo->datos.lista_sentencias.sentencia, f_salida);
        }
    }
}
void generar_main_body(NodoAST* nodo, FILE* f_salida) {
    if (!nodo) return;
    if (nodo->tipo_nodo == NODO_LISTA_SENTENCIAS) {
        generar_main_body(nodo->datos.lista_sentencias.siguiente, f_salida);
        if (nodo->datos.lista_sentencias.sentencia && nodo->datos.lista_sentencias.sentencia->tipo_nodo != NODO_DECLARACION_FUNCION) {
            generar_codigo(nodo->datos.lista_sentencias.sentencia, f_salida);
        }
    }
}
void generar_codigo(NodoAST* nodo, FILE* f_salida) {
    if (!nodo) return;

    switch (nodo->tipo_nodo) {
        case NODO_PROGRAMA:
            fprintf(f_salida, "#include <stdio.h>\n");
            fprintf(f_salida, "#include <string.h>\n\n");
            fprintf(f_salida, "int leer_entero() {\n");
            fprintf(f_salida, "    int valor;\n");
            fprintf(f_salida, "    scanf(\"%%d\", &valor);\n");
            fprintf(f_salida, "    return valor;\n");
            fprintf(f_salida, "}\n\n");
            generar_funciones(nodo->datos.programa.lista_sentencias_hijo, f_salida);
            fprintf(f_salida, "\nint main() {\n");
            generar_main_body(nodo->datos.programa.lista_sentencias_hijo, f_salida);
            fprintf(f_salida, "    return 0;\n}\n");
            break;

        case NODO_LISTA_SENTENCIAS:
            generar_codigo(nodo->datos.lista_sentencias.siguiente, f_salida);
            generar_codigo(nodo->datos.lista_sentencias.sentencia, f_salida);
            break;
            
        // --- RESTAURAR TODOS LOS CASOS FALTANTES ---
        case NODO_DECLARACION_FUNCION: {
             if (nodo->datos.declaracion_funcion.tipo_retorno == TIPO_VOID) {
                fprintf(f_salida, "void %s(", nodo->datos.declaracion_funcion.nombre);
            } else {
                fprintf(f_salida, "int %s(", nodo->datos.declaracion_funcion.nombre);
           }
            NodoAST* current_param = nodo->datos.declaracion_funcion.params;
            int count = 0;
            while (current_param) {
                if (count > 0) fprintf(f_salida, ", ");
                fprintf(f_salida, "int %s", current_param->datos.lista_args.nombre_param);
                count++;
                current_param = current_param->datos.lista_args.siguiente;
            }
            fprintf(f_salida, ") {\n");
            generar_codigo(nodo->datos.declaracion_funcion.cuerpo, f_salida);
            fprintf(f_salida, "}\n\n");
            break;
        }
        case NODO_SENTENCIA_RETORNO:
            fprintf(f_salida, "    return ");
            generar_codigo_expresion(nodo->datos.retorno.valor_retorno, f_salida);
            fprintf(f_salida, ";\n");
            break;
        case NODO_DECLARACION:
            if (nodo->datos.declaracion.tipo_dato == TIPO_ENTERO) fprintf(f_salida, "    int %s;\n", nodo->datos.declaracion.id_nombre);
            else if (nodo->datos.declaracion.tipo_dato == TIPO_CADENA) fprintf(f_salida, "    char* %s = NULL;\n", nodo->datos.declaracion.id_nombre);
            break;
        case NODO_ASIGNACION:
            fprintf(f_salida, "    %s = ", nodo->datos.asignacion.id_nombre);
            generar_codigo_expresion(nodo->datos.asignacion.expresion_derecha, f_salida);
            fprintf(f_salida, ";\n");
            break;
        case NODO_SALIDA: {
            TipoDato tipo_expr = deducir_tipo_expr(nodo->datos.salida.expresion_salida);
            if (tipo_expr == TIPO_CADENA) {
                fprintf(f_salida, "    printf(\"%%s\\n\", ");
            } else {
                fprintf(f_salida, "    printf(\"%%d\\n\", ");
            }
            generar_codigo_expresion(nodo->datos.salida.expresion_salida, f_salida);
            fprintf(f_salida, ");\n");
            break;
        }
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
        case NODO_LLAMADA_FUNCION:
            fprintf(f_salida, "    "); // Para la indentación
            generar_codigo_expresion(nodo, f_salida); // Reutilizamos la lógica que ya genera la llamada
            fprintf(f_salida, ";\n");                 // Y le añadimos el punto y coma final
            break;    

        // Añadir un default para seguridad
        default:
            // Opcional: imprimir un mensaje si se encuentra un nodo inesperado
            // fprintf(stderr, "Advertencia: Nodo de tipo %d no manejado en generar_codigo.\n", nodo->tipo_nodo);
            break;
    }
}
void generar_codigo_expresion(NodoAST* nodo, FILE* f_salida) {
    if (!nodo) return;
    switch (nodo->tipo_nodo) {
        case NODO_EXPRESION_ENTERO: fprintf(f_salida, "%d", nodo->datos.entero.valor_entero); break;
        case NODO_EXPRESION_ID: fprintf(f_salida, "%s", nodo->datos.id_expr.id_nombre); break;
        case NODO_EXPRESION_CADENA_LITERAL: fprintf(f_salida, "\"%s\"", nodo->datos.cadena_literal.valor_cadena); break;
        case NODO_EXPRESION_LEER:
            fprintf(f_salida, "leer_entero()");
            break;
        case NODO_LLAMADA_FUNCION: {
            fprintf(f_salida, "%s(", nodo->datos.llamada_funcion.nombre);
            NodoAST* current_arg = nodo->datos.llamada_funcion.args;
            int count = 0;
            while(current_arg) {
                if (count > 0) fprintf(f_salida, ", ");
                generar_codigo_expresion(current_arg->datos.lista_args.expresion_arg, f_salida);
                count++;
                current_arg = current_arg->datos.lista_args.siguiente;
            }
            fprintf(f_salida, ")");
            break;
        }
        case NODO_EXPRESION_BINARIA: {
            fprintf(f_salida, "(");
            generar_codigo_expresion(nodo->datos.expresion_binaria.izquierda, f_salida);
            char op = ' ';
            switch(nodo->datos.expresion_binaria.operador){case OP_SUMA:op='+';break; case OP_RESTA:op='-';break; case OP_MULT:op='*';break; case OP_DIV:op='/';break; default:break;}
            fprintf(f_salida, " %c ", op);
            generar_codigo_expresion(nodo->datos.expresion_binaria.derecha, f_salida);
            fprintf(f_salida, ")");
            break;
        }
        case NODO_CONDICION_BINARIA: {
            fprintf(f_salida, "(");
            generar_codigo_expresion(nodo->datos.condicion_binaria.izquierda, f_salida);
            char* op = "";
            switch(nodo->datos.condicion_binaria.operador){case OP_IGUAL_IGUAL:op="==";break; case OP_DIFERENTE:op="!=";break; case OP_MENOR:op="<";break; case OP_MAYOR:op=">";break; case OP_MENOR_IGUAL:op="<=";break; case OP_MAYOR_IGUAL:op=">=";break; default:break;}
            fprintf(f_salida, " %s ", op);
            generar_codigo_expresion(nodo->datos.condicion_binaria.derecha, f_salida);
            fprintf(f_salida, ")");
            break;
        }
        default: break;
    }
}