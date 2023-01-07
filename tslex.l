%{
#include <stdio.h>
#include <stdlib.h>

//Keywords
#define TKN_CLASS 1  
#define TKN_PUBLIC 2  
#define TKN_STATIC 3 
#define TKN_MAIN 4 
#define TYPE_INT 5 
#define TYPE_FLOAT 6 
#define TYPE_BOOL 7 
#define TKN_VOID 8 
#define TYPE_CHAR 9 
#define STM_IF 10 
#define STM_FOR 11 
#define STM_WHILE 12 
#define STM_SWITCH 13 
#define STM_CASE 14 
#define STM_ELSE 15

//Punctuation
#define LEFT_CURL
#define RIGHT_CURL
#define LEFT_PARE
#define RIGHT_PARE
#define LEFT_SQPARE
#define RIGHT_SQPARE

//Operators
#define PLUS
#define MULT
#define MINUS
#define SM_THEN
#define GR_THEN
#define REVE
#define DIV
#define OR
#define AND
#define COMMA
%}

/*** This is the declaration of the (keywords,integers,floats,booleans,punctuations,strings,characters,operators and identifiers) ***/


keyword "class"|"public"|"static"|"main"|"int"|"float"|"bool"|"void"|"char"|"if"|"else"|"for"|"while"|"switch"|"case"|"public static void main"
integer [1-9][0-9]*
float [-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?
booleans "true"|"false"
punctuation [(){}[\];]
string \"(\\.|[^"\\])*\"
character '(\\.|[^\\'])'
operator "+"|"*"|"-"|"<"|">"|"!"|"/"|"||"|"&&"|","
identifier [a-zA-Z_][a-zA-Z_0-9]*
blank [  \t\n]+



%%
{keyword}       {printf("%s\t is a keyword \t\t\t||", yytext);
                if (strcmp(yytext, "class") == 0) {
                    
                    printf("TKN_CLASS\n");
                    //return TKN_CLASS;
                }   
                else if (strcmp(yytext, "public") == 0) {
                    
                    printf(" TKN_PUBLIC \n");
                    //return TKN_PUBLIC;
                } 
                else if (strcmp(yytext, "static") == 0) {
                    
                    printf("TKN_STATIC \n");
                    //return TKN_STATIC;
                }
                else if (strcmp(yytext, "main") == 0) {
                   
                    printf("TKN_MAIN \n");
                    //return TKN_MAIN;
                } 
                else if (strcmp(yytext, "int") == 0) {
                    
                    printf("TYPE_INT \n");
                    //return TYPE_INT;
                }
                else if (strcmp(yytext, "float") == 0) {
                    
                    printf("TYPE_FLOAT \n");
                    //return TYPE_FLOAT;
                } 
                else if (strcmp(yytext, "bool") == 0) {
                    
                    printf("TYPE_BOOL \n");
                    //return TYPE_BOOL;
                }
                else if (strcmp(yytext, "void") == 0) {
                    
                    printf("TKN_VOID \n");
                    //return TKN_VOID;
                }else if (strcmp(yytext, "char") == 0) {
                    
                    printf("TYPE_CHAR \n");
                    //return TYPE_CHAR;
                } else if (strcmp(yytext, "if") == 0) {
                    
                    printf("STM_IF \n");
                    //return STM_IF;
                }else if (strcmp(yytext, "for") == 0) {
                    
                    printf("STM_FOR \n");
                    //return STM_FOR;
                } else if (strcmp(yytext, "while") == 0) {
                    
                    printf("STM_WHILE \n");
                    //return STM_WHILE;
                }
                else if (strcmp(yytext, "switch") == 0) {
                    
                    printf("STM_SWITCH \n");
                    //return STM_SWITCH;
                } 
                else if (strcmp(yytext, "case") == 0) {
                    
                    printf("STM_CASE \n");
                    //return STM_CASE;
                }
                else if (strcmp(yytext, "else") == 0) {
                    
                    printf("STM_ELSE \n");
                    //return STM_ELSE;
                }


}
{integer}       {printf("%s\t is an integer\n", yytext);}
{float}         {printf("%s\t is a float\n", yytext);}
{booleans}      {printf("%s\t is a boolean\n", yytext);}
{punctuation}   {printf("%s\t is punctuation \t\t||", yytext);
                if (strcmp(yytext, "(") == 0) { 
                    printf("LEFT_PARE\n");
                    
                }   
                else if (strcmp(yytext, ")") == 0) {
                    
                    printf("RIGHT_PARE\n");
                   
                } 
                else if (strcmp(yytext, "{") == 0) {
                    
                    printf("LEFT_CURL\n");
                    
                }
                else if (strcmp(yytext, "}") == 0) {
                   
                    printf("RIGHT_CURL \n");
                    
                } 
                else if (strcmp(yytext, "[") == 0) {
                    
                    printf("LEFT_SQPARE \n");
                    
                }
                else if (strcmp(yytext, "]") == 0) {
                   
                    printf("RIGHT_SQPARE \n");
                    
                } 
}
{string}        {printf("%s\t is a string\n", yytext);}
{character}     {printf("%s\t is a character\n", yytext);}
{operator}      {printf("%s\t is an operator\t\t\t||", yytext);
                if (strcmp(yytext, "+") == 0) { 
                    printf("PLUS\n");
                    
                }   
                else if (strcmp(yytext, "*") == 0) {
                    
                    printf("MULT\n");
                   
                } 
                else if (strcmp(yytext, "-") == 0) {
                    
                    printf("MINUS\n");
                    
                }
                else if (strcmp(yytext, "<") == 0) {
                   
                    printf("SM_THEN \n");
                    
                } 
                else if (strcmp(yytext, ">") == 0) {
                    
                    printf("GR_THEN \n");
                    
                }
                else if (strcmp(yytext, "!") == 0) {
                   
                    printf("REVE \n");
                    
                }
                else if (strcmp(yytext, "/") == 0) {
                    
                    printf("DIV\n");
                    
                }
                else if (strcmp(yytext, "||") == 0) {
                   
                    printf("OR \n");
                    
                } 
                else if (strcmp(yytext, "&&") == 0) {
                    
                    printf("AND \n");
                    
                }
                else if (strcmp(yytext, ",") == 0) {
                   
                    printf("COMMA \n");
                    
                }
}
{identifier}    {printf("%s     is an identifier\n", yytext);}
{blank}         {}
.               {printf("%s     ERROR:Invalid string\n", yytext); }
%%

int yywrap()
{
  return 1;
}
int main(int argc, char **argv){
    yylex();
    return 0;
}
