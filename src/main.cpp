#include <iostream>
#include <fstream>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "tsompcc/codegen.h"
#include "tsompcc/parser.h"
#include "../build/parser.hpp"  

extern FILE *yyin;  // Explicitly declare yyin
//extern int yyparse();  // Explicitly declare yyparse

llvm::LLVMContext Context;
llvm::IRBuilder<> Builder(Context);
std::unique_ptr<llvm::Module> Module = std::make_unique<llvm::Module>("my_module", Context);
std::unique_ptr<tsompcc::ASTNode> tsompcc::root;
int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            std::cerr << "Cannot open file: " << argv[1] << std::endl;
            return 1;
        }
        yyin = file; // Use yyin directly
    } else {
        std::cerr << "Usage: " << argv[0] << " <source-file>" << std::endl;
        return 1;
    }

    std::cout << "Starting parsing..." << std::endl;
    yyparse();
    std::cout << "Parsing completed." << std::endl;

    if (tsompcc::root) {
        std::cout << "Generating code..." << std::endl;
        tsompcc::CodeGenContext codegen;
        codegen.generateCode(tsompcc::root.get());
        std::cout << "Code generation completed." << std::endl;
    } else {
        std::cout << "No AST generated." << std::endl;
    }

    return 0;
}
