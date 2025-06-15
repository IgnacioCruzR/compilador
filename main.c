#include <stdio.h>

extern int yyparse(void);

int main(void) {
    return yyparse();
}