#include "tsompcc/AST.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

extern llvm::LLVMContext Context;
extern llvm::IRBuilder<> Builder;
extern std::unique_ptr<llvm::Module> Module;

namespace tsompcc {

llvm::Value* NumberNode::codegen() {
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), value);
}

llvm::Value* FloatNode::codegen() {
    return llvm::ConstantFP::get(llvm::Type::getFloatTy(Context), value);
}

llvm::Value* VariableNode::codegen() {
    // Implement variable lookup and retrieval here
    return nullptr;
}

llvm::Value* BinaryOpNode::codegen() {
    llvm::Value* L = lhs->codegen();
    llvm::Value* R = rhs->codegen();
    if (!L || !R) return nullptr;

    if (op == "==") return Builder.CreateICmpEQ(L, R, "eqtmp");
    else if (op == "!=") return Builder.CreateICmpNE(L, R, "neqtmp");
    else if (op == "<=") return Builder.CreateICmpSLE(L, R, "letmp");
    else if (op == ">=") return Builder.CreateICmpSGE(L, R, "getmp");
    else if (op == "<") return Builder.CreateICmpSLT(L, R, "lttmp");
    else if (op == ">") return Builder.CreateICmpSGT(L, R, "gttmp");
    else if (op == "&&") return Builder.CreateAnd(L, R, "andtmp");
    else if (op == "||") return Builder.CreateOr(L, R, "ortmp");
    else if (op == "+") return Builder.CreateAdd(L, R, "addtmp");
    else if (op == "-") return Builder.CreateSub(L, R, "subtmp");
    else if (op == "*") return Builder.CreateMul(L, R, "multmp");
    else if (op == "/") return Builder.CreateSDiv(L, R, "divtmp");
    return nullptr;
}

llvm::Value* BlockNode::codegen() {
    llvm::Value* last = nullptr;
    for (auto& stmt : statements) {
        last = stmt->codegen();
    }
    return last;
}

llvm::Value* VarDeclNode::codegen() {
    llvm::Value* initVal = expr->codegen();
    if (!initVal) return nullptr;
    // Implement variable declaration and initialization here
    return nullptr;
}

llvm::Value* FuncDeclNode::codegen() {
    // Implement function declaration and code generation here
    return nullptr;
}

llvm::Value* IfNode::codegen() {
    llvm::Value* condVal = condition->codegen();
    if (!condVal) return nullptr;

    condVal = Builder.CreateICmpNE(condVal, llvm::ConstantInt::get(llvm::Type::getInt1Ty(Context), 0, true), "ifcond");

    llvm::Function* func = Builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(Context, "then", func);
    llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(Context, "ifcont");

    Builder.CreateCondBr(condVal, thenBB, mergeBB);

    Builder.SetInsertPoint(thenBB);

    llvm::Value* thenVal = then_stmt->codegen();
    if (!thenVal) return nullptr;

    Builder.CreateBr(mergeBB);
    thenBB = Builder.GetInsertBlock();

    func->getBasicBlockList().push_back(mergeBB);
    Builder.SetInsertPoint(mergeBB);

    return mergeBB;
}

llvm::Value* IfElseNode::codegen() {
    llvm::Value* condVal = condition->codegen();
    if (!condVal) return nullptr;

    condVal = Builder.CreateICmpNE(condVal, llvm::ConstantInt::get(llvm::Type::getInt1Ty(Context), 0, true), "ifcond");

    llvm::Function* func = Builder.GetInsertBlock()->getParent();
    llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(Context, "then", func);
    llvm::BasicBlock* elseBB = llvm::BasicBlock::Create(Context, "else");
    llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(Context, "ifcont");

    Builder.CreateCondBr(condVal, thenBB, elseBB);

    Builder.SetInsertPoint(thenBB);

    llvm::Value* thenVal = then_stmt->codegen();
    if (!thenVal) return nullptr;

    Builder.CreateBr(mergeBB);
    thenBB = Builder.GetInsertBlock();

    func->getBasicBlockList().push_back(elseBB);
    Builder.SetInsertPoint(elseBB);

    llvm::Value* elseVal = else_stmt->codegen();
    if (!elseVal) return nullptr;

    Builder.CreateBr(mergeBB);
    elseBB = Builder.GetInsertBlock();

    func->getBasicBlockList().push_back(mergeBB);
    Builder.SetInsertPoint(mergeBB);

    return mergeBB;
}

llvm::Value* WhileNode::codegen() {
    // Implement while loop code generation here
    return nullptr;
}

llvm::Value* ForNode::codegen() {
    // Implement for loop code generation here
    return nullptr;
}

llvm::Value* ReturnNode::codegen() {
    llvm::Value* retVal = expr->codegen();
    if (!retVal) return nullptr;
    return Builder.CreateRet(retVal);
}


llvm::Value* PrintNode::codegen() {
    llvm::Value* val = expr->codegen();
    if (!val) return nullptr;

    llvm::Function* printfFunc = Module->getFunction("printf");
    if (!printfFunc) {
        // Create the printf function if not already defined
        std::vector<llvm::Type*> printfArgs;
        printfArgs.push_back(llvm::Type::getInt8PtrTy(Context));
        llvm::FunctionType* printfType = llvm::FunctionType::get(llvm::Type::getInt32Ty(Context), printfArgs, true);
        printfFunc = llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", Module.get());
    }

    llvm::Value* formatStr = Builder.CreateGlobalStringPtr("%d\n");
    return Builder.CreateCall(printfFunc, { formatStr, val }, "calltmp");
}

} // namespace tsompcc
