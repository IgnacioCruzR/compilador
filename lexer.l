%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
%}
%option noyywrap yylineno
%%
"//".* ;
[ \t\n\r]+ ;
"entero" return ENTERO;
"decimal" return DECIMAL;
"cadena" return CADENA;
"if" return IF;
"else" return ELSE;
"while" return WHILE;
"leer" return LEER;
"imprimir" return IMPRIMIR;
"funcion" return FUNCION;
"retornar" return RETORNAR;
"==" return IGUAL_IGUAL;
"!=" return DIFERENTE;
"<=" return MENOR_IGUAL;
">=" return MAYOR_IGUAL;
"<" return MENOR;
">" return MAYOR;
"(" return LPAREN;
")" return RPAREN;
"," return COMA;
"{" return LLAVE_IZQ;
"}" return LLAVE_DER;
"=" return IGUAL;
";" return PUNTOYCOMA;
"+" return SUMA;
"-" return RESTA;
"*" return MULT;
"/" return DIV;
\"[^\n\"]*\" {
    yytext[yyleng-1] = '\0';
    yylval.str = strdup(yytext+1);
    return CADENA_LITERAL;
}
[0-9]+\.[0-9]+ { 
    yylval.str = strdup(yytext); 
    return NDECIMAL; 
}
[0-9]+ { 
    yylval.str = strdup(yytext); 
    return NENTERO; 
}
[a-zA-Z_][a-zA-Z0-9_]* { 
    yylval.str = strdup(yytext); 
    return ID; 
}
. { 
    fprintf(stderr, "Error Léxico en línea %d: Carácter no reconocido '%s'\n", yylineno, yytext); 
}
%%