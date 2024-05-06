%{
#include <stdio.h>
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
    node->str = strdup(str);
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
    node's type = NodeType_Switch;
    node->switch_stmt.var = var;
    node->switch_stmt.cases = cases;
    return node;
}

ASTNode* new_case_node(int value, ASTNode* body, ASTNode* next_case) {
    ASTNode* node = malloc(sizeof(ASTNode));
    if (node == NULL) return NULL;
    node->type = NodeType_Case;
    node->case_stmt.value = value;
    node's case_stmt.body = body;
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

extern int yylex(void);
void yyerror(char *s) {
    fprintf(stderr, "Error : %s\n", s);
}

ASTNode* root;  // Root of the AST
%}

%union {
    int num;        // For integer literals
    char* str;      // For strings
    ASTNode* node;  // For AST nodes
} 
%token <num> NUMBER
%token <str> IDENTIFIER
%token PLUS MINUS MULT DIV ASSIGN EQ NEQ LT GT LE GE
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON IF ELSE WHILE SWITCH CASE DEFAULT COLON

%type <node> program statement expression assignment_statement control_structure case_statements variable

%%

program:
      /* Empty program */
      | program statement
    ;

statement:
      expression SEMICOLON
      {
          printf("Evaluated Expression: %d\n", $1->num);  // Adjust to how you wish to handle expressions
      }
    | assignment_statement
    | control_structure
    ;

expression:
      NUMBER
      {
          $$ = new_number_node($1);
      }
    | variable
    | expression PLUS expression
      {
          $$ = new_operation_node($1, $3, PLUS);
      }
    | expression MINUS expression
      {
          $$ = new_operation_node($1, $3, MINUS);
      }
    | expression MULT expression
      {
          $$ = new_operation_node($1, $3, MULT);
      }
    | expression DIV expression
      {
          $$ = new_operation_node($1, $3, DIV);
      }
    | LPAREN expression RPAREN
      {
          $$ = $2;  // Simply pass up the inner expression node
      }
    | expression EQ expression
      {
          $$ = new_operation_node($1, $3, EQ);
      }
    | expression NEQ expression
      {
          $$ = new_operation_node($1, $3, NEQ);
      }
    | expression LT expression
      {
          $$ = new_operation_node($1, $3, LT);
      }
    | expression GT expression
      {
          $$ = new_operation_node($1, $3, GT);
      }
    | expression LE expression
      {
          $$ = new_operation_node($1, $3, LE);
      }
    | expression GE expression
      {
          $$ = new_operation_node($1, $3, GE);
      }
    ;

variable:
      IDENTIFIER
      {
          $$ = new_variable_node($1);
      }
    ;

assignment_statement:
      variable ASSIGN expression SEMICOLON
      {
          $$ = new_assignment_node($1, $3);
          printf("Assignment Node Created\n");
      }
    ;

control_structure:
      IF LPAREN expression RPAREN statement
      {
          $$ = new_if_node($3, $5, NULL);
      }
    | IF LPAREN expression RPAREN statement ELSE statement
      {
          $$ = new_if_node($3, $5, $7);
      }
    | WHILE LPAREN expression RPAREN statement
      {
          $$ = new_while_node($3, $5);
      }
    | SWITCH LPAREN variable RPAREN LBRACE case_statements RBRACE
      {
          $$ = new_switch_node($3, $6);
      }
    ;

case_statements:
      CASE NUMBER COLON statement
      {
          $$ = new_case_node($2, $4, NULL);
      }
    | case_statements CASE NUMBER COLON statement
      {
          $$ = new_case_node($3, $5, $1);
      }
    | DEFAULT COLON statement
      {
          $$ = new_default_node($3);
      }
    ;

%%

int main(void) {
    printf("Starting parser...\n");
    if (yyparse() == 0) {
        printf("Parsing complete!\n");
    } else {
        printf("Parsing failed.\n");
    }
    return 0;
}

