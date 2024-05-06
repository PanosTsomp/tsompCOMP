%{
#include "ts_parser.h"  // This is a Bison generated header (YYSTYPE, yylval)
int line_number = 1;  
  
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
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.str = strdup(yytext); return IDENTIFIER; }

"//"                    { /* skip single-line comment */
                          while(*yytext != '\n' && *yytext != '\0') yylex();
                        }
"/*"                     { /* skip multi-line comment */
                          char c, prev = 0;
                          while((c = input()) != 0) {
                            if(c == '/' && prev == '*') break;
                            prev = c;
                          }
                        }

[\t\r]+                 { /* skip horizontal whitespace */ }
\n                      { /* increment line number on new lines */
                          line_number++; 
                        }
.                       { printf("Unexpected character: %s on line %d\n", yytext, line_number);}

%%