Proyecto de Compilador para un Lenguaje Propio:

    Este repositorio contiene el código fuente y la documentación para un compilador de un lenguaje de programación propio, desarrollado como parte del laboratorio de compiladores entre el 17 de marzo y el 17 de junio.  El objetivo principal de este proyecto es comprender en profundidad el proceso de compilación y adquirir experiencia práctica en la creación de lenguajes de programación desde cero. 

Tecnologías Utilizadas:
    El desarrollo del compilador se ha realizado utilizando las siguientes herramientas:

    Flex: Para el análisis léxico. 


    Bison: Para el análisis sintáctico. 


    C/C++: Para la implementación de la lógica y la generación de código. 

Entorno:
    El compilador está diseñado para ser ejecutable en un entorno Linux. 

Características del Lenguaje:
    El lenguaje de programación propio diseñado para este proyecto incluye los siguientes elementos fundamentales:

Declaración de Variables:
     Soporte para tipos de datos enteros, flotantes y cadenas. 

Operaciones Aritméticas:
     Incluye las operaciones de suma (+), resta (-), multiplicación (*) y división (/). 

Operaciones de Comparación:
     Soporta ==, !=, <, >, <= y >=. 

Sentencias de Control:
     Implementa las estructuras condicionales if-else y el bucle while. 

Funciones de Entrada/Salida:
     Permite interactuar con el usuario mediante funciones básicas como imprimir en pantalla y leer desde el teclado. 

Funcionalidad del Compilador:
    El compilador implementa las etapas clave del proceso de traducción de un lenguaje:

Análisis Léxico y Sintáctico:
     Es capaz de analizar un programa fuente, identificar sus componentes (tokens) y detectar errores léxicos y sintácticos. 
Árbol de Sintaxis Abstracta (AST):
     Construye un AST a partir del código fuente válido. 
Análisis Semántico:
     Realiza comprobaciones semánticas básicas, como la correcta declaración de variables y la verificación de tipos. 
Generación de Código:
     Traduce el AST a un código ejecutable (en este caso, se genera código C) que refleja la lógica del programa original. 

DELIVERABLES:

    Junto con el código fuente del compilador, el proyecto incluye un informe detallado con la siguiente información:
    La gramática formal del lenguaje propio. 
    El diseño y la estructura del Árbol de Sintaxis Abstracta (AST). 
    Una descripción del proceso de generación de código. 
    Un manual de usuario para el lenguaje. 
    Ejemplos de código en el lenguaje propio y el código C que se genera a partir de ellos. 

Criterios de Evaluación:
    El proyecto fue evaluado según la siguiente pauta:

    Análisis Léxico 
    Análisis Sintáctico 
    Análisis Semántico
    Generación de Código
    Documentación 
    Presentación
Cómo Compilar y Ejecutar:
    Para compilar el proyecto, sitúese en el directorio raíz y ejecute el siguiente comando:
        
                make

    Esto generará un ejecutable llamado compilador.

    Para utilizar el compilador, puede pasarle un archivo de código fuente de su lenguaje de la siguiente manera:

                ./compilador < mi_programa.txt

    Esto generará un archivo output.c con el código C traducido, lo compilará y ejecutará el resultado final.