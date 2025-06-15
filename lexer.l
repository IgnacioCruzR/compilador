%{
#include "parser.tab.h"
#include <stdio.h>
%}

%%
[ \t\n]+             ;             // Ignorar espacios
"imprimir"           return PRINT;
"("                  return LPAREN;
")"                  return RPAREN;
\"[^"]*\"            yylval.str = strdup(yytext); return STRING;
.                    return *yytext;
%%

int yywrap() { return 1; }
