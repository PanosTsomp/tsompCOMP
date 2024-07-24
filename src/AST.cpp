#include "tsompcc/AST.h"
#include "tsompcc/codegen.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include <iostream>
#include <ostream>

extern llvm::LLVMContext Context;
extern llvm::IRBuilder<> Builder;
extern std::unique_ptr<llvm::Module> Module;

namespace tsompcc {

llvm::Value* NumberNode::codegen() {
    std::cout << "Generating code for NumberNode with value: " << value << std::endl;
    return llvm::ConstantInt::get(llvm::Type::getInt32Ty(Context), value);
}

llvm::Value* FloatNode::codegen() {
    std::cout << "Generating code for FloatNode with value: " << value << std::endl;
    return llvm::ConstantFP::get(llvm::Type::getFloatTy(Context), value);
}

llvm::Value* VariableNode::codegen() {
    std::cout << "Generating code for VariableNode with name: " << name << std::endl;
    llvm::Value* value = CodeGenContext::symbols[name];
    if (!value) {
        std::cerr << "Error: Unknown variable name: " << name << std::endl;
        return nullptr;
    }
    return Builder.CreateLoad(value->getType()->getPointerElementType(), value, name.c_str());
}

llvm::Value* BinaryOpNode::codegen() {
    std::cout << "Generating code for BinaryOpNode with operator: " << op << std::endl;
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
    std::cout << "Generating code for BlockNode with " << statements.size() << " statements." << std::endl;
    llvm::Value* last = nullptr;
    for (auto& stmt : statements) {
        last = stmt->codegen();
    }
    return last;
}

llvm::Value* VarDeclNode::codegen() {
    std::cout << "Generating code for VarDeclNode with name: " << name << std::endl;
    llvm::Value* initVal = expr->codegen();
    if (!initVal) return nullptr;

    // Create an alloca instruction in the entry block of the function
    llvm::Function* function = Builder.GetInsertBlock()->getParent();
    llvm::IRBuilder<> tmpB(&function->getEntryBlock(), function->getEntryBlock().begin());
    llvm::AllocaInst* alloca = tmpB.CreateAlloca(initVal->getType(), nullptr, name);

    // Store the initial value into the alloca
    Builder.CreateStore(initVal, alloca);

    // Add the variable to the symbol table
    CodeGenContext::symbols[name] = alloca;

    return alloca;
}

llvm::Value* FuncDeclNode::codegen() {
    std::cout << "Generating code for FuncDeclNode with name: " << name << std::endl;
    std::vector<llvm::Type*> argTypes(args.size(), llvm::Type::getInt32Ty(Context)); 
    llvm::FunctionType* funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(Context), argTypes, false);

    llvm::Function* function = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, name, Module.get());

    unsigned idx = 0;
    for (auto &arg : function->args()) {
        arg.setName(args[idx++]);
    }

    llvm::BasicBlock* bb = llvm::BasicBlock::Create(Context, "entry", function);
    Builder.SetInsertPoint(bb);

    std::unordered_map<std::string, llvm::AllocaInst*> oldSymbols = CodeGenContext::symbols;
    CodeGenContext::symbols.clear();

    for (auto &arg : function->args()) {
        llvm::AllocaInst* alloca = Builder.CreateAlloca(arg.getType(), nullptr, arg.getName());
        Builder.CreateStore(&arg, alloca);
        CodeGenContext::symbols[arg.getName().str()] = alloca;
    }

    if (llvm::Value* retVal = body->codegen()) {
        Builder.CreateRet(retVal);
        llvm::verifyFunction(*function);

        CodeGenContext::symbols = oldSymbols;
        return function;
    }

    function->eraseFromParent();
    CodeGenContext::symbols = oldSymbols;
    return nullptr;
}

llvm::Value* IfNode::codegen() {
    std::cout << "Generating code for IfNode" << std::endl;
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
    std::cout << "Generating code for IfElseNode" << std::endl;
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
    std::cout << "Generating code for WhileNode" << std::endl;
    llvm::Function* function = Builder.GetInsertBlock()->getParent();

    llvm::BasicBlock* condBB = llvm::BasicBlock::Create(Context, "cond", function);
    llvm::BasicBlock* loopBB = llvm::BasicBlock::Create(Context, "loop");
    llvm::BasicBlock* afterBB = llvm::BasicBlock::Create(Context, "after");

    Builder.CreateBr(condBB);
    Builder.SetInsertPoint(condBB);

    llvm::Value* condVal = condition->codegen();
    if (!condVal) return nullptr;

    condVal = Builder.CreateICmpNE(condVal, llvm::ConstantInt::get(llvm::Type::getInt1Ty(Context), 0, true), "whilecond");
    Builder.CreateCondBr(condVal, loopBB, afterBB);

    function->getBasicBlockList().push_back(loopBB);
    Builder.SetInsertPoint(loopBB);

    llvm::Value* bodyVal = body->codegen();
    if (!bodyVal) return nullptr;

    Builder.CreateBr(condBB);

    function->getBasicBlockList().push_back(afterBB);
    Builder.SetInsertPoint(afterBB);

    return afterBB;
}

llvm::Value* ForNode::codegen() {
    std::cout << "Generating code for ForNode" << std::endl;
    llvm::Function* function = Builder.GetInsertBlock()->getParent();

    llvm::BasicBlock* preheaderBB = Builder.GetInsertBlock();
    llvm::BasicBlock* condBB = llvm::BasicBlock::Create(Context, "cond", function);
    llvm::BasicBlock* loopBB = llvm::BasicBlock::Create(Context, "loop");
    llvm::BasicBlock* afterBB = llvm::BasicBlock::Create(Context, "after");

    Builder.CreateBr(condBB);
    Builder.SetInsertPoint(condBB);

    llvm::Value* initVal = init->codegen();
    if (!initVal) return nullptr;

    llvm::Value* condVal = condition->codegen();
    if (!condVal) return nullptr;

    condVal = Builder.CreateICmpNE(condVal, llvm::ConstantInt::get(llvm::Type::getInt1Ty(Context), 0, true), "forcond");
    Builder.CreateCondBr(condVal, loopBB, afterBB);

    function->getBasicBlockList().push_back(loopBB);
    Builder.SetInsertPoint(loopBB);

    llvm::Value* bodyVal = body->codegen();
    if (!bodyVal) return nullptr;

    llvm::Value* stepVal = step->codegen();
    if (!stepVal) return nullptr;

    Builder.CreateBr(condBB);

    function->getBasicBlockList().push_back(afterBB);
    Builder.SetInsertPoint(afterBB);

    return afterBB;
}

llvm::Value* ReturnNode::codegen() {
    std::cout << "Generating code for ReturnNode" << std::endl;
    llvm::Value* retVal = expr->codegen();
    if (!retVal) return nullptr;
    return Builder.CreateRet(retVal);
}

llvm::Value* PrintNode::codegen() {
    std::cout << "Generating code for PrintNode" << std::endl;
    llvm::Value* val = expr->codegen();
    if (!val) return nullptr;

    llvm::Function* printfFunc = Module->getFunction("printf");
    if (!printfFunc) {
        std::vector<llvm::Type*> printfArgs;
        printfArgs.push_back(llvm::Type::getInt8PtrTy(Context));
        llvm::FunctionType* printfType = llvm::FunctionType::get(llvm::Type::getInt32Ty(Context), printfArgs, true);
        printfFunc = llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, "printf", Module.get());
    }

    llvm::Value* formatStr;
    if (val->getType()->isIntegerTy(32)) {
        formatStr = Builder.CreateGlobalStringPtr("%d\n");
    } else if (val->getType()->isFloatTy()) {
        formatStr = Builder.CreateGlobalStringPtr("%f\n");
    } else if (val->getType()->isIntegerTy(1)) {
        formatStr = Builder.CreateGlobalStringPtr("%d\n");
    } else {
        std::cerr << "Unsupported type for print" << std::endl;
        return nullptr;
    }

    return Builder.CreateCall(printfFunc, { formatStr, val }, "calltmp");
}

} // namespace tsompcc
