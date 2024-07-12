#ifndef TSOMPCC_LEXER_H
#define TSOMPCC_LEXER_H

#include <FlexLexer.h>

namespace tsompcc {

class Lexer : public yyFlexLexer {
public:
    Lexer(std::istream* in) : yyFlexLexer(in) {}
    virtual ~Lexer() = default;
    virtual int yylex(); // Declaration of yylex()
};

} // namespace tsompcc

#endif // TSOMPCC_LEXER_H
