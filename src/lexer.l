%{
#include "tsompcc/parser.h"
#include "../build/parser.hpp"
#include "tsompcc/lexer.h"

#define YY_DECL int yylex()

%}

%%
"int"           { std::cout << "Token: int" << std::endl; return INT; }
"float"         { std::cout << "Token: float" << std::endl; return FLOAT; }
"bool"          { std::cout << "Token: bool" << std::endl; return BOOL; }
"void"          { std::cout << "Token: void" << std::endl; return VOID; }
"if"            { std::cout << "Token: if" << std::endl; return IF; }
"else"          { std::cout << "Token: else" << std::endl; return ELSE; }
"while"         { std::cout << "Token: while" << std::endl; return WHILE; }
"for"           { std::cout << "Token: for" << std::endl; return FOR; }
"return"        { std::cout << "Token: return" << std::endl; return RETURN; }
"print"         { std::cout << "Token: print" << std::endl; return PRINT; }

[0-9]+          { std::cout << "Token: number" << std::endl; yylval.num = atoi(yytext); return NUMBER; }
[0-9]+"."[0-9]* { std::cout << "Token: float number" << std::endl; yylval.fnum = atof(yytext); return FLOATNUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { std::cout << "Token: identifier" << std::endl; yylval.id = strdup(yytext); return IDENTIFIER; }

"=="            { std::cout << "Token: ==" << std::endl; return EQ; }
"!="            { std::cout << "Token: !=" << std::endl; return NEQ; }
"<="            { std::cout << "Token: <=" << std::endl; return LE; }
">="            { std::cout << "Token: >=" << std::endl; return GE; }
"&&"            { std::cout << "Token: &&" << std::endl; return AND; }
"||"            { std::cout << "Token: ||" << std::endl; return OR; }

"+"             { std::cout << "Token: +" << std::endl; return '+'; }
"-"             { std::cout << "Token: -" << std::endl; return '-'; }
"*"             { std::cout << "Token: *" << std::endl; return '*'; }
"/"             { std::cout << "Token: /" << std::endl; return '/'; }
"="             { std::cout << "Token: =" << std::endl; return '='; }
"<"             { std::cout << "Token: <" << std::endl; return '<'; }
">"             { std::cout << "Token: >" << std::endl; return '>'; }
"!"             { std::cout << "Token: !" << std::endl; return '!'; }

"("             { std::cout << "Token: (" << std::endl; return '('; }
")"             { std::cout << "Token: )" << std::endl; return ')'; }
"{"             { std::cout << "Token: {" << std::endl; return '{'; }
"}"             { std::cout << "Token: }" << std::endl; return '}'; }
";"             { std::cout << "Token: ;" << std::endl; return ';'; }
","             { std::cout << "Token: ," << std::endl; return ','; }

[ \t\n]+        { /* Ignore whitespace */ }
"//".*          { /* Ignore comments */ }

.               { std::cout << "Token: " << yytext << std::endl; return yytext[0]; }

%%

int yywrap() {
    return 1;
}
