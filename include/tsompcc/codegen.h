#ifndef TSOMPCC_CODEGEN_H
#define TSOMPCC_CODEGEN_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "tsompcc/AST.h"
#include <unordered_map>
#include <string>

namespace tsompcc {

class CodeGenContext {
public:
    llvm::LLVMContext Context;
    llvm::IRBuilder<> Builder;
    std::unique_ptr<llvm::Module> Module;
    static std::unordered_map<std::string, llvm::AllocaInst*> symbols;

    CodeGenContext() : Builder(Context), Module(std::make_unique<llvm::Module>("my_module", Context)) {}
    llvm::Module* getModule() { return Module.get(); }
    void generateCode(ASTNode* root);
};

} // namespace tsompcc

#endif // TSOMPCC_CODEGEN_H
