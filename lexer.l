%{
#include "ast.h"
#include "parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%option noyywrap

DIGIT       [0-9]+
ID          [a-zA-Z_][a-zA-Z0-9_]*
WS          [ \t\r\n]+

%%

"int"           { return TYPE_INT; }
"return"        { return RETURN; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }

"=="            { yylval.str = strdup(yytext); return EQ; }
"!="            { yylval.str = strdup(yytext); return NE; }
"<="            { yylval.str = strdup(yytext); return LE; }
">="            { yylval.str = strdup(yytext); return GE; }
"<"             { yylval.str = strdup(yytext); return LT; }
">"             { yylval.str = strdup(yytext); return GT; }
"="             { return ASSIGN; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULT; }
"/"             { return DIV; }

"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
";"             { return SEMICOLON; }
","             { return COMMA; }

{DIGIT}         { yylval.num = atoi(yytext); return NUMBER; }
{ID}            { yylval.str = strdup(yytext); return IDENTIFIER; }
"//".*          { /* ignore */ }
"/*"([^*]|\*+[^*/])*\*+"/" { /* ignore */ }
{WS}            { /* ignore */ }

.               { printf("Невідомий символ: %s\n", yytext); }

%%
