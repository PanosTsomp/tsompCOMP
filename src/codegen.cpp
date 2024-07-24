#include "tsompcc/codegen.h"
#include <iostream>

namespace tsompcc {

std::unordered_map<std::string, llvm::AllocaInst*> CodeGenContext::symbols;

void CodeGenContext::generateCode(ASTNode* root) {
    std::cerr << "Hello from codegen" << std::endl;
    if (root) {
        root->codegen();
        Module->print(llvm::outs(), nullptr);
    } else {
        std::cerr << "Error: No root node provided for code generation." << std::endl;
    }
    std::cerr << "Code generation completed." << std::endl;
}

} // namespace tsompcc
