%{
#include <stdio.h>
#include <stdlib.h>
char* yytext;
void yyerror(const char* s) { fprintf(stderr, "Error: %s\n", s); }
%}

%union {
    char* str;
}

%token <str> STRING
%token PRINT LPAREN RPAREN
%start programa

%%
programa:
      instruccion
    ;

instruccion:
      PRINT LPAREN STRING RPAREN { printf(">> %s\n", $3); }
    ;
%%
