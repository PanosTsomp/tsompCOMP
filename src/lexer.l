%{
#include "tsompcc/parser.h"
#include "../build/parser.hpp"
#include "tsompcc/lexer.h"

#define YY_DECL int yylex()
%}

%%
"int"           { return INT; }
"float"         { return FLOAT; }
"bool"          { return BOOL; }
"void"          { return VOID; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"for"           { return FOR; }
"return"        { return RETURN; }
"print"         { return PRINT; }

[0-9]+          { yylval.num = atoi(yytext); return NUMBER; }
[0-9]+"."[0-9]* { yylval.fnum = atof(yytext); return FLOATNUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.id = strdup(yytext); return IDENTIFIER; }

"=="            { return EQ; }
"!="            { return NEQ; }
"<="            { return LE; }
">="            { return GE; }
"&&"            { return AND; }
"||"            { return OR; }

"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"="             { return '='; }
"<"             { return '<'; }
">"             { return '>'; }
"!"             { return '!'; }

"("             { return '('; }
")"             { return ')'; }
"{"             { return '{'; }
"}"             { return '}'; }
";"             { return ';'; }
","             { return ','; }

[ \t\n]+        { /* Ignore whitespace */ }
"//".*          { /* Ignore comments */ }

.               { return yytext[0]; }

%%

int yywrap() {
    return 1;
}
