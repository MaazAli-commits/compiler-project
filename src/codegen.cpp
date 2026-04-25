#include "codegen.h"
#include <stdexcept>

using namespace std;

int Codegen::getVarIndex(const string& name) {
    for (int i = 0; i < (int)variables.size(); i++)
        if (variables[i] == name) return i;
    variables.push_back(name);
    return variables.size() - 1;
}

int Codegen::emit(OpCode op, int operand) {
    instructions.push_back({op, operand});
    return instructions.size() - 1; // return index for patching
}

void Codegen::patch(int instrIndex, int operand) {
    instructions[instrIndex].operand = operand;
}

void Codegen::generate(const vector<ASTNodePtr>& program) {
    for (const auto& node : program)
        genStatement(node.get());
    emit(OpCode::HALT);
}

void Codegen::genStatement(const ASTNode* node) {
    if (auto* n = dynamic_cast<const VarDeclNode*>(node)) {
    genExpr(n->initializer.get());
    declaredVars.insert(n->name);  // mark as declared
    int idx = getVarIndex(n->name);
    emit(OpCode::STORE, idx);
}
    else if (auto* n = dynamic_cast<const AssignNode*>(node)) {
        genExpr(n->value.get());
        int idx = getVarIndex(n->name);
        emit(OpCode::STORE, idx);
    }
    else if (auto* n = dynamic_cast<const PrintNode*>(node)) {
        genExpr(n->expr.get());
        emit(OpCode::PRINT);
    }
    else if (auto* n = dynamic_cast<const IfNode*>(node)) {
        // generate condition
        genExpr(n->condition.get());

        // emit JUMP_IF_FALSE — we'll patch the target later
        int jumpFalse = emit(OpCode::JUMP_IF_FALSE, 0);

        // generate then block
        for (const auto& stmt : n->thenBlock)
            genStatement(stmt.get());

        if (!n->elseBlock.empty()) {
            // jump over else block
            int jumpEnd = emit(OpCode::JUMP, 0);
            patch(jumpFalse, instructions.size());

            // generate else block
            for (const auto& stmt : n->elseBlock)
                genStatement(stmt.get());

            patch(jumpEnd, instructions.size());
        } else {
            patch(jumpFalse, instructions.size());
        }
    }
    else if (auto* n = dynamic_cast<const WhileNode*>(node)) {
        int loopStart = instructions.size();

        // generate condition
        genExpr(n->condition.get());

        // jump out if false
        int jumpFalse = emit(OpCode::JUMP_IF_FALSE, 0);

        // generate body
        for (const auto& stmt : n->body)
            genStatement(stmt.get());

        // jump back to loop start
        emit(OpCode::JUMP, loopStart);

        // patch exit jump
        patch(jumpFalse, instructions.size());
    }
    else {
        throw runtime_error("Unknown statement node");
    }
}

void Codegen::genExpr(const ASTNode* node) {
    if (auto* n = dynamic_cast<const NumberNode*>(node)) {
        emit(OpCode::PUSH, n->value);
    }
    else if (auto* n = dynamic_cast<const VariableNode*>(node)) {
    if (declaredVars.find(n->name) == declaredVars.end())
        throw runtime_error("Variable '" + n->name + "' used before declaration");
    int idx = getVarIndex(n->name);
    emit(OpCode::LOAD, idx);
}
    else if (auto* n = dynamic_cast<const BinaryOpNode*>(node)) {
        // --- Constant Folding ---
        auto* leftNum  = dynamic_cast<const NumberNode*>(n->left.get());
        auto* rightNum = dynamic_cast<const NumberNode*>(n->right.get());

        if (leftNum && rightNum) {
            int l = leftNum->value;
            int r = rightNum->value;
            int result = 0;
            if      (n->op == "+")  result = l + r;
            else if (n->op == "-")  result = l - r;
            else if (n->op == "*")  result = l * r;
            else if (n->op == "/")  result = r != 0 ? l / r : 0;
            else if (n->op == "==") result = l == r;
            else if (n->op == "!=") result = l != r;
            else if (n->op == "<")  result = l < r;
            else if (n->op == ">")  result = l > r;
            else if (n->op == "<=") result = l <= r;
            else if (n->op == ">=") result = l >= r;
            // emit single PUSH instead of two PUSHes + operator
            emit(OpCode::PUSH, result);
            return;
        }

        // No folding possible — generate normally
        genExpr(n->left.get());
        genExpr(n->right.get());

        if      (n->op == "+")  emit(OpCode::ADD);
        else if (n->op == "-")  emit(OpCode::SUB);
        else if (n->op == "*")  emit(OpCode::MUL);
        else if (n->op == "/")  emit(OpCode::DIV);
        else if (n->op == "==") emit(OpCode::EQ);
        else if (n->op == "!=") emit(OpCode::NEQ);
        else if (n->op == "<")  emit(OpCode::LT);
        else if (n->op == ">")  emit(OpCode::GT);
        else if (n->op == "<=") emit(OpCode::LEQ);
        else if (n->op == ">=") emit(OpCode::GEQ);
        else throw runtime_error("Unknown operator: " + n->op);
    }
    else {
        throw runtime_error("Unknown expression node");
    }
}