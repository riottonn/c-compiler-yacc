%{
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
void yyerror(const char *s);
ASTNode *root_ast = NULL;
%}

%union {
    int num;
    char *str;
    ASTNode *node;
}

%token <str> IDENTIFIER EQ NE LE GE LT GT
%token <num> NUMBER
%token TYPE_INT RETURN IF ELSE WHILE ASSIGN
%token PLUS MINUS MULT DIV
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right ASSIGN
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV
%right UMINUS

%type <node> program func_def statement statement_list expr block var_decl

%%

program:
    func_def { root_ast = $1; }
    ;

func_def:
    TYPE_INT IDENTIFIER LPAREN RPAREN block {
        $$ = create_node(NODE_FUNC_DECL, $2, 0);
        add_child($$, $5);
        free($2);
    }
    ;

block:
    LBRACE statement_list RBRACE {
        $$ = $2;
    }
    | LBRACE RBRACE {
        $$ = create_node(NODE_BLOCK, NULL, 0);
    }
    ;

statement_list:
    statement {
        $$ = create_node(NODE_BLOCK, NULL, 0);
        add_child($$, $1);
    }
    | statement_list statement {
        add_child($1, $2);
        $$ = $1;
    }
    ;

statement:
    var_decl SEMICOLON { $$ = $1; }
    | IDENTIFIER ASSIGN expr SEMICOLON {
        $$ = create_node(NODE_ASSIGN, $1, 0);
        add_child($$, $3);
        free($1);
    }
    | RETURN expr SEMICOLON {
        $$ = create_node(NODE_RETURN, "return", 0);
        add_child($$, $2);
    }
    | IF LPAREN expr RPAREN statement %prec LOWER_THAN_ELSE {
        $$ = create_node(NODE_IF, "if", 0);
        add_child($$, $3);
        add_child($$, $5);
    }
    | IF LPAREN expr RPAREN statement ELSE statement {
        $$ = create_node(NODE_IF, "if-else", 0);
        add_child($$, $3);
        add_child($$, $5);
        add_child($$, $7);
    }
    | WHILE LPAREN expr RPAREN statement {
        $$ = create_node(NODE_WHILE, "while", 0);
        add_child($$, $3);
        add_child($$, $5);
    }
    | block { $$ = $1; }
    ;

var_decl:
    TYPE_INT IDENTIFIER {
        $$ = create_node(NODE_VAR_DECL, $2, 0);
        free($2);
    }
    | TYPE_INT IDENTIFIER ASSIGN expr {
        $$ = create_node(NODE_VAR_DECL, $2, 0);
        add_child($$, $4);
        free($2);
    }
    ;

expr:
    expr PLUS expr {
        $$ = create_node(NODE_BIN_OP, "+", 0);
        add_child($$, $1); add_child($$, $3);
    }
    | expr MINUS expr {
        $$ = create_node(NODE_BIN_OP, "-", 0);
        add_child($$, $1); add_child($$, $3);
    }
    | expr MULT expr {
        $$ = create_node(NODE_BIN_OP, "*", 0);
        add_child($$, $1); add_child($$, $3);
    }
    | expr DIV expr {
        $$ = create_node(NODE_BIN_OP, "/", 0);
        add_child($$, $1); add_child($$, $3);
    }
    | expr LT expr {
        $$ = create_node(NODE_BIN_OP, "<", 0);
        add_child($$, $1); add_child($$, $3);
    }
    | expr GT expr {
        $$ = create_node(NODE_BIN_OP, ">", 0);
        add_child($$, $1); add_child($$, $3);
    }
    | expr EQ expr {
        $$ = create_node(NODE_BIN_OP, "==", 0);
        add_child($$, $1); add_child($$, $3);
    }
    | LPAREN expr RPAREN { $$ = $2; }
    | MINUS expr %prec UMINUS {
        $$ = create_node(NODE_UNARY_OP, "-", 0);
        add_child($$, $2);
    }
    | IDENTIFIER {
        $$ = create_node(NODE_IDENTIFIER, $1, 0);
        free($1);
    }
    | NUMBER {
        $$ = create_node(NODE_INT_LITERAL, NULL, $1);
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Синтаксична помилка: %s\n", s);
}
