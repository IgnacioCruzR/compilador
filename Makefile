all: compilador

compilador: lex.yy.c parser.tab.c main.c
	gcc -o compilador parser.tab.c lex.yy.c main.c -lfl

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

clean:
	rm -f compilador lex.yy.c parser.tab.c parser.tab.h
