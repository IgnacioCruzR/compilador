#include <stdio.h>

extern int yyparse();

int main() {
    printf("Compilando...\n");
    yyparse();
    printf("Compilación terminada\n");
    return 0;
}
