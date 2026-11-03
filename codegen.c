#include "codegen.h"

static void gen_indent(int level, FILE* out) {
    for (int i = 0; i < level; i++) fprintf(out, "    ");
}

void generate_code(ASTNode* node, int indent, FILE* out) {
    if (!node) return;

    switch (node->type) {
        case NODE_FUNC_DECL:
            fprintf(out, "def %s():\n", node->name);
            generate_code(node->children[0], indent + 1, out);
            fprintf(out, "\nif __name__ == '__main__':\n    print(f'Результат виконання: {%s()}')\n", node->name);
            break;

        case NODE_BLOCK:
            for (int i = 0; i < node->child_count; i++) {
                generate_code(node->children[i], indent, out);
            }
            break;

        case NODE_VAR_DECL:
            gen_indent(indent, out);
            if (node->child_count > 0) {
                fprintf(out, "%s = ", node->name);
                generate_code(node->children[0], 0, out);
            } else {
                fprintf(out, "%s = 0", node->name);
            }
            fprintf(out, "\n");
            break;

        case NODE_ASSIGN:
            gen_indent(indent, out);
            fprintf(out, "%s = ", node->name);
            generate_code(node->children[0], 0, out);
            fprintf(out, "\n");
            break;

        case NODE_RETURN:
            gen_indent(indent, out);
            fprintf(out, "return ");
            generate_code(node->children[0], 0, out);
            fprintf(out, "\n");
            break;

        case NODE_IF:
            gen_indent(indent, out);
            fprintf(out, "if ");
            generate_code(node->children[0], 0, out);
            fprintf(out, ":\n");
            generate_code(node->children[1], indent + 1, out);
            if (node->child_count > 2) {
                gen_indent(indent, out);
                fprintf(out, "else:\n");
                generate_code(node->children[2], indent + 1, out);
            }
            break;

        case NODE_WHILE:
            gen_indent(indent, out);
            fprintf(out, "while ");
            generate_code(node->children[0], 0, out);
            fprintf(out, ":\n");
            generate_code(node->children[1], indent + 1, out);
            break;

        case NODE_BIN_OP:
            fprintf(out, "(");
            generate_code(node->children[0], 0, out);
            fprintf(out, " %s ", node->name);
            generate_code(node->children[1], 0, out);
            fprintf(out, ")");
            break;

        case NODE_UNARY_OP:
            fprintf(out, "%s", node->name);
            generate_code(node->children[0], 0, out);
            break;

        case NODE_IDENTIFIER:
            fprintf(out, "%s", node->name);
            break;

        case NODE_INT_LITERAL:
            fprintf(out, "%d", node->int_val);
            break;

        default:
            break;
    }
}
