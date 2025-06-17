#include <stdio.h>
#include <string.h>

int factorial(int n) {
    if ((n < 2)) {
    return 1;
    }
    else {
    return (n * factorial((n - 1)));
    }
}


int main() {
    printf("%s\n", "--- Probando la función factorial recursiva ---");
    int num;
    num = 5;
    int resultado_fact;
    resultado_fact = factorial(num);
    printf("%s\n", "Calculando el factorial de 5...");
    printf("%d\n", resultado_fact);
    printf("%s\n", "Calculando el factorial de 7...");
    resultado_fact = factorial(7);
    printf("%d\n", resultado_fact);
    printf("%s\n", "Calculando el factorial de 1 (caso base)...");
    resultado_fact = factorial(1);
    printf("%d\n", resultado_fact);
    return 0;
}
