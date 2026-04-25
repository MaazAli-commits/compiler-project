#pragma once
#include "ast.h"
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

enum class OpCode {
    PUSH,
    LOAD,
    STORE,
    ADD,
    SUB,
    MUL,
    DIV,
    EQ,
    NEQ,
    LT,
    GT,
    LEQ,
    GEQ,
    JUMP,
    JUMP_IF_FALSE,
    PRINT,
    HALT
};

struct Instruction {
    OpCode op;
    int operand;
};

class Codegen {
public:
    vector<Instruction> instructions;
    vector<string> variables;
    unordered_set<string> declaredVars;  // track declared variables

    void generate(const vector<ASTNodePtr>& program);

private:
    void genStatement(const ASTNode* node);
    void genExpr(const ASTNode* node);

    int getVarIndex(const string& name);
    int emit(OpCode op, int operand = 0);
    void patch(int instrIndex, int operand);
};