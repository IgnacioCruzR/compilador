%{
#include "parser.tab.h"
#include <stdio.h>
#include <string.h>
%}

%%
[ \t\n]+              ;
"entero"              return ENTERO;
"decimal"             return DECIMAL;
"cadena"              return CADENA;
[a-zA-Z_][a-zA-Z0-9_]* {
                        yylval.str = strdup(yytext);
                        return ID;
                      }
";"                   return PUNTOYCOMA;
.                     return *yytext;
%%

int yywrap() { return 1; }
