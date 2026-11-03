#include <stdio.h>
#include "ast.h"
#include "codegen.h"

extern int yyparse();
extern FILE *yyin;
extern ASTNode *root_ast;

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("Помилка відкриття файлу");
            return 1;
        }
        yyin = file;
    }

    printf("==> [1/4] Синтаксичний розбір вихідного коду C...\n");
    if (yyparse() == 0 && root_ast) {
        printf(" -> Успішно розібрано!\n\n");

        printf("==> [2/4] Обчислення значення через AST (інтерпретація):\n");
        int result = evaluate_ast(root_ast);
        printf(" -> Результат: %d\n\n", result);

        printf("==> [3/4] Експорт AST у visualizer/ast.json...\n");
        FILE *json_file = fopen("visualizer/ast.json", "w");
        if (json_file) {
            export_ast_to_json(root_ast, json_file);
            fclose(json_file);
            printf(" -> Файл visualizer/ast.json створено.\n\n");
        }

        printf("==> [4/4] Генерація цільового коду (output.py)...\n");
        FILE *py_file = fopen("output.py", "w");
        if (py_file) {
            generate_code(root_ast, 0, py_file);
            fclose(py_file);
            printf(" -> Файл output.py збережено.\n\n");
        }

        free_ast(root_ast);
    } else {
        printf("Помилка синтаксичного аналізу.\n");
    }

    return 0;
}
