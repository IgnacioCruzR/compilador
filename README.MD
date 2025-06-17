Compilador para el Lenguaje BitStream:

     Este repositorio contiene el código fuente y la documentación para un compilador del lenguaje de programación BitStream, desarrollado como parte del laboratorio de compiladores entre el 17 de marzo y el 17 de junio. El objetivo principal de este proyecto es comprender en profundidad el proceso de compilación y adquirir experiencia práctica en la creación de lenguajes de programación desde cero.


 Tecnologías y Entorno
     El desarrollo del compilador se ha realizado utilizando un conjunto de herramientas estándar para la creación de compiladores en un entorno tipo Unix.

Herramientas Principales:
     Flex: Para el análisis léxico.
     Bison: Para el análisis sintáctico.


     C/C++: Para la implementación de la lógica del AST y la generación de código.

Entorno de Compilación:
     Se utilizó gcc como compilador de C y make para la automatización del proceso.
     El compilador fue desarrollado y probado en Windows utilizando la terminal MSYS, y está diseñado para ser completamente ejecutable en un entorno Linux.
 Características del Lenguaje BitStream
     El lenguaje propio diseñado para este proyecto, BitStream, incluye los siguientes elementos fundamentales:

Declaración de Variables:
      Soporte para tipos de datos enteros, flotantes y cadenas.
Funciones: 
     Soporte para funciones que retornan un valor (entero) y funciones que no retornan valor (void).
Operaciones Aritméticas:
      Incluye +, -, *, /.
Operaciones de Comparación:
      Soporta ==, !=, <, >, <=, >=.
Sentencias de Control:
      Implementa las estructuras condicionales if-else y el bucle while.
Funciones de Entrada/Salida:
      Permite la interacción con el usuario mediante imprimir() (salida) y leer() (entrada).
 Funcionalidad del Compilador
     El compilador implementa las etapas clave del proceso de traducción de un lenguaje:

Análisis Léxico y Sintáctico:
      Analiza el código fuente en BitStream para detectar errores léxicos y sintácticos.
Árbol de Sintaxis Abstracta (AST):
      Construye un AST a partir del código fuente si este es sintácticamente válido.
Análisis Semántico:
      Realiza comprobaciones semánticas básicas, como la verificación de la declaración de variables antes de su uso.
Generación de Código:
      Traduce el AST a código C equivalente, que luego se compila para generar un programa ejecutable.
deliverables
     Junto con el código fuente del compilador, el proyecto incluye un informe detallado con la siguiente información:
     La gramática formal del lenguaje BitStream.
     El diseño y la estructura del Árbol de Sintaxis Abstracta (AST).
     Una descripción del proceso de generación de código.
     Un manual de usuario para el lenguaje BitStream.
     Ejemplos de código en BitStream y el código C que se genera a partir de ellos.
Pauta de Evaluación
     El proyecto fue evaluado según la siguiente pauta:

Análisis Léxico :
      Correcta identificación de tokens y manejo de errores.
Análisis Sintáctico :
      Gramática bien definida, correcta construcción del AST y manejo de errores.
Análisis Semántico : 
     Verificación de tipos y declaración de variables.
Generación de Código:
      Generación de código ejecutable correcto y eficiente.
Documentación:
      Claridad y completitud del informe y manual de usuario.
Presentación:
      Claridad y conocimiento en la exposición del proyecto.
Cómo Compilar y Ejecutar
     Compilación del Compilador
     Para compilar el proyecto, sitúese en el directorio raíz y ejecute el siguiente comando. Esto generará un ejecutable llamado compilador.

     make

Ejecución del Compilador
     Para utilizar el compilador, se le debe pasar el nombre del archivo de código fuente como un argumento de la línea de comandos.

     ./compilador mi_programa.bts

     (Se sugiere usar la extensión .bts para los archivos de BitStream).

     Este comando realizará el siguiente proceso:

     Analizará mi_programa.bts.
     Generará un archivo output.c con el código C traducido.
     Compilará output.c para crear un ejecutable final llamado resultado.
     Ejecutará resultado, mostrando la salida del programa en la terminal.