#include <stdio.h>

extern int yyparse();
//extern int yydebug;

int main() {
    //yydebug = 1;
    return yyparse();
}