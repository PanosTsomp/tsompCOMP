%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Declarations for the lexer */
int yylex(void);
void yyerror(char*);

/* AST node types */
enum {
    NODE_NUMBER,
    NODE_ADD,
    NODE_SUB,
    NODE_MUL,
    NODE_DIV,
};

/* AST node */
typedef struct AstNode AstNode;
struct AstNode {
    int type;
    double value;
    AstNode* left;
    AstNode* right;
};

/* AST node constructors */
AstNode* new_number(double value);
AstNode* new_add(AstNode* left, AstNode* right);
AstNode* new_sub(AstNode* left, AstNode* right);
AstNode* new_mul(AstNode* left, AstNode* right);
AstNode* new_div(AstNode* left, AstNode* right);

/* AST node destructor */
void free_node(AstNode* node);