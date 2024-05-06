#include "ast.h"
#include <stdio.h>

// Forward declarations for recursive functions
void generate_code(ASTNode* node);
void generate_expression(ASTNode* node);

// Generate C code for the entire AST
void generate_code(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NodeType_Number:
            printf("%d", node->num);
            break;
        case NodeType_Variable:
            printf("%s", node->str);
            break;
        case NodeType_Operation:
            printf("(");
            generate_expression(node->op.left);
            switch (node->op.op) {
                case PLUS:  printf(" + "); break;
                case MINUS: printf(" - "); break;
                case MULT:  printf(" * "); break;
                case DIV:   printf(" / "); break;
                case EQ:    printf(" == "); break;
                case NEQ:   printf(" != "); break;
                case LT:    printf(" < "); break;
                case GT:    printf(" > "); break;
                case LE:    printf(" <= "); break;
                case GE:    printf(" >= "); break;
                default:    printf(" ? "); break;
            }
            generate_expression(node->op.right);
            printf(")");
            break;
        case NodeType_Assignment:
            printf("%s = ", node->assign.left->str);
            generate_expression(node->assign.right);
            printf(";\n");
            break;
        case NodeType_If:
            printf("if (");
            generate_expression(node->if_stmt.condition);
            printf(") {\n");
            generate_code(node->if_stmt.true_branch);
            if (node->if_stmt.false_branch) {
                printf("} else {\n");
                generate_code(node->if_stmt.false_branch);
            }
            printf("}\n");
            break;
        case NodeType_While:
            printf("while (");
            generate_expression(node->while_loop.condition);
            printf(") {\n");
            generate_code(node->while_loop.body);
            printf("}\n");
            break;
        case NodeType_Switch:
            printf("switch (%s) {\n", node->switch_stmt.var->str);
            generate_code(node->switch_stmt.cases);
            printf("}\n");
            break;
        case NodeType_Case:
            printf("case %d:\n", node->case_stmt.value);
            generate_code(node->case_stmt.body);
            printf("break;\n");
            if (node->case_stmt.next_case) {
                generate_code(node->case_stmt.next_case);
            }
            break;
        case NodeType_Default:
            printf("default:\n");
            generate_code(node->default_stmt.body);
            printf("break;\n");
            break;
        default:
            fprintf(stderr, "Unknown node type\n");
            break;
    }
}

// Special handling for expressions to avoid unnecessary parentheses
void generate_expression(ASTNode* node) {
    if (node->type == NodeType_Operation && (node->op.op == PLUS || node->op.op == MINUS ||
        node->op.op == MULT || node->op.op == DIV)) {
        generate_code(node);
    } else {
        printf("(");
        generate_code(node);
        printf(")");
    }
}

int main() {
    // Assume 'root' is the root of your AST after parsing
    printf("#include <stdio.h>\n");
    printf("int main() {\n");
    generate_code(root);
    printf("return 0;\n");
    printf("}\n");
    return 0;
}
