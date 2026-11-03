#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_PROGRAM,
    NODE_VAR_DECL,
    NODE_FUNC_DECL,
    NODE_ASSIGN,
    NODE_BIN_OP,
    NODE_UNARY_OP,
    NODE_IF,
    NODE_WHILE,
    NODE_RETURN,
    NODE_BLOCK,
    NODE_IDENTIFIER,
    NODE_INT_LITERAL
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char *name;
    int int_val;
    struct ASTNode **children;
    int child_count;
} ASTNode;

ASTNode* create_node(NodeType type, const char* name, int int_val);
void add_child(ASTNode* parent, ASTNode* child);
void free_ast(ASTNode* node);
void export_ast_to_json(ASTNode* node, FILE* out);
int evaluate_ast(ASTNode* node);

#endif
