%{
#include <stdio.h>
#include "ast.h"

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