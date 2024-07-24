%{
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include "tsompcc/AST.h"
#include "../build/parser.hpp"
#include "tsompcc/codegen.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
void yyerror(const char *s);
std::unique_ptr<tsompcc::ASTNode> root;
%}

%union {
    int num;
    float fnum;
    char* id;
    const char* type_name; // Changed to const char*
    tsompcc::ASTNode* node;
    std::vector<tsompcc::ASTNodePtr>* node_list;
    std::vector<std::string>* str_list; // Add this for param_list
}

%token <num> NUMBER
%token <fnum> FLOATNUMBER
%token <id> IDENTIFIER

%token INT FLOAT BOOL VOID IF ELSE WHILE FOR RETURN PRINT
%token EQ NEQ LE GE AND OR

%type <node> program stmt expr simple_stmt compound_stmt
%type <node> func_decl var_decl block if_stmt while_stmt for_stmt return_stmt print_stmt
%type <type_name> type // Use const char* for type names
%type <node_list> stmt_list
%type <str_list> param_list // Add this line for param_list

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%left '+' '-'
%left '*' '/'
%left EQ NEQ LE GE '<' '>'
%left AND OR

%%

program:
    stmt_list {
        std::cout << "Building AST for program" << std::endl;
        root = std::unique_ptr<tsompcc::ASTNode>(new tsompcc::BlockNode(std::move(*$1)));
        std::cout << "Root set with BlockNode, containing statements: " << $1->size() << std::endl;
        delete $1;
    }
;

stmt_list:
    stmt_list stmt {
        std::cout << "Adding statement to statement list" << std::endl;
        $1->push_back(tsompcc::ASTNodePtr($2));
        $$ = $1;
    }
    | /* empty */ {
        std::cout << "Creating new statement list" << std::endl;
        $$ = new std::vector<tsompcc::ASTNodePtr>();
    }
;

stmt:
    simple_stmt
    | compound_stmt
;

simple_stmt:
    var_decl
    | func_decl
    | expr ';'
    | return_stmt
    | print_stmt
;

compound_stmt:
    if_stmt
    | while_stmt
    | for_stmt
;

var_decl:
    type IDENTIFIER '=' expr ';' {
        std::cout << "Variable declaration: " << $2 << std::endl;
        $$ = new tsompcc::VarDeclNode($2, tsompcc::ASTNodePtr($4));
    }
;

func_decl:
    type IDENTIFIER '(' param_list ')' block {
        std::cout << "Function declaration: " << $2 << std::endl;
        $$ = new tsompcc::FuncDeclNode($2, std::move(*$4), tsompcc::ASTNodePtr($6));
        delete $4;
    }
;

param_list:
    param_list ',' type IDENTIFIER {
        std::cout << "Adding parameter to function: " << $4 << std::endl;
        $1->push_back($4);
        $$ = $1;
    }
    | type IDENTIFIER {
        std::cout << "First parameter of function: " << $2 << std::endl;
        $$ = new std::vector<std::string>();
        $$->push_back($2);
    }
    | /* empty */ {
        std::cout << "Function with no parameters" << std::endl;
        $$ = new std::vector<std::string>();
    }
;

type:
    INT { $$ = "int"; }
    | FLOAT { $$ = "float"; }
    | BOOL { $$ = "bool"; }
    | VOID { $$ = "void"; }
;

block:
    '{' stmt_list '}' {
        std::cout << "Block of statements" << std::endl;
        $$ = new tsompcc::BlockNode(std::move(*$2));
        delete $2;
    }
;

if_stmt:
    IF '(' expr ')' stmt %prec LOWER_THAN_ELSE {
        std::cout << "If statement" << std::endl;
        $$ = new tsompcc::IfNode(tsompcc::ASTNodePtr($3), tsompcc::ASTNodePtr($5));
    }
    | IF '(' expr ')' stmt ELSE stmt {
        std::cout << "If-Else statement" << std::endl;
        $$ = new tsompcc::IfElseNode(tsompcc::ASTNodePtr($3), tsompcc::ASTNodePtr($5), tsompcc::ASTNodePtr($7));
    }
;

while_stmt:
    WHILE '(' expr ')' stmt {
        std::cout << "While statement" << std::endl;
        $$ = new tsompcc::WhileNode(tsompcc::ASTNodePtr($3), tsompcc::ASTNodePtr($5));
    }
;

for_stmt:
    FOR '(' expr ';' expr ';' expr ')' stmt {
        std::cout << "For statement" << std::endl;
        $$ = new tsompcc::ForNode(tsompcc::ASTNodePtr($3), tsompcc::ASTNodePtr($5), tsompcc::ASTNodePtr($7), tsompcc::ASTNodePtr($9));
    }
;

return_stmt:
    RETURN expr ';' {
        std::cout << "Return statement" << std::endl;
        $$ = new tsompcc::ReturnNode(tsompcc::ASTNodePtr($2));
    }
;

print_stmt:
    PRINT '(' expr ')' ';' {
        std::cout << "Print statement" << std::endl;
        $$ = new tsompcc::PrintNode(tsompcc::ASTNodePtr($3));
    }
;

expr:
    expr EQ expr {
        std::cout << "Equality expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode("==", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr NEQ expr {
        std::cout << "Not-equality expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode("!=", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr LE expr {
        std::cout << "Less-or-equal expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode("<=", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr GE expr {
        std::cout << "Greater-or-equal expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode(">=", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '<' expr {
        std::cout << "Less-than expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode("<", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '>' expr {
        std::cout << "Greater-than expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode(">", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr AND expr {
        std::cout << "Logical AND expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode("&&", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr OR expr {
        std::cout << "Logical OR expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode("||", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '+' expr {
        std::cout << "Addition expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode("+", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '-' expr {
        std::cout << "Subtraction expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode("-", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '*' expr {
        std::cout << "Multiplication expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode("*", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '/' expr {
        std::cout << "Division expression" << std::endl;
        $$ = new tsompcc::BinaryOpNode("/", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | IDENTIFIER {
        std::cout << "Identifier: " << $1 << std::endl;
        $$ = new tsompcc::VariableNode($1);
    }
    | NUMBER {
        std::cout << "Number: " << $1 << std::endl;
        $$ = new tsompcc::NumberNode($1);
    }
    | FLOATNUMBER {
        std::cout << "Float number: " << $1 << std::endl;
        $$ = new tsompcc::FloatNode($1);
    }
    | '(' expr ')' {
        $$ = $2;
    }
;

%%

void yyerror(const char *s) {
    std::cerr << "Error: " << s << std::endl;
}
