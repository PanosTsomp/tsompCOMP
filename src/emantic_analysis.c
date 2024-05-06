#include "ast.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct SymbolTableEntry {
    char* identifier;
    int type;  // Simplified: All types are integers for now
    struct SymbolTableEntry* next;
} SymbolTableEntry;

SymbolTableEntry* symbolTable = NULL;

// Utility function to add symbols to the symbol table
void add_symbol(char* identifier, int type) {
    SymbolTableEntry* entry = malloc(sizeof(SymbolTableEntry));
    if (!entry) return;
    entry->identifier = strdup(identifier);
    entry->type = type;
    entry->next = symbolTable;
    symbolTable = entry;
}

// Function to find a symbol in the symbol table
SymbolTableEntry* find_symbol(char* identifier) {
    SymbolTableEntry* current = symbolTable;
    while (current) {
        if (strcmp(current->identifier, identifier) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Semantic analysis function declarations
void analyze_node(ASTNode* node);
void analyze_expression(ASTNode* node);

// Analyze the whole AST
void analyze_node(ASTNode* node) {
    if (!node) return;

    switch (node->type) {
        case NodeType_Variable:
            if (!find_symbol(node->str)) {
                fprintf(stderr, "Semantic Error: Undeclared variable %s\n", node->str);
            }
            break;
        case NodeType_Assignment:
            analyze_node(node->assign.left);  // Check the variable
            analyze_node(node->assign.right); // Check the expression
            break;
        case NodeType_Operation:
            analyze_expression(node);
            break;
        case NodeType_If:
            analyze_node(node->if_stmt.condition);
            analyze_node(node->if_stmt.true_branch);
            if (node->if_stmt.false_branch) {
                analyze_node(node->if_stmt.false_branch);
            }
            break;
        case NodeType_While:
            analyze_node(node->while_loop.condition);
            analyze_node(node->while_loop.body);
            break;
        case NodeType_Switch:
            analyze_node(node->switch_stmt.var);
            analyze_node(node->switch_stmt.cases);
            break;
        default:
            break;
    }
}

// Analyze expressions to check for type mismatches
void analyze_expression(ASTNode* node) {
    if (node->type == NodeType_Operation) {
        analyze_node(node->op.left);
        analyze_node(node->op.right);
    } else if (node->type == NodeType_Number) {
        // It's a number, type is inherently correct
    } else if (node->type == NodeType_Variable) {
        if (!find_symbol(node->str)) {
            fprintf(stderr, "Semantic Error: Undeclared variable %s used in expression\n", node->str);
        }
    }
}

int main() {
    // Assume 'root' is the root of your AST after parsing
    analyze_node(root);
    return 0;
}
