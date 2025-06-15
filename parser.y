%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int yylex(void);
void yyerror(const char* s) {
    fprintf(stderr, "Error: %s\n", s);
}
%}


%union {
    char* str;
}

%token ENTERO DECIMAL CADENA PUNTOYCOMA IGUAL
%token <str> NENTERO NDECIMAL CADENA_LITERAL ID
%token SUMA RESTA MULT DIV LPAREN RPAREN
%token LEER
%token IMPRIMIR
%type <str> declaracion
%type <str> expresion
%start programa
%left SUMA RESTA
%left MULT DIV

%%
programa:
      lista_sentencias { printf("Programa aceptado.\n"); }
    ;

lista_sentencias:
      lista_sentencias sentencia
    | sentencia
    ;

sentencia:
      declaracion
    | asignacion
    | entrada
    | salida
    ;

declaracion:
      ENTERO ID PUNTOYCOMA   { printf("Declaración de entero: %s\n", $2); free($2); }
    | DECIMAL ID PUNTOYCOMA  { printf("Declaración de decimal: %s\n", $2); free($2); }
    | CADENA ID PUNTOYCOMA   { printf("Declaración de cadena: %s\n", $2); free($2); }
    ;

asignacion:
      ID IGUAL NENTERO PUNTOYCOMA         { printf("%s = %s (entero)\n", $1, $3); free($1); free($3); }
    | ID IGUAL NDECIMAL PUNTOYCOMA        { printf("%s = %s (decimal)\n", $1, $3); free($1); free($3); }
    | ID IGUAL CADENA_LITERAL PUNTOYCOMA  { printf("%s = %s (cadena)\n", $1, $3); free($1); free($3); }
    | ID IGUAL expresion PUNTOYCOMA { printf("%s = %s\n", $1, $3); free($1); free($3); }
    ;
entrada:
      LEER LPAREN ID RPAREN PUNTOYCOMA {
          printf("Leer variable: %s\n", $3);
          free($3);
      }
    ;
salida:
      IMPRIMIR LPAREN ID RPAREN PUNTOYCOMA {
          printf("Imprimir variable: %s\n", $3);
          free($3);
      }
    | IMPRIMIR LPAREN CADENA_LITERAL RPAREN PUNTOYCOMA {
          printf("Imprimir cadena: %s\n", $3);
          free($3);
      }
    ;
expresion:
      NENTERO                   { $$ = $1; }
    | NDECIMAL                  { $$ = $1; }
    | expresion SUMA expresion  { 
          char* buffer = malloc(strlen($1) + strlen($3) + 5);
          sprintf(buffer, "(%s+%s)", $1, $3);
          $$ = buffer;
          free($1); free($3);
      }
    | expresion RESTA expresion {
          char* buffer = malloc(strlen($1) + strlen($3) + 5);
          sprintf(buffer, "(%s-%s)", $1, $3);
          $$ = buffer;
          free($1); free($3);
      }
    | expresion MULT expresion {
          char* buffer = malloc(strlen($1) + strlen($3) + 5);
          sprintf(buffer, "(%s*%s)", $1, $3);
          $$ = buffer;
          free($1); free($3);
      }
    | expresion DIV expresion  {
          char* buffer = malloc(strlen($1) + strlen($3) + 5);
          sprintf(buffer, "(%s/%s)", $1, $3);
          $$ = buffer;
          free($1); free($3);
      }
    | LPAREN expresion RPAREN { $$ = $2; }
    ;   
%%