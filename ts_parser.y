%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern int yylex(void);
void yyerror(char *s);


%}

%union {
    int num;
    char *str;
}

%token <num> NUMBER
%token <str> IDENTIFIER
%token PLUS MINUS MULT DIV ASSIGN EQ NEQ LT GT LE GE
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON IF ELSE WHILE SWITCH CASE DEFAULT COLON

%type <num> expression
%type <str> variable

%%

program:
    | program statement
    ;

statement:
      expression SEMICOLON                { printf("Expression Value: %d\n", $1); }
    | assignment_statement
    | control_structure
    ;

expression:
    NUMBER                             { $$ = $1; }
    | variable                           { $$ = atoi($1); free($1); }
    | expression PLUS expression         { $$ = $1 + $3; }
    | expression MINUS expression        { $$ = $1 - $3; }
    | expression MULT expression         { $$ = $1 * $3; }
    | expression DIV expression          { $$ = $1 / $3; }
    | LPAREN expression RPAREN           { $$ = $2; }
    | expression EQ expression           { $$ = $1 == $3; }
    | expression NEQ expression          { $$ = $1 != $3; }
    | expression LT expression           { $$ = $1 < $3; }
    | expression GT expression           { $$ = $1 > $3; }
    | expression LE expression           { $$ = $1 <= $3; }
    | expression GE expression           { $$ = $1 >= $3; }
    ;

variable:
      IDENTIFIER                         { $$ = strdup($1); }
    ;

assignment_statement:
      variable ASSIGN expression SEMICOLON {
          printf("Assign %s = %d\n", $1, $3); free($1);
      }
    ;

control_structure:
      IF LPAREN expression RPAREN statement
    | IF LPAREN expression RPAREN statement ELSE statement
    | WHILE LPAREN expression RPAREN statement
    | SWITCH LPAREN variable RPAREN LBRACE case_statements RBRACE
    ;

case_statements:
      CASE NUMBER COLON statement case_statements
    | DEFAULT COLON statement
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    return yyparse();
}
