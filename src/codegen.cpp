#include "tsompcc/codegen.h"

namespace tsompcc {

void CodeGenContext::generateCode(ASTNode* root) {
    root->codegen();
    Module->print(llvm::outs(), nullptr);
}

} // namespace tsompcc
