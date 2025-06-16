# --- Variables para Herramientas y Nombres ---
CC = gcc
YACC = bison
LEX = flex
TARGET = compilador

# --- Variables para Flags ---
# CFLAGS: Flags para el compilador de C (-g para depuración, -Wall para warnings)
# LDFLAGS: Flags para el enlazador (-lfl para la librería de flex)
CFLAGS = -g -Wall
LDFLAGS = -lfl

# --- Ficheros Fuente y Objeto ---
# Lista de ficheros objeto necesarios para el ejecutable final
OBJECTS = parser.tab.o lex.yy.o main.o


# --- Reglas ---

# Regla por defecto: se ejecuta al escribir "make"
all: $(TARGET)

# Regla de Enlazado: Crea el ejecutable final a partir de los ficheros objeto
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

# Regla de Compilación: Convierte cualquier .c en un .o
# $< es el primer prerrequisito (el .c), $@ es el objetivo (el .o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla de Generación: Crea el parser. El flag -d es para crear el .h
parser.tab.c parser.tab.h: parser.y
	$(YACC) -d parser.y

# Regla de Generación: Crea el lexer. Depende del .h del parser
lex.yy.c: lexer.l parser.tab.h
	$(LEX) lexer.l

# Regla de Limpieza: Borra todos los ficheros generados
clean:
	rm -f $(TARGET) $(OBJECTS) lex.yy.c parser.tab.c parser.tab.h