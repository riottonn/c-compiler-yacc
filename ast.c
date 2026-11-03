#include "ast.h"

ASTNode* create_node(NodeType type, const char* name, int int_val) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->name = name ? strdup(name) : NULL;
    node->int_val = int_val;
    node->children = NULL;
    node->child_count = 0;
    return node;
}

void add_child(ASTNode* parent, ASTNode* child) {
    if (!parent || !child) return;
    parent->child_count++;
    parent->children = (ASTNode**)realloc(parent->children, sizeof(ASTNode*) * parent->child_count);
    parent->children[parent->child_count - 1] = child;
}

void free_ast(ASTNode* node) {
    if (!node) return;
    for (int i = 0; i < node->child_count; i++) {
        free_ast(node->children[i]);
    }
    free(node->children);
    if (node->name) free(node->name);
    free(node);
}

static const char* node_type_to_str(NodeType type) {
    switch (type) {
        case NODE_PROGRAM: return "Program";
        case NODE_VAR_DECL: return "VarDecl";
        case NODE_FUNC_DECL: return "FuncDecl";
        case NODE_ASSIGN: return "Assign";
        case NODE_BIN_OP: return "BinaryOp";
        case NODE_UNARY_OP: return "UnaryOp";
        case NODE_IF: return "IfStatement";
        case NODE_WHILE: return "WhileStatement";
        case NODE_RETURN: return "Return";
        case NODE_BLOCK: return "Block";
        case NODE_IDENTIFIER: return "Identifier";
        case NODE_INT_LITERAL: return "Literal";
        default: return "Unknown";
    }
}

void export_ast_to_json(ASTNode* node, FILE* out) {
    if (!node) return;
    fprintf(out, "{\n  \"name\": \"%s", node_type_to_str(node->type));
    if (node->name) {
        fprintf(out, ": %s", node->name);
    } else if (node->type == NODE_INT_LITERAL) {
        fprintf(out, ": %d", node->int_val);
    }
    fprintf(out, "\"");

    if (node->child_count > 0) {
        fprintf(out, ",\n  \"children\": [\n");
        for (int i = 0; i < node->child_count; i++) {
            export_ast_to_json(node->children[i], out);
            if (i < node->child_count - 1) fprintf(out, ",\n");
        }
        fprintf(out, "\n  ]");
    }
    fprintf(out, "\n}");
}

typedef struct {
    char name[32];
    int val;
} Symbol;
static Symbol sym_table[128];
static int sym_count = 0;

static void set_var(const char* name, int val) {
    for (int i = 0; i < sym_count; i++) {
        if (strcmp(sym_table[i].name, name) == 0) {
            sym_table[i].val = val;
            return;
        }
    }
    strcpy(sym_table[sym_count].name, name);
    sym_table[sym_count++].val = val;
}

static int get_var(const char* name) {
    for (int i = 0; i < sym_count; i++) {
        if (strcmp(sym_table[i].name, name) == 0) return sym_table[i].val;
    }
    return 0;
}

int evaluate_ast(ASTNode* node) {
    if (!node) return 0;
    switch (node->type) {
        case NODE_INT_LITERAL:
            return node->int_val;
        case NODE_IDENTIFIER:
            return get_var(node->name);
        case NODE_BIN_OP: {
            int left = evaluate_ast(node->children[0]);
            int right = evaluate_ast(node->children[1]);
            if (strcmp(node->name, "+") == 0) return left + right;
            if (strcmp(node->name, "-") == 0) return left - right;
            if (strcmp(node->name, "*") == 0) return left * right;
            if (strcmp(node->name, "/") == 0) return right ? (left / right) : 0;
            if (strcmp(node->name, "<") == 0) return left < right;
            if (strcmp(node->name, ">") == 0) return left > right;
            if (strcmp(node->name, "==") == 0) return left == right;
            return 0;
        }
        case NODE_ASSIGN: {
            int val = evaluate_ast(node->children[0]);
            set_var(node->name, val);
            return val;
        }
        case NODE_VAR_DECL: {
            if (node->child_count > 0) {
                int val = evaluate_ast(node->children[0]);
                set_var(node->name, val);
                return val;
            }
            set_var(node->name, 0);
            return 0;
        }
        case NODE_FUNC_DECL:
            return evaluate_ast(node->children[0]);
        case NODE_BLOCK:
        case NODE_PROGRAM: {
            int res = 0;
            for (int i = 0; i < node->child_count; i++) {
                res = evaluate_ast(node->children[i]);
            }
            return res;
        }
        case NODE_IF: {
            int cond = evaluate_ast(node->children[0]);
            if (cond) return evaluate_ast(node->children[1]);
            else if (node->child_count > 2) return evaluate_ast(node->children[2]);
            return 0;
        }
        case NODE_WHILE: {
            int res = 0;
            while (evaluate_ast(node->children[0])) {
                res = evaluate_ast(node->children[1]);
            }
            return res;
        }
        case NODE_RETURN:
            return evaluate_ast(node->children[0]);
        default:
            return 0;
    }
}
