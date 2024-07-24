#ifndef TSOMPCC_AST_H
#define TSOMPCC_AST_H

#include <vector>
#include <string>
#include <memory>
#include "llvm/IR/Value.h"

namespace tsompcc {

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual llvm::Value* codegen() = 0;
};

using ASTNodePtr = std::unique_ptr<ASTNode>;

class NumberNode : public ASTNode {
    int value;

public:
    NumberNode(int value) : value(value) {}
    llvm::Value* codegen() override;
};

class FloatNode : public ASTNode {
    float value;

public:
    FloatNode(float value) : value(value) {}
    llvm::Value* codegen() override;
};

class VariableNode : public ASTNode {
    std::string name;

public:
    VariableNode(const std::string &name) : name(name) {}
    llvm::Value* codegen() override;
};

class BinaryOpNode : public ASTNode {
    std::string op;
    ASTNodePtr lhs, rhs;

public:
    BinaryOpNode(const std::string &op, ASTNodePtr lhs, ASTNodePtr rhs)
        : op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}
    llvm::Value* codegen() override;
};

class BlockNode : public ASTNode {
    std::vector<ASTNodePtr> statements;

public:
    BlockNode(std::vector<ASTNodePtr> &&stmts) : statements(std::move(stmts)) {}
    llvm::Value* codegen() override;
    
    size_t size() const { return statements.size(); } // Added size method
};

class VarDeclNode : public ASTNode {
    std::string name;
    ASTNodePtr expr;

public:
    VarDeclNode(const std::string &name, ASTNodePtr expr)
        : name(name), expr(std::move(expr)) {}
    llvm::Value* codegen() override;
};

class FuncDeclNode : public ASTNode {
    std::string name;
    std::vector<std::string> args;
    ASTNodePtr body;

public:
    FuncDeclNode(const std::string &name, std::vector<std::string> &&args, ASTNodePtr body)
        : name(name), args(std::move(args)), body(std::move(body)) {}
    llvm::Value* codegen() override;
};

class IfNode : public ASTNode {
    ASTNodePtr condition;
    ASTNodePtr then_stmt;

public:
    IfNode(ASTNodePtr condition, ASTNodePtr then_stmt)
        : condition(std::move(condition)), then_stmt(std::move(then_stmt)) {}
    llvm::Value* codegen() override;
};

class IfElseNode : public ASTNode {
    ASTNodePtr condition;
    ASTNodePtr then_stmt;
    ASTNodePtr else_stmt;

public:
    IfElseNode(ASTNodePtr condition, ASTNodePtr then_stmt, ASTNodePtr else_stmt)
        : condition(std::move(condition)), then_stmt(std::move(then_stmt)), else_stmt(std::move(else_stmt)) {}
    llvm::Value* codegen() override;
};

class WhileNode : public ASTNode {
    ASTNodePtr condition;
    ASTNodePtr body;

public:
    WhileNode(ASTNodePtr condition, ASTNodePtr body)
        : condition(std::move(condition)), body(std::move(body)) {}
    llvm::Value* codegen() override;
};

class ForNode : public ASTNode {
    ASTNodePtr init;
    ASTNodePtr condition;
    ASTNodePtr step;
    ASTNodePtr body;

public:
    ForNode(ASTNodePtr init, ASTNodePtr condition, ASTNodePtr step, ASTNodePtr body)
        : init(std::move(init)), condition(std::move(condition)), step(std::move(step)), body(std::move(body)) {}
    llvm::Value* codegen() override;
};

class ReturnNode : public ASTNode {
    ASTNodePtr expr;

public:
    ReturnNode(ASTNodePtr expr) : expr(std::move(expr)) {}
    llvm::Value* codegen() override;
};

class PrintNode : public ASTNode {
    ASTNodePtr expr;

public:
    PrintNode(ASTNodePtr expr) : expr(std::move(expr)) {}
    llvm::Value* codegen() override;
};

} // namespace tsompcc

#endif // TSOMPCC_AST_H
