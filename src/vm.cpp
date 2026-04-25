#include "vm.h"
#include <stdexcept>

using namespace std;

VM::VM(const vector<Instruction>& instructions, const vector<string>& variables)
    : instructions(instructions), variableNames(variables) {
    memory.resize(variables.size(), 0);
    profile.resize(instructions.size(), 0);
    stack.reserve(256);
}

void VM::push(int val) {
    stack.push_back(val);
}

int VM::pop() {
    if (stack.empty()) throw runtime_error("Stack underflow");
    int val = stack.back();
    stack.pop_back();
    return val;
}

void VM::run() {
    ip = 0;
    while (ip < (int)instructions.size()) {
        Instruction instr = instructions[ip];
        profile[ip]++;      // profiling: count how many times each instruction runs
        ip++;

        switch (instr.op) {
            case OpCode::PUSH:
                push(instr.operand);
                break;

            case OpCode::LOAD:
                push(memory[instr.operand]);
                break;

            case OpCode::STORE:
                memory[instr.operand] = pop();
                break;

            case OpCode::ADD:  { int b = pop(), a = pop(); push(a + b); break; }
            case OpCode::SUB:  { int b = pop(), a = pop(); push(a - b); break; }
            case OpCode::MUL:  { int b = pop(), a = pop(); push(a * b); break; }
            case OpCode::DIV:  {
                int b = pop(), a = pop();
                if (b == 0) throw runtime_error("Division by zero");
                push(a / b);
                break;
            }

            case OpCode::EQ:   { int b = pop(), a = pop(); push(a == b); break; }
            case OpCode::NEQ:  { int b = pop(), a = pop(); push(a != b); break; }
            case OpCode::LT:   { int b = pop(), a = pop(); push(a < b);  break; }
            case OpCode::GT:   { int b = pop(), a = pop(); push(a > b);  break; }
            case OpCode::LEQ:  { int b = pop(), a = pop(); push(a <= b); break; }
            case OpCode::GEQ:  { int b = pop(), a = pop(); push(a >= b); break; }

            case OpCode::JUMP:
                ip = instr.operand;
                break;

            case OpCode::JUMP_IF_FALSE:
                if (pop() == 0) ip = instr.operand;
                break;

            case OpCode::PRINT:
                cout << pop() << endl;
                break;

            case OpCode::HALT:
                return;

            default:
                throw runtime_error("Unknown opcode");
        }
    }
}

void VM::printProfile() {
    cout << "\n--- Runtime Profile ---\n";
    int hotThreshold = 0;
    for (int c : profile) hotThreshold = max(hotThreshold, c);
    hotThreshold = max(1, hotThreshold / 2);  // top 50% = hot

    for (int i = 0; i < (int)instructions.size(); i++) {
        string label = (profile[i] >= hotThreshold && profile[i] > 1) ? "  << HOT" : "";
        cout << "  [" << i << "] count=" << profile[i] << label << "\n";
    }
}