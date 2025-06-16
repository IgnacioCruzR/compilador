#include <stdio.h>
#include <string.h>

int main() {
    int op1 = 0;
    int op2 = 0;
    int resultado = 0;
    char* operacion = NULL;
    printf("--- Probando funcion_sumar(100, 50) ---\n");
    op1 = 100;
    op2 = 50;
    operacion = "Suma";
    resultado = (op1 + op2);
    printf("Operacion:\n");
    printf("%s\n", operacion);
    printf("Resultado:\n");
    printf("%d\n", resultado);
    printf("\n");
    printf("--- Probando funcion_multiplicar(20, 5) ---\n");
    op1 = 20;
    op2 = 5;
    operacion = "Multiplicacion";
    resultado = (op1 * op2);
    printf("Operacion:\n");
    printf("%s\n", operacion);
    printf("Resultado:\n");
    printf("%d\n", resultado);
    printf("\n");
    printf("--- Probando funcion_exponente(3, 4) con bucle WHILE ---\n");
    op1 = 3;
    op2 = 4;
    operacion = "Exponente (3^4)";
    resultado = 1;
    int contador = 0;
    contador = op2;
    while ((contador > 0)) {
    resultado = (resultado * op1);
    contador = (contador - 1);
    }
    printf("Operacion:\n");
    printf("%s\n", operacion);
    printf("Resultado:\n");
    printf("%d\n", resultado);
    printf("\n");
    printf("--- Calculadora finalizada con éxito ---\n");
    return 0;
}
