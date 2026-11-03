CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11
LEX = flex
YACC = bison -d

all: compiler

parser.tab.c parser.tab.h:
	$(YACC) parser.y

lexer.yy.c:
	$(LEX) -o lexer.yy.c lexer.l

compiler: parser.tab.c lexer.yy.c ast.c codegen.c main.c
	$(CC) $(CFLAGS) -o compiler parser.tab.c lexer.yy.c ast.c codegen.c main.c

clean:
	rm -f compiler parser.tab.c parser.tab.h lexer.yy.c visualizer/ast.json output.py
