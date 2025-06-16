/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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


#line 282 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ENTERO = 3,                     /* ENTERO  */
  YYSYMBOL_DECIMAL = 4,                    /* DECIMAL  */
  YYSYMBOL_CADENA = 5,                     /* CADENA  */
  YYSYMBOL_PUNTOYCOMA = 6,                 /* PUNTOYCOMA  */
  YYSYMBOL_IGUAL = 7,                      /* IGUAL  */
  YYSYMBOL_NENTERO = 8,                    /* NENTERO  */
  YYSYMBOL_NDECIMAL = 9,                   /* NDECIMAL  */
  YYSYMBOL_CADENA_LITERAL = 10,            /* CADENA_LITERAL  */
  YYSYMBOL_ID = 11,                        /* ID  */
  YYSYMBOL_SUMA = 12,                      /* SUMA  */
  YYSYMBOL_RESTA = 13,                     /* RESTA  */
  YYSYMBOL_MULT = 14,                      /* MULT  */
  YYSYMBOL_DIV = 15,                       /* DIV  */
  YYSYMBOL_LPAREN = 16,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 17,                    /* RPAREN  */
  YYSYMBOL_LEER = 18,                      /* LEER  */
  YYSYMBOL_IMPRIMIR = 19,                  /* IMPRIMIR  */
  YYSYMBOL_IF = 20,                        /* IF  */
  YYSYMBOL_ELSE = 21,                      /* ELSE  */
  YYSYMBOL_WHILE = 22,                     /* WHILE  */
  YYSYMBOL_LLAVE_IZQ = 23,                 /* LLAVE_IZQ  */
  YYSYMBOL_LLAVE_DER = 24,                 /* LLAVE_DER  */
  YYSYMBOL_IGUAL_IGUAL = 25,               /* IGUAL_IGUAL  */
  YYSYMBOL_DIFERENTE = 26,                 /* DIFERENTE  */
  YYSYMBOL_MENOR = 27,                     /* MENOR  */
  YYSYMBOL_MAYOR = 28,                     /* MAYOR  */
  YYSYMBOL_MENOR_IGUAL = 29,               /* MENOR_IGUAL  */
  YYSYMBOL_MAYOR_IGUAL = 30,               /* MAYOR_IGUAL  */
  YYSYMBOL_YYACCEPT = 31,                  /* $accept  */
  YYSYMBOL_programa = 32,                  /* programa  */
  YYSYMBOL_lista_sentencias = 33,          /* lista_sentencias  */
  YYSYMBOL_sentencia = 34,                 /* sentencia  */
  YYSYMBOL_declaracion = 35,               /* declaracion  */
  YYSYMBOL_asignacion = 36,                /* asignacion  */
  YYSYMBOL_entrada = 37,                   /* entrada  */
  YYSYMBOL_salida = 38,                    /* salida  */
  YYSYMBOL_condicion = 39,                 /* condicion  */
  YYSYMBOL_sentencia_if = 40,              /* sentencia_if  */
  YYSYMBOL_sentencia_while = 41,           /* sentencia_while  */
  YYSYMBOL_expresion = 42                  /* expresion  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  26
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   120

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  12
/* YYNRULES -- Number of rules.  */
#define YYNRULES  36
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  85

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   285


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   238,   238,   245,   246,   254,   258,   259,   260,   261,
     262,   263,   267,   268,   269,   273,   282,   298,   302,   303,
     307,   308,   309,   310,   311,   312,   316,   320,   326,   332,
     333,   334,   335,   336,   337,   338,   339
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ENTERO", "DECIMAL",
  "CADENA", "PUNTOYCOMA", "IGUAL", "NENTERO", "NDECIMAL", "CADENA_LITERAL",
  "ID", "SUMA", "RESTA", "MULT", "DIV", "LPAREN", "RPAREN", "LEER",
  "IMPRIMIR", "IF", "ELSE", "WHILE", "LLAVE_IZQ", "LLAVE_DER",
  "IGUAL_IGUAL", "DIFERENTE", "MENOR", "MAYOR", "MENOR_IGUAL",
  "MAYOR_IGUAL", "$accept", "programa", "lista_sentencias", "sentencia",
  "declaracion", "asignacion", "entrada", "salida", "condicion",
  "sentencia_if", "sentencia_while", "expresion", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-63)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      71,    -2,     5,    11,    40,     9,    33,    34,    42,    54,
      71,   -63,   -63,   -63,   -63,   -63,   -63,   -63,    60,    63,
      65,    35,    49,     1,    -3,    -3,   -63,   -63,   -63,   -63,
     -63,   -63,   -63,    66,   -63,    -3,    18,    48,    75,    77,
      78,    72,    79,   -63,    91,   -63,    -3,    -3,    -3,    -3,
      74,    82,   101,    90,    -3,    -3,    -3,    -3,    -3,    -3,
      92,   -63,    38,    38,   -63,   -63,   -63,   -63,   -63,    71,
      97,    97,    97,    97,    97,    97,    71,    -1,    37,    93,
     -63,    94,    71,    59,   -63
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     3,     6,     7,     8,     9,    10,    11,     0,     0,
       0,     0,     0,     0,     0,     0,     1,     4,    12,    13,
      14,    29,    30,     0,    31,     0,     0,     0,     0,     0,
       0,     0,     0,    16,     0,    15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    36,    32,    33,    34,    35,    17,    19,    18,     5,
      20,    21,    22,    23,    24,    25,     5,     0,     0,    27,
      28,     0,     5,     0,    26
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -63,   -63,   -62,   -10,   -63,   -63,   -63,   -63,    95,   -63,
     -63,   -20
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     9,    10,    11,    12,    13,    14,    15,    40,    16,
      17,    41
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      27,    36,     1,     2,     3,    31,    32,    77,    34,    18,
       4,    38,    39,    35,    78,    44,    19,     5,     6,     7,
      83,     8,    20,    79,    45,    22,    62,    63,    64,    65,
      46,    47,    48,    49,    70,    71,    72,    73,    74,    75,
       1,     2,     3,    31,    32,    33,    34,    21,     4,    23,
      24,    35,    48,    49,    26,     5,     6,     7,    25,     8,
      37,    80,     1,     2,     3,    50,    28,    27,    27,    29,
       4,    30,    43,    27,     1,     2,     3,     5,     6,     7,
      66,     8,     4,    84,    46,    47,    48,    49,    67,     5,
       6,     7,    51,     8,    52,    53,    60,    54,    55,    56,
      57,    58,    59,    46,    47,    48,    49,    68,    61,    46,
      47,    48,    49,    69,    81,    76,     0,    82,     0,     0,
      42
};

static const yytype_int8 yycheck[] =
{
      10,    21,     3,     4,     5,     8,     9,    69,    11,    11,
      11,    10,    11,    16,    76,    35,    11,    18,    19,    20,
      82,    22,    11,    24,     6,    16,    46,    47,    48,    49,
      12,    13,    14,    15,    54,    55,    56,    57,    58,    59,
       3,     4,     5,     8,     9,    10,    11,     7,    11,    16,
      16,    16,    14,    15,     0,    18,    19,    20,    16,    22,
      11,    24,     3,     4,     5,    17,     6,    77,    78,     6,
      11,     6,     6,    83,     3,     4,     5,    18,    19,    20,
       6,    22,    11,    24,    12,    13,    14,    15,     6,    18,
      19,    20,    17,    22,    17,    17,    17,    25,    26,    27,
      28,    29,    30,    12,    13,    14,    15,     6,    17,    12,
      13,    14,    15,    23,    21,    23,    -1,    23,    -1,    -1,
      25
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,    11,    18,    19,    20,    22,    32,
      33,    34,    35,    36,    37,    38,    40,    41,    11,    11,
      11,     7,    16,    16,    16,    16,     0,    34,     6,     6,
       6,     8,     9,    10,    11,    16,    42,    11,    10,    11,
      39,    42,    39,     6,    42,     6,    12,    13,    14,    15,
      17,    17,    17,    17,    25,    26,    27,    28,    29,    30,
      17,    17,    42,    42,    42,    42,     6,     6,     6,    23,
      42,    42,    42,    42,    42,    42,    23,    33,    33,    24,
      24,    21,    23,    33,    24
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    33,    33,    34,    34,    34,    34,
      34,    34,    35,    35,    35,    36,    36,    37,    38,    38,
      39,    39,    39,    39,    39,    39,    40,    40,    41,    42,
      42,    42,    42,    42,    42,    42,    42
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     0,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     4,     4,     5,     5,     5,
       3,     3,     3,     3,     3,     3,    11,     7,     7,     1,
       1,     1,     3,     3,     3,     3,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* programa: lista_sentencias  */
#line 238 "parser.y"
                     {
        (yyval.nodo_ast) = crear_nodo_programa((yyvsp[0].nodo_ast), yylineno);
        g_ast_raiz = (yyval.nodo_ast);
    }
#line 1368 "parser.tab.c"
    break;

  case 3: /* lista_sentencias: sentencia  */
#line 245 "parser.y"
              { (yyval.nodo_ast) = crear_nodo_lista_sentencias((yyvsp[0].nodo_ast), NULL, yylineno); }
#line 1374 "parser.tab.c"
    break;

  case 4: /* lista_sentencias: lista_sentencias sentencia  */
#line 246 "parser.y"
                                 {
        NodoAST* actual = (yyvsp[-1].nodo_ast);
        while(actual->datos.lista_sentencias.siguiente != NULL) {
            actual = actual->datos.lista_sentencias.siguiente;
        }
        actual->datos.lista_sentencias.siguiente = crear_nodo_lista_sentencias((yyvsp[0].nodo_ast), NULL, yylineno);
        (yyval.nodo_ast) = (yyvsp[-1].nodo_ast);
    }
#line 1387 "parser.tab.c"
    break;

  case 5: /* lista_sentencias: %empty  */
#line 254 "parser.y"
             { (yyval.nodo_ast) = NULL; }
#line 1393 "parser.tab.c"
    break;

  case 6: /* sentencia: declaracion  */
#line 258 "parser.y"
                { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1399 "parser.tab.c"
    break;

  case 7: /* sentencia: asignacion  */
#line 259 "parser.y"
                  { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1405 "parser.tab.c"
    break;

  case 8: /* sentencia: entrada  */
#line 260 "parser.y"
                  { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1411 "parser.tab.c"
    break;

  case 9: /* sentencia: salida  */
#line 261 "parser.y"
                  { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1417 "parser.tab.c"
    break;

  case 10: /* sentencia: sentencia_if  */
#line 262 "parser.y"
                  { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1423 "parser.tab.c"
    break;

  case 11: /* sentencia: sentencia_while  */
#line 263 "parser.y"
                      { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1429 "parser.tab.c"
    break;

  case 12: /* declaracion: ENTERO ID PUNTOYCOMA  */
#line 267 "parser.y"
                          { (yyval.nodo_ast) = crear_nodo_declaracion(TIPO_ENTERO, (yyvsp[-1].str), yylineno); free((yyvsp[-1].str)); }
#line 1435 "parser.tab.c"
    break;

  case 13: /* declaracion: DECIMAL ID PUNTOYCOMA  */
#line 268 "parser.y"
                            { (yyval.nodo_ast) = crear_nodo_declaracion(TIPO_DECIMAL, (yyvsp[-1].str), yylineno); free((yyvsp[-1].str)); }
#line 1441 "parser.tab.c"
    break;

  case 14: /* declaracion: CADENA ID PUNTOYCOMA  */
#line 269 "parser.y"
                            { (yyval.nodo_ast) = crear_nodo_declaracion(TIPO_CADENA, (yyvsp[-1].str), yylineno); free((yyvsp[-1].str)); }
#line 1447 "parser.tab.c"
    break;

  case 15: /* asignacion: ID IGUAL expresion PUNTOYCOMA  */
#line 273 "parser.y"
                                  {
        int idx = buscar_variable((yyvsp[-3].str));
        if (idx == -1) {
            char err_msg[100]; sprintf(err_msg, "Línea %d: Variable no declarada '%s'", yylineno, (yyvsp[-3].str));
            yyerror(err_msg); YYABORT;
        }
        (yyval.nodo_ast) = crear_nodo_asignacion((yyvsp[-3].str), (yyvsp[-1].nodo_ast), tabla[idx].tipo, yylineno);
        free((yyvsp[-3].str));
    }
#line 1461 "parser.tab.c"
    break;

  case 16: /* asignacion: ID IGUAL CADENA_LITERAL PUNTOYCOMA  */
#line 282 "parser.y"
                                         {
        int idx = buscar_variable((yyvsp[-3].str));
        if (idx == -1) {
            char err_msg[100]; sprintf(err_msg, "Línea %d: Variable no declarada '%s'", yylineno, (yyvsp[-3].str));
            yyerror(err_msg); YYABORT;
        }
        if (tabla[idx].tipo != TIPO_CADENA) {
            char err_msg[100]; sprintf(err_msg, "Línea %d: Incompatibilidad de tipos en asignación para '%s'", yylineno, (yyvsp[-3].str));
            yyerror(err_msg); YYABORT;
        }
        (yyval.nodo_ast) = crear_nodo_asignacion((yyvsp[-3].str), crear_nodo_cadena_literal_expresion((yyvsp[-1].str), yylineno), TIPO_CADENA, yylineno);
        free((yyvsp[-3].str)); free((yyvsp[-1].str));
    }
#line 1479 "parser.tab.c"
    break;

  case 17: /* entrada: LEER LPAREN ID RPAREN PUNTOYCOMA  */
#line 298 "parser.y"
                                     { (yyval.nodo_ast) = crear_nodo_entrada((yyvsp[-2].str), yylineno); free((yyvsp[-2].str)); }
#line 1485 "parser.tab.c"
    break;

  case 18: /* salida: IMPRIMIR LPAREN ID RPAREN PUNTOYCOMA  */
#line 302 "parser.y"
                                         { (yyval.nodo_ast) = crear_nodo_salida_id((yyvsp[-2].str), yylineno); free((yyvsp[-2].str)); }
#line 1491 "parser.tab.c"
    break;

  case 19: /* salida: IMPRIMIR LPAREN CADENA_LITERAL RPAREN PUNTOYCOMA  */
#line 303 "parser.y"
                                                       { (yyval.nodo_ast) = crear_nodo_salida_literal((yyvsp[-2].str), yylineno); free((yyvsp[-2].str)); }
#line 1497 "parser.tab.c"
    break;

  case 20: /* condicion: expresion IGUAL_IGUAL expresion  */
#line 307 "parser.y"
                                      { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_IGUAL_IGUAL, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1503 "parser.tab.c"
    break;

  case 21: /* condicion: expresion DIFERENTE expresion  */
#line 308 "parser.y"
                                        { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_DIFERENTE, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1509 "parser.tab.c"
    break;

  case 22: /* condicion: expresion MENOR expresion  */
#line 309 "parser.y"
                                        { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_MENOR, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1515 "parser.tab.c"
    break;

  case 23: /* condicion: expresion MAYOR expresion  */
#line 310 "parser.y"
                                        { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_MAYOR, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1521 "parser.tab.c"
    break;

  case 24: /* condicion: expresion MENOR_IGUAL expresion  */
#line 311 "parser.y"
                                        { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_MENOR_IGUAL, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1527 "parser.tab.c"
    break;

  case 25: /* condicion: expresion MAYOR_IGUAL expresion  */
#line 312 "parser.y"
                                        { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_MAYOR_IGUAL, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1533 "parser.tab.c"
    break;

  case 26: /* sentencia_if: IF LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER ELSE LLAVE_IZQ lista_sentencias LLAVE_DER  */
#line 317 "parser.y"
                                              {
        (yyval.nodo_ast) = crear_nodo_if((yyvsp[-8].nodo_ast), (yyvsp[-5].nodo_ast), (yyvsp[-1].nodo_ast), yylineno);
    }
#line 1541 "parser.tab.c"
    break;

  case 27: /* sentencia_if: IF LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER  */
#line 320 "parser.y"
                                                                                 {
        (yyval.nodo_ast) = crear_nodo_if((yyvsp[-4].nodo_ast), (yyvsp[-1].nodo_ast), NULL, yylineno);
    }
#line 1549 "parser.tab.c"
    break;

  case 28: /* sentencia_while: WHILE LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER  */
#line 326 "parser.y"
                                                                       {
        (yyval.nodo_ast) = crear_nodo_while((yyvsp[-4].nodo_ast), (yyvsp[-1].nodo_ast), yylineno);
    }
#line 1557 "parser.tab.c"
    break;

  case 29: /* expresion: NENTERO  */
#line 332 "parser.y"
                          { (yyval.nodo_ast) = crear_nodo_entero(atoi((yyvsp[0].str)), yylineno); free((yyvsp[0].str)); }
#line 1563 "parser.tab.c"
    break;

  case 30: /* expresion: NDECIMAL  */
#line 333 "parser.y"
                          { (yyval.nodo_ast) = crear_nodo_decimal(atof((yyvsp[0].str)), yylineno); free((yyvsp[0].str)); }
#line 1569 "parser.tab.c"
    break;

  case 31: /* expresion: ID  */
#line 334 "parser.y"
                          { (yyval.nodo_ast) = crear_nodo_id_expresion((yyvsp[0].str), yylineno); free((yyvsp[0].str)); }
#line 1575 "parser.tab.c"
    break;

  case 32: /* expresion: expresion SUMA expresion  */
#line 335 "parser.y"
                                { (yyval.nodo_ast) = crear_nodo_expresion_binaria(OP_SUMA, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1581 "parser.tab.c"
    break;

  case 33: /* expresion: expresion RESTA expresion  */
#line 336 "parser.y"
                                { (yyval.nodo_ast) = crear_nodo_expresion_binaria(OP_RESTA, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1587 "parser.tab.c"
    break;

  case 34: /* expresion: expresion MULT expresion  */
#line 337 "parser.y"
                                { (yyval.nodo_ast) = crear_nodo_expresion_binaria(OP_MULT, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1593 "parser.tab.c"
    break;

  case 35: /* expresion: expresion DIV expresion  */
#line 338 "parser.y"
                                { (yyval.nodo_ast) = crear_nodo_expresion_binaria(OP_DIV, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1599 "parser.tab.c"
    break;

  case 36: /* expresion: LPAREN expresion RPAREN  */
#line 339 "parser.y"
                                { (yyval.nodo_ast) = (yyvsp[-1].nodo_ast); }
#line 1605 "parser.tab.c"
    break;


#line 1609 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 341 "parser.y"


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
