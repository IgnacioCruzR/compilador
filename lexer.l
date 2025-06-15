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

"if"                  return IF;
"else"                return ELSE;
"while"               return WHILE;
"leer"                return LEER;
"imprimir"            return IMPRIMIR;

"=="                  return IGUAL_IGUAL;
"!="                  return DIFERENTE;
"<="                  return MENOR_IGUAL;
">="                  return MAYOR_IGUAL;
"<"                   return MENOR;
">"                   return MAYOR;

"("                   return LPAREN;
")"                   return RPAREN;
"{"                   return LLAVE_IZQ;
"}"                   return LLAVE_DER;
"="                   return IGUAL;
";"                   return PUNTOYCOMA;
"+"                   return SUMA;
"-"                   return RESTA;
"*"                   return MULT;
"/"                   return DIV;

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


[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext);
    return ID;
}



.                     ;
%%

int yywrap() { return 1; }