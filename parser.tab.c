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
#line 4 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VARS 100

/* --- Definiciones de Tipos y Estructuras --- */
typedef enum {
    TIPO_ENTERO, TIPO_CADENA, TIPO_INDEFINIDO
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
        struct { char* nombre; struct NodoAST* params; struct NodoAST* cuerpo; } declaracion_funcion;
        struct { char* nombre; struct NodoAST* args; } llamada_funcion;
        struct { struct NodoAST* valor_retorno; } retorno;
        struct { char* nombre_param; struct NodoAST* expresion_arg; struct NodoAST* siguiente; } lista_args;
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
NodoAST* crear_nodo_declaracion_funcion(char* nombre, NodoAST* params, NodoAST* cuerpo, int linenum);
NodoAST* crear_nodo_llamada_funcion(char* nombre, NodoAST* args, int linenum);
NodoAST* crear_nodo_sentencia_retorno(NodoAST* valor, int linenum);
NodoAST* crear_nodo_lista_args(char* nombre_param, NodoAST* expresion_arg, NodoAST* siguiente, int linenum);
NodoAST* crear_nodo_leer(int linenum);
void generar_codigo(NodoAST* nodo, FILE* f_salida);

// Declaración de variables externas que vienen del lexer
extern int yylineno;
extern char* yytext;
extern FILE* yyin;


#line 163 "parser.tab.c"

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
  YYSYMBOL_CADENA = 4,                     /* CADENA  */
  YYSYMBOL_PUNTOYCOMA = 5,                 /* PUNTOYCOMA  */
  YYSYMBOL_IGUAL = 6,                      /* IGUAL  */
  YYSYMBOL_COMA = 7,                       /* COMA  */
  YYSYMBOL_NENTERO = 8,                    /* NENTERO  */
  YYSYMBOL_CADENA_LITERAL = 9,             /* CADENA_LITERAL  */
  YYSYMBOL_ID = 10,                        /* ID  */
  YYSYMBOL_NDECIMAL = 11,                  /* NDECIMAL  */
  YYSYMBOL_SUMA = 12,                      /* SUMA  */
  YYSYMBOL_RESTA = 13,                     /* RESTA  */
  YYSYMBOL_MULT = 14,                      /* MULT  */
  YYSYMBOL_DIV = 15,                       /* DIV  */
  YYSYMBOL_LPAREN = 16,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 17,                    /* RPAREN  */
  YYSYMBOL_IMPRIMIR = 18,                  /* IMPRIMIR  */
  YYSYMBOL_IF = 19,                        /* IF  */
  YYSYMBOL_ELSE = 20,                      /* ELSE  */
  YYSYMBOL_WHILE = 21,                     /* WHILE  */
  YYSYMBOL_FUNCION = 22,                   /* FUNCION  */
  YYSYMBOL_RETORNAR = 23,                  /* RETORNAR  */
  YYSYMBOL_LLAVE_IZQ = 24,                 /* LLAVE_IZQ  */
  YYSYMBOL_LLAVE_DER = 25,                 /* LLAVE_DER  */
  YYSYMBOL_IGUAL_IGUAL = 26,               /* IGUAL_IGUAL  */
  YYSYMBOL_DIFERENTE = 27,                 /* DIFERENTE  */
  YYSYMBOL_MENOR = 28,                     /* MENOR  */
  YYSYMBOL_MAYOR = 29,                     /* MAYOR  */
  YYSYMBOL_MENOR_IGUAL = 30,               /* MENOR_IGUAL  */
  YYSYMBOL_MAYOR_IGUAL = 31,               /* MAYOR_IGUAL  */
  YYSYMBOL_DECIMAL = 32,                   /* DECIMAL  */
  YYSYMBOL_LEER = 33,                      /* LEER  */
  YYSYMBOL_YYACCEPT = 34,                  /* $accept  */
  YYSYMBOL_programa = 35,                  /* programa  */
  YYSYMBOL_lista_sentencias = 36,          /* lista_sentencias  */
  YYSYMBOL_sentencia = 37,                 /* sentencia  */
  YYSYMBOL_declaracion_funcion = 38,       /* declaracion_funcion  */
  YYSYMBOL_lista_parametros = 39,          /* lista_parametros  */
  YYSYMBOL_sentencia_retorno = 40,         /* sentencia_retorno  */
  YYSYMBOL_expresion = 41,                 /* expresion  */
  YYSYMBOL_llamada_funcion = 42,           /* llamada_funcion  */
  YYSYMBOL_lista_argumentos = 43,          /* lista_argumentos  */
  YYSYMBOL_declaracion = 44,               /* declaracion  */
  YYSYMBOL_asignacion = 45,                /* asignacion  */
  YYSYMBOL_salida = 46,                    /* salida  */
  YYSYMBOL_condicion = 47,                 /* condicion  */
  YYSYMBOL_sentencia_if = 48,              /* sentencia_if  */
  YYSYMBOL_sentencia_while = 49            /* sentencia_while  */
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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   193

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  44
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  100

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   288


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
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,   127,   127,   134,   135,   139,   140,   141,   142,   143,
     144,   145,   146,   150,   157,   159,   161,   164,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   181,   185,
     186,   187,   191,   192,   196,   203,   207,   208,   209,   210,
     211,   212,   216,   217,   221
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
  "\"end of file\"", "error", "\"invalid token\"", "ENTERO", "CADENA",
  "PUNTOYCOMA", "IGUAL", "COMA", "NENTERO", "CADENA_LITERAL", "ID",
  "NDECIMAL", "SUMA", "RESTA", "MULT", "DIV", "LPAREN", "RPAREN",
  "IMPRIMIR", "IF", "ELSE", "WHILE", "FUNCION", "RETORNAR", "LLAVE_IZQ",
  "LLAVE_DER", "IGUAL_IGUAL", "DIFERENTE", "MENOR", "MAYOR", "MENOR_IGUAL",
  "MAYOR_IGUAL", "DECIMAL", "LEER", "$accept", "programa",
  "lista_sentencias", "sentencia", "declaracion_funcion",
  "lista_parametros", "sentencia_retorno", "expresion", "llamada_funcion",
  "lista_argumentos", "declaracion", "asignacion", "salida", "condicion",
  "sentencia_if", "sentencia_while", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-80)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -80,    29,   136,   -80,    32,    34,   -80,   -80,    -2,     2,
      37,    38,    47,    46,     2,    48,   -80,   -80,   -80,    33,
     -80,   -80,   -80,   -80,   -80,   -80,    45,    60,     2,     2,
      59,   170,     2,     2,     2,    63,   162,    61,   -80,     2,
       2,     2,     2,   -80,   -80,   166,    -6,    65,   -80,   176,
     135,    67,    68,    70,   -80,   -80,     1,     1,   -80,   -80,
     -80,     2,   -80,    85,     2,     2,     2,     2,     2,     2,
      71,    72,    86,   -80,   -80,    57,    57,    57,    57,    57,
      57,   -80,   -80,    87,    81,    18,    58,    92,    77,    88,
     -80,    86,   -80,    80,   -80,    84,   -80,   -80,   110,   -80
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     2,     1,     0,     0,    18,    20,    19,     0,
       0,     0,     0,     0,     0,     0,     4,    11,    10,     0,
      26,     5,     6,     7,     8,     9,     0,     0,     0,    29,
      19,     0,     0,     0,     0,     0,     0,     0,    12,     0,
       0,     0,     0,    32,    33,     0,    30,     0,    25,     0,
       0,     0,     0,     0,    17,    27,    21,    22,    23,    24,
      34,    29,    28,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    14,    31,    35,    36,    37,    38,    39,    40,
      41,     3,     3,     0,     0,     0,     0,    15,     0,    42,
      44,    14,     3,     0,    16,     0,     3,    13,     0,    43
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -80,   -80,   -79,   -80,   -80,    19,   -80,    -9,   -80,    50,
     -80,   -80,   -80,    78,   -80,   -80
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     2,    16,    17,    84,    18,    19,    20,    47,
      21,    22,    23,    51,    24,    25
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      31,    61,    85,    86,    28,    36,    39,    40,    41,    42,
       6,     7,    30,    95,    29,    41,    42,    98,     9,    45,
      46,     4,     5,    49,    50,    50,     6,     7,     8,     3,
      56,    57,    58,    59,     9,    15,    10,    11,    38,    12,
      13,    14,    26,    89,    27,    39,    40,    41,    42,    35,
      43,    15,    46,    32,    33,    75,    76,    77,    78,    79,
      80,     4,     5,    34,    37,    44,     6,     7,     8,    39,
      40,    41,    42,    53,     9,    29,    10,    11,    55,    12,
      13,    14,    62,    90,    70,    71,    72,     4,     5,    83,
      74,    15,     6,     7,     8,    81,    82,    87,    88,    91,
       9,    92,    10,    11,    96,    12,    13,    14,    93,    97,
      94,    73,    52,     4,     5,     0,     0,    15,     6,     7,
       8,     0,     0,     0,     0,     0,     9,     0,    10,    11,
       0,    12,    13,    14,     0,    99,     0,     0,     0,     4,
       5,     0,     0,    15,     6,     7,     8,    39,    40,    41,
      42,     0,     9,     0,    10,    11,     0,    12,    13,    14,
       0,    64,    65,    66,    67,    68,    69,    54,     0,    15,
       0,    60,     0,     0,    39,    40,    41,    42,    39,    40,
      41,    42,    39,    40,    41,    42,     0,    48,    39,    40,
      41,    42,     0,    63
};

static const yytype_int8 yycheck[] =
{
       9,     7,    81,    82,     6,    14,    12,    13,    14,    15,
       8,     9,    10,    92,    16,    14,    15,    96,    16,    28,
      29,     3,     4,    32,    33,    34,     8,     9,    10,     0,
      39,    40,    41,    42,    16,    33,    18,    19,     5,    21,
      22,    23,    10,    25,    10,    12,    13,    14,    15,     3,
       5,    33,    61,    16,    16,    64,    65,    66,    67,    68,
      69,     3,     4,    16,    16,     5,     8,     9,    10,    12,
      13,    14,    15,    10,    16,    16,    18,    19,    17,    21,
      22,    23,    17,    25,    17,    17,    16,     3,     4,     3,
       5,    33,     8,     9,    10,    24,    24,    10,    17,     7,
      16,    24,    18,    19,    24,    21,    22,    23,    20,    25,
      91,    61,    34,     3,     4,    -1,    -1,    33,     8,     9,
      10,    -1,    -1,    -1,    -1,    -1,    16,    -1,    18,    19,
      -1,    21,    22,    23,    -1,    25,    -1,    -1,    -1,     3,
       4,    -1,    -1,    33,     8,     9,    10,    12,    13,    14,
      15,    -1,    16,    -1,    18,    19,    -1,    21,    22,    23,
      -1,    26,    27,    28,    29,    30,    31,     5,    -1,    33,
      -1,     5,    -1,    -1,    12,    13,    14,    15,    12,    13,
      14,    15,    12,    13,    14,    15,    -1,    17,    12,    13,
      14,    15,    -1,    17
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    35,    36,     0,     3,     4,     8,     9,    10,    16,
      18,    19,    21,    22,    23,    33,    37,    38,    40,    41,
      42,    44,    45,    46,    48,    49,    10,    10,     6,    16,
      10,    41,    16,    16,    16,     3,    41,    16,     5,    12,
      13,    14,    15,     5,     5,    41,    41,    43,    17,    41,
      41,    47,    47,    10,     5,    17,    41,    41,    41,    41,
       5,     7,    17,    17,    26,    27,    28,    29,    30,    31,
      17,    17,    16,    43,     5,    41,    41,    41,    41,    41,
      41,    24,    24,     3,    39,    36,    36,    10,    17,    25,
      25,     7,    24,    20,    39,    36,    24,    25,    36,    25
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    34,    35,    36,    36,    37,    37,    37,    37,    37,
      37,    37,    37,    38,    39,    39,    39,    40,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    42,    43,
      43,    43,    44,    44,    45,    46,    47,    47,    47,    47,
      47,    47,    48,    48,    49
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     9,     0,     2,     4,     3,     1,     1,
       1,     3,     3,     3,     3,     3,     1,     3,     4,     0,
       1,     3,     3,     3,     4,     5,     3,     3,     3,     3,
       3,     3,     7,    11,     7
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
#line 127 "parser.y"
                     {
        (yyval.nodo_ast) = crear_nodo_programa((yyvsp[0].nodo_ast), yylineno);
        g_ast_raiz = (yyval.nodo_ast); // <-- ¡ESTA ES LA LÍNEA CLAVE QUE FALTA!
    }
#line 1278 "parser.tab.c"
    break;

  case 3: /* lista_sentencias: %empty  */
#line 134 "parser.y"
           { (yyval.nodo_ast) = NULL; }
#line 1284 "parser.tab.c"
    break;

  case 4: /* lista_sentencias: lista_sentencias sentencia  */
#line 135 "parser.y"
                                 { (yyval.nodo_ast) = crear_nodo_lista_sentencias((yyvsp[0].nodo_ast), (yyvsp[-1].nodo_ast), yylineno); }
#line 1290 "parser.tab.c"
    break;

  case 5: /* sentencia: declaracion  */
#line 139 "parser.y"
                { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1296 "parser.tab.c"
    break;

  case 6: /* sentencia: asignacion  */
#line 140 "parser.y"
                  { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1302 "parser.tab.c"
    break;

  case 7: /* sentencia: salida  */
#line 141 "parser.y"
                  { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1308 "parser.tab.c"
    break;

  case 8: /* sentencia: sentencia_if  */
#line 142 "parser.y"
                  { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1314 "parser.tab.c"
    break;

  case 9: /* sentencia: sentencia_while  */
#line 143 "parser.y"
                      { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1320 "parser.tab.c"
    break;

  case 10: /* sentencia: sentencia_retorno  */
#line 144 "parser.y"
                        { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1326 "parser.tab.c"
    break;

  case 11: /* sentencia: declaracion_funcion  */
#line 145 "parser.y"
                          { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1332 "parser.tab.c"
    break;

  case 12: /* sentencia: expresion PUNTOYCOMA  */
#line 146 "parser.y"
                           { (yyval.nodo_ast) = (yyvsp[-1].nodo_ast); }
#line 1338 "parser.tab.c"
    break;

  case 13: /* declaracion_funcion: FUNCION ENTERO ID LPAREN lista_parametros RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER  */
#line 150 "parser.y"
                                                                                          {
        (yyval.nodo_ast) = crear_nodo_declaracion_funcion((yyvsp[-6].str), (yyvsp[-4].nodo_ast), (yyvsp[-1].nodo_ast), yylineno);
        free((yyvsp[-6].str));
    }
#line 1347 "parser.tab.c"
    break;

  case 14: /* lista_parametros: %empty  */
#line 157 "parser.y"
           { (yyval.nodo_ast) = NULL; }
#line 1353 "parser.tab.c"
    break;

  case 15: /* lista_parametros: ENTERO ID  */
#line 159 "parser.y"
                { (yyval.nodo_ast) = crear_nodo_lista_args((yyvsp[0].str), NULL, NULL, yylineno); free((yyvsp[0].str)); }
#line 1359 "parser.tab.c"
    break;

  case 16: /* lista_parametros: ENTERO ID COMA lista_parametros  */
#line 161 "parser.y"
                                      { (yyval.nodo_ast) = crear_nodo_lista_args((yyvsp[-2].str), NULL, (yyvsp[0].nodo_ast), yylineno); free((yyvsp[-2].str)); }
#line 1365 "parser.tab.c"
    break;

  case 17: /* sentencia_retorno: RETORNAR expresion PUNTOYCOMA  */
#line 164 "parser.y"
                                  { (yyval.nodo_ast) = crear_nodo_sentencia_retorno((yyvsp[-1].nodo_ast), yylineno); }
#line 1371 "parser.tab.c"
    break;

  case 18: /* expresion: NENTERO  */
#line 168 "parser.y"
            { (yyval.nodo_ast) = crear_nodo_entero(atoi((yyvsp[0].str)), yylineno); free((yyvsp[0].str)); }
#line 1377 "parser.tab.c"
    break;

  case 19: /* expresion: ID  */
#line 169 "parser.y"
         { (yyval.nodo_ast) = crear_nodo_id_expresion((yyvsp[0].str), yylineno); free((yyvsp[0].str)); }
#line 1383 "parser.tab.c"
    break;

  case 20: /* expresion: CADENA_LITERAL  */
#line 170 "parser.y"
                     { (yyval.nodo_ast) = crear_nodo_cadena_literal_expresion((yyvsp[0].str), yylineno); free((yyvsp[0].str)); }
#line 1389 "parser.tab.c"
    break;

  case 21: /* expresion: expresion SUMA expresion  */
#line 171 "parser.y"
                               { (yyval.nodo_ast) = crear_nodo_expresion_binaria(OP_SUMA, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1395 "parser.tab.c"
    break;

  case 22: /* expresion: expresion RESTA expresion  */
#line 172 "parser.y"
                                { (yyval.nodo_ast) = crear_nodo_expresion_binaria(OP_RESTA, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1401 "parser.tab.c"
    break;

  case 23: /* expresion: expresion MULT expresion  */
#line 173 "parser.y"
                                { (yyval.nodo_ast) = crear_nodo_expresion_binaria(OP_MULT, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1407 "parser.tab.c"
    break;

  case 24: /* expresion: expresion DIV expresion  */
#line 174 "parser.y"
                                { (yyval.nodo_ast) = crear_nodo_expresion_binaria(OP_DIV, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1413 "parser.tab.c"
    break;

  case 25: /* expresion: LPAREN expresion RPAREN  */
#line 175 "parser.y"
                                { (yyval.nodo_ast) = (yyvsp[-1].nodo_ast); }
#line 1419 "parser.tab.c"
    break;

  case 26: /* expresion: llamada_funcion  */
#line 176 "parser.y"
                      { (yyval.nodo_ast) = (yyvsp[0].nodo_ast); }
#line 1425 "parser.tab.c"
    break;

  case 27: /* expresion: LEER LPAREN RPAREN  */
#line 177 "parser.y"
                         { (yyval.nodo_ast) = crear_nodo_leer(yylineno); }
#line 1431 "parser.tab.c"
    break;

  case 28: /* llamada_funcion: ID LPAREN lista_argumentos RPAREN  */
#line 181 "parser.y"
                                      { (yyval.nodo_ast) = crear_nodo_llamada_funcion((yyvsp[-3].str), (yyvsp[-1].nodo_ast), yylineno); free((yyvsp[-3].str)); }
#line 1437 "parser.tab.c"
    break;

  case 29: /* lista_argumentos: %empty  */
#line 185 "parser.y"
           { (yyval.nodo_ast) = NULL; }
#line 1443 "parser.tab.c"
    break;

  case 30: /* lista_argumentos: expresion  */
#line 186 "parser.y"
                { (yyval.nodo_ast) = crear_nodo_lista_args(NULL, (yyvsp[0].nodo_ast), NULL, yylineno); }
#line 1449 "parser.tab.c"
    break;

  case 31: /* lista_argumentos: expresion COMA lista_argumentos  */
#line 187 "parser.y"
                                      { (yyval.nodo_ast) = crear_nodo_lista_args(NULL, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1455 "parser.tab.c"
    break;

  case 32: /* declaracion: ENTERO ID PUNTOYCOMA  */
#line 191 "parser.y"
                          { (yyval.nodo_ast) = crear_nodo_declaracion(TIPO_ENTERO, (yyvsp[-1].str), yylineno); free((yyvsp[-1].str)); }
#line 1461 "parser.tab.c"
    break;

  case 33: /* declaracion: CADENA ID PUNTOYCOMA  */
#line 192 "parser.y"
                            { (yyval.nodo_ast) = crear_nodo_declaracion(TIPO_CADENA, (yyvsp[-1].str), yylineno); free((yyvsp[-1].str)); }
#line 1467 "parser.tab.c"
    break;

  case 34: /* asignacion: ID IGUAL expresion PUNTOYCOMA  */
#line 196 "parser.y"
                                  {
        (yyval.nodo_ast) = crear_nodo_asignacion((yyvsp[-3].str), (yyvsp[-1].nodo_ast), yylineno);
        free((yyvsp[-3].str));
    }
#line 1476 "parser.tab.c"
    break;

  case 35: /* salida: IMPRIMIR LPAREN expresion RPAREN PUNTOYCOMA  */
#line 203 "parser.y"
                                                { (yyval.nodo_ast) = crear_nodo_salida((yyvsp[-2].nodo_ast), yylineno); }
#line 1482 "parser.tab.c"
    break;

  case 36: /* condicion: expresion IGUAL_IGUAL expresion  */
#line 207 "parser.y"
                                      { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_IGUAL_IGUAL, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1488 "parser.tab.c"
    break;

  case 37: /* condicion: expresion DIFERENTE expresion  */
#line 208 "parser.y"
                                        { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_DIFERENTE, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1494 "parser.tab.c"
    break;

  case 38: /* condicion: expresion MENOR expresion  */
#line 209 "parser.y"
                                        { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_MENOR, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1500 "parser.tab.c"
    break;

  case 39: /* condicion: expresion MAYOR expresion  */
#line 210 "parser.y"
                                        { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_MAYOR, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1506 "parser.tab.c"
    break;

  case 40: /* condicion: expresion MENOR_IGUAL expresion  */
#line 211 "parser.y"
                                        { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_MENOR_IGUAL, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1512 "parser.tab.c"
    break;

  case 41: /* condicion: expresion MAYOR_IGUAL expresion  */
#line 212 "parser.y"
                                        { (yyval.nodo_ast) = crear_nodo_condicion_binaria(OP_MAYOR_IGUAL, (yyvsp[-2].nodo_ast), (yyvsp[0].nodo_ast), yylineno); }
#line 1518 "parser.tab.c"
    break;

  case 42: /* sentencia_if: IF LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER  */
#line 216 "parser.y"
                                                                    { (yyval.nodo_ast) = crear_nodo_if((yyvsp[-4].nodo_ast), (yyvsp[-1].nodo_ast), NULL, yylineno); }
#line 1524 "parser.tab.c"
    break;

  case 43: /* sentencia_if: IF LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER ELSE LLAVE_IZQ lista_sentencias LLAVE_DER  */
#line 217 "parser.y"
                                                                                                              { (yyval.nodo_ast) = crear_nodo_if((yyvsp[-8].nodo_ast), (yyvsp[-5].nodo_ast), (yyvsp[-1].nodo_ast), yylineno); }
#line 1530 "parser.tab.c"
    break;

  case 44: /* sentencia_while: WHILE LPAREN condicion RPAREN LLAVE_IZQ lista_sentencias LLAVE_DER  */
#line 221 "parser.y"
                                                                       { (yyval.nodo_ast) = crear_nodo_while((yyvsp[-4].nodo_ast), (yyvsp[-1].nodo_ast), yylineno); }
#line 1536 "parser.tab.c"
    break;


#line 1540 "parser.tab.c"

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

#line 224 "parser.y"

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
NodoAST* crear_nodo_declaracion_funcion(char* nom, NodoAST* p, NodoAST* c, int l) { NodoAST* n=crear_nodo(NODO_DECLARACION_FUNCION,l); n->datos.declaracion_funcion.nombre=strdup(nom); n->datos.declaracion_funcion.params=p; n->datos.declaracion_funcion.cuerpo=c; return n; }
NodoAST* crear_nodo_llamada_funcion(char* nom, NodoAST* a, int l) { NodoAST* n=crear_nodo(NODO_LLAMADA_FUNCION,l); n->datos.llamada_funcion.nombre=strdup(nom); n->datos.llamada_funcion.args=a; return n; }
NodoAST* crear_nodo_sentencia_retorno(NodoAST* v, int l) { NodoAST* n=crear_nodo(NODO_SENTENCIA_RETORNO,l); n->datos.retorno.valor_retorno=v; return n; }
NodoAST* crear_nodo_lista_args(char* nom, NodoAST* expr, NodoAST* sig, int l) { NodoAST* n=crear_nodo(NODO_LISTA_ARGS,l); if(nom) n->datos.lista_args.nombre_param=strdup(nom); n->datos.lista_args.expresion_arg=expr; n->datos.lista_args.siguiente=sig; return n; }
NodoAST* crear_nodo_leer(int l) {
    return crear_nodo(NODO_EXPRESION_LEER, l);
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
            fprintf(f_salida, "int %s(", nodo->datos.declaracion_funcion.nombre);
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
