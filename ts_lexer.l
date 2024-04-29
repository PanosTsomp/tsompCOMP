%{
#include "ts_parser.tab.h"  // Include Bison-generated header with YYSTYPE and yylval definitions
%}


%%

"switch"                { return SWITCH; }
"case"                  { return CASE; }
"default"               { return DEFAULT; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"while"                 { return WHILE; }

"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MULT; }
"/"                     { return DIV; }

"="                     { return ASSIGN; }
"=="                    { return EQ; }
"!="                    { return NEQ; }
"<="                    { return LE; }
">="                    { return GE; }
"<"                     { return LT; }
">"                     { return GT; }

"("                     { return LPAREN; }
")"                     { return RPAREN; }
"{"                     { return LBRACE; }
"}"                     { return RBRACE; }
";"                     { return SEMICOLON; }
":"                     { return COLON; }

[0-9]+                  { yylval.num = atoi(yytext); return NUMBER; }
[a-zA-Z][a-zA-Z0-9]*    { yylval.str = strdup(yytext); return IDENTIFIER; }


[\t\r\n]+               { /* skip whitespace */ }
.                       { printf("Unexpected character: %s\n", yytext); exit(1); } // Add error handling

%%