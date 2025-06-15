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

%token ENTERO DECIMAL CADENA PUNTOYCOMA
%token <str> ID
%type <str> declaracion
%start programa

%%
programa:
      lista_decl { printf("Programa aceptado.\n"); }
    ;

lista_decl:
      lista_decl declaracion
    | declaracion
    ;

declaracion:
      ENTERO ID PUNTOYCOMA   { printf("Declaración de entero: %s\n", $2); free($2); }
    | DECIMAL ID PUNTOYCOMA  { printf("Declaración de decimal: %s\n", $2); free($2); }
    | CADENA ID PUNTOYCOMA   { printf("Declaración de cadena: %s\n", $2); free($2); }
    ;
%%
