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
std::unique_ptr<tsompcc::ASTNode> root; // Declaration of root
%}

%union {
    int num;
    float fnum;
    char* id;
    const char* type_name; // Changed to const char*
    tsompcc::ASTNode* node;
    std::vector<tsompcc::ASTNodePtr>* node_list;
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

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%left '+' '-'
%left '*' '/'
%left EQ NEQ LE GE '<' '>'
%left AND OR

%%

program:
    stmt_list {
        root = std::unique_ptr<tsompcc::ASTNode>(new tsompcc::BlockNode(std::move(*$1)));
        delete $1;
    }
;

stmt_list:
    stmt_list stmt {
        $1->push_back(tsompcc::ASTNodePtr($2));
        $$ = $1;
    }
    | /* empty */ {
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
        $$ = new tsompcc::VarDeclNode($2, tsompcc::ASTNodePtr($4));
    }
;

func_decl:
    type IDENTIFIER '(' ')' block {
        $$ = new tsompcc::FuncDeclNode($2, std::vector<std::string>(), tsompcc::ASTNodePtr($5));
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
        $$ = new tsompcc::BlockNode(std::move(*$2));
        delete $2;
    }
;

if_stmt:
    IF '(' expr ')' stmt %prec LOWER_THAN_ELSE {
        $$ = new tsompcc::IfNode(tsompcc::ASTNodePtr($3), tsompcc::ASTNodePtr($5));
    }
    | IF '(' expr ')' stmt ELSE stmt {
        $$ = new tsompcc::IfElseNode(tsompcc::ASTNodePtr($3), tsompcc::ASTNodePtr($5), tsompcc::ASTNodePtr($7));
    }
;

while_stmt:
    WHILE '(' expr ')' stmt {
        $$ = new tsompcc::WhileNode(tsompcc::ASTNodePtr($3), tsompcc::ASTNodePtr($5));
    }
;

for_stmt:
    FOR '(' expr ';' expr ';' expr ')' stmt {
        $$ = new tsompcc::ForNode(tsompcc::ASTNodePtr($3), tsompcc::ASTNodePtr($5), tsompcc::ASTNodePtr($7), tsompcc::ASTNodePtr($9));
    }
;

return_stmt:
    RETURN expr ';' {
        $$ = new tsompcc::ReturnNode(tsompcc::ASTNodePtr($2));
    }
;

print_stmt:
    PRINT '(' expr ')' ';' {
        $$ = new tsompcc::PrintNode(tsompcc::ASTNodePtr($3));
    }
;

expr:
    expr EQ expr {
        $$ = new tsompcc::BinaryOpNode("==", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr NEQ expr {
        $$ = new tsompcc::BinaryOpNode("!=", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr LE expr {
        $$ = new tsompcc::BinaryOpNode("<=", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr GE expr {
        $$ = new tsompcc::BinaryOpNode(">=", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '<' expr {
        $$ = new tsompcc::BinaryOpNode("<", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '>' expr {
        $$ = new tsompcc::BinaryOpNode(">", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr AND expr {
        $$ = new tsompcc::BinaryOpNode("&&", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr OR expr {
        $$ = new tsompcc::BinaryOpNode("||", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '+' expr {
        $$ = new tsompcc::BinaryOpNode("+", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '-' expr {
        $$ = new tsompcc::BinaryOpNode("-", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '*' expr {
        $$ = new tsompcc::BinaryOpNode("*", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | expr '/' expr {
        $$ = new tsompcc::BinaryOpNode("/", tsompcc::ASTNodePtr($1), tsompcc::ASTNodePtr($3));
    }
    | IDENTIFIER {
        $$ = new tsompcc::VariableNode($1);
    }
    | NUMBER {
        $$ = new tsompcc::NumberNode($1);
    }
    | FLOATNUMBER {
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

