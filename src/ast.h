#pragma once
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Base class for all AST nodes
struct ASTNode {
    virtual ~ASTNode() = default;
};

using ASTNodePtr = unique_ptr<ASTNode>;

// Number literal: 42
struct NumberNode : ASTNode {
    int value;
    NumberNode(int value) : value(value) {}
};

// Variable: x
struct VariableNode : ASTNode {
    string name;
    VariableNode(const string& name) : name(name) {}
};

// Binary operation: a + b, a * b
struct BinaryOpNode : ASTNode {
    string op;
    ASTNodePtr left;
    ASTNodePtr right;
    BinaryOpNode(const string& op, ASTNodePtr left, ASTNodePtr right)
        : op(op), left(move(left)), right(move(right)) {}
};

// Variable declaration: int x = 5;
struct VarDeclNode : ASTNode {
    string name;
    ASTNodePtr initializer;
    VarDeclNode(const string& name, ASTNodePtr initializer)
        : name(name), initializer(move(initializer)) {}
};

// Assignment: x = 10;
struct AssignNode : ASTNode {
    string name;
    ASTNodePtr value;
    AssignNode(const string& name, ASTNodePtr value)
        : name(name), value(move(value)) {}
};

// Print statement: print(x);
struct PrintNode : ASTNode {
    ASTNodePtr expr;
    PrintNode(ASTNodePtr expr) : expr(move(expr)) {}
};

// If statement: if (cond) { ... } else { ... }
struct IfNode : ASTNode {
    ASTNodePtr condition;
    vector<ASTNodePtr> thenBlock;
    vector<ASTNodePtr> elseBlock;
    IfNode(ASTNodePtr condition, vector<ASTNodePtr> thenBlock, vector<ASTNodePtr> elseBlock)
        : condition(move(condition)), thenBlock(move(thenBlock)), elseBlock(move(elseBlock)) {}
};

// While loop: while (cond) { ... }
struct WhileNode : ASTNode {
    ASTNodePtr condition;
    vector<ASTNodePtr> body;
    WhileNode(ASTNodePtr condition, vector<ASTNodePtr> body)
        : condition(move(condition)), body(move(body)) {}
};

// Block of statements
struct BlockNode : ASTNode {
    vector<ASTNodePtr> statements;
    BlockNode(vector<ASTNodePtr> statements) : statements(move(statements)) {}
};