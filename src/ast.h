#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>

typedef enum {
    NodeType_Number,
    NodeType_Variable,
    NodeType_Operation,
    NodeType_Assignment,
    NodeType_If,
    NodeType_While,
    NodeType_Switch,
    NodeType_Case,
    NodeType_Default
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        int num;  // For number literals
        char* str;  // For variables, ensure to free this memory
        struct {
            struct ASTNode* left;
            struct ASTNode* right;
            int op;  // Operation type (PLUS, MINUS, etc.)
        } op;
        struct {
            struct ASTNode* left;
            struct ASTNode* right;
        } assign;
        struct {
            struct ASTNode* condition;
            struct ASTNode* true_branch;
            struct ASTNode* false_branch;
        } if_stmt;
        struct {
            struct ASTNode* condition;
            struct ASTNode* body;
        } while_loop;
        struct {
            struct ASTNode* var;
            struct ASTNode* cases;  // List of case nodes
        } switch_stmt;
        struct {
            int value;
            struct ASTNode* body;
            struct ASTNode* next_case;
        } case_stmt;
        struct {
            struct ASTNode* body;
        } default_stmt;
    };
} ASTNode;

// Function prototypes for creating different types of AST nodes
ASTNode* new_number_node(int num) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->type = NodeType_Number;
    node->num = num;
    return node;
}

ASTNode* new_variable_node(char* str) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->type = NodeType_Variable;
    node->str = strdup(str);  // Copy string to ensure ownership
    return node;
}

ASTNode* new_operation_node(ASTNode* left, ASTNode* right, int op) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->type = NodeType_Operation;
    node->op.left = left;
    node->op.right = right;
    node->op.op = op;
    return node;
}

ASTNode* new_assignment_node(ASTNode* left, ASTNode* right) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->type = NodeType_Assignment;
    node->assign.left = left;
    node->assign.right = right;
    return node;
}

ASTNode* new_if_node(ASTNode* condition, ASTNode* true_branch, ASTNode* false_branch) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->type = NodeType_If;
    node->if_stmt.condition = condition;
    node->if_stmt.true_branch = true_branch;
    node->if_stmt.false_branch = false_branch;
    return node;
}

ASTNode* new_while_node(ASTNode* condition, ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->type = NodeType_While;
    node->while_loop.condition = condition;
    node->while_loop.body = body;
    return node;
}

ASTNode* new_switch_node(ASTNode* var, ASTNode* cases) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->type = NodeType_Switch;
    node->switch_stmt.var = var;
    node->switch_stmt.cases = cases;
    return node;
}

ASTNode* new_case_node(int value, ASTNode* body, ASTNode* next_case) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->type = NodeType_Case;
    node->case_stmt.value = value;
    node->case_stmt.body = body;
    node->case_stmt.next_case = next_case;
    return node;
}

ASTNode* new_default_node(ASTNode* body) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->type = NodeType_Default;
    node->default_stmt.body = body;
    return node;
}

#endif // AST_H