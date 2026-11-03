#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <stdio.h>

void generate_code(ASTNode* node, int indent, FILE* out);

#endif
