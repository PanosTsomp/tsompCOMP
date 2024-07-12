#ifndef TSOMPCC_PARSER_H
#define TSOMPCC_PARSER_H

#include "tsompcc/AST.h"
#include <memory>

namespace tsompcc {

extern std::unique_ptr<ASTNode> root;
extern "C" int yylex();
void yyerror(const char* s);

} 

#endif // TSOMPCC_PARSER_H

