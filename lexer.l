%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
%}

%%
[ \t\n]+              ;  // Ignorar espacios y saltos de línea

"entero"              return ENTERO;
"decimal"             return DECIMAL;
"cadena"              return CADENA;

"="                   return IGUAL;
";"                   return PUNTOYCOMA;

\"[^\"]*\"            {
                        yylval.str = strdup(yytext);
                        return CADENA_LITERAL;
                     }

[0-9]+"."[0-9]+       {
                        yylval.str = strdup(yytext);
                        return NDECIMAL;
                     }

[0-9]+                {
                        yylval.str = strdup(yytext);
                        return NENTERO;
                     }

"leer"                 return LEER;
"imprimir"             return IMPRIMIR;

[a-zA-Z_][a-zA-Z0-9_]* {
                        yylval.str = strdup(yytext);
                        return ID;
                     }

"("                    return LPAREN;
")"                    return RPAREN;
"+"                    return SUMA;
"-"                    return RESTA;
"*"                    return MULT;
"/"                    return DIV;
.                      ; 
%%

int yywrap() { return 1; }