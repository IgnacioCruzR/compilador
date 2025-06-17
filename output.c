#include <stdio.h>
#include <string.h>

int leer_entero() {
    int valor;
    scanf("%d", &valor);
    return valor;
}

int jugar() {
    int numero_secreto;
    numero_secreto = 42;
    int intento;
    intento = 0;
    int contador_intentos;
    contador_intentos = 0;
    printf("%s\n", "¡He pensado en un número entre 1 y 100!");
    printf("%s\n", "Intenta adivinar cuál es.");
    while ((intento != numero_secreto)) {
    printf("%s\n", "Ingresa tu número:");
    intento = leer_entero();
    contador_intentos = (contador_intentos + 1);
    if ((intento < numero_secreto)) {
    printf("%s\n", "¡Muy bajo! Intenta de nuevo.");
    }
    else {
    if ((intento > numero_secreto)) {
    printf("%s\n", "¡Muy alto! Intenta de nuevo.");
    }
    }
    }
    printf("%s\n", "¡CORRECTO! Adivinaste el número.");
    printf("%s\n", "Lo lograste en:");
    printf("%d\n", contador_intentos);
    printf("%s\n", "intentos.");
    return 1;
}


int main() {
    printf("%s\n", "--- Iniciando Juego Final de Prueba ---");
    jugar();
    printf("%s\n", "--- Juego Terminado. El compilador funciona. ---");
    return 0;
}
