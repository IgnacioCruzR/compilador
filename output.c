#include <stdio.h>
#include <string.h>

int leer_entero() {
    int valor;
    scanf("%d", &valor);
    return valor;
}

void sumar_y_mostrar(int a, int b) {
    int suma_resultado;
    suma_resultado = (a + b);
    printf("%s\n", "El resultado de la suma es:");
    printf("%d\n", suma_resultado);
}

void restar_y_mostrar(int a, int b) {
    int resta_resultado;
    resta_resultado = (a - b);
    printf("%s\n", "El resultado de la resta es:");
    printf("%d\n", resta_resultado);
}

void multiplicar_y_mostrar(int a, int b) {
    int mult_resultado;
    mult_resultado = (a * b);
    printf("%s\n", "El resultado de la multiplicación es:");
    printf("%d\n", mult_resultado);
}

void dividir_y_mostrar(int a, int b) {
    int div_resultado;
    div_resultado = (a / b);
    printf("%s\n", "El resultado de la división es:");
    printf("%d\n", div_resultado);
}

int exponente(int base, int pot) {
    int res;
    res = 1;
    int cont;
    cont = pot;
    while ((cont > 0)) {
    res = (res * base);
    cont = (cont - 1);
    }
    return res;
}


int main() {
    printf("%s\n", "--- BIENVENIDO A LA CALCULADORA BITSTREAM ---");
    int opcion;
    opcion = 0;
    while ((opcion != 6)) {
    printf("%s\n", "------------------------------------");
    printf("%s\n", "Por favor, elija una operación:");
    printf("%s\n", "1. Sumar");
    printf("%s\n", "2. Restar");
    printf("%s\n", "3. Multiplicar");
    printf("%s\n", "4. Dividir");
    printf("%s\n", "5. Exponente");
    printf("%s\n", "6. Salir del programa");
    printf("%s\n", "------------------------------------");
    printf("%s\n", "Ingrese su opción:");
    opcion = leer_entero();
    if ((opcion > 0)) {
    if ((opcion < 5)) {
    int n1;
    int n2;
    printf("%s\n", "Ingrese el primer número:");
    n1 = leer_entero();
    printf("%s\n", "Ingrese el segundo número:");
    n2 = leer_entero();
    if ((opcion == 1)) {
    sumar_y_mostrar(n1, n2);
    }
    if ((opcion == 2)) {
    restar_y_mostrar(n1, n2);
    }
    if ((opcion == 3)) {
    multiplicar_y_mostrar(n1, n2);
    }
    if ((opcion == 4)) {
    dividir_y_mostrar(n1, n2);
    }
    }
    }
    if ((opcion == 5)) {
    int base;
    int pot;
    printf("%s\n", "Ingrese el número base:");
    base = leer_entero();
    printf("%s\n", "Ingrese el exponente:");
    pot = leer_entero();
    int resultado_exp;
    resultado_exp = exponente(base, pot);
    printf("%s\n", "El resultado del exponente es:");
    printf("%d\n", resultado_exp);
    }
    }
    printf("%s\n", "¡Gracias por usar la calculadora BitStream! Saliendo...");
    return 0;
}
